#ifdef SERVER
modded class BaseBuildingBase
{
	//! RAID ALERT - Damage alarm
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
		{
			if (GetZenDiscordConfig().BaseDamageTriggersRaidAlert)
			{
				TriggerZenRaidAlarm();
			}
			
			return true;
		}

		return false;
	}

	//! RAID ALERT - Dismantle alarm
	override void OnPartDismantledServer(notnull Man player, string part_name, int action_id)
	{
		super.OnPartDismantledServer(player, part_name, action_id);

		if (GetZenDiscordConfig().BaseDismantleTriggersRaidAlert)
		{
			PlayerBase pb = PlayerBase.Cast(player);
			if (pb != NULL)
			{
				if (!Zen_RaidAlarmStation.GetTerritoryPermission(pb.GetCachedID(), pb.GetPosition()))
					TriggerZenRaidAlarm();
			}
		}
	}

	protected void TriggerZenRaidAlarm()
	{
		ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
		plugin.AlertNearestRaidStation(GetPosition());
	}
}
#endif