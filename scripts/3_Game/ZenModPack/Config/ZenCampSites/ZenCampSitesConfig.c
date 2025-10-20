class ZenCampSitesConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenCampSitesConfig.json";

	// Main config data
	string ConfigVersion = "";
	bool LogsOn = false;
	bool DebugOn = false;
	int SpawnSecs = 2700; // 2700 = 45 mins
	int PlayerDetectZone = 300; // 300m
	int MinHealthPercent = 33;
	int MaxHealthPercent = 100;
	float MinFireDistance = 3;
	float MaxFireDistance = 4;
	int MinZombies = 0;
	int MaxZombies = 2;
	ref array<string> ZombieTypes = new array<string>;
	ref array<ref PredefinedLootSpawn> PredefinedLoot = new array<ref PredefinedLootSpawn>;
	ref array<ref CampSite> CampSites = new array<ref CampSite>;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenCampSitesConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenCampSitesConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		ZombieTypes.Clear();
		PredefinedLoot.Clear();
		CampSites.Clear();

		// Get map name
		string mapName = GetGame().GetWorldName();
		mapName.ToLower();

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Insert zombies
		ZombieTypes.Insert("ZmbM_HunterOld_Autumn");
		ZombieTypes.Insert("ZmbM_HunterOld_Spring");
		ZombieTypes.Insert("ZmbM_HunterOld_Summer");
		ZombieTypes.Insert("ZmbM_HunterOld_Winter");
		ZombieTypes.Insert("ZmbM_HermitSkinny_Beige");
		ZombieTypes.Insert("ZmbM_HermitSkinny_Black");
		ZombieTypes.Insert("ZmbM_HermitSkinny_Green");
		ZombieTypes.Insert("ZmbM_HermitSkinny_Red");
		ZombieTypes.Insert("ZmbM_FishermanOld_Blue");
		ZombieTypes.Insert("ZmbM_FishermanOld_Green");
		ZombieTypes.Insert("ZmbM_FishermanOld_Grey");
		ZombieTypes.Insert("ZmbM_FishermanOld_Red");

		// Define default predefined loot
		PredefinedLootSpawn huntingLoot = new PredefinedLootSpawn("HuntingLoot");
		huntingLoot.Loot.Insert(new LootSpawn("Mosin9130_Camo", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("PUScopeOptic", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x54", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("DeerSteakMeat", 1, 6, 0.75));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x39", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x39", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_762x39", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("HuntingKnife", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Compass", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Winchester70", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("HuntingOptic", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_308Win", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("TetracyclineAntibiotics", 1, 12, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Carp", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("GPSReceiver", 1, 1, 0.2));
		huntingLoot.Loot.Insert(new LootSpawn("Bait", 1, 3, 0.33));
		huntingLoot.Loot.Insert(new LootSpawn("BoneBait", 1, 3, 0.33));
		huntingLoot.Loot.Insert(new LootSpawn("Nail", 1, 70, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("VitaminBottle", 1, 12, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("BearSteakMeat", 1, 3, 0.75));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_22", 10, 50, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_22", 10, 50, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_22", 10, 50, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Magnum", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_357", 10, 20, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_357", 10, 50, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Ammo_357", 10, 50, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("LongHorn", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Heatpack", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("UnknownFoodCan", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("MKII", 1, 1, 0.5));
		huntingLoot.Loot.Insert(new LootSpawn("Mag_MKII_10Rnd", 1, 1, 0.5));

		if (mapName == "namalsk")
		{
			huntingLoot.Loot.Insert(new LootSpawn("dzn_NamalskMap", 1, 1, 0.5));
		}
		else
		{
			huntingLoot.Loot.Insert(new LootSpawn("ChernarusMap", 1, 1, 0.5));
		}

		PredefinedLoot.Insert(huntingLoot);

		//! TODO: Add chernarus default config

		// Insert default camps based on map
		if (mapName == "chernarusplus")
		{

		} else
		if (mapName == "enoch")
		{
			// Hunting camps
			CampSites.Insert(new CampSite("HuntingCamp", "620.979980 447.176208 12031.560547", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "3120.632568 256.021912 11843.434570", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "1355.235962 397.757507 11287.964844", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "1775.639160 305.709930 11329.270508", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "4418.835938 231.009308 11447.009766", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "6315.587891 189.267776 11475.123047", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "7104.383301 254.771240 12345.489258", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "7784.236816 173.925766 11815.803711", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "8913.566406 223.973801 12360.272461", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "9417.135742 173.952316 11844.597656", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "9822.388672 172.276489 11617.090820", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "10812.285156 182.144119 11617.397461", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11888.949219 273.951385 12203.635742", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11311.755859 252.875854 12518.887695", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "12364.637695 218.443909 12623.344727", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11337.068359 173.648605 11246.863281", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "12371.926758 170.950287 10890.107422", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11803.323242 173.606110 11034.299805", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "3589.331543 250.167419 10763.105469", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "2915.202393 216.359390 9468.896484", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "2197.920654 178.071167 8048.816406", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "512.982483 201.823868 8029.538086", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "871.300354 301.486145 9610.497070", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "1060.556885 268.068085 8666.416016", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "2387.775391 220.315857 8885.740234", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "844.260437 338.584442 10388.509766", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "7498.920410 246.347824 10690.374023", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "8473.406250 235.169617 10926.871094", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "10506.172852 213.210175 10117.380859", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11293.255859 175.188400 10420.676758", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "7419.764648 277.073669 9443.128906", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "12125.447266 235.618805 8841.250000", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "10344.944336 288.682220 7781.329102", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "5840.477051 228.866394 8077.209961", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "5999.079590 317.546112 6167.663086", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "5208.020996 305.352600 6177.275391", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "3301.659912 279.584930 5843.296387", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "3975.107910 243.150879 6407.535156", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "8221.912109 252.021774 7162.993652", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "2422.895020 309.080078 6405.990723", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "1074.020874 280.797302 6182.577148", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "795.555115 193.627106 7346.588379", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "394.959259 170.851730 7375.050781", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "5876.468262 173.641724 10478.366211", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "3447.679199 170.383530 8991.466797", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "438.220886 253.713318 6385.443359", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "11881.860352 286.238464 6364.807617", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("HuntingCamp", "10293.116211 265.269653 7038.465332", "ZenCamps_ShelterLeather"));
			// River camps
			CampSites.Insert(new CampSite("RiverHuntingCamp", "2167.746582 172.156479 7936.093750", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "2499.664795 175.080780 8191.466309", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "3236.678711 174.209579 8539.329102", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "3346.230713 175.200302 9259.276367", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "4052.258789 180.220200 9287.438477", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "4254.172852 177.367203 8805.427734", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "5734.839844 174.329849 10261.946289", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "5878.734375 176.735565 10559.238281", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "6179.151367 175.887894 10819.055664", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "6046.827148 180.731750 11013.015625", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "6326.828125 172.101746 10854.616211", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "7668.125977 172.462799 11718.085938", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "8019.500977 174.690826 11823.327148", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "8161.202637 173.772354 11985.760742", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "9418.880859 175.598755 11784.750000", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("RiverHuntingCamp", "11275.781250 176.523697 11151.088867", "ZenCamps_ShelterLeather"));
		}
		else
		if (GetGame().GetWorldName() == "namalsk")
		{
			CampSites.Insert(new CampSite("Camp", "4643.138184 147.140015 7466.028320", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5322.959473 2.254150 10249.319336", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6387.221191 8.884911 11132.167969", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5163.647949 2.087982 11478.932617", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6555.279785 3.286298 12010.170898", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7625.961914 14.231907 11635.702148", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "8661.403320 7.400011 11127.309570", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "8885.369141 14.942801 10039.025391", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "8084.120117 4.452200 10081.491211", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6672.361816 14.868994 10524.717773", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4221.182617 3.543552 10634.899414", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6555.167480 25.289000 9754.090820", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5907.802246 11.233980 9814.749023", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4619.043457 5.428899 10079.748047", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7777.672852 10.040771 9434.399414", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6939.533691 5.593505 8561.101563", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "8047.984375 8.431540 8177.562500", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6468.424316 2.899099 7595.597656", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "8213.768555 6.052248 6156.052246", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7695.100586 13.804774 5787.046387", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6734.763184 10.812622 5615.458496", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6650.670898 8.988511 6554.023926", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7297.548828 238.842697 7018.933594", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5491.818848 77.799797 6020.127930", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5946.761230 7.384638 5586.881348", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6184.469238 8.682106 6180.355957", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6277.546387 4.538385 5861.136719", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6596.238770 6.119870 5850.604980", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6114.487305 4.350614 6674.482910", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5870.557129 6.406547 7424.149902", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6194.821289 1.833870 8413.541016", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5605.861816 118.256775 8003.210938", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5268.450684 78.356323 9355.341797", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4628.126953 55.431889 9432.307617", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3880.455811 158.404068 8524.665039", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5262.750977 40.139656 8433.265625", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4705.338867 41.272293 8115.554199", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3937.678223 4.630241 9396.667969", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3567.970947 4.566809 8898.000977", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3152.129883 8.135510 8296.623047", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3362.713623 136.373901 7494.893555", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "2658.202637 4.357471 7152.052246", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3596.422363 52.289646 7201.944824", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3752.339600 39.088200 6159.705566", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3627.454346 41.441189 6447.234375", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5065.383301 3.901421 6098.248535", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4917.762207 39.735641 6322.909180", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4342.950684 131.026260 5358.545410", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4866.387695 2.947811 5167.739258", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "4065.170410 5.714513 4767.069824", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3927.912109 9.334865 5412.506348", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3799.066406 1.801640 5901.798828", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3464.182373 2.254441 6077.235840", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3063.234131 5.584246 5787.808105", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "2510.179199 13.043930 5594.870605", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "2143.708252 6.503868 5860.569336", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "2221.809326 2.935515 6329.210938", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "3176.899902 75.125336 7120.772461", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7660.383789 76.967079 8656.071289", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7563.249023 41.329926 6226.062500", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "7280.078613 134.275269 6476.583984", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "5883.850098 34.093315 9053.133789", "ZenCamps_ShelterLeather"));
			CampSites.Insert(new CampSite("Camp", "6916.572266 104.595352 7318.408203", "ZenCamps_ShelterLeather"));
		}

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{	// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenCampSitesConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Shuffle the loot arrays so loot isn't always spawned in the same config order to get better dispersion/randomness
	void ShuffleLoot()
	{
		foreach(PredefinedLootSpawn spawn : PredefinedLoot)
		{
			spawn.Loot.ShuffleArray();
		}

		foreach(CampSite camp : CampSites)
		{
			if (camp.Loot)
			{
				camp.Loot.ShuffleArray();
			}
		}
	}

	// Get predefined loot
	PredefinedLootSpawn GetPredefinedLoot(string name)
	{
		foreach (PredefinedLootSpawn spawn : PredefinedLoot)
		{
			if (spawn.ConfigName == name)
			{
				return spawn;
			}
		}

		return NULL;
	}
}

// Define a camp site location
class CampSite
{
	string CampName;
	vector Location;
	string TentType;
	string PredefinedLoot;
	ref array<ref LootSpawn> Loot = new array<ref LootSpawn>;

	void CampSite(string p_campname, vector p_camploc, string p_tentname, string p_loot = "HuntingLoot")
	{
		CampName = p_campname;
		Location = p_camploc;
		TentType = p_tentname;
		PredefinedLoot = p_loot;
	}
};

// Define a loot spawn item
class LootSpawn
{
	string ClassName;
	int MinQuantity;
	int MaxQuantity;
	float SpawnChance;

	void LootSpawn(string p_type, int p_min_quantity, int p_max_quantity, float p_chance)
	{
		this.ClassName = p_type;
		this.MinQuantity = p_min_quantity;
		this.MaxQuantity = p_max_quantity;
		this.SpawnChance = p_chance;
	}
};

// Define default loot config
class PredefinedLootSpawn
{
	string ConfigName;
	int MinZombies = 0;
	int MaxZombies = 2;
	ref array<ref LootSpawn> Loot = new array<ref LootSpawn>;

	void PredefinedLootSpawn(string name)
	{
		ConfigName = name;
	}
};

// Save config data
ref ZenCampSitesConfig m_ZenCampSitesConfig;

// Helper function to return Config data storage object
static ZenCampSitesConfig GetZenCampSitesConfig()
{
	if (!m_ZenCampSitesConfig)
	{
		Print("[ZenCampSitesConfig] Init for map=" + GetGame().GetWorldName());
		m_ZenCampSitesConfig = new ZenCampSitesConfig;

		// Only load JSON config on the server
		if (GetGame().IsDedicatedServer())
		{
			m_ZenCampSitesConfig.Load();
		}
	}

	return m_ZenCampSitesConfig;
};