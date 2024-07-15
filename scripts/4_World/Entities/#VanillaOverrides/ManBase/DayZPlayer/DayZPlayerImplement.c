modded class DayZPlayerImplement
{
	//! BROKEN GLASSES - TODO: Find a better way to hijack optics. Surely there's a way to slide into the PPE Effects's DMs?
	static ref array<string> DAMAGED_SCOPES = 
	{
		"HuntingOptic",
		"Binoculars",
		"KazuarOptic",
		"LongRangeOptic",
		"Expansion_PMII25Optic",
		"AD_LeupoldMk4",
		"AD_Pilad",
		"AD_DHF5",
		"AD_NFATACR",
		"AD_NFATACR_1_8",
		"AD_PVS4"
	};

	override void SetOptics(bool value)
	{
		super.SetOptics(value);

		#ifndef SERVER
		if (!value)
		{
			ZenBrokenGlassesWidget.GetZenBrokenGlassesWidget().HideFX_Optics();
		}
		#endif
	}

	override void HandleOptic(notnull ItemOptics optic, bool inHands, HumanInputController pInputs, out bool pExitOptics)
	{
		super.HandleOptic(optic, inHands, pInputs, pExitOptics);

		#ifndef SERVER
		if (!IsInOptics())
			return;

		// Only show on relevant optics that don't already have vanilla damage effects
		bool damagedScope = false;
		foreach (string s : DAMAGED_SCOPES)
		{
			if (optic.GetType().Contains(s) || optic.IsKindOf(s))
			{
				damagedScope = true;
				break;
			}
		}

		if (!damagedScope)
			return;

		ZenBrokenGlassesWidget.GetZenBrokenGlassesWidget().ShowFX_Optics(optic.GetHealthLevel());
		#endif
	}

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
