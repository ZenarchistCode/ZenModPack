modded class ZombieBase
{
	static int ZENMOD_ZOMBIE_COUNT = 0;

#ifdef SERVER
	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		ZENMOD_ZOMBIE_COUNT++;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZENMOD_ZOMBIE_COUNT--;
	}

	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			Weapon_Base gun = Weapon_Base.Cast(killer);
			if (gun && gun.GetZenEngravedPlayerName() != "")
			{
				gun.IncreaseZenZombieKills();
			}
		}		
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (ZenWasAlive() && source && source.IsWeapon())
		{
			Weapon_Base weapon = Weapon_Base.Cast(source);
			if (weapon && weapon.IsZenEngraved())
			{
				weapon.UpdateZenLongestShot(vector.Distance(source.GetPosition(), GetPosition()));
			}
		}
	}
#endif
}