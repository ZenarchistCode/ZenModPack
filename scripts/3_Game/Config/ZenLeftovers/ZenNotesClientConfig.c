class ZenNotesClientConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenNotesClientConfig.json";

	// Main config data
	int LastUsedFontStyle;

	// Load config file
	void Load()
	{
		if (!GetGame().IsClient())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenNotesClientConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
		}
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenNotesClientConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenNotesClientConfig m_ZenNotesClientConfig;

// Helper function to return Config data storage object
static ZenNotesClientConfig GetZenNotesClientConfig()
{
	if (!m_ZenNotesClientConfig)
	{
		ZMPrint("[ZenNotesClientConfig] Init");
		m_ZenNotesClientConfig = new ZenNotesClientConfig;
		m_ZenNotesClientConfig.Load();
	}

	return m_ZenNotesClientConfig;
};