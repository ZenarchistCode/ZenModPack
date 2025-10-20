modded class DayZGame
{
	//! SERVER DIVERSION
	void ZenServerRedirect(UIScriptedMenu menu)
	{
		SetConnecting(true);
		DeleteTitleScreen();
		GetGame().Connect(menu, GetZenServerDiversionConfig().ServerIP, GetZenServerDiversionConfig().ServerPort, GetZenServerDiversionConfig().ServerPass);
	}
}