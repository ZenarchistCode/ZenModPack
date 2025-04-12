modded class MissionMainMenu 
{
	//! SERVER DIVERSION
	static bool m_ServerDiverted = false;

	override void OnMissionStart()
	{
		super.OnMissionStart();

		if (!m_ServerDiverted && GetZenServerDiversionConfig().ServerIP != "")
		{
			string currentServerIP;
			int port;
			GetGame().GetHostAddress(currentServerIP, port);

			if (currentServerIP != GetZenServerDiversionConfig().ServerIP)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(DoServerReconnect);
			}
		}
	}

	void DoServerReconnect() 
	{ 
		m_ServerDiverted = true;
		GetDayZGame().ZenServerRedirect(m_mainmenu);
	}
}