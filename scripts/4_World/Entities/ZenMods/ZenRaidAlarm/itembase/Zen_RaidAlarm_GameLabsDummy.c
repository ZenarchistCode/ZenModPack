class Zen_RaidAlarm_GameLabsDummy extends BuildingSuper
{
	override bool IsBuilding()
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

#ifdef GAMELABS
	ref _Event _registeredInstanceZenGL;
	ref Timer m_deleteTimer;

	void Zen_RaidAlarm_GameLabsDummy()
	{
		if (GetGame().IsDedicatedServer())
		{
			SetAllowDamage(false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateGameLabsIcon, 1000);
		}
	}

	void CreateGameLabsIcon()
	{
		array<Object> nearObjects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(GetPosition(), 5, nearObjects, NULL);

		foreach (Object nearObj : nearObjects) 
		{
			if (nearObj.IsInherited(Zen_RaidAlarm_GameLabsDummy) && nearObj != this)
			{
				Zen_RaidAlarm_GameLabsDummy dummy = Zen_RaidAlarm_GameLabsDummy.Cast(nearObj);
				dummy.m_deleteTimer.Stop();
				dummy.DeleteSafe();
			}
		}

		m_deleteTimer = new Timer(CALL_CATEGORY_SYSTEM);
		m_deleteTimer.Run(3600, this, "DeleteSafe", NULL, true);

		if (!_registeredInstanceZenGL)
		{
			_registeredInstanceZenGL = new _Event("RAID IN PROGRESS!", "download", this);
			GetGameLabs().RegisterEvent(_registeredInstanceZenGL);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGame().IsDedicatedServer())
			return;

		if (!GetGameLabs())
			return;

		if (_registeredInstanceZenGL)
			GetGameLabs().RemoveEvent(_registeredInstanceZenGL);
	}
#endif
}