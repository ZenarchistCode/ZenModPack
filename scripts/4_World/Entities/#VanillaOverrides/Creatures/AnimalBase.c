modded class AnimalBase
{
	static int ZENMOD_ANIMAL_COUNT = 0;

#ifdef SERVER
	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		ZENMOD_ANIMAL_COUNT++;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZENMOD_ANIMAL_COUNT--;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo") && GetGame().IsDedicatedServer() && GetZenArtilleryConfig().Animal_Kill)
		{
			SetHealth(0);
		}
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			if (killer)
			{
				Weapon_Base gun = Weapon_Base.Cast(killer);
				if (gun && gun.GetZenEngravedPlayerName() != "")
				{
					gun.IncreaseZenAnimalKills();
				}
			}
		}		
	}
#endif
}