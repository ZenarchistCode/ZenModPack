ref ZenUtilitiesConfig g_ZenUtilitiesConfig;

static ZenUtilitiesConfig GetZenUtilitiesConfig()
{
	if (!g_ZenUtilitiesConfig) GetZenConfigRegister().RegisterConfig(ZenUtilitiesConfig);
	return g_ZenUtilitiesConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenUtilitiesConfig);
	}
}

class ZenUtilitiesConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenUtilitiesConfig = this;
	}
	
	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.2"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenUtilitiesConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenUtilitiesConfig>.SaveFile(path, this, err);
	}

	// -------------------------
	// CONFIG VARS
	// -------------------------

	// Settings data
	//bool CountServerFPS = false;
	//string FpsChatCommand = "fps";
	//string ServerFPSWarningMsg = "PERFORMANCE WARNING: Server FPS is currently %1. This is low and very bad for vehicle desync. Be careful if driving!";
	//int ServerFPSWarning = 50; // Raw fps
	//int ServerActionFPSDropWarningPct = 0; // %
	//bool ShouldLogLootCyclers;
	bool ShouldLogPVP;
	bool ShouldLogDeathGear;
	bool ShouldLogItemCount;
	bool ItemTextContains;
	ref array<string> LogItemTypes;
	bool ShouldLogRaids;
	ref array<string> RaidAmmo;

	override void SetDefaults()
	{
		//ShouldLogLootCyclers = true;
		ShouldLogPVP = true;
		ShouldLogDeathGear = true;
		ShouldLogItemCount = false;
		ItemTextContains = true;
		LogItemTypes = new array<string>;
		LogItemTypes.Insert("Plastic_Explosive");
		LogItemTypes.Insert("ExpansionSatchel");
		ShouldLogRaids = false;
		RaidAmmo = new array<string>;
		RaidAmmo.Insert("ExpansionC4_Ammo");
		RaidAmmo.Insert("Plastic_Explosive_Ammo");
	}

	bool FindItemLog(string type)
	{
		type.ToLower();

		foreach (string checkType : LogItemTypes)
		{
			checkType.ToLower();

			if (!ItemTextContains && checkType == type)
				return true;
			else
			if (checkType.Contains(type))
				return true;
		}

		return false;
	}
}