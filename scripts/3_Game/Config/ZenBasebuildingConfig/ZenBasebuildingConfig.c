class ZenBasebuildingConfig
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenBasebuildingConfig.json";

	bool DebugOn;
	bool PackTentsAnywhere;
	ref map<string, float> CantPlaceInsideOfSelf;
	ref array<ref ZenBasebuildingLimits> BasebuildingLimits;
	string NoBuildZoneDefaultMessage;
	ref array<ref ZenNoBuildZone> NoBuildZones;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenBasebuildingConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}
		
		SetDefaultValues();

		Save();
	}

	void SetDefaultValues()
	{
		DebugOn = false;

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

		JsonFileLoader<ZenBasebuildingConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	void SetConfig(ZenBasebuildingConfig cfg)
	{
		m_ZenBasebuildingConfig = cfg;
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

	void ZenNoBuildZone(string name, vector pos, float dist, bool height)
	{
		ZoneName = name;
		Position = pos;
		MinimumDistance = dist;
		IgnoreHeight = height;
	}
}

ref ZenBasebuildingConfig m_ZenBasebuildingConfig;

static ZenBasebuildingConfig GetZenBasebuildingConfig()
{
	if (!m_ZenBasebuildingConfig)
	{
		ZMPrint("[ZenBasebuildingConfig] Init");
		m_ZenBasebuildingConfig = new ZenBasebuildingConfig;
		m_ZenBasebuildingConfig.Load();
	}

	return m_ZenBasebuildingConfig;
}