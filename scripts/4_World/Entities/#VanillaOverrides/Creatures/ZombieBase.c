modded class ZombieBase
{
	static int ZENMOD_ZOMBIE_COUNT = 0;

#ifdef SERVER
	static int ZENMOD_BUILDING_SEARCH_RADIUS = 5; // 5 meters
	protected ref array<Object> m_ZenZombieDoorsCache;
	protected vector m_ZenZombieDoorsLastCheckPos;

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		ZENMOD_ZOMBIE_COUNT++;

		if (!ZenModEnabled("ZenZombieDoors"))
			return;

		//! ZOMBIE DOORS
		SetupZombieDoors();
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZENMOD_ZOMBIE_COUNT--;
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			if (killer)
			{
				Weapon_Base gun = Weapon_Base.Cast(killer);
				if (gun && gun.GetZenEngravedPlayerName() != "")
				{
					gun.IncreaseZenZombieKills();
				}
			}
		}		
	}

	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//! ZOMBIE DOORS
		bool superman = super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished);
		if (!ZenModEnabled("ZenZombieDoors"))
			return superman;

		if (GetMindStateSynced() <= DayZInfectedConstants.MINDSTATE_DISTURBED || !IsAlive())
			return superman;

		m_ZenDoorsCheckTimer += pDt;

		if (m_ZenDoorsCheckTimer >= GetZombieDoorsConfig().HitDoorDelaySecs)
		{
			m_ZenDoorsCheckTimer = 0;
			CheckBuildingDoors();
		}

		return superman;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo") && GetZenArtilleryConfig().Zombie_Kill)
		{
			SetHealth(0);
		}
	}

	//! ZOMBIE DOORS 
	protected static ref ZenZombieDoorsPlugin m_ZenDoorPlugin;
	protected ref NoiseParams m_ZenDoorNoiseParams;
	protected ref ZenZombieData m_ZenZombieData;
	protected float m_ZenDoorsCheckTimer;
	protected bool m_ZenDoorHittingPaused;

	void SetupZombieDoors()
	{
		if (!m_ZenDoorPlugin)
			m_ZenDoorPlugin = ZenZombieDoorsPlugin.Cast(GetPlugin(ZenZombieDoorsPlugin));

		m_ZenDoorsCheckTimer = 0;
		m_ZenDoorHittingPaused = false;

		m_ZenZombieData = GetZombieDoorsConfig().GetZombieData(GetType());
		bool canHitDoor = GetZombieDoorsConfig().KnockDownDoors && GetZombieDoorsConfig().DoorHitCount > 0;

		if (m_ZenZombieData)
			canHitDoor = m_ZenZombieData.KnockDownDoors && m_ZenZombieData.DoorHitCount > 0;

		if (canHitDoor && !m_ZenDoorNoiseParams)
			m_ZenDoorNoiseParams = new NoiseParams();

		m_ZenZombieDoorsCache = new array<Object>;
		m_ZenZombieDoorsLastCheckPos = vector.Zero;
	}

	protected void CheckBuildingDoors()
	{
		if (m_ZenDoorHittingPaused)
			return;

		if (vector.Distance(GetPosition(), m_ZenZombieDoorsLastCheckPos) > ZENMOD_BUILDING_SEARCH_RADIUS)
		{
			GetGame().GetObjectsAtPosition(GetPosition(), ZENMOD_BUILDING_SEARCH_RADIUS, m_ZenZombieDoorsCache, NULL);
			m_ZenZombieDoorsLastCheckPos = GetPosition();
		}

		if (m_ZenZombieDoorsCache == NULL || m_ZenZombieDoorsCache.Count() == 0)
			return;

		// Get nearby players
		array<Object> nearbyPlayers = new array<Object>;
		int i;
		for (i = m_ZenZombieDoorsCache.Count(); i >= 0; i--)
		{
			// Check if cache object no longer exists
			if (m_ZenZombieDoorsCache.Get(i) == NULL)
			{
				m_ZenZombieDoorsCache.Remove(i);
				continue;
			}

			if (m_ZenZombieDoorsCache.Get(i).IsMan())
			{
				nearbyPlayers.Insert(m_ZenZombieDoorsCache.Get(i));
			}
		}

		if (!nearbyPlayers || nearbyPlayers.Count() == 0)
			return;

		// Check nearby doors
		for (i = 0; i < m_ZenZombieDoorsCache.Count(); i++)
		{
			Building building = Building.Cast(m_ZenZombieDoorsCache.Get(i));
			if (building != NULL && !building.GetType().Contains("Wreck") && !building.GetType().Contains("wreck"))
			{
				HandleBuildingDoors(building, nearbyPlayers);
				break;
			}
		}
	}

	protected void HandleBuildingDoors(Building building, array<Object> nearbyPlayers)
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

			foreach (Object pb : nearbyPlayers)
			{
				// If zombie is super close to player don't hit door - hit player instead
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
				openedDoor = m_ZenZombieData.KnockDownLockedDoors && m_ZenDoorPlugin.GetDoorHitCount(doorPos) >= m_ZenZombieData.KnockDownLockedDoors;
			else
				openedDoor = GetZombieDoorsConfig().KnockDownLockedDoors && m_ZenDoorPlugin.GetDoorHitCount(doorPos) >= GetZombieDoorsConfig().DoorHitLockedCount;
		}
		else
		{
			if (m_ZenZombieData)
				openedDoor = m_ZenZombieData.KnockDownDoors && m_ZenDoorPlugin.GetDoorHitCount(doorPos) >= m_ZenZombieData.DoorHitCount;
			else
				openedDoor = GetZombieDoorsConfig().KnockDownDoors && m_ZenDoorPlugin.GetDoorHitCount(doorPos) >= GetZombieDoorsConfig().DoorHitCount;
		}

		// Trigger sound and other fx
		TriggerZombieDoorSoundFX(doorPos, building, doorIndex, attackType == 12, openedDoor);
	}

	protected void ZombieOpenedDoor(Building building, int index, vector doorPos)
	{
		if (building.IsDoorLocked(index))
			building.UnlockDoor(index);

		building.OpenDoor(index);
		m_ZenDoorPlugin.RemoveDoorAt(doorPos);
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
		m_ZenDoorPlugin.RemoveDoorAt(doorPos);
	}

	protected void SpawnZombieDoorSoundFX(vector pos)
	{
		// Trigger server-side noise (not a sound, attracts AI like other zombies)
		m_ZenDoorNoiseParams.LoadFromPath("CfgSoundSets Zombie_Hit_Door_SoundSet Noise");
		GetGame().GetNoiseSystem().AddNoiseTarget(pos, 1.0, m_ZenDoorNoiseParams, 1.0);

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
				m_ZenDoorHittingPaused = true;
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ReactivateDoorHitting);
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ReactivateDoorHitting, GetZombieDoorsConfig().HitDoorDelaySecs + Math.RandomFloatInclusive(3000, 6000), false);
			}
		}

		return super.FightLogic(pCurrentCommandID, pInputController, pDt);
	}

	protected void ReactivateDoorHitting()
	{
		m_ZenDoorHittingPaused = false;
	}
#endif
}