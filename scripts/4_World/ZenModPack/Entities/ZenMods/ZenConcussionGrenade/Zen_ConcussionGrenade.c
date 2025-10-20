class Zen_ConcussionGrenade : FlashGrenade
{
	void Zen_ConcussionGrenade()
	{
		SetAmmoType("Zen_ConcussionGrenade_Ammo");
		SetFuseDelay(4);
		SetGrenadeType(EGrenadeType.ILLUMINATING);
		SetParticleExplosion(ParticleList.GRENADE_M84);

		// Load concussion config if it hasn't been loaded already.
		// I put this here so if a server doesn't have this grenade in their types.xml the config is not loaded since it's not needed.
		GetZenConcussionGrenadeConfig();
	}

	override protected void OnExplode()
	{
		super.OnExplode();

		KillZeds();
	}

	void KillZeds()
	{
		if (GetZenConcussionGrenadeConfig().KillZombieRadius <= 0)
			return;

		array<Object> nearbyZombies = new array<Object>;
		GetGame().GetObjectsAtPosition3D(GetPosition(), GetZenConcussionGrenadeConfig().KillZombieRadius, nearbyZombies, NULL);
		EntityAI entity;

		for (int i = 0; i < nearbyZombies.Count(); i++)
		{
			entity = EntityAI.Cast(nearbyZombies.Get(i));
			if (entity && entity.IsZombie())
				entity.SetHealth(0);
		}
	}
};