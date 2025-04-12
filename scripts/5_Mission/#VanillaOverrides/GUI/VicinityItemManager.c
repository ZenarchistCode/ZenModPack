//! MUSIC
// This is for stopping the player from picking up items outside a car while inside it
modded class VicinityItemManager
{
	override bool ExcludeFromContainer_Phase1(Object actor_in_radius)
	{
		if (ZenModEnabled("ZenMusic"))
		{
			if (Zen_PlayerInCar())
				return true;
		}

		return super.ExcludeFromContainer_Phase1(actor_in_radius);
	}

	override bool ExcludeFromContainer_Phase2(Object object_in_radius)
	{
		if (ZenModEnabled("ZenMusic"))
		{
			if (Zen_PlayerInCar())
				return true;
		}

		return super.ExcludeFromContainer_Phase2(object_in_radius);
	}

	override bool ExcludeFromContainer_Phase3(Object object_in_cone)
	{
		if (ZenModEnabled("ZenMusic"))
		{
			if (Zen_PlayerInCar() && object_in_cone != NULL && !object_in_cone.IsKindOf("CarScript"))
				return true;
		}

		return super.ExcludeFromContainer_Phase3(object_in_cone);
	}

	bool Zen_PlayerInCar()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && player.IsInVehicle())
			return true;

		return false;
	}
}
