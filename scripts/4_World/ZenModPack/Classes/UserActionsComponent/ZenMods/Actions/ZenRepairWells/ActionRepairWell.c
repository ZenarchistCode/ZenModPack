class ActionZenRepairWellCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		// Make it take longer to repair with a small wrench
		float time_spent = 20;
		if (m_ActionData.m_MainItem.IsInherited(PipeWrench))
			time_spent = 10;

		m_ActionData.m_ActionComponent = new CAContinuousTime(time_spent);
	}
}

class ActionZenRepairWell : ActionContinuousBase
{
	void ActionZenRepairWell()
	{
		m_CallbackClass = ActionZenRepairWellCB;
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
		Well wellObj = Well.Cast(target.GetObject());
		if (!wellObj)
			return false;

		// Server checks
		if (GetGame().IsDedicatedServer())
		{
			if (wellObj.IsZenRepaired())
			{
				// Send a message to the client that the well is already repaired
				player.Zen_SendMessage(GetZenWellsConfig().MessageRepaired);
				return false;
			}
		}

		return !wellObj.IsZenRepaired();
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;

		Well wellObj = Well.Cast(action_data.m_Target.GetObject());
		if (!wellObj)
			return;

		wellObj.SetZenRepaired(true);

		int wellIndex = GetZenWellsDB().GetRepairableWellIndex(wellObj.GetPosition());
		GetZenWellsDB().SetWellRepaired(wellIndex, wellObj.GetPosition(), true, true); // If index == -1 then a new well will be saved.
		action_data.m_MainItem.DecreaseHealth("", "", GetZenWellsConfig().DamageTool);
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