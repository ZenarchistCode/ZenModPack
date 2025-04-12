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
	ref array<ref RepairablePump> RepairablePumps = new array<ref RepairablePump>; // List of repaired/non-repaired Pumps

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

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
		RepairablePumps.Clear();
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

	// Get a repairable Pump index (if it exists) based on location
	int GetRepairablePumpIndex(vector location)
	{
		for (int i = 0; i < RepairablePumps.Count(); i++)
		{
			RepairablePump Pump = RepairablePumps.Get(i);

			float l1 = location[0];
			float l2 = location[1];
			float l3 = location[2];

			float w1 = Pump.Location[0];
			float w2 = Pump.Location[1];
			float w3 = Pump.Location[2];

			// Match location to config location with 1m tolerance (just to be safe considering json doesn't always save float values perfectly)
			if (l1 >= w1 - 1 && l1 <= w1 + 1 && l2 >= w2 - 1 && l2 <= w2 + 1 && l3 >= w3 - 1 && l3 <= w3 + 1)
			{
				return i;
			}
		}

		return -1;
	}

	// Save reparied Pump state to json config
	void SetPumpRepaired(int index, vector location, bool repaired, bool forceSave)
	{
		int ts = JMDate.Now(false).GetTimestamp() + TimeToBreak;

		if (index < 0 || index > RepairablePumps.Count())
		{
			RepairablePump newPump = new RepairablePump(location, true, ts);
			RepairablePumps.Insert(newPump);
		}
		else
		{
			if (repaired)
			{
				RepairablePumps.Get(index).Repaired = true;
				RepairablePumps.Get(index).BreakTime = ts;
			}
			else
			{
				RepairablePumps.Get(index).Repaired = false;
				RepairablePumps.Get(index).BreakTime = 0;
			}
		}

		if (forceSave)
			Save(); // Save new Pump config.
	}
}

// Define repairable Pumps
class RepairablePump
{
	vector Location;
	bool Invincible = false;
	bool Repaired = false;
	int BreakTime = 0;

	void RepairablePump(vector loc, bool rep, int bt, bool inv = false)
	{
		Location = loc;
		Repaired = rep;
		BreakTime = bt;
		Invincible = inv;
	}
};

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