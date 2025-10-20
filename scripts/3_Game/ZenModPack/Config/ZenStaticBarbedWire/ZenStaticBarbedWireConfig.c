class ZenStaticBarbedWireConfig
{
	// Set actual config version (doesn't save to json)
	private static const string CONFIG_VERSION = "3";

	// Config version 
	string ConfigVersion = "";

	// Config location
	private static const string zenModFolder = "$profile:\\Zenarchist\\";
	private static string zenConfigName = "ZenStaticBarbedWireConfig.json";

	// Main config data
	bool TurnedOn;
	bool DumpObjects;
	ref array<ref ZenStaticBarbedWireType> WireTypes = new array<ref ZenStaticBarbedWireType>;
	ref array<ref ZenStaticBarbedWireObject> WirePositions = new array<ref ZenStaticBarbedWireObject>;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{	
			// If config exists, load file
			JsonFileLoader<ZenStaticBarbedWireConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenStaticBarbedWireConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				ConfigVersion = CONFIG_VERSION;
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Clear arrays if they've been loaded
		WireTypes.Clear();
		WirePositions.Clear();

		// Insert default barbed wire config - Vanilla barbed wire extents: [-1 0 -0.4, 1 0.7 0.4]
		WireTypes.Insert(new ZenStaticBarbedWireType("misc_barbedwire", "-2.2 -1 -1", "2 1 1"));
		WireTypes.Insert(new ZenStaticBarbedWireType("misc_razorwire", "-4.3 -1 -1.1", "4.3 1.1 1.1"));

		// All disabled as I'm guessing creating this many damage zones in DayZ crashes the server -.-
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_fen5_5", "-2.5 0.9 -0.6", "2.6 1.5 -0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_fen5_10", "-5 0.9 -0.6", "5.1 1.5 -0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_fen5_10_d", "-5 0.9 -0.25", "-2 1.5 -1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indcnc3_1_4", "-2.7 1.6 -0.1", "2.9 1.9 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indcnc3_1_d", "-2.7 1.6 -0.1", "2.9 1.9 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indcnc3_2_4", "-2.7 1.6 -0.1", "2.9 1.9 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indcnc3_2_d", "-2.7 1.6 -0.1", "2.9 1.9 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc2_3", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc2_3_nolc", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc2_corner", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc_corner", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc_3", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc_3_hole", "-1.5 1.3 -0.1", "1.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc2_9", "-4.5 1.3 -0.1", "4.5 1.5 0.1", false));
		WireTypes.Insert(new ZenStaticBarbedWireType("wall_indfnc_9", "-4.9 1.3 -0.1", "4.1 1.5 0.1", false));

		// Generate new default config
		ConfigVersion			= CONFIG_VERSION;
		TurnedOn				= true;
		DumpObjects				= true;

		// Save new config
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
		JsonFileLoader<ZenStaticBarbedWireConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get the offset for the given light type
	ZenStaticBarbedWireType GetWireType(string type)
	{
		type.ToLower();

		foreach (ZenStaticBarbedWireType wireType : WireTypes)
		{
			string findType = wireType.TypeName;
			findType.ToLower();

			if (type.Contains(findType))
				return wireType;
		}

		return new ZenStaticBarbedWireType;
	}

	// Once the static barbed wire objects are spawned, we no longer need this config in memory so delete that shiz.
	void Destroy()
	{
		WireTypes.Clear();
		WirePositions.Clear();

		delete WireTypes;
		delete WirePositions;
		delete m_ZenStaticBarbedWireConfig;
	}
}

// Define a barbed wire type
class ZenStaticBarbedWireType
{
	string TypeName;
	bool TurnedOn;
	string MinExtents;
	string MaxExtents;
	string AmmoName = "BarbedWireHit";
	float LoopInterval = 2;
	float DeferDuration = 0.5;
	ref array<string> DamageZones = new array<string>;

	void ZenStaticBarbedWireType(string p_name = "", string p_min = "", string p_max = "", bool p_on = true)
	{
		TypeName = p_name;
		TurnedOn = p_on;
		DamageZones.Insert("Torso");
		DamageZones.Insert("LeftHand");
		DamageZones.Insert("RightHand");
		DamageZones.Insert("RightLeg");
		DamageZones.Insert("LeftLeg");
		DamageZones.Insert("RightFoot");
		DamageZones.Insert("LeftFoot");
		MinExtents = p_min;
		MaxExtents = p_max;
	}
};

// Define a static barbed wire object
class ZenStaticBarbedWireObject
{
	string TypeName;
	ref array<vector> Locations = new array<vector>;
};

// Save config data
ref ZenStaticBarbedWireConfig m_ZenStaticBarbedWireConfig;

// Helper function to return Config data storage object
static ZenStaticBarbedWireConfig GetZenStaticBarbedWireConfig()
{
	if (!m_ZenStaticBarbedWireConfig)
	{
		Print("[ZenStaticBarbedWireConfig] Init");
		m_ZenStaticBarbedWireConfig = new ZenStaticBarbedWireConfig;
		m_ZenStaticBarbedWireConfig.Load();
	}

	return m_ZenStaticBarbedWireConfig;
};