modded class VicinityItemManager
{
	override bool ExcludeFromContainer_Phase1(Object actor_in_radius)
	{
		if (ZenModEnabled("ZenGlovebox") || ZenModEnabled("ZenCarCompass") || ZenModEnabled("ZenMusic"))
		{
			if (ZenModPack_PlayerInCar())
				return true;
		}

		return super.ExcludeFromContainer_Phase1(actor_in_radius);
	}

	override bool ExcludeFromContainer_Phase2(Object object_in_radius)
	{
		if (ZenModEnabled("ZenGlovebox") || ZenModEnabled("ZenCarCompass") || ZenModEnabled("ZenMusic"))
		{
			if (ZenModPack_PlayerInCar())
				return true;
		}

		return super.ExcludeFromContainer_Phase2(object_in_radius);
	}

	override bool ExcludeFromContainer_Phase3(Object object_in_cone)
	{
		if (ZenModEnabled("ZenGlovebox") || ZenModEnabled("ZenCarCompass") || ZenModEnabled("ZenMusic"))
		{
			if (ZenModPack_PlayerInCar() && object_in_cone != NULL && !object_in_cone.IsKindOf("CarScript"))
				return true;
		}

		return super.ExcludeFromContainer_Phase3(object_in_cone);
	}

	bool ZenModPack_PlayerInCar()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && player.IsInVehicle())
			return true;

		return false;
	}
}