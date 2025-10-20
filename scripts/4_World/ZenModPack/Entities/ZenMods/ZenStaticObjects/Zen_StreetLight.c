class Zen_StreetLight extends Inventory_Base
{
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			Particle.Play(ParticleList.IMPACT_GLASS_EXIT, GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			SEffectManager.PlaySound("offroad_hit_window_small_SoundSet", GetPosition());
		}

		DeleteSafe();
	}
}