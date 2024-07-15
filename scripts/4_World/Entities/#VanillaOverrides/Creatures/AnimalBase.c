modded class AnimalBase
{
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo") && GetGame().IsDedicatedServer() && GetZenArtilleryConfig().Animal_Kill)
		{
			SetHealth(0);
		}
	}
}