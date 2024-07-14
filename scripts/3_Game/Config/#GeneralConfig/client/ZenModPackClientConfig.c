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

	void Load()
	{
		// Only load on client
		if (GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenModPackClientConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				// Mismatch, back up old version.
				JsonFileLoader<ZenModPackClientConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches.
				return;
			}
		}

		CONFIG_VERSION = CURRENT_VERSION;
		Save();
	};

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenModPackClientConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
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