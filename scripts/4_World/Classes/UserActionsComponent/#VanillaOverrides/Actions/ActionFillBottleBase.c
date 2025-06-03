modded class ActionFillBottleBase
{
	override bool CanContinue(ActionData action_data)
	{
		//! REPAIR WELLS
		Well well = Well.Cast(action_data.m_Target.GetObject());
		if (ZenModEnabled("ZenRepairWells") && well != NULL && action_data.m_Player.IsPreventedWellUsage())
			return false;

		//! REPAIR PUMPS
		Land_FuelStation_Feed pump = Land_FuelStation_Feed.Cast(action_data.m_Target.GetObject());
		if (!pump)
			return super.CanContinue(action_data);

		#ifdef SERVER
		if (ZenModEnabled("ZenRepairPumps") && !pump.IsRepaired())
		{
			action_data.m_Player.SetPreventPumpUsage(true);
			action_data.m_Player.Zen_SendMessageDelayed(GetZenPumpsConfig().MessagePumpNotWork, 1);
			return false;
		}

		action_data.m_Player.SetPreventPumpUsage(false);
		#endif

		if (ZenModEnabled("ZenRepairPumps") && action_data.m_Player.IsPreventedPumpUsage())
		{
			if (GetGame().IsClient())
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetPumpPrevention, 1000, false, action_data.m_Player);

			return false;
		}

		return super.CanContinue(action_data);
	}

	//! REPAIR PUMPS
	void ResetPumpPrevention(PlayerBase player)
	{
		if (player)
			player.SetPreventPumpUsage(false);
	}

	//! REPAIR WELLS 
	override void OnStartServer(ActionData action_data)
	{
		if (!ZenModEnabled("ZenRepairWells"))
		{
			super.OnStartServer(action_data);
			return;
		}

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
}