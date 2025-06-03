class ZenZombieDoorsConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "3";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenZombieDoorsConfig.json";

	// Main config data
	string ConfigVersion = "";
	bool KnockDownDoors = true;
	bool KnockDownLockedDoors = false;
	bool OrientateToDoor = true;
	int DoorHitCount = 6;
	int DoorHitLockedCount = 12;
	float DoorHitCounterTimerSecs = 45;
	float HitDoorDelaySecs = 2;
	ref array<ref ZenZombieData> ZombieData = new array<ref ZenZombieData>;

	// Load server config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		// If config exists, load file
		if (FileExist(zenModFolder + zenConfigName))
		{ 
			JsonFileLoader<ZenZombieDoorsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenZombieDoorsConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Set default data
		ZombieData.Clear();
		ZenZombieData zed1 = new ZenZombieData("ZmbM_Soldier");
		zed1.KnockDownLockedDoors = true;
		zed1.DoorHitCount = 3;
		zed1.DoorHitCount = 6;
		ZombieData.Insert(zed1);

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenZombieDoorsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get zombie config for given type
	ZenZombieData GetZombieData(string type)
	{
		string zedType = type;
		zedType.ToLower();

		foreach (ZenZombieData zed : ZombieData)
		{
			if (zed && zed.ClassName)
			{
				string toFind = zed.ClassName;
				toFind.ToLower();
				if (zedType.Contains(toFind))
				{
					return zed;
				}
			}
		}

		return NULL;
	}
}

// Define a zombie data array
class ZenZombieData
{
	string ClassName;
	bool KnockDownDoors = true;
	bool KnockDownLockedDoors = false;
	int DoorHitCount = 6;
	int DoorHitLockedCount = 12;

	void ZenZombieData(string type)
	{
		ClassName = type;
	}
};

// Save config data
ref ZenZombieDoorsConfig m_ZenZombiesConfig;

// Helper function to return Config data storage object
static ZenZombieDoorsConfig GetZombieDoorsConfig()
{
	if (!m_ZenZombiesConfig)
	{
		//ZMPrint("[ZenZombieDoorsConfig] Init");
		m_ZenZombiesConfig = new ZenZombieDoorsConfig;
		m_ZenZombiesConfig.Load();
	}

	return m_ZenZombiesConfig;
};