class ZenContaminatedZoneLootConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenDynamicContaminatedZoneLootConfig.json";

	// Config data
	int ItemLifetime = -1;
	ref array<ref ZenContaminatedZoneLootSpawn> LootSpawns;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			// If config exists, load file
			JsonFileLoader<ZenContaminatedZoneLootConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		// Default config (vanilla loot)
		LootSpawns = new array<ref ZenContaminatedZoneLootSpawn>;
		LootSpawns.Insert(new ZenContaminatedZoneLootSpawn("Grenade_ChemGas", 2, 5, 5, 15, 1.0));

		// Save config
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
		JsonFileLoader<ZenContaminatedZoneLootConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

class ZenContaminatedZoneLootSpawn
{
	string ItemType;
	int ItemCountMinimum;
	int ItemCountMaximum;
	float ItemQuantityMinimum;
	float ItemQuantityMaximum;
	int SpawnDistanceMinimum;
	int SpawnDistanceMaximum;
	float SpawnChance;

	void ZenContaminatedZoneLootSpawn(string p_type, int p_countmin, int p_countmax, int p_spawndistmin, int p_spawndistmax, float p_chance)
	{
		ItemType = p_type;
		ItemCountMinimum = p_countmin;
		ItemCountMaximum = p_countmax;
		SpawnDistanceMinimum = p_spawndistmin;
		SpawnDistanceMaximum = p_spawndistmax;
		SpawnChance = p_chance;
		ItemQuantityMinimum = 1.0;
		ItemQuantityMaximum = 1.0;
	}
}

ref ZenContaminatedZoneLootConfig m_ZenContaminatedZoneLootConfig;

static ZenContaminatedZoneLootConfig GetZenContaminatedZoneLootConfig()
{
	if (!m_ZenContaminatedZoneLootConfig)
	{
		m_ZenContaminatedZoneLootConfig = new ZenContaminatedZoneLootConfig;
		m_ZenContaminatedZoneLootConfig.Load();
	}

	return m_ZenContaminatedZoneLootConfig;
}