class ZenCauseOfDeathConfig
{
	// Set actual config version (doesn't save to json)
	private static const string CONFIG_VERSION = "2";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenCauseOfDeathConfig.json";

	// Config data
	string ConfigVersion = "";
	string CauseOfDeathPrefix1 = "It appears";
	string DeadPlayerPrefix = "this";
	string MaleGender = "man's";
	string FemaleGender = "woman's";
	string CauseOfDeathPrefix2 = "cause of death was";
	ref array<ref CauseOfDeathDef> CauseOfDeathDefs = new array<ref CauseOfDeathDef>;
	bool DebugOn = false;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	// If config exists, load file
			JsonFileLoader<ZenCauseOfDeathConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenCauseOfDeathConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				ConfigVersion = CONFIG_VERSION;
				Save();
			}
		}
		else
		{ // Config file does not exist, create default file
			// Save default values
			ConfigVersion = "2";
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("unknown", "unknown"));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("falldamage", "the result of falling from a great height."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("melee", "from some kind of blunt force trauma."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("zombie", "probably the result of a zombie attack."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("bullet", "from a gunshot wound..."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("explosion", "from an explosion of some kind."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("grenade", "from a grenade explosion perhaps..."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("landmine", "from a land mine explosion perhaps..."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("transport", "from being run over by a vehicle."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("wolf", "from a wolf attack maybe?"));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("bear", "from being mauled by a large creature, probably a bear..."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("fire", "third-degree burns."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("bleeding", "from bleeding out."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("hunger", "starvation."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("thirst", "dehydration."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("woundinfection", "wound infection."));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("contamination", "toxic contamination. I better not get too close!"));
			CauseOfDeathDefs.Insert(new CauseOfDeathDef("suicide", "suicide, but I could be wrong..."));
			// Save config
			Save();
		}
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{ // If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenCauseOfDeathConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get a cause of death from config list
	CauseOfDeathDef GetCauseOfDeath(string ammoType)
	{
		ammoType.ToLower();
		for (int i = 0; i < CauseOfDeathDefs.Count(); i++)
		{
			string toFind = CauseOfDeathDefs.Get(i).AmmoType;
			toFind.ToLower();
			if (ammoType == toFind) // Use Contains() to find similar items with an item base (eg. Zagorky / ZagorkyChocolate)
			{
				return CauseOfDeathDefs.Get(i);
			}
		}

		return new CauseOfDeathDef();
	}
};

// Define a cause of death
class CauseOfDeathDef
{
	string AmmoType = "";
	string CauseMessage = "";

	void CauseOfDeathDef(string pAmmoType = "", string pCauseMessage = "")
	{
		AmmoType = pAmmoType;
		CauseMessage = pCauseMessage;
	}
};

// Save config data for reference
ref ZenCauseOfDeathConfig m_ZenCauseOfDeathConfig;

// Helper function to return Config data storage object
static ZenCauseOfDeathConfig GetCauseOfDeathConfig()
{
	#ifdef SERVER
	if (!m_ZenCauseOfDeathConfig)
	{
		ZMPrint("[ZenCOD] Init");
		m_ZenCauseOfDeathConfig = new ZenCauseOfDeathConfig;
		m_ZenCauseOfDeathConfig.Load();
	}
	#endif

	return m_ZenCauseOfDeathConfig;
};