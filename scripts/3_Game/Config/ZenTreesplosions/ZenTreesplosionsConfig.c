class ZenTreesplosionsConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenTreesplosionsConfig.json";

	// Main config data
	string ConfigVersion = "";
	ref array<ref TreesplosionType> TreesplosionTypes = new array<ref TreesplosionType>;

	// Load server config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		// If config exists, load file
		if (FileExist(zenModFolder + zenConfigName))
		{ 
			JsonFileLoader<ZenTreesplosionsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenTreesplosionsConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Default treesplosion config
		TreesplosionTypes.Clear();
		TreesplosionTypes.Insert(new TreesplosionType("M67Grenade", 10, false));
		TreesplosionTypes.Insert(new TreesplosionType("RGD5Grenade", 10, false));
		TreesplosionTypes.Insert(new TreesplosionType("ClaymoreMine", 15));
		TreesplosionTypes.Insert(new TreesplosionType("Zen40mmTreeKiller", 10));
		TreesplosionTypes.Insert(new TreesplosionType("Plastic_Explosive", 15));
		TreesplosionTypes.Insert(new TreesplosionType("ImprovisedExplosive", 15));

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenTreesplosionsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get zombie config for given type
	TreesplosionType GetTreesplosionType(string type)
	{
		string explosiveType = type;
		explosiveType.ToLower();

		foreach (TreesplosionType cfg : TreesplosionTypes)
		{
			string toFind = cfg.ExplosiveType;
			toFind.ToLower();
			if (explosiveType.Contains(toFind))
			{
				return cfg;
			}
		}

		return NULL;
	}
}

// Define a tree explosion type
class TreesplosionType
{
	string ExplosiveType;
	int Radius;
	bool CutTreeSoft = true;
	bool CutTreeHard = true;
	bool CutBushSoft = true;
	bool CutBushHard = true;

	void TreesplosionType(string p_type, int p_radius, bool p_treehard = true)
	{
		ExplosiveType = p_type;
		Radius = p_radius;
		CutTreeHard = p_treehard;
	}
};

// Save config data
ref ZenTreesplosionsConfig m_ZenTreesplosionsConfig;

// Helper function to return Config data storage object
static ZenTreesplosionsConfig GetTreesplosionsConfig()
{
	if (!m_ZenTreesplosionsConfig)
	{
		//ZMPrint("[ZenTreesplosionsConfig] Init");
		m_ZenTreesplosionsConfig = new ZenTreesplosionsConfig;
		m_ZenTreesplosionsConfig.Load();
	}

	return m_ZenTreesplosionsConfig;
};