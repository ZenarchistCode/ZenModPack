class ActionViewRaidAlarmGUI: ActionInteractBase
{	
	void ActionViewRaidAlarmGUI()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#STR_ZenGui_RaidAlarmViewSettings";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(target.GetObject());
		if (station)
		{
			if (station.HasEnergyManager() && station.GetCompEM().IsWorking() && station.GetAlarmStatus() == Zen_RaidAlarmStation.ZEN_ALARM_STATUS_ARMED)
			{
				return true;
			}
		}
		
		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(action_data.m_Target.GetObject());
		if (!station)
		{
			Error("[ZenRaidAlarm] Player tried to operate raid alarm, but server found object to be NULL.");
			return;
		}

		if (!station.GetTerritoryPermission(action_data.m_Player.GetCachedID(), station.GetPosition()))
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(action_data.m_Player.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, GetZenDiscordConfig().RaidAlarmRequiresTerritoryText, "set:ccgui_enforce image:MapShieldBooster");
			return;
		}

		if (station.GetCompEM().IsWorking())
		{
			station.OpenRaidAlarmGUI(action_data.m_Player.GetIdentity());
		}
	}
}