class ZenWellsConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenRepairWellsConfig.json";

	// Main config data
	string ConfigVersion = "";
	string MessageNotDrink = "This well doesn't seem to work anymore! I need to repair it with a wrench, or find a natural water source...";
	string MessageRepaired = "It seems this well has already been repaired...";
	bool DisableWellsByDefault = true; // If true then all wells will be disabled by default. If false, only custom-added wells will be disabled.
	int DamageTool = 50; // Damages pipe wrench
	int TimeToBreak = 1209600; // How long the well stays working once repaired until it breaks again

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;
		
		GetZenWellsDB();

		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenWellsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenWellsConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
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
		JsonFileLoader<ZenWellsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenWellsConfig m_ZenWellsConfig;

// Helper function to return Config data storage object
static ZenWellsConfig GetZenWellsConfig()
{
	if (!m_ZenWellsConfig)
	{
		ZMPrint("[ZenWellsConfig] Init");
		m_ZenWellsConfig = new ZenWellsConfig;
		m_ZenWellsConfig.Load();
	}

	return m_ZenWellsConfig;
};