class ActionRepairPumpCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		// Make it take longer to repair with a small wrench
		float time_spent = 18;
		if (m_ActionData.m_MainItem.GetType() == "PipeWrench")
			time_spent = 10;

		m_ActionData.m_ActionComponent = new CAContinuousTime(time_spent);
	}
};

class ActionRepairPump : ActionContinuousBase
{
	void ActionRepairPump()
	{
		m_CallbackClass = ActionRepairPumpCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
	}

	override string GetText()
	{
		return "#repair";
	}

	override bool IsLockTargetOnUse()
	{
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenRepairPumps"))
			return false;

		Land_FuelStation_Feed Pump = Land_FuelStation_Feed.Cast(target.GetObject());
		if (!Pump)
			return false;

		// Server checks
		#ifdef SERVER
		if (Pump.IsRepaired())
		{
			// Send a message to the client that the Pump is already repaired
			player.Zen_SendMessage(GetZenPumpsConfig().MessageRepaired);
			return false;
		}
		#endif

		return !Pump.IsRepaired();
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;

		Land_FuelStation_Feed Pump = Land_FuelStation_Feed.Cast(action_data.m_Target.GetObject());
		if (!Pump)
			return;

		Pump.SetRepaired(true);

		int PumpIndex = GetZenPumpsConfig().GetRepairablePumpIndex(Pump.GetPosition());
		GetZenPumpsConfig().SetPumpRepaired(PumpIndex, Pump.GetPosition(), true, true); // If index == -1 then a new Pump will be saved.
		action_data.m_MainItem.DecreaseHealth("", "", GetZenPumpsConfig().DamageTool);
	}

#ifdef SERVER
	override void OnStartAnimationLoop( ActionData action_data )
	{
		super.OnStartAnimationLoop( action_data );

		Param2<bool, string> play = new Param2<bool, string>( true, "wrench_loop_SoundSet" );
		GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
	}
	
	override void OnEnd( ActionData action_data )
	{
		super.OnEnd(action_data);

		Param2<bool, string> play = new Param2<bool, string>( false, "wrench_loop_SoundSet" );
		GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
	}
	
	override void OnEndAnimationLoop( ActionData action_data )
	{
		super.OnEndAnimationLoop( action_data );

		Param2<bool, string> play = new Param2<bool, string>( false, "wrench_loop_SoundSet" );
		GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
	}
#endif
}