class ZenStaticCarWorkbenchConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenStaticCarWorkbenchConfig.json";

	int MaxAttachmentCount = 4;
	ref map<string, float> AttachmentTypes = new map<string, float>;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenStaticCarWorkbenchConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		AttachmentTypes.Insert("LugWrench", 0.1);
		AttachmentTypes.Insert("TireRepairKit", 0.1);
		AttachmentTypes.Insert("CanisterGasoline", 0.01);
		AttachmentTypes.Insert("CarRadiator", 0.1);
		AttachmentTypes.Insert("CarBattery", 0.1);
		AttachmentTypes.Insert("TruckBattery", 0.1);
		AttachmentTypes.Insert("Sledgehammer", 0.1);
		AttachmentTypes.Insert("SparkPlug", 0.1);
		AttachmentTypes.Insert("SparkPlug", 0.1);
		AttachmentTypes.Insert("GlowPlug", 0.1);
		AttachmentTypes.Insert("Rag", 0.1);
		AttachmentTypes.Insert("Pliers", 0.1);
		AttachmentTypes.Insert("Screwdriver", 0.1);
		AttachmentTypes.Insert("Pipewrench", 0.1);
		AttachmentTypes.Insert("Wrench", 0.1);
		AttachmentTypes.Insert("Zen_SprayCan_Black", 0.1);
		AttachmentTypes.Insert("Zen_SprayCan_Green", 0.1);
		AttachmentTypes.Insert("Blowtorch", 0.1);

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

		JsonFileLoader<ZenStaticCarWorkbenchConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenStaticCarWorkbenchConfig m_ZenStaticCarWorkbenchConfig;

static ZenStaticCarWorkbenchConfig GetZenStaticCarWorkbenchConfig()
{
	if (!m_ZenStaticCarWorkbenchConfig)
	{
		ZMPrint("[ZenStaticCarWorkbenchConfig] Init");
		m_ZenStaticCarWorkbenchConfig = new ZenStaticCarWorkbenchConfig;
		m_ZenStaticCarWorkbenchConfig.Load();
	}

	return m_ZenStaticCarWorkbenchConfig;
}