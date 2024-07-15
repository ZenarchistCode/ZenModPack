// Extend grenade chemgas to create chemical cloud upon explosion
class Zen_ArtilleryBombChem extends Grenade_ChemGas
{
	void Zen_ArtilleryBombChem()
	{
		SetAmmoType("Zen_ArtilleryBomb_Ammo");
		SetFuseDelay(1);
	}

	override void Explode(int damageType, string ammoType = "")
	{
		super.Explode(damageType, ammoType);

		if (!GetGame().IsDedicatedServer())
			return;

		SynchExplosion();
		DamageSystem.ExplosionDamage(this, NULL, ammoType, GetPosition() + "0 0.1 0", damageType);
		DeleteThis();

		// Damage nearby objects defined in config
		Zen_ArtilleryBomb.DamageNearbyObjects(GetPosition());
	}

	override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		if (GetGame().IsDedicatedServer() || !GetGame().IsClient())
			return;

		vector n = surfNormal.VectorToAngles() + "0 90 0";

		Particle p2 = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
		p2.SetOrientation(n);

		Particle p3 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, pos);
		p3.SetOrientation(n);

		Particle p4 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);
		p4.SetOrientation(n);

		float dist = vector.Distance(pos, GetGame().GetPlayer().GetPosition());

		if (dist < 50)
			GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(Math.InverseLerp(100, 0, dist) * 2);
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		Explode(DT_EXPLOSION, "Zen_ArtilleryBomb_Ammo");

		if (GetGame().IsDedicatedServer())
			SynchExplosion();
	}

	void DeleteThis()
	{
		m_DeleteTimer = new Timer(CALL_CATEGORY_SYSTEM);
		m_DeleteTimer.Run(30, this, "DeleteSafe");
	}
}