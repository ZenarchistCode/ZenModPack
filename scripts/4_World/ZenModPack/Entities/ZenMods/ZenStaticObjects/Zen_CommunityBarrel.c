class Zen_CommunityBarrel extends Barrel_ColorBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		if (!GetGame().IsDedicatedServer())
			return;

		if (GetOrientation() == "0 0 0")
		{
			ZenFunctions.AlignToTerrain(this);
		}

		SetAllowDamage(false);
        SetLifetimeMax(999999);
        SetLifetime(999999);
        IncreaseLifetime();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool IsTakeable()
	{
        return false;
	}

	override bool IsRefresherSignalingViable()
	{
		return false;
	}
}

class Zen_CommunityBarrel_Holes extends BarrelHoles_ColorBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		if (!GetGame().IsDedicatedServer())
			return;

		if (GetOrientation() == "0 0 0")
		{
			ZenFunctions.AlignToTerrain(this);
		}

		SetAllowDamage(false);
        SetLifetimeMax(999999);
        SetLifetime(999999);
        IncreaseLifetime();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool IsTakeable()
	{
        return false;
	}

	override bool IsRefresherSignalingViable()
	{
		return false;
	}
}

// For placement with admin tools, on the next server restart the spawn code will be printed to logs to make it easier to spawn all these barrels.
class Zen_CommunityBarrel_DebugPrinter extends Barrel_ColorBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsDedicatedServer())
		{
			SetLifetime(3600); // to avoid them despawning when player leaves area
			SetLifetimeMax(3600);
			Print("SpawnObject(\"Zen_CommunityBarrel_Holes\", \"" + GetPosition().ToString(false) + "\", \"" + GetOrientation().ToString(false) + "\", 1);");
		}
	}
}