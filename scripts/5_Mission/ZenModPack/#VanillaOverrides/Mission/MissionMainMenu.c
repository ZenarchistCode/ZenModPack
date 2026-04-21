modded class MissionMainMenu 
{
	//! SERVER DIVERSION
	static bool m_ServerDiverted = false;

	override void OnMissionStart()
	{
		super.OnMissionStart();

		if (!m_ServerDiverted && GetZenServerDiversionConfig() && GetZenServerDiversionConfig().ServerIP != "")
		{
			string currentServerIP;
			int port;
			g_Game.GetHostAddress(currentServerIP, port);

			if (currentServerIP != GetZenServerDiversionConfig().ServerIP)
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DoServerReconnect);
			}
		}
	}

	void DoServerReconnect() 
	{ 
		m_ServerDiverted = true;
		g_Game.ZenServerRedirect(m_mainmenu);
	}
}