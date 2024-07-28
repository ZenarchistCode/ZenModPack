class Zen_RaidAlarmRadarKit extends ZenKitBoxBase
{
	protected bool m_WasHologrammed;

    void SetHologrammed(bool hologram)
    {
        m_WasHologrammed = hologram;

        if (m_WasHologrammed)
        {
            string textureAlpha = "#(argb,8,8,3)color(1,1,1,0.1,ca)";
            SetObjectTexture(0, textureAlpha);
        }
        else
        {
            string textureNoAlpha = GetHiddenSelectionsTextures().Get(0);
            SetObjectTexture(0, textureNoAlpha);
        }
    }

	string GetDeployedClassname()
	{
		return "Zen_RaidAlarmRadar";
	}

	vector GetDeployPositionOffset()
	{
		return "0 -0.15 0";
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