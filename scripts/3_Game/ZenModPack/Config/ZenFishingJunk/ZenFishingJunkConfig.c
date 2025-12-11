class ZenFishingJunkConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenFishingJunkConfig.json";

	ref map<string, int> FishingJunk = new map<string, int>;
	ref map<string, int> FishingJunkEmpty = new map<string, int>;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenFishingJunkConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		FishingJunk.Set("Wellies_Brown", 4);
		FishingJunk.Set("Wellies_Grey", 4);
		FishingJunk.Set("Wellies_Green", 4);
		FishingJunk.Set("Wellies_Black", 4);
		FishingJunk.Set("Compass", 1);
		FishingJunk.Set("OrienteeringCompass", 1);
		FishingJunk.Set("ChernarusMap", 1);
		FishingJunk.Set("PlantMaterial", 1);
		FishingJunk.Set("Zen_Empty_SodaCan_Random", 1);
		FishingJunk.Set("Rope", 1);
		FishingJunk.Set("Netting", 1);
		FishingJunk.Set("MetalWire", 1);

		FishingJunkEmpty.Set("Pot", 4);
		FishingJunkEmpty.Set("ZenFlask", 1);
		FishingJunkEmpty.Set("WaterBottle", 1);
		FishingJunkEmpty.Set("Canteen", 1);

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

		JsonFileLoader<ZenFishingJunkConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

ref ZenFishingJunkConfig m_ZenFishingJunkConfig;

static ZenFishingJunkConfig GetZenFishingJunkConfig()
{
	if (!m_ZenFishingJunkConfig)
	{
		ZMPrint("[ZenFishingJunkConfig] Init");
		m_ZenFishingJunkConfig = new ZenFishingJunkConfig;
		m_ZenFishingJunkConfig.Load();
	}

	return m_ZenFishingJunkConfig;
}