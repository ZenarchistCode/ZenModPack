ref ZenCarCompassConfig g_ZenCarCompassConfig;

static ZenCarCompassConfig GetZenCarCompassConfig()
{
	if (!g_ZenCarCompassConfig) GetZenConfigRegister().RegisterConfig(ZenCarCompassConfig);
	return g_ZenCarCompassConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenCarCompassConfig);
	}
}

class ZenCarCompassConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenCarCompassConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenCarCompassConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenCarCompassConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	ref array<ref ZenCarCompassOffset> CarConfig;

	// Load config file or create default file if config doesn't exsit
	override void SetDefaults()
	{
		CarConfig = new array<ref ZenCarCompassOffset>;

		CarConfig.Insert(new ZenCarCompassOffset("Sedan_02", "0 0.97 0.765", "0 5 0"));
		CarConfig.Insert(new ZenCarCompassOffset("CivilianSedan", "-0.19 1.044 0.88", "0 10 0"));
		CarConfig.Insert(new ZenCarCompassOffset("OffroadHatchback", "0 1.17 0.5", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Hatchback_02", "0.275 0.956 0.68", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Offroad_02", "-0.4 1.281 0.515", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Truck_01_Covered", "0.1 1.5225 1.6", "0 10 0"));

		// Expansion
		CarConfig.Insert(new ZenCarCompassOffset("Expansion_Landrover_Base", "0.0 1.475 0.9", "0 10 0"));
	}
}

class ZenCarCompassOffset
{
	string CarTypeCfg;
	vector Position;
	vector Orientation;

	void ZenCarCompassOffset(string type, vector pos, vector ori)
	{
		CarTypeCfg = type;
		Position = pos;
		Orientation = ori;
	}
}