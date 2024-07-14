class Zen_SpotlightObject extends Inventory_Base
{
	void Zen_SpotlightObject()
	{
		SetPilotLight(true);
		SetIsHologram(true);
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		SetPilotLight(false);
		SetIsHologram(false);

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, this.GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			Particle.Play(ParticleList.IMPACT_GLASS_EXIT, this.GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			SEffectManager.PlaySound("offroad_hit_window_small_SoundSet", this.GetPosition());
		}

		this.Delete();
	}
}