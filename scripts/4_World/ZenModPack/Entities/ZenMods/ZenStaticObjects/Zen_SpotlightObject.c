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

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			Particle.Play(ParticleList.IMPACT_METAL_RICOCHET, GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			Particle.Play(ParticleList.IMPACT_GLASS_EXIT, GetPosition() + Vector(0, -0.3, 0)).SetOrientation("0.0 180.0 0.0");
			SEffectManager.PlaySound("offroad_hit_window_small_SoundSet", GetPosition());
		}

		DeleteSafe();
	}

	override bool DisableVicinityIcon()
	{
		return true;
	}

	override int GetHideIconMask()
	{
		return EInventoryIconVisibility.HIDE_VICINITY;
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

	override bool CanBeActionTarget()
	{
        return false;
    }
}