modded class ZombieBase
{
	//! SHARED
	override void Init()
	{
		super.Init();

		if (!GetGame().IsDedicatedServer() || !ZenModEnabled("ZenZombieDoors"))
			return;

		//! ZOMBIE DOORS
		SetupZombieDoors();
	}

	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//! ZOMBIE DOORS
		bool superman = super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished);
		if (!ZenModEnabled("ZenZombieDoors"))
			return superman;

		if (GetMindStateSynced() <= DayZInfectedConstants.MINDSTATE_DISTURBED || !IsAlive())
			return superman;

		m_DoorsCheckTimer += pDt;

		if (m_DoorsCheckTimer >= GetZombieDoorsConfig().HitDoorDelaySecs)
		{
			m_DoorsCheckTimer = 0;
			CheckBuildingDoors();
		}

		return superman;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo") && GetGame().IsDedicatedServer() && GetZenArtilleryConfig().Zombie_Kill)
		{
			SetHealth(0);
		}
	}

	//! ZOMBIE DOORS 
	protected ref NoiseParams m_DoorNoiseParams;
	protected ref ZenZombieData m_ZenZombieData;
	protected float m_DoorsCheckTimer;
	protected bool m_DoorHittingPaused;

	void SetupZombieDoors()
	{
		m_DoorsCheckTimer = 0;
		m_DoorHittingPaused = false;

		m_ZenZombieData = GetZombieDoorsConfig().GetZombieData(GetType());
		bool canHitDoor = GetZombieDoorsConfig().KnockDownDoors && GetZombieDoorsConfig().DoorHitCount > 0;

		if (m_ZenZombieData)
			canHitDoor = m_ZenZombieData.KnockDownDoors && m_ZenZombieData.DoorHitCount > 0;

		if (canHitDoor && !m_DoorNoiseParams)
			m_DoorNoiseParams = new NoiseParams();
	}

	protected void CheckBuildingDoors()
	{
		if (m_DoorHittingPaused)
			return;

		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition(GetPosition(), 20.0, objects, NULL);

		if (!objects || objects.Count() == 0)
			return;

		// Get nearby players
		array<PlayerBase> nearbyPlayers = new array<PlayerBase>;
		foreach (Object obj1 : objects)
		{
			PlayerBase pb = PlayerBase.Cast(obj1);
			if (pb)
				nearbyPlayers.Insert(pb);
		};

		if (!nearbyPlayers || nearbyPlayers.Count() == 0)
			return;

		// Check nearby doors
		foreach (Object obj2 : objects)
		{
			Building building = Building.Cast(obj2);
			if (building && !building.GetType().Contains("Wreck") && !building.GetType().Contains("wreck"))
			{
				HandleBuildingDoors(building, nearbyPlayers);
				break;
			}
		}
	}

	protected void HandleBuildingDoors(Building building, array<PlayerBase> nearbyPlayers)
	{
		int doorsCount = GetGame().ConfigGetChildrenCount("CfgVehicles " + building.GetType() + " Doors");
		vector doorPos = vector.Zero;
		int doorIndex = -1;
		int dist = 9999;

		// Check all nearby door positions to zombie, prioritizing doors close to players
		for (int i = 0; i < doorsCount; i++)
		{
			if (building.IsDoorOpen(i))
				continue;

			vector checkDoorPos = building.GetDoorSoundPos(i);

			if (vector.Distance(GetPosition(), checkDoorPos) >= 1.8)
				continue;

			foreach (PlayerBase pb : nearbyPlayers)
			{
				// If zombie is super close to player don't hit door
				if (vector.Distance(GetPosition(), pb.GetPosition()) < 0.4)
					return;

				float t_dist = vector.Distance(pb.GetPosition(), checkDoorPos);
				if (t_dist < dist)
				{
					doorPos = checkDoorPos;
					dist = t_dist;
					doorIndex = i;
				}
			}
		}

		if (doorPos == vector.Zero)
			return;

		// Send attack animation sync
		int attackType = Math.RandomIntInclusive(9, 13);
		m_ActualAttackType = GetDayZInfectedType().ChooseAttack(DayZInfectedAttackGroupType.FIGHT, 1, 0);
		DayZInfectedCommandAttack dayZInfectedCommandAttackTest = StartCommand_Attack(NULL, attackType, 1);

		// Check if door should open based on relevant config (global or zed-type specific)
		bool openedDoor = false;
		if (building.IsDoorLocked(i))
		{
			if (m_ZenZombieData)
				openedDoor = m_ZenZombieData.KnockDownLockedDoors && GetZenZombieDoorManager().GetDoorHitCount(doorPos) >= m_ZenZombieData.KnockDownLockedDoors;
			else
				openedDoor = GetZombieDoorsConfig().KnockDownLockedDoors && GetZenZombieDoorManager().GetDoorHitCount(doorPos) >= GetZombieDoorsConfig().DoorHitLockedCount;
		}
		else
		{
			if (m_ZenZombieData)
				openedDoor = m_ZenZombieData.KnockDownDoors && GetZenZombieDoorManager().GetDoorHitCount(doorPos) >= m_ZenZombieData.DoorHitCount;
			else
				openedDoor = GetZombieDoorsConfig().KnockDownDoors && GetZenZombieDoorManager().GetDoorHitCount(doorPos) >= GetZombieDoorsConfig().DoorHitCount;
		}

		// Trigger sound and other fx
		TriggerZombieDoorSoundFX(doorPos, building, doorIndex, attackType == 12, openedDoor);
	}

	protected void ZombieOpenedDoor(Building building, int index, vector doorPos)
	{
		if (building.IsDoorLocked(index))
			building.UnlockDoor(index);

		building.OpenDoor(index);
		GetZenZombieDoorManager().RemoveDoorAt(doorPos);
	}

	protected void TriggerZombieDoorSoundFX(vector doorPos, Building building, int doorIndex, bool doubleHit, bool openedDoor)
	{
		OrientateZombieToDoor(doorPos);

		// Delay sound to match animation
		float delay = Math.RandomFloatInclusive(300, 500);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnZombieDoorSoundFX, delay, false, doorPos);

		if (!doubleHit)
		{
			// If door was opened, open it after sound delay
			if (openedDoor)
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZombieOpenedDoor, delay + 10, false, building, doorIndex, doorPos);
		}
		else
		{
			// If attackType == 12 then the zed does a double-hit, so play sound twice with delay
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnZombieDoorSoundFX, delay * 3, false, doorPos);

			if (openedDoor)
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZombieOpenedDoor, (delay * 3) + 10, false, building, doorIndex, doorPos);
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ResetDoorHitCounter);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetDoorHitCounter, GetZombieDoorsConfig().DoorHitCounterTimerSecs * 1000, false, doorPos);
	}

	protected void ResetDoorHitCounter(vector doorPos)
	{
		GetZenZombieDoorManager().RemoveDoorAt(doorPos);
	}

	protected void SpawnZombieDoorSoundFX(vector pos)
	{
		// Trigger server-side noise (not a sound, attracts AI like other zombies)
		m_DoorNoiseParams.LoadFromPath("CfgSoundSets Zombie_Hit_Door_SoundSet Noise");
		GetGame().GetNoiseSystem().AddNoiseTarget(pos, 1.0, m_DoorNoiseParams, 1.0);

		// Spawn sound object (deletes itself and plays a door bang sound)
		GetGame().CreateObject("ZenZombieDoorBangerBang", pos);

		// Make the zombie dance
		OrientateZombieToDoor(pos);
	}

	// Thanks ChatGPT ;) I might have failed math, but I still know how to write a good prompt
	protected void OrientateZombieToDoor(vector targetPos)
	{
		if (!GetZombieDoorsConfig().OrientateToDoor)
			return;

		// Calculate direction vector from object to target
		vector startPos = GetPosition();
		vector direction = targetPos - startPos;

		// Calculate yaw angle in radians. Note that in Enfusion's coordinate system,
		// Z is typically the forward axis and X is the side axis.
		float yawRadians = Math.Atan2(direction[0], direction[2]); // [0] is X, [2] is Z

		// Convert radians to degrees
		float yawDegrees = yawRadians * Math.RAD2DEG;

		// Adjust the angle to be between 0 and 360 degrees if necessary
		if (yawDegrees < 0)
			yawDegrees += 360;

		// Set object orientation's yaw angle
		vector objectOri = GetOrientation();
		objectOri[0] = yawDegrees;
		SetOrientation(objectOri);
		SetSynchDirty();
	}

	// Disable hitting doors temporarily if zombie recently hit a player
	override bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_ATTACK)
		{
			if (!m_ActualAttackType) // Nullpointer bugfix in super() which happens occasionally, probably caused by doing my janky overrides to fight logic
				return false;

			if (m_ActualAttackType.m_Subtype != 1) // Subtype 1 = door attack animation
			{
				m_DoorHittingPaused = true;
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ReactivateDoorHitting);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ReactivateDoorHitting, GetZombieDoorsConfig().HitDoorDelaySecs + Math.RandomFloatInclusive(3000, 6000), false);
			}
		}

		return super.FightLogic(pCurrentCommandID, pInputController, pDt);
	};

	protected void ReactivateDoorHitting()
	{
		m_DoorHittingPaused = false;
	}
}