//! ANTI-COMBAT LOG

/*
* This lists all the various firearms and their combat log raycast distance (shorter for pistols & shotties, longer for scoped rifles etc)
*/

// Shotguns
modded class Izh43Shotgun_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
};

modded class Izh18Shotgun_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
};

modded class Mp133Shotgun_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
};

modded class Remington12
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
};

modded class Saiga_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
};


// Pistols
modded class Pistol_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 500;
	}
};

modded class Derringer_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 500;
	}
};

modded class Magnum_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 500;
	}
};

modded class LongHorn_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsCrossbow"))
			return 800; // Pistol scope

		return 500;
	}
};

modded class Deagle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsCrossbow"))
			return 800; // Pistol scope

		return 500;
	}
};

// Grenade launcher
modded class M79_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 500;
	}
};

// Rifles
modded class Rifle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsHunting"))
			return 2000; // Hunting scope

		return 1000;
	}
};

modded class BoltRifle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsHunting"))
			return 2000; // Hunting scope

		return 1000;
	}
};

modded class BoltActionRifle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsHunting"))
			return 2000; // Hunting scope

		return 1000;
	}
}
