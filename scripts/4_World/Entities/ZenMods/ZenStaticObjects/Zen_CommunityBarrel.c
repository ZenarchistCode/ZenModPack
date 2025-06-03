class Zen_CommunityBarrel extends Barrel_ColorBase
{
	void Zen_CommunityBarrel()
	{
		if (GetGame().IsDedicatedServer())
		{
			SetAllowDamage(false);
		}
	}

	override void EEInit()
	{
		super.EEInit();

		SetLifetimeMax(999999);
		SetLifetime(999999);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		return;
	}

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		if (GetGame().IsDedicatedServer())
			SetHealth(GetMaxHealth());
	}
}

class Zen_CommunityBarrel_Holes extends BarrelHoles_ColorBase
{
	void Zen_CommunityBarrel_Holes()
	{
		if (GetGame().IsDedicatedServer())
		{
			SetAllowDamage(false);
		}
	}

	override void EEInit()
	{
		super.EEInit();

		SetLifetimeMax(999999);
		SetLifetime(999999);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		return;
	}

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		if (GetGame().IsDedicatedServer())
			SetHealth(GetMaxHealth());
	}
}

class Zen_CommunityBarrel_DebugPrinter extends Barrel_ColorBase
{
	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsDedicatedServer())
		{
			SetLifetime(3600);
			SetLifetimeMax(3600);
			Print("SpawnObject(\"Zen_CommunityBarrel_Holes\", \"" + GetPosition().ToString(false) + "\", \"" + GetOrientation().ToString(false) + "\", 1);");
		}
	}
}