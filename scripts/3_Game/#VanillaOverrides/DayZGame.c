modded class DayZGame
{
#ifdef SERVER 
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
	}
#endif

	//! SERVER DIVERSION
	void ZenServerRedirect(UIScriptedMenu menu)
	{
		SetConnecting(true);
		DeleteTitleScreen();
		GetGame().Connect(menu, GetZenServerDiversionConfig().ServerIP, GetZenServerDiversionConfig().ServerPort, GetZenServerDiversionConfig().ServerPass);
	}
}