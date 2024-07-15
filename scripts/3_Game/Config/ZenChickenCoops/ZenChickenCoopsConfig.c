class ZenChickenCoopsConfig
{
	// Set actual config version (doesn't save to json)
	private static const string CONFIG_VERSION = "1";

	// Config version 
	string ConfigVersion = "";

	// Config location
	private static const string zenModFolder = "$profile:\\Zenarchist\\";
	private static string zenConfigName = "ZenChickenCoopsConfig.json";

	// Main config data
	bool LogsOn;
	bool DebugOn;
	bool DumpObjectLocations;
	bool SpawnChickenCoops;
	float ChanceOfSpawn = 0.5;
	string NoFeathersLeftMessage;
	autoptr array<ref ZenChickenCoopType> CoopTypes;
	autoptr array<ref ZenChickenCoopObject> CoopPositions;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	
			// If config exists, load file
			JsonFileLoader<ZenChickenCoopsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenChickenCoopsConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				ConfigVersion = CONFIG_VERSION;
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Prepare new config parameters
		ZenChickenCoopsLogger.Log("Generating new config...");

		// Prepare arrays
		CoopTypes = new array<ref ZenChickenCoopType>;
		CoopPositions = new array<ref ZenChickenCoopObject>;
		CoopTypes.Clear();
		CoopPositions.Clear();

		NoFeathersLeftMessage = "It seems there are no feathers left...";

		// Insert coop types
		CoopTypes.Insert(new ZenChickenCoopType("_chickencoop", "Zen_ChickenCoop", "0 0 0", "90 0 0", 10, 3)); // Small chicken coops
		CoopTypes.Insert(new ZenChickenCoopType("_hutch", "Zen_ChickenCoop2", "0 0.8 0.2", "90 0 0", 30, 6)); // Large hutches

		// Generate new default config
		ConfigVersion			= CONFIG_VERSION;	// Set current config version
		LogsOn					= false;			// Turn text logs off by default
		DebugOn					= false;			// Set to true to turn debug objects on (visible coop trigger objects)
		DumpObjectLocations		= true;				// Since this is first start, dump object locations to json
		SpawnChickenCoops		= true;				// Spawn the coop objects after dumping them

		// Save new config
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
		JsonFileLoader<ZenChickenCoopsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Returns the zenobject name for the given type (invisible action trigger object)
	string GetObjectName(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.ObjectName;
		}

		return "";
	}

	// Returns the chance of getting a bleed when gathering feathers from the given type without gloves
	float GetChanceOfBleed(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.ChanceOfBleed;
		}

		return 0;
	}

	// Returns the damage dealt to gloves when gathering feathers from the given coop type
	float GetGloveDamage(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.DamageGloves;
		}

		return 0;
	}

	// Get the minimum amount of feathers this coop will give per server restart
	int GetMinFeathers(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.MinFeathers;
		}

		return 0;
	}

	// Get the maximum amount of feathers this coop will give per server restart
	int GetMaxFeathers(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.MaxFeathers;
		}

		return 0;
	}

	// Get the maximum amount of feathers you can collect from this coop at a time
	int GetMaxFeathersCollection(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.MaxFeathersPerCollect;
		}

		return 0;
	}

	// Return the object offset for the given coop type
	vector GetOffset(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.Offset;
		}

		return Vector(0,0,0);
	}

	// Return the object orientation offset for the given coop type
	vector GetOrient(string objType)
	{
		objType.ToLower();
		foreach(ZenChickenCoopType coopObj : CoopTypes)
		{
			if (objType.Contains(coopObj.TypeName))
				return coopObj.Orient;
		}

		return Vector(0, 0, 0);
	}

	// Clean up any stuff not required anymore after spawn 
	void CleanUp()
	{
		CoopTypes.Clear();
		CoopPositions.Clear();

		delete CoopTypes;
		delete CoopPositions;
	}
}

// Define a chicken coop type
class ZenChickenCoopType
{
	string TypeName;
	string ObjectName;
	int MinFeathers = 0;
	int MaxFeathers;
	int MaxFeathersPerCollect;
	float ChanceOfBleed = 0.05;
	float DamageGloves = 5;
	vector Offset = "0 0 0";
	vector Orient = "0 0 0";

	void ZenChickenCoopType(string p_type, string p_obj, vector p_offset, vector p_orient, int p_maxfeathers, int p_maxcollect)
	{
		TypeName = p_type;
		ObjectName = p_obj;
		Offset = p_offset;
		Orient = p_orient;
		MaxFeathers = p_maxfeathers;
		MaxFeathersPerCollect = p_maxcollect;
	}
};

// Define a static chicken coop object
class ZenChickenCoopObject
{
	string TypeName;
	ref array<vector> Locations = new array<vector>;
};

// Save config data
ref ZenChickenCoopsConfig m_ZenChickenCoopsConfig;

// Helper function to return Config data storage object
static ZenChickenCoopsConfig GetZenChickenCoopsConfig()
{
	#ifdef SERVER
	if (!m_ZenChickenCoopsConfig)
	{
		ZMPrint("[ZenChickenCoopsConfig] Init");
		m_ZenChickenCoopsConfig = new ZenChickenCoopsConfig;
		m_ZenChickenCoopsConfig.Load();
	}
	#endif

	return m_ZenChickenCoopsConfig;
};