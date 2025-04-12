modded class Screwdriver
{
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionBuildZenRaidAlarmStation);
		AddAction(ActionDismantleZenRaidAlarmStation);
		AddAction(ActionDismantleZenRaidAlarmRadar);
	}
}