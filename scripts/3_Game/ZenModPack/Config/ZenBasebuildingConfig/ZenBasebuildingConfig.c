ref ZenBasebuildingConfig g_ZenBasebuildingConfig;

static ZenBasebuildingConfig GetZenBasebuildingConfig()
{
	if (!g_ZenBasebuildingConfig) GetZenConfigRegister().RegisterConfig(ZenBasebuildingConfig);
	return g_ZenBasebuildingConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenBasebuildingConfig);
	}
}

class ZenBasebuildingConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenBasebuildingConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool		ShouldSyncToClient() 		
	{
		return ZenModEnabled("ZenBasebuildingConfig"); 
	}

	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenBasebuildingConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenBasebuildingConfig>.SaveFile(path, this, err);
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenBasebuildingConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenBasebuildingConfig>.LoadData(payload, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	bool DebugOn;
	bool PackTentsAnywhere;
	ref map<string, float> CantPlaceInsideOfSelf;
	ref array<ref ZenBasebuildingLimits> BasebuildingLimits;
	bool NoBuildZonesExpansionAutoCreate;
	string NoBuildZoneDefaultMessage;
	ref array<ref ZenNoBuildZone> NoBuildZones;
	ref array<string> Whitelist;

	override void SetDefaults()
	{
		DebugOn = false;

		NoBuildZonesExpansionAutoCreate = true;
		PackTentsAnywhere = true;

		CantPlaceInsideOfSelf = new map<string, float>;
		CantPlaceInsideOfSelf.Set("MediumTent", 2.6);
		CantPlaceInsideOfSelf.Set("LargeTent", 5);
		CantPlaceInsideOfSelf.Set("CarTent", 2);
		CantPlaceInsideOfSelf.Set("PartyTent", 2.8);

		BasebuildingLimits = new array<ref ZenBasebuildingLimits>;
		BasebuildingLimits.Insert(new ZenBasebuildingLimits("Fence", 0.5, 4, 1.5, 5));

		NoBuildZoneDefaultMessage = "You can't build within %distance% of this area!";
		NoBuildZones = new array<ref ZenNoBuildZone>;
		NoBuildZones.Insert(new ZenNoBuildZone("Example", "0 0 0", 1, true));

		Whitelist = new array<string>;
		Whitelist.Insert("ExplosivesBase");
		Whitelist.Insert("Trap_Base");
		Whitelist.Insert("ZenNote");
		Whitelist.Insert("ZenSleepingBag_Base");
		Whitelist.Insert("Zen_ChessBoard");
		Whitelist.Insert("Zen_BoomBox");
		Whitelist.Insert("Zen_TireRack");
	}
}

class ZenBasebuildingLimits
{
	string ClassName;
	float MinForwardTolerance;
	float MinRightLeftTolerance;
	float MinHeightSeparation;
	float MinSearchDistance;

	void ZenBasebuildingLimits(string type, float minForward, float minRightLeft, float minHeight, float minSearch)
	{
		ClassName = type;
		MinForwardTolerance = minForward;
		MinRightLeftTolerance = minRightLeft;
		MinHeightSeparation = minHeight;
		MinSearchDistance = minSearch;
	}
}

class ZenNoBuildZone
{
	string ZoneName;
	string WarningMessage = "";
	vector Position;
	float MinimumDistance;
	bool IgnoreHeight;

	void ZenNoBuildZone(string name, vector pos, float dist, bool ignoreHeight)
	{
		ZoneName = name;
		Position = pos;
		MinimumDistance = dist;
		IgnoreHeight = ignoreHeight;
	}
}