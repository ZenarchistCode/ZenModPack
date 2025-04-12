modded class Hit_MeatBones 
{
	//! BLOOD SPLATTER
	override void BloodSplatWall()
	{
		// BI:	Commented out due to age rating process :(
		// Zen: I ain't got no age rating ;)
		if (m_OutSpeed.Length() > 0)
		{
			Object projected_surface;
			vector hit_pos;
			vector hit_normal;
			float hit_fraction; 
			DayZPhysics.RayCastBullet(m_ExitPos, m_ExitPos + m_OutSpeed, PhxInteractionLayers.BUILDING, m_Object, projected_surface, hit_pos, hit_normal, hit_fraction);
			
			hit_normal = hit_normal.VectorToAngles();
			hit_normal[1] = hit_normal[1] + 90;
			
			EffectParticle eff = new BloodSplatter();
			eff.SetAutodestroy(true);
			SEffectManager.PlayInWorld(eff, hit_pos);
			Particle blood = eff.GetParticle();
			blood.SetOrientation(hit_normal);
		}
	}
}