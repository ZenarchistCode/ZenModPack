ref ZenStaticTireRackConfig g_ZenStaticTireRackConfig;

static ZenStaticTireRackConfig GetZenStaticTireRackConfig()
{
	if (!g_ZenStaticTireRackConfig) GetZenConfigRegister().RegisterConfig(ZenStaticTireRackConfig);
	return g_ZenStaticTireRackConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenStaticTireRackConfig);
	}
}

class ZenStaticTireRackConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenStaticTireRackConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenStaticTireRackConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenStaticTireRackConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	int MaxAttachmentCount;
	ref map<string, float> AttachmentTypes;

	override void SetDefaults()
	{
		MaxAttachmentCount = 4;
		AttachmentTypes = new map<string, float>;
		AttachmentTypes.Insert("HatchbackWheel", 0.1);
		AttachmentTypes.Insert("CivSedanWheel", 0.1);
		AttachmentTypes.Insert("Hatchback_02_Wheel", 0.1);
		AttachmentTypes.Insert("Sedan_02_Wheel", 0.1);
		AttachmentTypes.Insert("Truck_01_Wheel", 0.05);
		AttachmentTypes.Insert("Offroad_02_Wheel", 0.05);
		AttachmentTypes.Insert("LugWrench", 0.1);
		AttachmentTypes.Insert("CanisterGasoline", 0.01);
		AttachmentTypes.Insert("TireRepairKit", 0.1);
	}
}