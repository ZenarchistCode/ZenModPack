modded class ActionWashHandsWell
{
	override bool CanContinue(ActionData action_data)
	{
		if (!ZenModEnabled("ZenRepairWells"))
			return super.CanContinue(action_data);

		if (!action_data || !action_data.m_Player)
			return false;

		//! REPAIR WELLS
		if (action_data.m_Player.IsPreventedWellUsage())
			return false;

		return super.CanContinue(action_data);
	}

	override void OnStartServer(ActionData action_data)
	{
		if (!ZenModEnabled("ZenRepairWells"))
		{
			super.OnStartServer(action_data);
			return;
		}

		if (!action_data || !action_data.m_Player)
			return;

		//! REPAIR WELLS
		Well well = Well.Cast(action_data.m_Target.GetObject());
		if (!well)
		{
			action_data.m_Player.SetPreventWellUsage(false);
			return;
		}

		if (!well.IsRepaired())
		{
			action_data.m_Player.SetPreventWellUsage(true);
			action_data.m_Player.Zen_SendMessageDelayed(GetZenWellsConfig().MessageNotDrink, 3000);
		}
		else
		{
			action_data.m_Player.SetPreventWellUsage(false);
		}

		super.OnStartServer(action_data);
	}
};
