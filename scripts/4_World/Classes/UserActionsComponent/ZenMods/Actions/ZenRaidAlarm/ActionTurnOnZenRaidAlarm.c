class ActionTurnOnZenRaidAlarm: ActionInteractBase
{	
	void ActionTurnOnZenRaidAlarm()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#switch_on";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(target.GetObject());

		if (station)
		{
			if (station.HasWorkingBaseRadio() && !station.GetCompEM().IsWorking())
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
			return;

		if (station.GetCompEM().CanWork())
		{
			station.TurnOnAlarm(action_data.m_Player.GetIdentity().GetName());

			if (!station.HasValidWebhook())
			{
				station.OpenRaidAlarmGUI(action_data.m_Player.GetIdentity());
			}
		}
	}
}