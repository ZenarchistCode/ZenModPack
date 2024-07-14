class ActionZenSwissKnifeConvert_Scissors : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_CfgVehicles_SewingKit0";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_Scissors";
	}
}
