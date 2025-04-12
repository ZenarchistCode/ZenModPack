class ZenKitBoxBase extends DeployableContainer_Base
{
	override bool ShouldZenHologram()
	{
		return true;
	}

	string GetDeployedClassname()
	{
		return "";
	}

	vector GetDeployPositionOffset()
	{
		return "0 0 0";
	}

	vector GetDeployOrientationOffset()
	{
		return "0 0 0";
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (!GetGame().IsDedicatedServer() || position == vector.Zero)
			return;

		PlayerBase pb = PlayerBase.Cast(player);
		if (!pb)
			return;

		ItemBase deployedItem = ItemBase.Cast(GetGame().CreateObject(GetDeployedClassname(), pb.GetLocalProjectionPosition(), false));
		if (!deployedItem)
		{
			Error("ZenKitBoxBase - failed to deploy classname: " + GetDeployedClassname());
			return;
		}

		SetIsDeploySound(true);
		deployedItem.SetPosition(position);
		deployedItem.SetOrientation(orientation);
		DeleteSafe();
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