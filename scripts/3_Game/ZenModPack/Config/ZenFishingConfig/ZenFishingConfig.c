ref ZenFishingConfig g_ZenFishingConfig;

static ZenFishingConfig GetZenFishingConfig()
{
	if (!g_ZenFishingConfig) GetZenConfigRegister().RegisterConfig(ZenFishingConfig);
	return g_ZenFishingConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenFishingConfig);
	}
}

class ZenFishingConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenFishingConfig = this;
	}
	
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool		ShouldSyncToClient() 		
	{
		return ZenModEnabled("ZenFishingConfig"); 
	}

	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenFishingConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenFishingConfig>.SaveFile(path, this, err);
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenFishingConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenFishingConfig>.LoadData(payload, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	ref map<string, int> FishingJunk;
	ref map<string, int> FishingJunkEmpty;
	
	ref array<float> DefaultFishHourlyCycleCoefs;
	float DefaultFishQualityBase;
	ref array<ref ZenFishingFishEntry> Fish;

	override void SetDefaults()
	{
		FishingJunk = new map<string, int>;
		FishingJunkEmpty = new map<string, int>;
		DefaultFishHourlyCycleCoefs = new array<float>;
		Fish = new array<ref ZenFishingFishEntry>;
		DefaultFishQualityBase = 0;
		
		/*
		Print("MASK_ENVIRO_POND=" + AnimalCatchingConstants.MASK_ENVIRO_POND);
		Print("MASK_ENVIRO_SEA=" + AnimalCatchingConstants.MASK_ENVIRO_SEA);
		Print("MASK_ENVIRO_FOREST=" + AnimalCatchingConstants.MASK_ENVIRO_FOREST);
		Print("MASK_ENVIRO_FIELD=" + AnimalCatchingConstants.MASK_ENVIRO_FIELD);
		Print("MASK_ENVIRO_WATER_ALL=" + AnimalCatchingConstants.MASK_ENVIRO_WATER_ALL);
		Print("MASK_ENVIRO_LAND_ALL=" + AnimalCatchingConstants.MASK_ENVIRO_LAND_ALL);
		
		Print("MASK_METHOD_ROD=" + AnimalCatchingConstants.MASK_METHOD_ROD);
		Print("MASK_METHOD_FISHTRAP_LARGE=" + AnimalCatchingConstants.MASK_METHOD_FISHTRAP_LARGE);
		Print("MASK_METHOD_FISHTRAP_SMALL=" + AnimalCatchingConstants.MASK_METHOD_FISHTRAP_SMALL);
		Print("MASK_METHOD_LANDTRAP_SNARE=" + AnimalCatchingConstants.MASK_METHOD_LANDTRAP_SNARE);
		*/

		FishingJunk.Set("Wellies_Brown", 4);
		FishingJunk.Set("Wellies_Grey", 4);
		FishingJunk.Set("Wellies_Green", 4);
		FishingJunk.Set("Wellies_Black", 4);
		FishingJunk.Set("Compass", 1);
		FishingJunk.Set("OrienteeringCompass", 1);
		FishingJunk.Set("ChernarusMap", 1);
		FishingJunk.Set("PlantMaterial", 2);
		FishingJunk.Set("Zen_Empty_SodaCan_Random", 1);
		FishingJunk.Set("Rope", 1);
		FishingJunk.Set("WoodenStick", 1);
		FishingJunk.Set("Netting", 1);
		FishingJunk.Set("MetalWire", 1);

		FishingJunkEmpty.Set("Pot", 4);
		FishingJunkEmpty.Set("Empty_ZenFlask", 1);
		FishingJunkEmpty.Set("WaterBottle", 1);
		FishingJunkEmpty.Set("Canteen", 1);
		
		// Vanilla FishYieldItemBase defaults (hourly coefs + quality) 
		DefaultFishHourlyCycleCoefs = {1,1,1,1,0.5,0.2,0,0,0,0.2,0.5,1,1,1,1,1,0.75,0.5,0.2,0,0,0,0.2,0.5};
		DefaultFishQualityBase = AnimalCatchingConstants.QUALITY_FISH_BASE;

		Fish.Clear();
		
		// Why does this result in zero? Script bug?
		//int rodLargeTrapMask = AnimalCatchingConstants.MASK_METHOD_ROD|AnimalCatchingConstants.MASK_METHOD_FISHTRAP_LARGE;
		int rodLargeTrapMask = 1;

		// IMPORTANT: vanilla fish weights differ per world:
		// - Chernarus/Enoch use MissionBase.InitWorldYieldDataDefaults (Carp/Mackerel/Sardines/Bitterlings). 
		// - Sakhal overrides InitYieldBank and uses Pollock/Steelhead/Shrimp. 

		string world = g_Game.GetWorldName();
		world.ToLower();
		Print("[ZenFishingJunk] WorldName=" + world);

		if (world == "sakhal")
		{
			AddFish("WalleyePollock", 61, AnimalCatchingConstants.MASK_ENVIRO_SEA,
				rodLargeTrapMask,
				{1,1,1,1,1,1,1,0.8,0.7,0.5,0.2,0,0,0,0,0,0.2,0.5,0.7,0.8,1,1,1,1});

			AddFish("SteelheadTrout", 36, AnimalCatchingConstants.MASK_ENVIRO_SEA,
				rodLargeTrapMask,
				{1,1,1,1,1,1,1,1,1,0.5,0.2,0,0,0,0,0.2,0.5,0.7,1,1,1,1,1,1});

			AddFish("Shrimp", 1, AnimalCatchingConstants.MASK_ENVIRO_SEA,
				AnimalCatchingConstants.MASK_METHOD_FISHTRAP_SMALL,
				{1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1});
		}
		else
		{
			// Weights come from MissionBase.InitWorldYieldDataDefaults 
			AddFish("Carp",        42, AnimalCatchingConstants.MASK_ENVIRO_POND, rodLargeTrapMask, null);
			AddFish("Mackerel",    42, AnimalCatchingConstants.MASK_ENVIRO_SEA, rodLargeTrapMask, null);
			AddFish("Sardines",    15, AnimalCatchingConstants.MASK_ENVIRO_SEA, AnimalCatchingConstants.MASK_METHOD_FISHTRAP_SMALL, null); 
			AddFish("Bitterlings", 15, AnimalCatchingConstants.MASK_ENVIRO_POND, AnimalCatchingConstants.MASK_METHOD_FISHTRAP_SMALL, null);
			AddFish("Shrimp", 10, AnimalCatchingConstants.MASK_ENVIRO_WATER_ALL, AnimalCatchingConstants.MASK_METHOD_FISHTRAP_SMALL, {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1}); 
		}
	}
	
	protected void AddFish(string type, int weight, int envMask, int methodMask, array<float> coefs)
	{
		Print(string.Format("[ZenFishingConfig] AddFish %1 env=%2 method=%3 weight=%4", type, envMask, methodMask, weight));
	
		ZenFishingFishEntry e = new ZenFishingFishEntry();
		e.Type = type;
		e.Weight = weight;
		e.EnviroMask = envMask;
		e.MethodMask = methodMask;
		e.HourlyCycleCoefs = coefs; // null => uses DefaultFishHourlyCycleCoefs
		Fish.Insert(e);
	}
	
	/*
	override void DoDebugPrint(string trigger = "")
	{
		foreach (string s1, int s2 : FishingJunk)
		{
			Print("ZenFishingJunk - " + s1 + "=" + s2);
		}
		
		foreach (string s3, int s4 : FishingJunkEmpty)
		{
			Print("FishingJunkEmpty - " + s3 + "=" + s4);
		}
	}
	*/
}

class ZenFishingFishEntry
{
	string Type;          // e.g. "Carp"
	int Weight;           // baseWeight used by YieldItemBase
	int EnviroMask;       // AnimalCatchingConstants.MASK_ENVIRO_*
	int MethodMask;       // AnimalCatchingConstants.MASK_METHOD_*

	// Optional overrides
	float QualityBase = -1;                 // -1 = use config DefaultFishQualityBase
	ref array<float> HourlyCycleCoefs;      // null/empty = use config DefaultFishHourlyCycleCoefs
	float ChanceMult = 1.0;                 // multiplies super.GetChanceForYieldItem(ctx)
}