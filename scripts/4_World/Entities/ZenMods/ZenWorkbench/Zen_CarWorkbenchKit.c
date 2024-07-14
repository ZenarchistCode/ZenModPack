class Zen_CarWorkbenchKit : DeployableContainer_Base
{
	string GetDeployedClassname()
	{
		return "Zen_CarWorkbench";
	}

	vector GetDeployOrientationOffset()
	{
		return "180 0 0";
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (!GetGame().IsDedicatedServer())
			return;

		PlayerBase pb = PlayerBase.Cast(player);

		if (!pb)
			return;
		
		Zen_CarWorkbench bench = Zen_CarWorkbench.Cast(GetGame().CreateObject(GetDeployedClassname(), pb.GetLocalProjectionPosition(), false));

		if (!bench)
			return;

		SetIsDeploySound(true);
		bench.SetPosition(position);
		bench.SetOrientation(orientation);
		this.DeleteSafe();
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