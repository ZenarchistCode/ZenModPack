class ZenServerDiversionConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "ZenServerDiversionConfig.json";

	string ServerIP = "";
	int ServerPort = -1;
	string ServerPass = "";
	string RedirectMessage = "";

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenServerDiversionConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			RedirectMessage.Replace("%ip", ServerIP);
			RedirectMessage.Replace("%port", ServerPort.ToString());
			return;
		}

		RedirectMessage = "The server has moved to a new IP address (%ip:%port) - you will be automatically redirected shortly. Please update your server bookmarks in the launcher.";
		Save();
	}

	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist("$profile:\\Zenarchist"))
		{
			MakeDirectory(zenModFolder);
		}

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenServerDiversionConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenServerDiversionConfig m_ZenServerDiversionConfig;

static ZenServerDiversionConfig GetZenServerDiversionConfig()
{
	if (!m_ZenServerDiversionConfig)
	{
		Print("[ZenServerDiversionConfig] Init");
		m_ZenServerDiversionConfig = new ZenServerDiversionConfig;
		m_ZenServerDiversionConfig.Load();
	}

	return m_ZenServerDiversionConfig;
}