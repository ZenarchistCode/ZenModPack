class ActionZenSwissKnifeConvert_CanOpener : ActionZenSwissKnife_ConvertBase
{
	override string GetText()
	{
		return super.GetText() + "#STR_CfgVehicles_CanOpener0";
	}

	override string GetConvertToolType()
	{
		return "ZenSwissKnife_CanOpener";
	}
}
