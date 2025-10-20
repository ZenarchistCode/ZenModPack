// This class holds general config
class ZenTreesConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "2";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenTreesConfig.json";

	// Config data
	string ConfigVersion = "";
	int TimeToRespawn;
	bool TreeHard;
	bool TreeSoft;
	bool BushHard;
	bool BushSoft;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			// If config exists, load file
			JsonFileLoader<ZenTreesConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenTreesConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;
		TimeToRespawn = 86400 * 3; // 3 days
		TreeHard = true;
		TreeSoft = true;
		BushHard = true;
		BushSoft = true;

		// Save config
		Save();
	};

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenTreesConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Define tree state
class ZenTreeState
{
	vector Location;
	float Health = 0;
	int RenewTime = 0;

	void ZenTreeState(vector loc, int time, float hp)
	{
		Health = hp;
		Location = loc;
		RenewTime = time;
	}
};

// Save config data
ref ZenTreesConfig m_ZenTreesConfig;

// Helper function to return Config data storage object
static ZenTreesConfig GetZenTreesConfig()
{
	if (!m_ZenTreesConfig)
	{
		ZMPrint("[ZenTreesConfig] Init");
		m_ZenTreesConfig = new ZenTreesConfig;
		m_ZenTreesConfig.Load();
	}

	return m_ZenTreesConfig;
};