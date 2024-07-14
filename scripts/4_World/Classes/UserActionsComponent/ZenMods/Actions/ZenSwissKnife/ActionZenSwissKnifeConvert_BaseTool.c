class ActionZenSwissKnifeConvert_BaseTool : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return "#STR_ZenAction_Retract";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return true;
	}
}
