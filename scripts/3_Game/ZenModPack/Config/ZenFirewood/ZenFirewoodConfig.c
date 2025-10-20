class ZenFirewoodConfig
{
	// Set actual config version (doesn't save to json)
	private static const string CONFIG_VERSION = "1";

	// Config version 
	string ConfigVersion = "";

	// Config location
	private static const string zenModFolder = "$profile:\\Zenarchist\\";
	private static string zenConfigName = "ZenFirewoodConfig.json";

	// Main config data
	bool LogsOn;
	bool DebugOn;
	bool DumpObjectLocations;
	bool SpawnFirewoodObjects;
	float ChanceOfSpawn = 0.5;
	string NoWoodLeftMessage;
	ref array<ref ZenFirewoodType> WoodTypes = new array<ref ZenFirewoodType>;
	ref array<ref ZenFirewoodObject> WoodPositions = new array<ref ZenFirewoodObject>;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	// If config exists, load file
			JsonFileLoader<ZenFirewoodConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenFirewoodConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				ConfigVersion = CONFIG_VERSION;
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Prepare new config parameters
		ZenFirewoodLogger.Log("Generating new config...");
		WoodTypes.Clear();
		WoodPositions.Clear();
		NoWoodLeftMessage = "It seems the wood left in this pile is wet and rotten, I can't use this...";

		// Insert wood types
		WoodTypes.Insert(new ZenFirewoodType("_woodreserve", "0 0 0", "0 0 0")); // Wood reserves with a roof found in towns etc
		WoodTypes.Insert(new ZenFirewoodType("_woodpile_forest", "0 0.3 0", "90 0 90")); // Piles of firewood near hunting cabins etc
		WoodTypes.Insert(new ZenFirewoodType("_woodpile1", "0 -0.5 0", "0 0 0")); // Small stack of firewood near camps etc
		WoodTypes.Insert(new ZenFirewoodType("_woodpile2", "0 -0.5 0", "0 0 0")); // Small stack of firewood near camps etc

		// Generate new default config
		ConfigVersion			= CONFIG_VERSION;	// Set current config version
		LogsOn					= false;			// Turn text logs off by default
		DebugOn					= false;			// Set to true to turn debug objects on (visible firewood objects)
		DumpObjectLocations		= true;				// Since this is first start, dump object locations to json
		SpawnFirewoodObjects	= true;				// Spawn the firewood objects after dumping them

		// Save new config
		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{ // If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenFirewoodConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Returns the chance of getting a bleed when gathering wood from the given type without gloves
	float GetChanceOfBleed(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.ChanceOfBleed;
		}

		return 0;
	}

	// Returns the damage dealt to gloves when gathering wood from the given wood pile type
	float GetGloveDamage(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.DamageGloves;
		}

		return 0;
	}

	// Returns the damage dealt to axes when gathering wood from the given wood pile type
	float GetToolDamage(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.DamageTool;
		}

		return 0;
	}

	// Get the minimum amount of wood this wood pile will give per server restart
	int GetMinWood(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.MinWood;
		}

		return 0;
	}

	// Get the maximum amount of wood this wood pile will give per server restart
	int GetMaxWood(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.MaxWood;
		}

		return 0;
	}

	// Return the object offset for the given firewood type
	vector GetOffset(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.Offset;
		}

		return Vector(0,0,0);
	}

	// Return the object orientation offset for the given firewood type
	vector GetOrient(string objType)
	{
		objType.ToLower();
		foreach(ZenFirewoodType woodObj : WoodTypes)
		{
			if (objType.Contains(woodObj.TypeName))
				return woodObj.Orient;
		}

		return Vector(0, 0, 0);
	}

	// Clean up any stuff not required anymore after spawn 
	void CleanUp()
	{
		WoodPositions.Clear();
		delete WoodPositions;
	}
}

// Define a firewood type
class ZenFirewoodType
{
	string TypeName;
	int MinWood = 0;
	int MaxWood = 6;
	float ChanceOfBleed = 0.33;
	float DamageGloves = 10;
	float DamageTool = 10;
	vector Offset = "0 0 0";
	vector Orient = "0 0 0";

	void ZenFirewoodType(string p_type, vector p_offset, vector p_orient)
	{
		TypeName = p_type;
		Offset = p_offset;
		Orient = p_orient;
	}
};

// Define a static firewood object
class ZenFirewoodObject
{
	string TypeName;
	ref array<vector> Locations = new array<vector>;
};

// Save config data
ref ZenFirewoodConfig m_ZenFirewoodConfig;

// Helper function to return Config data storage object
static ZenFirewoodConfig GetZenFirewoodConfig()
{
	if (!m_ZenFirewoodConfig)
	{
		ZMPrint("[ZenFirewoodConfig] Init");
		m_ZenFirewoodConfig = new ZenFirewoodConfig;

		if (GetGame().IsDedicatedServer())
			m_ZenFirewoodConfig.Load();
	}

	return m_ZenFirewoodConfig;
};