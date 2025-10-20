class ZenPumpsConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenRepairPumpsConfig.json";

	// Main config data
	string ConfigVersion = "";
	string MessagePumpNotWork = "This pump doesn't seem to work anymore! I need to repair it with a wrench...";
	string MessageRepaired = "It seems this pump has already been repaired...";
	bool DisablePumpsByDefault = true; // If true then all Pumps will be disabled by default. If false, only custom-added Pumps will be disabled.
	int DamageTool = 50; // Damages pipe wrench
	int TimeToBreak = 1209600; // How long the Pump stays working once repaired until it breaks again

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;
		
		GetZenPumpsDB();

		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenPumpsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenPumpsConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}
		 
		// Config file does not exist or json structure has changed, create default file
		ConfigVersion = CONFIG_VERSION;

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (!FileExist(zenModFolder))
		{	
			// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenPumpsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenPumpsConfig m_ZenPumpsConfig;

// Helper function to return Config data storage object
static ZenPumpsConfig GetZenPumpsConfig()
{
	if (!m_ZenPumpsConfig)
	{
		ZMPrint("[ZenPumpsConfig] Init");
		m_ZenPumpsConfig = new ZenPumpsConfig;
		m_ZenPumpsConfig.Load();
	}

	return m_ZenPumpsConfig;
};