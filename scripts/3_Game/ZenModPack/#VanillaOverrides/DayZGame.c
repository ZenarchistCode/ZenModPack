modded class DayZGame
{
	//! SERVER DIVERSION
	void ZenServerRedirect(UIScriptedMenu menu)
	{
		SetConnecting(true);
		DeleteTitleScreen();
		GetGame().Connect(menu, GetZenServerDiversionConfig().ServerIP, GetZenServerDiversionConfig().ServerPort, GetZenServerDiversionConfig().ServerPass);
	}

	override bool GetSurfaceDigPile(string surface, out string result)
	{
        bool superCheck = super.GetSurfaceDigPile(surface, result);

        if (ZenModEnabled("ZenAutoConvertStash") && ZenGameFunctions.IsWinter())
        {
            result = "UndergroundStashSnow";
            return true;
        }

		return superCheck;
	}
}