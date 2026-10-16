static ref ZenModPackConfig g_ZenModPackConfig;

static ZenModPackConfig GetZenModPackConfig()
{
	if (!g_ZenModPackConfig)
	{
		g_ZenModPackConfig = ZenModPackConfig.Cast(GetZenConfigRegister().RegisterConfig(ZenModPackConfig));
	}
	
	return g_ZenModPackConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenModPackConfig);
	}
}

class ZenModPackConfig: ZenConfigBase
{
	override string 	GetFolderName()       		{ return "Core"; }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; } // Server-side config
	override bool		ShouldLoadOnClient() 		{ return true; } // Cached to client
	override bool		ShouldSyncToClient()		{ return true; } // Client is overridden by server config

	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenModPackConfig = this;
	}
	
	ref map<string, bool> ModEnabled;
	ref map<string, bool> ServerSideModEnabled;
	
	// This is used to merge all mods into one map cache after load.
	[NonSerialized()]
	ref map<string, bool> m_RuntimeCache;
	
	[NonSerialized()]
	ref array<string> m_TriggeredWarnings;

	override void SetDefaults()
	{
		m_RuntimeCache = new map<string, bool>;
		m_TriggeredWarnings = new array<string>;
		
		ModEnabled = new map<string, bool>;
		ServerSideModEnabled = new map<string, bool>;

		// Client sync mods
		ModEnabled.Insert("ZenFixCarAttachmentGUI", false);
		ModEnabled.Insert("ZenBuildingDescription", false);
		ModEnabled.Insert("ZenBasebuildingConfig", false);
		ModEnabled.Insert("ZenInventoryAnimation", false);
		ModEnabled.Insert("ZenChristmasTreeCraft", false);
		ModEnabled.Insert("ZenHologramControls", false);
		ModEnabled.Insert("ZenAntiCombatLogout", false);
		ModEnabled.Insert("ZenAutoWinterDetect", false);
		ModEnabled.Insert("ZenImmersiveChatHUD", false);
		ModEnabled.Insert("ZenCarsCutDownTrees", false);
		ModEnabled.Insert("ZenAllowMapBagSlot", false);
		ModEnabled.Insert("ZenMagObfuscation", false);
		ModEnabled.Insert("ZenImmersiveLogin", false);
		ModEnabled.Insert("ZenCarAttachments", false);
		ModEnabled.Insert("ZenHideGearInCars", false);
		ModEnabled.Insert("ZenCraftingSounds", false);
		ModEnabled.Insert("ZenGroundCleaning", false);
		ModEnabled.Insert("ZenBetterChatFont", false);
		ModEnabled.Insert("ZenCanteenTablets", false);
		ModEnabled.Insert("ZenFireplaceStick", false);
		ModEnabled.Insert("ZenDoubleArmbands", false);
		ModEnabled.Insert("ZenCarBatteryIcon", false);
		ModEnabled.Insert("ZenAutoStackItems", false);
		ModEnabled.Insert("ZenRearViewCamera", false);
		ModEnabled.Insert("ZenCompatibleGuns", false);
		ModEnabled.Insert("ZenWeaponEngrave", false);
		ModEnabled.Insert("ZenNotifications", false);
		ModEnabled.Insert("ZenBrokenGlasses", false);
		ModEnabled.Insert("ZenBackwardsCaps", false);
		ModEnabled.Insert("ZenCraftRagHands", false);
		ModEnabled.Insert("ZenGPSWatchChat", false);
		ModEnabled.Insert("ZenCauseOfDeath", false);
		ModEnabled.Insert("ZenOpenCansRock", false);
		ModEnabled.Insert("ZenCarWorkbench", false);
		ModEnabled.Insert("ZenKnifeGardens", false);
		ModEnabled.Insert("ZenRepairPumps", false);
		ModEnabled.Insert("ZenRepairWells", false);
		ModEnabled.Insert("ZenTerritories", false);
		ModEnabled.Insert("ZenLazyActions", false);
		ModEnabled.Insert("ZenCarGlovebox", false);
		ModEnabled.Insert("ZenCombineJSON", false);
		ModEnabled.Insert("ZenRepairJSON", false);
		ModEnabled.Insert("ZenCarCompass", false);
		ModEnabled.Insert("ZenComboLocks", false);
		ModEnabled.Insert("ZenPimpMyRide", false);
		ModEnabled.Insert("ZenServerGUI", false);
		ModEnabled.Insert("ZenRaidAlarm", false);
		ModEnabled.Insert("ZenCatchRain", false);
		ModEnabled.Insert("ZenBookshelf", false);
		ModEnabled.Insert("ZenHologram", false);
		ModEnabled.Insert("ZenEarPlugs", false);
		ModEnabled.Insert("ZenPainting", false);
		ModEnabled.Insert("ZenCamoCamp", false);
		ModEnabled.Insert("ZenFireFuel", false);
		ModEnabled.Insert("ZenTimeBomb", false);
		ModEnabled.Insert("ZenTireRack", false);
		ModEnabled.Insert("ZenGhillie", false);
		ModEnabled.Insert("ZenAutoRun", false);
		ModEnabled.Insert("ZenSplitUI", false);
		ModEnabled.Insert("ZenHints", false);
		ModEnabled.Insert("ZenNotes", false);
		ModEnabled.Insert("ZenChess", false);
		ModEnabled.Insert("ZenMusic", false);
		ModEnabled.Insert("ZenShove", false);
		ModEnabled.Insert("ZenSleep", false);
		ModEnabled.Insert("ZenDirty", false);
		ModEnabled.Insert("ZenMap", false);

		// No client sync required
		ServerSideModEnabled.Insert("ZenBatteryPASPersistence", false);
		ServerSideModEnabled.Insert("ZenInformWeaponStateBug", false);
		ServerSideModEnabled.Insert("ZenLootCyclingDetection", false);
		ServerSideModEnabled.Insert("ZenDropRuinedClothing", false);
		ServerSideModEnabled.Insert("ZenAutoConvertStash", false);
		ServerSideModEnabled.Insert("ZenRebuildableWells", false);
		ServerSideModEnabled.Insert("ZenZombieController", false);
		ServerSideModEnabled.Insert("ZenPersistentTrees", false);
		ServerSideModEnabled.Insert("ZenDynamicZoneLoot", false);
		ServerSideModEnabled.Insert("ZenShootingStars", false);
		ServerSideModEnabled.Insert("ZenFishingConfig", false);
		ServerSideModEnabled.Insert("ZenTreesplosions", false);
		ServerSideModEnabled.Insert("ZenZombieConfig", false);
		ServerSideModEnabled.Insert("ZenChickenCoops", false);
		ServerSideModEnabled.Insert("ZenShelterCargo", false);
		ServerSideModEnabled.Insert("ZenStaminaSlope", false);
		ServerSideModEnabled.Insert("ZenZombieDoors", false);
		ServerSideModEnabled.Insert("ZenRadioPlugin", false);
		ServerSideModEnabled.Insert("ZenNightConfig", false);
		ServerSideModEnabled.Insert("ZenBloodDrips", false);
		ServerSideModEnabled.Insert("ZenWoodHealth", false);
		ServerSideModEnabled.Insert("ZenLeftovers", false);
		ServerSideModEnabled.Insert("ZenCampSites", false);
		ServerSideModEnabled.Insert("ZenLogPiles", false);
		ServerSideModEnabled.Insert("ZenFireWood", false);
		ServerSideModEnabled.Insert("ZenTreasure", false);
		ServerSideModEnabled.Insert("ZenAlcohol", false);
		ServerSideModEnabled.Insert("ZenGraves", false);

		// Disable all mods for DayZEditor
#ifdef EDITOR
		int i;
		for (i = 0; i < ModEnabled.Count(); i++)
		{
			ModEnabled.Set(ModEnabled.GetKey(i), false);
		}

		for (i = 0; i < ServerSideModEnabled.Count(); i++)
		{
			ServerSideModEnabled.Set(ServerSideModEnabled.GetKey(i), false);
		}
#endif

		BuildRuntimeCache();
	}
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenModPackConfig>.LoadFile(path, this, err);
	}
	
	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenModPackConfig>.SaveFile(path, this, err);		
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenModPackConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenModPackConfig>.LoadData(payload, this, err);
	}

	override void AfterLoad()
	{
		super.AfterLoad();
		
		if (!m_TriggeredWarnings)
        	m_TriggeredWarnings = new array<string>;

		ZenModPackConfig defaults = new ZenModPackConfig();
		defaults.SetDefaults();

		if (!ModEnabled)
		{
			ModEnabled = new map<string, bool>;
		}

		if (!ServerSideModEnabled)
		{
			ServerSideModEnabled = new map<string, bool>;
		}

		MergeMissingBoolMap(ModEnabled, defaults.ModEnabled);
		MergeMissingBoolMap(ServerSideModEnabled, defaults.ServerSideModEnabled);
		
		BuildRuntimeCache();

		#ifdef SERVER 
		Save();
		#endif
	}
	
	void BuildRuntimeCache()
	{
		if (!m_RuntimeCache)
			m_RuntimeCache = new map<string, bool>;
		
		m_RuntimeCache.Clear();
	
		if (ModEnabled)
			CopyMapIntoCache(ModEnabled);
	
		if (ServerSideModEnabled)
			CopyMapIntoCache(ServerSideModEnabled);
	}
	
	override void AfterConfigReceived()
	{
		super.AfterConfigReceived();
		
		InternalDebugPrint();
	}
	
	void CopyMapIntoCache(map<string, bool> src)
	{
		for (int i = 0; i < src.Count(); i++)
		{
			string k = src.GetKey(i);
			bool v = src.GetElement(i);
			m_RuntimeCache.Set(k, v);
		}
	}
	
	static void MergeMissingBoolMap(map<string, bool> dst, map<string, bool> src)
	{
		if (!dst || !src)
			return;
	
		for (int i = 0; i < src.Count(); i++)
		{
			string k = src.GetKey(i);
			bool v = src.GetElement(i);
	
			bool tmp;
			if (!dst.Find(k, tmp))
				dst.Set(k, v);
		}
	}
	
	static void RenameBoolKey(map<string, bool> keyMap, string oldKey, string newKey)
	{
	    if (!keyMap) 
			return;
	
	    bool valueEntry;
	    if (keyMap.Find(oldKey, valueEntry))
	    {
	        if (newKey != "" && !keyMap.Contains(newKey))
	            keyMap.Set(newKey, valueEntry);
	
	        keyMap.Remove(oldKey);
	    }
	}
	
	void InternalDebugPrint()
	{
		//foreach (string key, bool element : m_RuntimeCache) Print("  " + key + "=" + element);
	}
}

