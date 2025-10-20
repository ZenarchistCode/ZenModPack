class ZenConcussionGrenadeConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenConcussionGrenadeConfig.json";

	// Main config data
	string ConfigVersion = "";
	int UnconMinTime;
	int UnconMaxTime;
	int KillZombieRadius;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	
			// If config exists, load file
			JsonFileLoader<ZenConcussionGrenadeConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenConcussionGrenadeConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Set default server config
		UnconMinTime = 120;
		UnconMaxTime = 120;
		KillZombieRadius = 0;

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{	
			// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenConcussionGrenadeConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenConcussionGrenadeConfig m_ZenConcussionGrenadeConfig;

// Helper function to return Config data storage object
static ZenConcussionGrenadeConfig GetZenConcussionGrenadeConfig()
{
	if (!m_ZenConcussionGrenadeConfig && GetGame().IsDedicatedServer())
	{
		ZMPrint("[ZenConcussionGrenadeConfig] Init");
		m_ZenConcussionGrenadeConfig = new ZenConcussionGrenadeConfig;
		m_ZenConcussionGrenadeConfig.Load();
	}

	return m_ZenConcussionGrenadeConfig;
};