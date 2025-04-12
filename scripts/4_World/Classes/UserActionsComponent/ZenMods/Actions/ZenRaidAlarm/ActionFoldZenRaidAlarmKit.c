class ActionFoldZenRaidAlarmKit: ActionInteractBase
{	
	void ActionFoldZenRaidAlarmKit()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#fold";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(target.GetObject());

		if (station)
		{
			if (station.GetCompEM().IsWorking() || station.GetBaseRadio() != NULL || station.GetMegaphone() != NULL)
			{
				return false;
			}
		}
		
		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(action_data.m_Target.GetObject());
		if (station)
		{
			if (station.GetCompEM().IsWorking() || station.GetBaseRadio() != NULL || station.GetMegaphone() != NULL)
				return;

			EntityAI newItem = EntityAI.Cast(GetGame().CreateObjectEx("Zen_RaidAlarmStationKit", station.GetPosition(), ECE_PLACE_ON_SURFACE));
			newItem.SetHealth01("", "", station.GetHealth01("", ""));
			station.DeleteSafe();
		}
	}
}