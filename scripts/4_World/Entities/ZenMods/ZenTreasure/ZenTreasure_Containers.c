class ZenTreasure_WoodenCrate extends WoodenCrate
{
	void ZenTreasure_WoodenCrate()
	{
		if (GetGame().IsDedicatedServer())
		{
			SetHealth(1);
			SetAllowDamage(false);
		}
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return parent && parent.IsKindOf("UndergroundStash");
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanBeDigged()
	{
		return false;
	}
};

class ZenTreasure_SeaChest extends SeaChest
{
	void ZenTreasure_SeaChest()
	{
		if (GetGame().IsDedicatedServer())
		{
			SetHealth(1);
			SetAllowDamage(false);
		}
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return parent && parent.IsKindOf("UndergroundStash");
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanBeDigged()
	{
		return false;
	}
};