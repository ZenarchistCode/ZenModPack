//! ANTI-COMBAT LOG

/*
* This lists all the various firearms and their combat log raycast distance (shorter for pistols & shotties, longer for scoped rifles etc)
*/

// Crossbow 
modded class Archery_Base : Weapon_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		return 150;
	}
}

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

modded class R12_Base
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
			return 1000; // Hunting scope

		return 500;
	}
};

modded class BoltRifle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsHunting"))
			return 1000; // Hunting scope

		return 500;
	}
};

modded class BoltActionRifle_Base
{
	override float GetAntiCombatLogWeaponDistance()
	{
		if (FindAttachmentBySlotName("weaponOpticsHunting"))
			return 1000; // Hunting scope

		return 500;
	}
}
