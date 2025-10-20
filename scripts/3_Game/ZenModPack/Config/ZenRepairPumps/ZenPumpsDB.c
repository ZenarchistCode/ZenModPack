class ZenPumpsDB
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	//private const static string zenModFolder = "$mission:storage_1\\data\\Zenarchist\\"; // CAUSES CFTOOLS TO BE UNABLE TO RESTART SERVER! PROBABLY DUE TO READ/WRITE FILE PERMISSIONS DURING BACKUP?
	private const static string zenModFolder = "$profile:\\Zenarchist\\DATABASE\\";
	private const static string zenConfigName = "ZenPumpsDB.json";

	// Main config data
	string ConfigVersion = "";
	ref array<ref ZenRepairablePump> RepairablePumps = new array<ref ZenRepairablePump>; // List of repaired/non-repaired Pumps

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenPumpsDB>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenPumpsDB>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
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
		JsonFileLoader<ZenPumpsDB>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get a repairable Pump index (if it exists) based on location
	int GetRepairablePumpIndex(vector location)
	{
		for (int i = 0; i < RepairablePumps.Count(); i++)
		{
			ZenRepairablePump Pump = RepairablePumps.Get(i);

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
		int ts = JMDate.Now(false).GetTimestamp() + GetZenPumpsConfig().TimeToBreak;

		if (index < 0 || index > RepairablePumps.Count())
		{
			ZenRepairablePump newPump = new ZenRepairablePump(location, true, ts);
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
class ZenRepairablePump
{
	vector Location;
	bool Invincible = false;
	bool Repaired = false;
	int BreakTime = 0;

	void ZenRepairablePump(vector loc, bool rep, int bt, bool inv = false)
	{
		Location = loc;
		Repaired = rep;
		BreakTime = bt;
		Invincible = inv;
	}
};

// Save config data
ref ZenPumpsDB m_ZenPumpsDB;

// Helper function to return Config data storage object
static ZenPumpsDB GetZenPumpsDB()
{
	if (!m_ZenPumpsDB)
	{
		ZMPrint("[ZenPumpsDB] Init");
		m_ZenPumpsDB = new ZenPumpsDB;
		m_ZenPumpsDB.Load();
	}

	return m_ZenPumpsDB;
};