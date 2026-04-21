ref ZenConcussionGrenadeConfig g_ZenConcussionGrenadeConfig;

static ZenConcussionGrenadeConfig GetZenConcussionGrenadeConfig()
{
	if (!g_ZenConcussionGrenadeConfig) GetZenConfigRegister().RegisterConfig(ZenConcussionGrenadeConfig);
	return g_ZenConcussionGrenadeConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenConcussionGrenadeConfig);
	}
}

class ZenConcussionGrenadeConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenConcussionGrenadeConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenConcussionGrenadeConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenConcussionGrenadeConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	int UnconMinTime;
	int UnconMaxTime;
	int KillZombieRadius;

	override void SetDefaults()
	{
		UnconMinTime = 120;
		UnconMaxTime = 120;
		KillZombieRadius = 0;
	}
}