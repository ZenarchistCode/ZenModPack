class ActionRefuelZenZippoCB : ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.25;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityLiquidTransferZenZippo(UAQuantityConsumed.DRAIN_LIQUID, TIME_TO_REPEAT, false);
	}
};

class ActionRefuelZenZippo: ActionContinuousBase
{
	void ActionRefuelZenZippo()
	{
		m_CallbackClass = ActionRefuelZenZippoCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTY_VESSEL;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_Text = "#fill";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{	
		ZenZippoLighter zippo	= ZenZippoLighter.Cast(target.GetObject());
		Bottle_Base bottle		= Bottle_Base.Cast(item);
		
		if (!zippo || !bottle)
			return false;

		if (zippo.GetQuantity() >= zippo.GetQuantityMax())
			return false; 

		if (bottle.GetQuantity() <= 0.01 || bottle.GetLiquidType() != LIQUID_GASOLINE)
			return false;

		return true;
	}

#ifdef SERVER
	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);

		Bottle_Base vessel_in_hands = Bottle_Base.Cast(action_data.m_Target.GetObject());
		Param1<bool> play = new Param1<bool>(true);
		GetGame().RPCSingleParam(vessel_in_hands, SoundTypeBottle.POURING, play, true);
	}
	
	override void OnEndAnimationLoop(ActionData action_data)
	{
		super.OnEndAnimationLoop(action_data);

		Bottle_Base target_vessel = Bottle_Base.Cast(action_data.m_Target.GetObject());
		Param1<bool> play = new Param1<bool>(false);
		GetGame().RPCSingleParam(target_vessel, SoundTypeBottle.POURING, play, true);
	}
	
	override void OnEnd(ActionData action_data)
	{
		super.OnEnd(action_data);

		Bottle_Base target_vessel = Bottle_Base.Cast(action_data.m_Target.GetObject());
		Param1<bool> play = new Param1<bool>(false);
		GetGame().RPCSingleParam(target_vessel, SoundTypeBottle.POURING, play, true);
	}
#endif
}