static bool ZenModEnabled(string mod)
{
	ZenModPackConfig cfg = GetZenModPackConfig();
	if (!cfg || !cfg.m_RuntimeCache)
		return false;

	bool enabled;
	if (cfg.m_RuntimeCache.Find(mod, enabled))
		return enabled;

	if (cfg.m_TriggeredWarnings.Find(mod) == -1)
	{
		// Only trigger warnings ONCE to prevent massive log spam
		Error("VERY BAD ERROR - unknown mod key (typo?): " + mod);
		cfg.m_TriggeredWarnings.Insert(mod);
	}
	
	return false;
}

static bool SetZenModEnabled(string mod, bool enabled)
{
	int i;
	for (i = 0; i < GetZenModPackConfig().ModEnabled.Count(); i++)
	{
		string clientKey = GetZenModPackConfig().ModEnabled.GetKey(i);

		if (mod == clientKey)
		{
			GetZenModPackConfig().ModEnabled.Set(GetZenModPackConfig().ModEnabled.GetKey(i), enabled);
			GetZenModPackConfig().BuildRuntimeCache();
			return true;
		}
	}

	for (i = 0; i < GetZenModPackConfig().ServerSideModEnabled.Count(); i++)
	{
		string serverKey = GetZenModPackConfig().ServerSideModEnabled.GetKey(i);

		if (mod == serverKey)
		{
			GetZenModPackConfig().ServerSideModEnabled.Set(GetZenModPackConfig().ServerSideModEnabled.GetKey(i), enabled);
			GetZenModPackConfig().BuildRuntimeCache();
			return true;
		}
	}

	return false;
}
