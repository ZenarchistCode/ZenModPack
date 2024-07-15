class ActionZenSwissKnifeConvert_Knife : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_ZenAction_Knife";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_Knife";
	}
}
