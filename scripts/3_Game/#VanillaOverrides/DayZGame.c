static int GetZenServerFPS()
{
	return DayZGame.m_ZenServerFPS;
}

static int GetZenAvgServerFPS()
{
	if (DayZGame.m_ZenTotalServerFPS == 0 || DayZGame.m_ZenTotalServerFPSCounter == 0)
		return 0;

	return DayZGame.m_ZenTotalServerFPS / DayZGame.m_ZenTotalServerFPSCounter;
}

modded class DayZGame
{
	//! UTILITIES - PERFORMANCE MONITORING
	private static bool ZEN_MONITOR_SERVER_FPS = -1;
	private static float m_ZenDeltaTime = 0; // Delta time, tracks update tick time
	private static int m_ZenFrames = 0; // Current server frames this second
	
	static int m_ZenServerFPS = 0;
	static int m_ZenTotalServerFPSCounter = 0;
	static int m_ZenTotalServerFPS = 0;

#ifdef SERVER 
	// Prepare required variables
	void DayZGame()
	{
		ZEN_MONITOR_SERVER_FPS = GetZenUtilitiesConfig().CountServerFPS;

		if (ZEN_MONITOR_SERVER_FPS)
		{
			Print("[ZenModPack::DayZGame] Monitoring server FPS.");
		}
	}

	// Calculate server FPS
    override void OnUpdate(bool doSim, float timeslice) 
	{
        super.OnUpdate(doSim, timeslice);

		if (!ZEN_MONITOR_SERVER_FPS)
			return;

		m_ZenDeltaTime += timeslice;
		m_ZenFrames++;

		if (m_ZenDeltaTime >= 1)
		{
			m_ZenServerFPS = m_ZenFrames;
			m_ZenTotalServerFPSCounter++;
			m_ZenTotalServerFPS += m_ZenServerFPS;
			m_ZenFrames = 0;
			m_ZenDeltaTime = 0;
		}
    }

	//! TREESPLOSIONS
	override void FirearmEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType)
	{
		super.FirearmEffects(source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType);

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
	}
#endif
}