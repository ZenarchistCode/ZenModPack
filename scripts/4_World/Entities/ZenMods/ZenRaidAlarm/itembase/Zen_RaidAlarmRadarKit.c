class Zen_RaidAlarmRadarKit extends ZenKitBoxBase
{
	override string GetDeployedClassname()
	{
		return "Zen_RaidAlarmRadar";
	}

	override vector GetDeployPositionOffset()
	{
		return "0 -0.15 0";
	}

	override vector GetDeployOrientationOffset()
	{
		return "0 0 0";
	}

	override bool IsBasebuildingKit()
	{
		return true;
	}

	override bool IsDeployable()
	{
		return true;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
}