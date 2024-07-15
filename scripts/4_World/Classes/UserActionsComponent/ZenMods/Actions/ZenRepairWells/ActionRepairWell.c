class ActionRepairWellCB : ActionContinuousBaseCB
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

class ActionRepairWell : ActionContinuousBase
{
	void ActionRepairWell()
	{
		m_CallbackClass = ActionRepairWellCB;
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

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Well well = Well.Cast(target.GetObject());
		if (!well)
			return false;

		// Server checks
		if (GetGame().IsDedicatedServer() && well.IsRepaired())
		{
			// Send a message to the client that the well is already repaired
			player.Zen_SendMessage(GetZenWellsConfig().MessageRepaired);
			return false;
		}

		return !well.IsRepaired();
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;

		Well well = Well.Cast(action_data.m_Target.GetObject());
		if (!well)
			return;

		well.SetRepaired(true);

		int wellIndex = GetZenWellsConfig().GetRepairableWellIndex(well.GetPosition());
		GetZenWellsConfig().SetWellRepaired(wellIndex, well.GetPosition(), true, true); // If index == -1 then a new well will be saved.
		action_data.m_MainItem.DecreaseHealth("", "", GetZenWellsConfig().DamageTool);
	}

	override void OnStartAnimationLoop( ActionData action_data )
	{
		super.OnStartAnimationLoop( action_data );

		if ( !GetGame().IsMultiplayer() || GetGame().IsDedicatedServer() )
		{
			Param2<bool, string> play = new Param2<bool, string>( true, "wrench_loop_SoundSet" );
			GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
		}
	}
	
	override void OnEnd( ActionData action_data )
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsDedicatedServer() )
		{
			Param2<bool, string> play = new Param2<bool, string>( false, "wrench_loop_SoundSet" );
			GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
		}
	}
	
	override void OnEndAnimationLoop( ActionData action_data )
	{
		super.OnEndAnimationLoop( action_data );

		if ( !GetGame().IsMultiplayer() || GetGame().IsDedicatedServer() )
		{
			Param2<bool, string> play = new Param2<bool, string>( false, "wrench_loop_SoundSet" );
			GetGame().RPCSingleParam( action_data.m_MainItem, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true );
		}
	}
};