ref ZenNightConfig g_ZenNightConfig;

static ZenNightConfig GetZenNightConfig()
{
	if (!g_ZenNightConfig) GetZenConfigRegister().RegisterConfig(ZenNightConfig);
	return g_ZenNightConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenNightConfig);
	}
}

class ZenNightConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenNightConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenNightConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenNightConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	float OvercastToTriggerDarkNights;
	bool UpdateOvercastLightingAnyTime;
	float ChanceOfDarkNight;
	int BrightNightLightingConfigID;
	int DarkNightLightingConfigID;

	override void SetDefaults()
	{
		OvercastToTriggerDarkNights = 0.6;
		UpdateOvercastLightingAnyTime = false;
		ChanceOfDarkNight = 0.0;
		BrightNightLightingConfigID = 0;
		DarkNightLightingConfigID = 1;
	}
}