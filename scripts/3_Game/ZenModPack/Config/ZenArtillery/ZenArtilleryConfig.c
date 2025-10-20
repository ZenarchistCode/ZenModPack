class ZenArtilleryConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "3";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenArtilleryConfig.json";

	// Main config data
	string ConfigVersion = "";
	bool DestroyTrees;
	bool Player_Kill;
	bool Player_BreakLegs;
	bool Player_CauseBleeds;
	float Player_ExtraDamage;
	bool SafeUnderRoof;
	bool Zombie_Kill;
	bool Animal_Kill;
	int MinStrikeSecs;
	int MaxStrikeSecs;
	int MinBombSecs;
	int MaxBombSecs;
	int MinBombs;
	int MaxBombs;
	int MinChemBombs;
	int MaxChemBombs;
	int BombDropRadius;
	int BombDamageRadius;
	bool ChemStrikeDoesDamage;
	int CarMinDamage;
	int CarMaxDamage;
	ref TStringArray DamageCarParts = {};
	ref array<ref ArtilleryDamageObject> DamageObjects;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{	
			// If config exists, load file
			JsonFileLoader<ZenArtilleryConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenArtilleryConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Set default server config
		DestroyTrees = true; // Knock over trees and bushes on explosion?
		Player_Kill = false; // Does bomb contact insta-kill players?
		Player_BreakLegs = true; // Does bomb contact break legs?
		Player_CauseBleeds = true; // Does bomb contact cause bleeds?
		Player_ExtraDamage = 0; // Specify extra damage to deal to the player (bombs do very little health damage by default)
		SafeUnderRoof = true; // Is player & DamageObjects/Car safe under roof? (Doesn't apply to zombies)
		Zombie_Kill = true; // Does bomb contact kill zombies?
		Animal_Kill = true; // Does bomb contact kill animals?
		MinStrikeSecs = 10; // Minimum delay before playing artillery sound
		MaxStrikeSecs = 20; // Max delay before playing artillery sound
		MinBombSecs = 15; // Min delay AFTER playing artillery sound for bombs to drop
		MaxBombSecs = 30; // Max delay AFTER playing artillery sound for bombs to drop
		MinBombs = 9; // Min bombs to drop
		MaxBombs = 12; // Max bombs to drop
		MinChemBombs = 1; // Minimum chemgas bombs to drop 
		MaxChemBombs = 1; // Maximum chemgas bombs to drop
		BombDropRadius = 30; // Bomb radius from smoke grenade - bombs are dropped within this (meters)
		BombDamageRadius = 30; // Bomb damage radius from explosions - any objects within this distance are affected by below settings (meters)
		ChemStrikeDoesDamage = true; // If disabled then chemical artillery strikes don't damage objects/cars, they only drop gas and affect players & zeds
		CarMinDamage = 100; // If a car is bombed, this defines how much damage to apply to a randomly selected car part from the below list
		CarMaxDamage = 250; // If a car is bombed, this defines how much damage to apply to a randomly selected car part from the below list
		DamageCarParts.Clear();
		DamageCarParts.Insert("Chassis");
		DamageCarParts.Insert("Engine");
		DamageCarParts.Insert("FuelTank");
		DamageCarParts.Insert("Reflector_1_1");
		DamageCarParts.Insert("Reflector_2_1");
		DamageCarParts.Insert("CarRadiator");
		DamageCarParts.Insert("SparkPlug");
		DamageCarParts.Insert("GlowPlug");
		DamageCarParts.Insert("Wheel");
		DamageCarParts.Insert("Trunk");
		DamageCarParts.Insert("Door");
		DamageCarParts.Insert("Hood");
		DamageObjects = new array<ref ArtilleryDamageObject>; // This array defines any other objects that are to be affected by bombs
		DamageObjects.Insert(new ArtilleryDamageObject("example_object_classname", 1000));

		// Save config
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
		JsonFileLoader<ZenArtilleryConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Define an artillery damage object
class ArtilleryDamageObject
{
	string Type;
	int Damage;

	void ArtilleryDamageObject(string p_type, int p_damage)
	{
		this.Type = p_type;
		this.Damage = p_damage;
	}
};

// Save config data
ref ZenArtilleryConfig m_ZenArtilleryConfig;

// Helper function to return Config data storage object
static ZenArtilleryConfig GetZenArtilleryConfig()
{
	if (!m_ZenArtilleryConfig && GetGame().IsDedicatedServer())
	{
		ZMPrint("[ZenArtilleryConfig] Init");
		m_ZenArtilleryConfig = new ZenArtilleryConfig;
		m_ZenArtilleryConfig.Load();
	}

	return m_ZenArtilleryConfig;
};