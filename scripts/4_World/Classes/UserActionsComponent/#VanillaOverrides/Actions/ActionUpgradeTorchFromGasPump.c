modded class ActionUpgradeTorchFromGasPump: ActionContinuousBase
{
	override bool CanContinue(ActionData action_data)
	{
		if (!ZenModEnabled("ZenRepairPumps"))
			return super.CanContinue(action_data);

		//! REPAIR PUMP
		if (!action_data || !action_data.m_Player || !action_data.m_Target)
			return super.CanContinue(action_data);

		Land_FuelStation_Feed Pump = Land_FuelStation_Feed.Cast(action_data.m_Target.GetObject());
		if (!Pump)
			return super.CanContinue(action_data);

		#ifdef SERVER
		if (!Pump.IsRepaired())
		{
			action_data.m_Player.SetPreventPumpUsage(true);
			action_data.m_Player.Zen_SendMessageDelayed(GetZenPumpsConfig().MessagePumpNotWork, 1);
			return false;
		}

		action_data.m_Player.SetPreventPumpUsage(false);
		#endif

		if (action_data.m_Player.IsPreventedPumpUsage())
		{
			if (GetGame().IsClient())
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetPumpPrevention, 1000, false, action_data.m_Player);

			return false;
		}

		return super.CanContinue(action_data);
	}

	void ResetPumpPrevention(PlayerBase player)
	{
		if (player)
			player.SetPreventPumpUsage(false);
	}
};