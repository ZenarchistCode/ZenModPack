class ZenNightConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenNightConfig.json";

	float OvercastToTriggerDarkNights = 0.0;
	bool UpdateOvercastLightingAnyTime = false;
	float ChanceOfDarkNight = 0.4;
	int BrightNightLightingConfigID = 0;
	int DarkNightLightingConfigID = 1;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenNightConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

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

		JsonFileLoader<ZenNightConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenNightConfig m_ZenNightConfig;

static ZenNightConfig GetZenNightConfig()
{
	if (!m_ZenNightConfig)
	{
		ZMPrint("[ZenNightConfig] Init");
		m_ZenNightConfig = new ZenNightConfig;
		m_ZenNightConfig.Load();
	}

	return m_ZenNightConfig;
}