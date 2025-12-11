class ZenStaticTireRackConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenStaticTireRackConfig.json";

	int MaxAttachmentCount = 4;
	ref map<string, float> AttachmentTypes = new map<string, float>;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenStaticTireRackConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		AttachmentTypes.Insert("HatchbackWheel", 0.1);
		AttachmentTypes.Insert("CivSedanWheel", 0.1);
		AttachmentTypes.Insert("Hatchback_02_Wheel", 0.1);
		AttachmentTypes.Insert("Sedan_02_Wheel", 0.1);
		AttachmentTypes.Insert("Truck_01_Wheel", 0.05);
		AttachmentTypes.Insert("Offroad_02_Wheel", 0.05);
		AttachmentTypes.Insert("LugWrench", 0.1);
		AttachmentTypes.Insert("CanisterGasoline", 0.01);
		AttachmentTypes.Insert("TireRepairKit", 0.1);

		Save();
	}

	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist("$profile:\\Zenarchist"))
		{
			MakeDirectory(zenModFolder);
		}

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenStaticTireRackConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenStaticTireRackConfig m_ZenStaticTireRackConfig;

static ZenStaticTireRackConfig GetZenStaticTireRackConfig()
{
	if (!m_ZenStaticTireRackConfig)
	{
		ZMPrint("[ZenStaticTireRackConfig] Init");
		m_ZenStaticTireRackConfig = new ZenStaticTireRackConfig;
		m_ZenStaticTireRackConfig.Load();
	}

	return m_ZenStaticTireRackConfig;
}