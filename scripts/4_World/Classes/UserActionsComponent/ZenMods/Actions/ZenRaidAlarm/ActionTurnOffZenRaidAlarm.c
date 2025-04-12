class ActionTurnOffZenRaidAlarm: ActionInteractBase
{	
	void ActionTurnOffZenRaidAlarm()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#switch_off";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast(targetObject);
		
		if (target_IB.HasEnergyManager() && target_IB.GetCompEM().IsWorking())
			return true;

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(action_data.m_Target.GetObject());
		if (station)
			station.TurnOffAlarm(action_data.m_Player.GetIdentity().GetName());
	}
}