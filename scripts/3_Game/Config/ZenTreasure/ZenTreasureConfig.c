class ZenTreasureConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenTreasureConfig.json";

	// Main config data
	string ConfigVersion = "";
	bool DebugTeleportToPhoto = false;
	int DebugAlwaysSpawnStashID = -1;
	int TreasurePersistenceSecs = 3600; // 1 hour, after player enters trigger & spawns stash.
	bool IsWinterMap = false;
	static ref array<string>					TreasureDescriptions	= new array<string>;
	ref array<ref ZenTreasurePredefinedType>	PredefinedTypes			= new array<ref ZenTreasurePredefinedType>;
	ref array<ref ZenTreasureStashType>			TreasureTypes			= new array<ref ZenTreasureStashType>;

	void Load()
	{
		// Load JSON on server only
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenTreasureConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenTreasureConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				ShuffleLoot();
				return;
			}
		}

		ConfigVersion = CONFIG_VERSION;
		SetupDefaultConfig();
		Save();
	}

	void SetupDefaultConfig()
	{
		// Prepare predefined loot types 
		// SVD
		ZenTreasureLootSpawn predef_SVD = new ZenTreasureLootSpawn("SVD", 1, 1);
		predef_SVD.Attachments.Insert("Mag_SVD_10Rnd");
		predef_SVD.Attachments.Insert("PSO11Optic|h=0.01|c=0.5");
		predef_SVD.Attachments.Insert("Battery9V");
		predef_SVD.Attachments.Insert("AmmoBox_762x54Tracer_20Rnd|c=0.9");
		predef_SVD.Attachments.Insert("AmmoBox_762x54Tracer_20Rnd|c=0.9");
		PredefinedTypes.Insert(new ZenTreasurePredefinedType("Predef_SVD", predef_SVD));
		// Mosin
		ZenTreasureLootSpawn predef_Mosin = new ZenTreasureLootSpawn("Mosin9130|Mosin9130_Camo|Mosin9130_Black|Mosin9130_Green", 1, 1);
		predef_Mosin.Attachments.Insert("PUScopeOptic");
		predef_Mosin.Attachments.Insert("AmmoBox_762x54Tracer_20Rnd");
		predef_Mosin.Attachments.Insert("AmmoBox_762x54Tracer_20Rnd|c=0.9");
		PredefinedTypes.Insert(new ZenTreasurePredefinedType("Predef_Mosin", predef_Mosin));
		// Winchester
		ZenTreasureLootSpawn predef_Winchester = new ZenTreasureLootSpawn("Winchester70|Winchester70_Black|Winchester70_Green", 1, 1);
		predef_Winchester.Attachments.Insert("HuntingOptic");
		predef_Winchester.Attachments.Insert("AmmoBox_308WinTracer_20Rnd");
		predef_Winchester.Attachments.Insert("AmmoBox_308WinTracer_20Rnd|c=0.9");
		PredefinedTypes.Insert(new ZenTreasurePredefinedType("Predef_Winchester", predef_Winchester));
		// AmmoBox 
		ZenTreasureLootSpawn predef_AmmoBox = new ZenTreasureLootSpawn("AmmoBox", 1, 1);
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		predef_AmmoBox.Attachments.Insert("ZenRandomAmmoBox");
		PredefinedTypes.Insert(new ZenTreasurePredefinedType("Predef_AmmoBox", predef_AmmoBox));
		// NBC Case
		ZenTreasureLootSpawn predef_NBC = new ZenTreasureLootSpawn("NBC_Case_DW", 1, 1);
		predef_NBC.Attachments.Insert("NBCGlovesGray|c=0.5");
		predef_NBC.Attachments.Insert("NBCHoodGray|c=0.5");
		predef_NBC.Attachments.Insert("NBCPantsGray|c=0.5");
		predef_NBC.Attachments.Insert("NBCBootsGray|c=0.5");
		predef_NBC.Attachments.Insert("GasMask|c=0.5");
		predef_NBC.Attachments.Insert("GasMaskWinter|c=0.5");
		predef_NBC.Attachments.Insert("NBCBootsWinter|c=0.5");
		predef_NBC.Attachments.Insert("NBCPantsWinter|c=0.5");
		predef_NBC.Attachments.Insert("NBCHoodWinter|c=0.5");
		predef_NBC.Attachments.Insert("NBCGlovesWinter|c=0.5");
		PredefinedTypes.Insert(new ZenTreasurePredefinedType("Predef_NBC", predef_NBC));

		// 0 - Generic loot small (wooden crate) - filled with all possible loot from other configs
		TreasureTypes.Insert(new ZenTreasureStashType("Generic_Small", "valuable", "ZenTreasure_WoodenCrate"));

		// 1 - Generic loot large (sea chest) - filled with all possible loot from other configs
		TreasureTypes.Insert(new ZenTreasureStashType("Generic_Large", "valuable", "ZenTreasure_SeaChest"));

		// 2 - Hunting loot (wooden crate)
		ZenTreasureStashType huntingLootSmall = new ZenTreasureStashType("HuntingLoot_Small", "hunting", "ZenTreasure_WoodenCrate");
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Predef_Winchester", 1, 1, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Predef_Mosin", 1, 1, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox_250", 1, 1, 0.1));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Whetstone", 1, 1, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("WeaponCleaningKit", 1, 1, 0.9));
		huntingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Expansion_Ammo_FlareSupplyRed", 1, 1, 0.1));
		TreasureTypes.Insert(huntingLootSmall);

		// 3 - Basebuilding loot (wooden crate)
		ZenTreasureStashType basebuildingLootSmall = new ZenTreasureStashType("Basebuilding_Small", "basebuilding", "ZenTreasure_WoodenCrate");
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox_250", 1, 1, 0.25));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("ExpansionCodeLock", 1, 1, 1.0));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("HandSaw", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Hatchet", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Pliers", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Whetstone", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Whetstone", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Whetstone", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Whetstone", 1, 1, 0.9));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.2));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.2));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.2));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("MetalWire", 1, 1, 0.2));
		basebuildingLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Expansion_Ammo_FlareSupplyGreen", 1, 1, 0.1));
		TreasureTypes.Insert(basebuildingLootSmall);

		// 4 - Military loot SVD (wooden crate)
		ZenTreasureStashType militaryLootSmall = new ZenTreasureStashType("Military_Small", "military", "ZenTreasure_WoodenCrate");
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Predef_SVD", 1, 1, 1.0));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AK_Suppressor", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Mag_SVD_10Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x54Tracer_20Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x54Tracer_20Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_308WinTracer_20Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_308WinTracer_20Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x39_20Rnd", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("MetalWire", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("ExpansionCodeLock", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("WeaponCleaningKit", 1, 1, 0.9));
		militaryLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Expansion_Ammo_FlareSupplyBlue", 1, 1, 0.1));
		TreasureTypes.Insert(militaryLootSmall);

		// 5 - Military loot high grade (sea chest)
		ZenTreasureStashType militaryLootLarge = new ZenTreasureStashType("Military_Large", "high-grade military", "ZenTreasure_SeaChest");
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("Predef_SVD", 1, 1, 1.0));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("AK_Suppressor", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("M4_Suppressor", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("Mag_SVD_10Rnd", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionSatchel", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("Plastic_Explosive", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("RemoteDetonator", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionAmmoRPG", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionRPG7", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionLAW", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionAmmoLAW", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x54Tracer_20Rnd", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x54Tracer_20Rnd", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("NailBox_250", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("AmmoBox_762x39_20Rnd", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("MetalWire", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("ExpansionCodeLock", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("WeaponCleaningKit", 1, 1, 0.9));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("Expansion_Ammo_FlareSupplyRed", 1, 1, 0.1));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("RGD5Grenade", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("M67Grenade", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("LandMineTrap", 1, 1, 0.5));
		militaryLootLarge.Loot.Insert(new ZenTreasureLootSpawn("Predef_AmmoBox", 1, 1, 0.9));
		TreasureTypes.Insert(militaryLootLarge);

		// 6 - Medical (crate Syberia)
		ZenTreasureStashType medicalLootSmall = new ZenTreasureStashType("Medical_Small", "medical", "ZenTreasure_SeaChest");
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("Predef_NBC", 1, 1, 1.0));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleAlcohol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleErytropoenin", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleAdrenalin30", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpoulePentacin", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleCarboxyme", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleMetralindol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleActaparoxetine", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleAmitriptyline", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleZivirol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("AmpouleFlemoclav", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorPromedol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorZivirol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorTopoizomeraza", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorErytropoenin", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorNeiroks", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorAmoksivan", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorKetorol", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorEpipen", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorNovichek", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorRadioprotector", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("InjectorExpBoost10x", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NailBox", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsB190", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsMexamine", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsAgteminol", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsAdepress", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsActaparoxetine", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsAmitriptyline", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("TabletsVenlafaxine", 10, 10, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("SyringeEmpty", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NBCHoodYellow", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NBCGlovesYellow", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NBCPantsYellow", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NBCJacketYellow", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("NBCBootsYellow", 1, 1, 0.6));
		medicalLootSmall.Loot.Insert(new ZenTreasureLootSpawn("GasMask_Filter", 1, 1, 0.6));
		TreasureTypes.Insert(medicalLootSmall);	
	}

	// Shuffle the loot arrays so loot isn't always spawned in the same config order to get better dispersion/randomness
	void ShuffleLoot()
	{
		foreach(ZenTreasureStashType spawn : TreasureTypes)
		{
			spawn.Loot.ShuffleArray();
		}
	}

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenTreasureConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Define a loot spawn item
class ZenTreasureLootSpawn
{
	string ClassName;
	int MinQuantity;
	int MaxQuantity;
	int MinHealth = 10;
	float SpawnChance;
	ref array<string> Attachments = new array<string>;

	void ZenTreasureLootSpawn(string p_type, int p_min_quantity, int p_max_quantity, float p_chance = 1.0)
	{
		this.ClassName = p_type;
		this.MinQuantity = p_min_quantity;
		this.MaxQuantity = p_max_quantity;
		this.SpawnChance = p_chance;
	}
};

// Define predefined type
class ZenTreasurePredefinedType
{
	string ConfigName;
	ref ZenTreasureLootSpawn Item;

	void ZenTreasurePredefinedType(string p_configName, ZenTreasureLootSpawn p_item)
	{
		ConfigName = p_configName;
		Item = p_item;
	}
};

// Define default loot config
class ZenTreasureStashType
{
	string ConfigName;
	string Description;
	string ContainerType;
	ref array<ref ZenTreasureLootSpawn> Loot = new array<ref ZenTreasureLootSpawn>;

	void ZenTreasureStashType(string p_configName, string p_description, string p_containerName)
	{
		ConfigName = p_configName;
		Description = p_description;
		ContainerType = p_containerName;
	}
};

ref ZenTreasureConfig m_ZenTreasureConfig;

static ZenTreasureConfig GetZenTreasureConfig()
{
	if (!m_ZenTreasureConfig)
	{
		ZMPrint("[ZenTreasureConfig] Init");
		m_ZenTreasureConfig = new ZenTreasureConfig;
		m_ZenTreasureConfig.Load();
	}

	return m_ZenTreasureConfig;
}
