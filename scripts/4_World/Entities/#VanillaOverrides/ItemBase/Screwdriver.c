modded class Screwdriver
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionCreateGreenhouseGardenPlotZenKnife);
		AddAction(ActionBuildZenRaidAlarmStation);
		AddAction(ActionDismantleZenRaidAlarmStation);
		AddAction(ActionDismantleZenRaidAlarmRadar);
	}
}