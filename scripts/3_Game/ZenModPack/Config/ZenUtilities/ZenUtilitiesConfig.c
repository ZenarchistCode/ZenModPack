class ZenUtilitiesConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "ZenUtilitiesConfig.json";
	private const static string CURRENT_VERSION = "3";
	string CONFIG_VERSION = "";

	// Settings data
	//bool CountServerFPS = false;
	//string FpsChatCommand = "fps";
	//string ServerFPSWarningMsg = "PERFORMANCE WARNING: Server FPS is currently %1. This is low and very bad for vehicle desync. Be careful if driving!";
	//int ServerFPSWarning = 50; // Raw fps
	//int ServerActionFPSDropWarningPct = 0; // %
	//bool ShouldLogLootCyclers;
	bool ShouldLogPVP;
	bool ShouldLogDeathGear;
	bool ShouldDeleteGhostItems;
	bool ShouldLogItemCount;
	bool ItemTextContains;
	ref array<string> LogItemTypes;
	bool ShouldLogRaids;
	ref array<string> RaidAmmo;

	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenUtilitiesConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				JsonFileLoader<ZenUtilitiesConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		CONFIG_VERSION = CURRENT_VERSION;

		// Prepare default config
		//ShouldLogLootCyclers = true;
		ShouldLogPVP = true;
		ShouldLogDeathGear = true;
		ShouldLogItemCount = false;
		ShouldDeleteGhostItems = true;
		ItemTextContains = true;
		LogItemTypes = new array<string>;
		LogItemTypes.Insert("Plastic_Explosive");
		LogItemTypes.Insert("ExpansionSatchel");
		ShouldLogRaids = false;
		RaidAmmo = new array<string>;
		RaidAmmo.Insert("ExpansionC4_Ammo");
		RaidAmmo.Insert("Plastic_Explosive_Ammo");

		Save();
	}

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenUtilitiesConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
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

ref ZenUtilitiesConfig m_ZenUtilitiesConfig;

static ZenUtilitiesConfig GetZenUtilitiesConfig()
{
	if (!m_ZenUtilitiesConfig)
	{
		m_ZenUtilitiesConfig = new ZenUtilitiesConfig;
		m_ZenUtilitiesConfig.Load();
	}

	return m_ZenUtilitiesConfig;
};