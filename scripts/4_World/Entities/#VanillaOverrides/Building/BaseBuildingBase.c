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

			if (!GetZenUtilitiesConfig().ShouldLogRaids)
				return true;

			// Check ammo config and log any players involved in a raid if the cfg ammo is found on the list.
			bool ammoFound = false;
			foreach(string s : GetZenUtilitiesConfig().RaidAmmo)
			{
				s.ToLower();
				ammo.ToLower();

				if (s.Contains(ammo))
				{
					ammoFound = true;
					break;
				}
			}

			if (!ammoFound)
				return true;

			array<Man> players = new array<Man>;
			g_Game.GetWorld().GetPlayerList(players);
			if (!players || players.Count() <= 0)
				return true;

			foreach(Man m : players)
			{
				if (vector.Distance(m.GetPosition(), GetPosition()) > 250)
					continue;

				PlayerBase pb = PlayerBase.Cast(m);
				if (pb)
				{
					float damage = damageResult.GetDamage(dmgZone, "Health");
					string identity = pb.GetIdentity().GetId() + " / " + pb.GetCachedID();
					ZenModLogger.Log(pb.GetCachedName() + " (" + identity + ") @ " + pb.GetPosition() + " involved in raid @ " + GetPosition() + " Object=" + GetType() + " ammo=" + ammo + " dmg=" + dmgZone + "/" + damage, "baseraid");
				}
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
#ifdef SERVER
		ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));

		if (plugin)
			plugin.AlertNearestRaidStation(GetPosition());
#endif
	}
}