modded class DayZPlayerImplement
{
	//! IMMERSIVE SODAS 
	override void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt)
	{
		SodaCan_ColorBase sodaCan = SodaCan_ColorBase.Cast(GetHumanInventory().GetEntityInHands());
		if (sodaCan && sodaCan.IsOpened())
		{
			// Don't play open sound effect again if soda can has already been opened
			return;
		}

		super.ProcessWeaponEvent(pEventType, pUserString, pUserInt);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo"))
		{
			if (GetZenArtilleryConfig().SafeUnderRoof)
			{
				if (MiscGameplayFunctions.IsUnderRoof(this))
					return false; // Player is safe - don't apply any damage for artillery strike.
			}

			if (!GetZenArtilleryConfig().Player_Kill)
			{
				if (IsUnconscious())
					return false; // Don't pass to super.EEHitBy if uncon and Player_Kill is disabled, or there's a high chance of killing player with explosive dmg
			}
		}

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
}
