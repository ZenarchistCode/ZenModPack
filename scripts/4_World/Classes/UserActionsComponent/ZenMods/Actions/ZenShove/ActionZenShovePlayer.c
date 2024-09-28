class ActionZenShovePlayer : ActionInteractBase
{
	void ActionZenShovePlayer()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTDummy();
	}

	override string GetText()
	{
		return "#STR_ZenGui_Shove";
	}

	override int Zen_GetDisplayPriority()
	{
		return 1;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		if(!ntarget || !ntarget.IsAlive())
			return false;

		return ntarget.IsRestrained();
	}

	override void OnStartServer(ActionData action_data)
	{
		PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (!targetPlayer)
			return;

		// Force erect ;)
		HumanCommandMove hcm = targetPlayer.GetCommand_Move();
		if (hcm)
		{
			hcm.ForceStance(DayZPlayerConstants.STANCEIDX_ERECT);
		}

		// Force direction
		targetPlayer.SetDirection(action_data.m_Player.GetDirection());

		// Apply stun hit (no health damage)
		targetPlayer.Zen_StunFromAngle(action_data.m_Player.GetPosition());
	}
};