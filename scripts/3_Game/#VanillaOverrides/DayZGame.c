modded class DayZGame
{
	//! TREESPLOSIONS
	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType)
	{
		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

		#ifdef SERVER
		//! TREESPLOSIONS
		if (ZenModEnabled("ZenTreesplosions"))
		{
			if (ammoType == "Bullet_40mm_Explosive" || ammoType == "ExpansionRocket")
				GetGame().CreateObjectEx("Zen40mmTreeKiller", pos, ECE_PLACE_ON_SURFACE);
		}

		//! ANTI-COMBAT LOG
		if (ZenModEnabled("ZenAntiCombatLogout"))
		{
			if ((ammoType == "Bullet_40mm_Explosive" || ammoType == "AType_Bullet_40mm_ChemGas" || ammoType == "ExpansionRocket") && GetZenAntiCombatLogoutConfig().TriggerOnExplosiveRadius > 0)
				GetGame().CreateObjectEx("Zen_CombatLogExplosiveTrigger", pos, ECE_PLACE_ON_SURFACE);
			else
			if (GetZenAntiCombatLogoutConfig().TriggerOnBulletImpactRadius > 0)
				GetGame().CreateObjectEx("Zen_CombatLogTrigger", pos, ECE_PLACE_ON_SURFACE);
		}
		#endif
	}
};