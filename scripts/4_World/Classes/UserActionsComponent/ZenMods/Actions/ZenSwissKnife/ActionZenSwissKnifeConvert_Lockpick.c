class ActionZenSwissKnifeConvert_Lockpick : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_CfgVehicles_Lockpick0";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_Lockpick";
	}
}
