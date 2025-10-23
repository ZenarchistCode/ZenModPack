class ZenWellsDB
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$mission:storage_%1\\zenarchist\\";
	private const static string zenConfigName = "ZenWellsDB.json";

	string GetDbFolder()
	{
		return string.Format(zenModFolder, GetGame().ServerConfigGetInt("instanceId"));
	}

	// Main config data
	string ConfigVersion = "";
	ref array<ref ZenRepairableWell> RepairableWells = new array<ref ZenRepairableWell>; // List of repaired/non-repaired wells

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(GetDbFolder() + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenWellsDB>.JsonLoadFile(GetDbFolder() + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenWellsDB>.JsonSaveFile(GetDbFolder() + zenConfigName + "_old", this);
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

		if (!FileExist(GetDbFolder()))
		{	
			// If config folder doesn't exist, create it.
			MakeDirectory(GetDbFolder());
		}

		// Save JSON config
		JsonFileLoader<ZenWellsDB>.JsonSaveFile(GetDbFolder() + zenConfigName, this);
	}

	// Get a repairable well index (if it exists) based on location
	int GetRepairableWellIndex(vector location)
	{
		for (int i = 0; i < RepairableWells.Count(); i++)
		{
			ZenRepairableWell well = RepairableWells.Get(i);

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
		int ts = JMDate.Now(false).GetTimestamp() + GetZenWellsConfig().TimeToBreak;

		if (index < 0 || index > RepairableWells.Count())
		{
			ZenRepairableWell newWell = new ZenRepairableWell(location, true, ts);
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
class ZenRepairableWell
{
	vector Location;
	bool Invincible = false;
	bool Repaired = false;
	int BreakTime = 0;

	void ZenRepairableWell(vector loc, bool rep, int bt, bool inv = false)
	{
		Location = loc;
		Repaired = rep;
		BreakTime = bt;
		Invincible = inv;
	}
};

// Save config data
ref ZenWellsDB m_ZenWellsDB;

// Helper function to return Config data storage object
static ZenWellsDB GetZenWellsDB()
{
	if (!m_ZenWellsDB)
	{
		ZMPrint("[ZenWellsDB] Init");
		m_ZenWellsDB = new ZenWellsDB;
		m_ZenWellsDB.Load();
	}

	return m_ZenWellsDB;
};