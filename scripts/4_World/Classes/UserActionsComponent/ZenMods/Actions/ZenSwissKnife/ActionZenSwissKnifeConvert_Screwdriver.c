class ActionZenSwissKnifeConvert_Screwdriver : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_CfgVehicles_Screwdriver0";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_Screwdriver";
	}
}
