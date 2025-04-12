class ActionZenSwissKnifeConvert_SmallSaw : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_ZenAction_Saw";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_SmallSaw";
	}
}
