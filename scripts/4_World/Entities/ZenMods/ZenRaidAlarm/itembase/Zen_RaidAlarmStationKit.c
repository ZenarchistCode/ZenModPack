class Zen_RaidAlarmStationKit extends DeployableContainer_Base
{
	string GetDeployedClassname()
	{
		return "Zen_RaidAlarmStation";
	}

	vector GetDeployOrientationOffset()
	{
		return "0 0 0";
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (!GetGame().IsDedicatedServer())
			return;

		PlayerBase pb = PlayerBase.Cast(player);
		if (!pb)
			return;
		
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(GetGame().CreateObject(GetDeployedClassname(), pb.GetLocalProjectionPosition(), false));
		if (!station)
			return;

		SetIsDeploySound(true);
		station.SetPosition(position);
		station.SetOrientation(orientation);
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
};