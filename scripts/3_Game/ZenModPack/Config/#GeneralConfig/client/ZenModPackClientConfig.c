class ZenModPackClientConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenModPackClientConfig.json";
	private const static string CURRENT_VERSION = "1";

	// Change this if we add/remove any variables
	string CONFIG_VERSION = "";

	// Config vars
	int CassetteMusicSetting = 1;
	bool ShowRadioBadge = true;
	bool ImmersiveLogin = true;
	bool BetterChatFont = false;
	bool MagObfuscation = false;

	void Load()
	{
		// Only load on client
		if (GetGame().IsDedicatedServer())
			return;

		string serverPath;
		GetCLIParam("connect", serverPath);
		serverPath.Replace(":", "");
		serverPath.Replace(".", "");

		if (FileExist(zenModFolder + serverPath + "\\" + zenConfigName))
		{
			JsonFileLoader<ZenModPackClientConfig>.JsonLoadFile(zenModFolder + serverPath + "\\" + zenConfigName, this);

			if (CONFIG_VERSION == CURRENT_VERSION)
			{
				// Config exists and version matches.
				Save();
				return;
			}
		}

		CONFIG_VERSION = CURRENT_VERSION;
		Save();
	}

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		string serverPath;
		GetCLIParam("connect", serverPath);
		serverPath.Replace(":", "");
		serverPath.Replace(".", "");

		if (!FileExist(zenModFolder + serverPath))
		{
			MakeDirectory(zenModFolder + serverPath);
		}

		JsonFileLoader<ZenModPackClientConfig>.JsonSaveFile(zenModFolder + serverPath + "\\" + zenConfigName, this);
	}
}

ref ZenModPackClientConfig m_ZenModPackClientConfig;

static ZenModPackClientConfig GetZenModPackClientConfig()
{
	if (!m_ZenModPackClientConfig)
	{
		m_ZenModPackClientConfig = new ZenModPackClientConfig;
		m_ZenModPackClientConfig.Load();
	}

	return m_ZenModPackClientConfig;
};