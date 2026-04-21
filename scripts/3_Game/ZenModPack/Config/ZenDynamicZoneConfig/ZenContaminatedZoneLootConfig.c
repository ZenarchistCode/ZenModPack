ref ZenContaminatedZoneLootConfig g_ZenContaminatedZoneLootConfig;

static ZenContaminatedZoneLootConfig GetZenContaminatedZoneLootConfig()
{
	if (!g_ZenContaminatedZoneLootConfig) GetZenConfigRegister().RegisterConfig(ZenContaminatedZoneLootConfig);
	return g_ZenContaminatedZoneLootConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenContaminatedZoneLootConfig);
	}
}

class ZenContaminatedZoneLootConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenContaminatedZoneLootConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenContaminatedZoneLootConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenContaminatedZoneLootConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	int ItemLifetime;
	ref array<ref ZenContaminatedZoneLootSpawn> LootSpawns;

	// Load config file or create default file if config doesn't exsit
	override void SetDefaults()
	{
		ItemLifetime = -1;
		LootSpawns = new array<ref ZenContaminatedZoneLootSpawn>;
		LootSpawns.Insert(new ZenContaminatedZoneLootSpawn("Grenade_ChemGas", 2, 5, 5, 15, 1.0));
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