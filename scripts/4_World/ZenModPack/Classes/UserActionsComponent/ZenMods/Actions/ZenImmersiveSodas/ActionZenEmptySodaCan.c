class ActionZenEmptySodaCanCB : ActionContinuousBaseCB
{
	bool m_RPCStopAlreadySent;//since stopping contains a sound tail, we need to stop it only once, this bool ensures that
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousEmpty(200);
	}
};

class ActionZenEmptySodaCan: ActionContinuousBase
{
	void ActionZenEmptySodaCan()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTY_VESSEL;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CallbackClass = ActionZenEmptySodaCanCB;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#empty";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (item.IsLiquidPresent())
		{
			return true;
		}

		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		if (action_data.m_MainItem.GetQuantity() > action_data.m_MainItem.GetQuantityMin())
		{
			return true;
		}

		return false;
	}

	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);

		SodaCan_ColorBase sodaCan = SodaCan_ColorBase.Cast(action_data.m_MainItem);
		if (!sodaCan || sodaCan.IsRuined())
			return;

		// Convert vanilla soda can to my open soda can texture
		sodaCan.SetOpenedTexture();
	}
	
	override void OnStartAnimationLoop( ActionData action_data )
	{
		SendRPC(action_data,true);
	}
	
	override void OnEndAnimationLoop( ActionData action_data )
	{
		SendRPC(action_data,false);
	}
	
	override void OnEndServer( ActionData action_data )
	{
		SendRPC(action_data,false);

		if (action_data.m_MainItem.GetQuantity() <= 0.01)
		{
			action_data.m_Player.GetHumanInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ReplaceJunkLambda(action_data.m_MainItem, "Empty_" + action_data.m_MainItem.GetType(), action_data.m_MainItem.GetHealth()));
		}
	}

	protected void SendRPC(ActionData actionData, bool enable)
	{
#ifdef SERVER
		ActionZenEmptySodaCanCB comp = ActionZenEmptySodaCanCB.Cast(actionData.m_Callback);
		if (comp.m_RPCStopAlreadySent)
			return;
			
		SodaCan_ColorBase target_vessel = SodaCan_ColorBase.Cast(actionData.m_MainItem);
		Param1<bool> play = new Param1<bool>(enable);
		GetGame().RPCSingleParam(target_vessel, SoundTypeBottle.EMPTYING, play, true);
		if (!enable)
			comp.m_RPCStopAlreadySent = true;
#endif
	}
}