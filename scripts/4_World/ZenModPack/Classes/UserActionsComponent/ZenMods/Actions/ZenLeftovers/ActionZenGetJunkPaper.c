class ActionZenGetJunkPaperCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.UNPACK);
	}
};

class ActionZenGetJunkPaper : ActionContinuousBase
{
	void ActionZenGetJunkPaper()
	{
		m_CallbackClass = ActionZenGetJunkPaperCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENITEM;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_OPENITEM;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasProneException()
	{
		return true;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#craft #STR_CfgVehicles_Paper0";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return true; // Set to false to disbale getting paper from junk items
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data.m_MainItem && action_data.m_MainItem.GetHierarchyRootPlayer() == action_data.m_Player)
		{
			UnboxLambda lambda = new UnboxLambda(action_data.m_MainItem, "Paper", action_data.m_Player, 1);
			action_data.m_Player.ServerReplaceItemInHandsWithNew(lambda);
			action_data.m_Player.GetSoftSkillsManager().AddSpecialty(m_SpecialtyWeight);
		}
	}
};