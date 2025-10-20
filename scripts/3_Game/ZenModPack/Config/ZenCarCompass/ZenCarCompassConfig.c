class ZenCarCompassConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenCarCompassConfig.json";

	// Config data
	ref array<ref ZenCarCompassOffset> CarConfig;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			// If config exists, load file
			JsonFileLoader<ZenCarCompassConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		// Save config
		CarConfig = new array<ref ZenCarCompassOffset>;

		CarConfig.Insert(new ZenCarCompassOffset("Sedan_02", "0 0.97 0.765", "0 5 0"));
		CarConfig.Insert(new ZenCarCompassOffset("CivilianSedan", "-0.19 1.044 0.88", "0 10 0"));
		CarConfig.Insert(new ZenCarCompassOffset("OffroadHatchback", "0 1.17 0.5", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Hatchback_02", "0.275 0.956 0.68", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Offroad_02", "-0.4 1.281 0.515", "0 0 0"));
		CarConfig.Insert(new ZenCarCompassOffset("Truck_01_Covered", "0.1 1.5225 1.6", "0 10 0"));

		// Expansion
		CarConfig.Insert(new ZenCarCompassOffset("Expansion_Landrover_Base", "0.0 1.475 0.9", "0 10 0"));

		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenCarCompassConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

class ZenCarCompassOffset
{
	string CarType;
	vector Position;
	vector Orientation;

	void ZenCarCompassOffset(string type, vector pos, vector ori)
	{
		CarType = type;
		Position = pos;
		Orientation = ori;
	}
}

ref ZenCarCompassConfig m_ZenCarCompassConfig;

static ZenCarCompassConfig GetZenCarCompassConfig()
{
	if (!m_ZenCarCompassConfig)
	{
		m_ZenCarCompassConfig = new ZenCarCompassConfig();
		m_ZenCarCompassConfig.Load();
	}

	return m_ZenCarCompassConfig;
}