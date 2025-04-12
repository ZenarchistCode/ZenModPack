class ZenModPackConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "ZenModPackConfig.json";

	// Main config data
	ref map<string, bool> ModEnabled;
	ref map<string, bool> ServerSideModEnabled;

	void Load()
	{
		SetDefaultValues();

		// Load JSON on server only
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			ref ZenModPackConfig savedCfg;
			JsonFileLoader<ZenModPackConfig>.JsonLoadFile(zenModFolder + zenConfigName, savedCfg);

			// The following code allows me to easily add new mod config without needing to override any previously set configs.
			// SetDefaultValues() will add any new cfg, while the following code will make sure to copy over the old cfg settings.
			if (savedCfg)
			{
				int i;
				for (i = 0; i < savedCfg.ModEnabled.Count(); i++)
				{
					ModEnabled.Set(savedCfg.ModEnabled.GetKey(i), savedCfg.ModEnabled.GetElement(i));
				}

				for (i = 0; i < savedCfg.ServerSideModEnabled.Count(); i++)
				{
					ServerSideModEnabled.Set(savedCfg.ServerSideModEnabled.GetKey(i), savedCfg.ServerSideModEnabled.GetElement(i));
				}

				delete savedCfg;
			}
		}

		// Call this after load, so that any new mods added to the config in SetDefaultValues() are added to the list and saved.
		Save();
	}

	void SetDefaultValues()
	{
		ModEnabled = new map<string, bool>;
		ServerSideModEnabled = new map<string, bool>;

		// Client sync mods
		ModEnabled.Insert("ZenInventoryAnimation", true);
		ModEnabled.Insert("ZenAntiCombatLogout", true);
		ModEnabled.Insert("ZenImmersiveLogin", true);
		ModEnabled.Insert("ZenCraftingSounds", true);
		ModEnabled.Insert("ZenBrokenGlasses", true);
		ModEnabled.Insert("ZenEngraveWeapon", false);
		ModEnabled.Insert("ZenCauseOfDeath", true);
		ModEnabled.Insert("ZenOpenCansRock", true);
		ModEnabled.Insert("ZenCarWorkbench", true);
		ModEnabled.Insert("ZenRepairPumps", true);
		ModEnabled.Insert("ZenRepairWells", true);
		ModEnabled.Insert("ZenPimpMyRide", true);
		ModEnabled.Insert("ZenRaidAlarm", true);
		ModEnabled.Insert("ZenCatchRain", true);
		ModEnabled.Insert("ZenHologram", true);
		ModEnabled.Insert("ZenEarPlugs", true);
		ModEnabled.Insert("ZenCamoCamp", true);
		ModEnabled.Insert("ZenFireFuel", true);
		ModEnabled.Insert("ZenTimeBomb", true);
		ModEnabled.Insert("ZenTireRack", true);
		ModEnabled.Insert("ZenWolfMask", true);
		ModEnabled.Insert("ZenGlovebox", true);
		ModEnabled.Insert("ZenGhillie", true);
		ModEnabled.Insert("ZenAutoRun", true);
		ModEnabled.Insert("ZenSplitUI", true);
		ModEnabled.Insert("ZenNotes", true);
		ModEnabled.Insert("ZenChess", true);
		ModEnabled.Insert("ZenMusic", true);

		// No client sync required
		ServerSideModEnabled.Insert("ZenPersistentTrees", true);
		ServerSideModEnabled.Insert("ZenStaticBarbedWire", false); // Disabled by default as not all servers may want this
		ServerSideModEnabled.Insert("ZenDynamicZoneLoot", true);
		ServerSideModEnabled.Insert("ZenWeatherConfig", false); // Disabled by default as not all servers may want this
		ServerSideModEnabled.Insert("ZenTreesplosions", true);
		ServerSideModEnabled.Insert("ZenChickenCoops", true);
		ServerSideModEnabled.Insert("ZenZombieDoors", true);
		ServerSideModEnabled.Insert("ZenModLogger", true);
		ServerSideModEnabled.Insert("ZenLeftovers", true);
		ServerSideModEnabled.Insert("ZenCampSites", false); // Disabled because not all playable maps are accounted for in default config
		ServerSideModEnabled.Insert("ZenFireWood", true);
		ServerSideModEnabled.Insert("ZenIceLakes", false);
		ServerSideModEnabled.Insert("ZenTreasure", true);
		ServerSideModEnabled.Insert("ZenGraves", true);
		ServerSideModEnabled.Insert("ZenZippo", true);

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
	}

	void Save()
	{
		if (!FileExist("$profile:\\Zenarchist"))
			MakeDirectory("$profile:\\Zenarchist");

		if (!FileExist("$profile:\\Zenarchist\\Utilities"))
			MakeDirectory("$profile:\\Zenarchist\\Utilities");

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenModPackConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenModPackConfig m_ZenModPackConfig;

static ZenModPackConfig GetZenModPackConfig()
{
	if (!m_ZenModPackConfig)
	{
		ZMPrint("[ZenModPackConfig] Init");
		m_ZenModPackConfig = new ZenModPackConfig;
		m_ZenModPackConfig.Load();
	}

	return m_ZenModPackConfig;
}

static bool ZenModEnabled(string mod)
{
	bool enabled = false;
	if (!GetZenModPackConfig().ModEnabled.Find(mod, enabled))
	{
#ifdef SERVER
		if (GetZenModPackConfig().ServerSideModEnabled.Find(mod, enabled))
			return enabled;
#endif

		Error("VERY BAD ERROR: ZenModEnabled(" + mod + ") - THAT MOD WASN'T FOUND! TYPO?"); // Print to Crash Logs
	}

	return enabled;
}

static bool SetZenModEnabled(string mod, bool enabled)
{
	int i;
	for (i = 0; i < GetZenModPackConfig().ModEnabled.Count(); i++)
	{
		string clientKey = GetZenModPackConfig().ModEnabled.GetKey(i);
		clientKey.ToLower();

		if (mod == clientKey)
		{
			GetZenModPackConfig().ModEnabled.Set(GetZenModPackConfig().ModEnabled.GetKey(i), enabled);
			return true;
		}
	}

	for (i = 0; i < GetZenModPackConfig().ServerSideModEnabled.Count(); i++)
	{
		string serverKey = GetZenModPackConfig().ServerSideModEnabled.GetKey(i);
		serverKey.ToLower();

		if (mod == serverKey)
		{
			GetZenModPackConfig().ServerSideModEnabled.Set(GetZenModPackConfig().ServerSideModEnabled.GetKey(i), enabled);
			return true;
		}
	}

	return false;
}
