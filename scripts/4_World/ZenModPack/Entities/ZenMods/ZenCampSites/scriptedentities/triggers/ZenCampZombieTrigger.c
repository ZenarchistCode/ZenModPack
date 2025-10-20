class ZenCampZombieTrigger extends ManTrigger
{
	int m_zombieCount;

	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);

		// ManTrigger can only be triggered by PlayerBase so no need to check if insider is player, just spawn zeds.
		SpawnRandomZombies();
	}

	void SpawnRandomZombies()
	{
		vector spawnPos;
		for (int i = 0; i < m_zombieCount; i++)
		{
			string zedType = GetZenCampSitesConfig().ZombieTypes.GetRandomElement();
			spawnPos = ZenFunctions.GetRandomPointInCircle(GetPosition(), 1, 6);
			ZombieBase zed = ZombieBase.Cast(GetGame().CreateObjectEx(zedType, spawnPos, ECE_PLACE_ON_SURFACE|ECE_INITAI|ECE_EQUIP_ATTACHMENTS));
			if (zed)
			{
				zed.SetLifetimeMax(GetZenCampSitesConfig().SpawnSecs);
				zed.SetLifetime(GetZenCampSitesConfig().SpawnSecs);
			}
			else
			{
				Error("ZenCampSites :: Couldn't spawn zombie type: " + zedType);
			}
		}

		// Delete this trigger
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteSafe, 1, false);
	}
}