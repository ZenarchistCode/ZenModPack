class LeftoversConfig
{
	// Set master config version (not saved to json)
	private static const string CONFIG_VERSION = "2";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenLeftoversConfig.json";

	// Config data
	string ConfigVersion = "";
	int CraftJunkHookHP = 10;
	int JunkLifetimeSecs = 1800;
	ref array<ref LeftoverItem> LeftoverItems = new array<ref LeftoverItem>;

	// Load config file or create default file if config doesn't exsit
	void Load() 
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	// If config exists, load file
			JsonFileLoader<LeftoversConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<LeftoversConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
				ConfigVersion = CONFIG_VERSION;
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Clear old config if it exists
		LeftoverItems.Clear();

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Set default config
		CraftJunkHookHP = 10;
			
		// Set default leftover items
		LeftoverItems.Insert(new LeftoverItem("SodaCan_Pipsi", "Empty_SodaCan_Pipsi", 0));
		LeftoverItems.Insert(new LeftoverItem("SodaCan_Cola", "Empty_SodaCan_Cola", 0));
		LeftoverItems.Insert(new LeftoverItem("SodaCan_Spite", "Empty_SodaCan_Spite", 0));
		LeftoverItems.Insert(new LeftoverItem("SodaCan_Kvass", "Empty_SodaCan_Kvass", 0));
		LeftoverItems.Insert(new LeftoverItem("SodaCan_Fronta", "Empty_SodaCan_Fronta", 0));
		LeftoverItems.Insert(new LeftoverItem("BoxCerealCrunchin", "Empty_BoxCerealCrunchin", 1));
		LeftoverItems.Insert(new LeftoverItem("Rice", "Empty_Rice", 1));
		LeftoverItems.Insert(new LeftoverItem("PowderedMilk", "Empty_PowderedMilk", 1));
		LeftoverItems.Insert(new LeftoverItem("Marmalade", "Empty_Marmalade", 0));
		LeftoverItems.Insert(new LeftoverItem("Honey", "Empty_Honey", 0));
		LeftoverItems.Insert(new LeftoverItem("Zagorky", "Empty_Zagorky", 1));
		LeftoverItems.Insert(new LeftoverItem("ZagorkyChocolate", "Empty_ZagorkyChocolate", 1));
		LeftoverItems.Insert(new LeftoverItem("ZagorkyPeanuts", "Empty_ZagorkyPeanuts", 1));
		LeftoverItems.Insert(new LeftoverItem("SaltySticks", "Empty_SaltySticks", 1));
		LeftoverItems.Insert(new LeftoverItem("Crackers", "Empty_Crackers", 1));
		LeftoverItems.Insert(new LeftoverItem("Chips", "Empty_Chips", 1));
		LeftoverItems.Insert(new LeftoverItem("BakedBeansCan_Opened", "Empty_BakedBeansCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("PeachesCan_Opened", "Empty_PeachesCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("TacticalBaconCan_Opened", "Empty_TacticalBaconCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("SpaghettiCan_Opened", "Empty_SpaghettiCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("UnknownFoodCan_Opened", "Empty_UnknownFoodCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("SardinesCan_Opened", "Empty_SardinesCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("TunaCan_Opened", "Empty_TunaCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("DogFoodCan_Opened", "Empty_DogFoodCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("CatFoodCan_Opened", "Empty_CatFoodCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("PorkCan_Opened", "Empty_PorkCan_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("Lunchmeat_Opened", "Empty_Lunchmeat_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("Pate_Opened", "Empty_Pate_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("Pajka_Opened", "Empty_Pate_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("BrisketSpread_Opened", "Empty_BrisketSpread_Opened", 0));
		LeftoverItems.Insert(new LeftoverItem("BloodBagIV", "Used_BloodBagIV", 1, true));
		LeftoverItems.Insert(new LeftoverItem("SalineBagIV", "Used_SalineBagIV", 1, true));
		LeftoverItems.Insert(new LeftoverItem("Morphine", "Used_Morphine", 1, true));
		LeftoverItems.Insert(new LeftoverItem("Epinephrine", "Used_Epinephrine", 1, true));
		LeftoverItems.Insert(new LeftoverItem("AntiChemInjector", "Used_AntiChemInjector", 1, true));
		// My custom items 
		LeftoverItems.Insert(new LeftoverItem("ZenJameson", "Empty_ZenJameson", 0));
		LeftoverItems.Insert(new LeftoverItem("ZenFlask", "Empty_ZenFlask", 0));
		// Example modded items
		LeftoverItems.Insert(new LeftoverItem("MassMREPB", "MassMREPB", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREJelly", "MassMREJelly", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREGrapeJelly", "MassMREGrapeJelly", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMRETobasco", "MassMRETobasco", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREHotCheeseSpread", "MassMREHotCheeseSpread", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREDBBPudding", "MassMREDBBPudding", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREDBBCobbler", "MassMREDBBCobbler", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREMCSpaghetti", "MassMREMCSpaghetti", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREMCBeefTaco", "MassMREMCBeefTaco", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREMCChilliMac", "MassMREMCChilliMac", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMREMCBeefStew", "MassMREMCBeefStew", 1));
		LeftoverItems.Insert(new LeftoverItem("MassMRETortillaW", "MassMRETortillaW", 1));
		// Save config
		Save();
	}

	// Save config
	void Save() 
	{
		if (!FileExist(zenModFolder))
		{ // If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<LeftoversConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Get an item from config list
	LeftoverItem GetLeftoverItem(string originalItem)
	{
		string itemType = originalItem;
		itemType.ToLower();
		for (int i = 0; i < LeftoverItems.Count(); i++) 
		{
			string toFind = LeftoverItems.Get(i).OriginalItemType;
			toFind.ToLower();
			if (itemType.Contains(toFind))
			{
				return LeftoverItems.Get(i);
			}
		}

		return new LeftoverItem();
	}
};

// Define a custom leftover item's types
class LeftoverItem 
{
	string OriginalItemType = ""; // Original item type name
	string LeftoverItemType = ""; // Replacement junk item type name
	int ItemHealth = 100; // Optional health parameter: -1 means ruined, 0 means copy original item health, anything else = item HP (eg. 1 = badly damaged & almost ruined)
	bool DropToGround = false; // 

	void LeftoverItem(string originalItemParam = "", string leftoverItemParam = "", int healthParam = 0, bool dropParam = false)
	{
		OriginalItemType = originalItemParam;
		LeftoverItemType = leftoverItemParam;
		ItemHealth = healthParam;
		DropToGround = dropParam;
	}
};

// Save config data
ref LeftoversConfig m_LeftoversConfig;

// Helper function to return Config data storage object
static LeftoversConfig GetLeftoversConfig()
{
	#ifdef SERVER
	if (!m_LeftoversConfig)
	{
		ZMPrint("[ZenLeftovers] Init");
		m_LeftoversConfig = new LeftoversConfig;
		m_LeftoversConfig.Load();
	}
	#endif

	return m_LeftoversConfig;
};

