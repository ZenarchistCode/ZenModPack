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
	ref array<ref RepairableWell> RepairableWells = new array<ref RepairableWell>; // List of repaired/non-repaired wells

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

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
		RepairableWells.Clear();
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

	// Get a repairable well index (if it exists) based on location
	int GetRepairableWellIndex(vector location)
	{
		for (int i = 0; i < RepairableWells.Count(); i++)
		{
			RepairableWell well = RepairableWells.Get(i);

			float l1 = location[0];
			float l2 = location[1];
			float l3 = location[2];

			float w1 = well.Location[0];
			float w2 = well.Location[1];
			float w3 = well.Location[2];

			// Match location to config location with 1m tolerance (just to be safe considering json doesn't always save float values perfectly)
			if (l1 >= w1 - 1 && l1 <= w1 + 1 && l2 >= w2 - 1 && l2 <= w2 + 1 && l3 >= w3 - 1 && l3 <= w3 + 1)
			{
				return i;
			}
		}

		return -1;
	}

	// Save reparied well state to json config
	void SetWellRepaired(int index, vector location, bool repaired, bool forceSave)
	{
		int ts = JMDate.Now(false).GetTimestamp() + TimeToBreak;

		if (index < 0 || index > RepairableWells.Count())
		{
			RepairableWell newWell = new RepairableWell(location, true, ts);
			RepairableWells.Insert(newWell);
		}
		else
		{
			if (repaired)
			{
				RepairableWells.Get(index).Repaired = true;
				RepairableWells.Get(index).BreakTime = ts;
			}
			else
			{
				RepairableWells.Get(index).Repaired = false;
				RepairableWells.Get(index).BreakTime = 0;
			}
		}

		if (forceSave)
			Save(); // Save new well config.
	}
}

// Define repairable wells
class RepairableWell
{
	vector Location;
	bool Invincible = false;
	bool Repaired = false;
	int BreakTime = 0;

	void RepairableWell(vector loc, bool rep, int bt, bool inv = false)
	{
		Location = loc;
		Repaired = rep;
		BreakTime = bt;
		Invincible = inv;
	}
};

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