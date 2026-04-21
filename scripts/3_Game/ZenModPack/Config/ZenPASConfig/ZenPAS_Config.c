ref ZenPAS_Config g_ZenPAS_Config;

static ZenPAS_Config GetZenPAS_Config()
{
	if (!g_ZenPAS_Config) GetZenConfigRegister().RegisterConfig(ZenPAS_Config);
	return g_ZenPAS_Config;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenPAS_Config);
	}
}

class ZenPAS_Config: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenPAS_Config = this;
	}
	
	override string 	GetRootFolder() 			{ return ZenConstants.GetDbFolder(); }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenPAS_Config>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenPAS_Config>.SaveFile(path, this, err);
	}

	// -------------------------
	// CONFIG VARS
	// -------------------------
	ref array<ref ZenPASObject> PAS_Positions;

	override void SetDefaults()
	{
		PAS_Positions = new array<ref ZenPASObject>;
	}
}

// Define a PAS
class ZenPASObject
{
	vector Location;
	string BatteryType;
	float BatteryHealth;
	float BatteryCharge;

	void ZenPASObject(vector loc, string t, float hp, float energy)
	{
		Location = loc;
		BatteryType = t;
		BatteryHealth = hp;
		BatteryCharge = energy;
	}
}