modded class MissionServer
{
	//! SHARED
	ref ZenModLogger m_ModLogger;
	ref map<string, int> m_ZenPlayerUIDs;
	static int ZEN_UNIQUE_PLAYER_ID_TRACKER;

	//! NIGHT CONFIG 
	protected int m_ZenNightConfigID;

	override void OnInit()
	{
		super.OnInit();

		//! UTILITIES 
		ZEN_UNIQUE_PLAYER_ID_TRACKER = 0;
		m_ZenPlayerUIDs = new map<string, int>;
		GetZenUtilitiesConfig();
		GetZenPlayerMessageConfig();
		GetZenUpdateMessage();
		GetZenUpdateMessagePersistence();
		GetZenPollConfig();
		GetZenServerDiversionConfig();
		m_ModLogger = new ZenModLogger;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenDeferredInit, 30000, false);

		//! GENERAL CONFIG
		ZMPrint("[ZenModPack] Loading config");
		GetZenModPackConfig();

		//! CAUSE OF DEATH
		if (ZenModEnabled("ZenCauseOfDeath") || ZenModEnabled("ZenGraves"))
			GetCauseOfDeathConfig();

		//! GRAVES
		if (ZenModEnabled("ZenGraves"))
			GetZenGravesConfig();

		//! REPAIR WELLS 
		if (ZenModEnabled("ZenRepairWells"))
			GetZenWellsConfig();

		//! REPAIR PUMPS
		if (ZenModEnabled("ZenRepairPumps"))
			GetZenPumpsConfig();

		//! LEFTOVERS 
		if (ZenModEnabled("ZenLeftovers"))
			GetLeftoversConfig();

		//! ARTILLERY 
		GetZenArtilleryConfig();

		//! TREESPLOSIONS 
		if (ZenModEnabled("ZenTreesplosions"))
			GetTreesplosionsConfig();

		//! NOTES 
		if (ZenModEnabled("ZenNotes"))
			GetZenNotesConfig();

		//! MUSIC 
		if (ZenModEnabled("ZenMusic"))
			GetZenMusicConfig();

		//! BASEBUILDING CONFIG 
		if (ZenModEnabled("ZenBasebuildingConfig"))
			GetZenBasebuildingConfig();

		//! PERSISTENT TREES
		if (ZenModEnabled("ZenPersistentTrees"))
		{
			GetZenTreesConfig();
			GetZenTrees_Load();
			GetZenTrees_Save();
			GetZenTrees_Load().ClearRespawnedTrees();
		}

		//! CHICKEN COOPS
		if (ZenModEnabled("ZenChickenCoops"))
			ZenChickenCoopsInit();

		//! FIREWOOD
		if (ZenModEnabled("ZenFireWood"))
			ZenFireWoodInit();

		//! STATIC BARBED WIRE 
		if (ZenModEnabled("ZenStaticBarbedWire"))
			ZenStaticBarbedWireInit();

		//! CUSTOM DYNAMIC CONTAMINATED ZONE LOOT
		if (ZenModEnabled("ZenDynamicZoneLoot"))
			GetZenContaminatedZoneLootConfig();

		//! IMMERSIVE LOGIN 
		m_ZenDisableFireSpawn = FileExist("$profile:\\Zenarchist\\disablespawnfire.txt");

		//! NIGHT CONFIG 
		if (ZenModEnabled("ZenNightConfig"))
		{
			if (GetZenNightConfig().OvercastToTriggerDarkNights <= 0)
			{
				if (Math.RandomFloat01() < GetZenNightConfig().ChanceOfDarkNight)
				{
					m_ZenNightConfigID = GetZenNightConfig().DarkNightLightingConfigID;
					ZMPrint("[ZenNightConfig] Night lighting config set to dark config id: " + m_ZenNightConfigID);
				}
				else 
				{
					m_ZenNightConfigID = GetZenNightConfig().BrightNightLightingConfigID;
					ZMPrint("[ZenNightConfig] Night lighting config set to bright config id: " + m_ZenNightConfigID);
				}
			}
		}

		//! DISCORD API
		GetZenDiscordConfig();
	}

	void ~MissionServer()
	{
		if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ZenDeferredInit);
		}
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		ZMPrint("[ZenModPack] OnMissionStart");

		//! PERSISTENT TREES 
		if (ZenModEnabled("ZenPersistentTrees"))
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Zen_Deforestation, 2500, false);

		//! GRAVES 
		if (ZenModEnabled("ZenGraves"))
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Zen_SpawnGraves, 5000);

		//! GENERAL SERVER-SIDE DEBUG 
		ZenServerDebugStartup();
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		//! REPAIR WELLS 
		if (ZenModEnabled("ZenRepairWells"))
			GetZenWellsConfig().Save();

		//! REPAIR PUMPS
		if (ZenModEnabled("ZenRepairPumps"))
			GetZenPumpsConfig().Save();

		//! PERSISTENT TREES 
		if (ZenModEnabled("ZenPersistentTrees"))
			GetZenTrees_Save().TransferSavedTrees();

		//! GRAVES 
		if (ZenModEnabled("ZenGraves"))
			Zen_Save_Graves();

		//! UTILITIES
		GetZenUpdateMessagePersistence().Save();
		GetZenPollConfig().UpdateResults();

		//PrintZenPlayerDropCounts();
	}

	//! TODO: Prints any players who dropped more than X items during the session
	/*void PrintZenPlayerDropCounts()
	{
		if (!GetZenUtilitiesConfig().ShouldLogLootCyclers)
			return;

		string time;
		for (int i = 0; i < PlayerBase.ZEN_LOOT_CYCLING_COUNTER.Count(); i++)
		{
			int dropCount = PlayerBase.ZEN_LOOT_CYCLING_COUNTER.GetElement(i);
			if (dropCount > 30)
			{
				int minsPlayed = PlayerBase.ZEN_LOOT_CYCLING_PLAYERAGE.Get(PlayerBase.ZEN_LOOT_CYCLING_COUNTER.GetKey(i)) / 60;
				time = "time played: " + minsPlayed + " minutes";
				int dropRate = dropCount / minsPlayed;
				ZenModLogger.Log(PlayerBase.ZEN_LOOT_CYCLING_COUNTER.GetKey(i) + " dropped " + PlayerBase.ZEN_LOOT_CYCLING_COUNTER.GetElement(i) + " items " + time + " (" + dropRate + "/min)", "lootcycle");
			}
		}

		PlayerBase.ZEN_LOOT_CYCLING_COUNTER.Clear();
		PlayerBase.ZEN_LOOT_CYCLING_PLAYERAGE.Clear();
		delete PlayerBase.ZEN_LOOT_CYCLING_COUNTER;
		delete PlayerBase.ZEN_LOOT_CYCLING_PLAYERAGE;
	}*/

	// Perform some general debuggin'
	private void ZenServerDebugStartup()
	{
		if (GetGame().ConfigGetInt("CfgVehicles ZenModPackConfig dumpCfgVehicles") != 1)
			return;
		
		string cfg_name = "";
		bool foundZenModPack = false;

		ZenModLogger.Log("[ZENMODPACK CFGVEHICLES DUMP START]", "CfgVehicles");
		ZenModLogger.Log("", "CfgVehicles");

		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgVehicles"); i++)
		{
			GetGame().ConfigGetChildName("CfgVehicles", i, cfg_name);

			if (cfg_name == "ZenModPackConfig")
			{
				foundZenModPack = true;
				continue;
			}

			if (cfg_name == "ZenModPack_EndDump")
				break;

			if (foundZenModPack && GetGame().ConfigGetInt("CfgVehicles " + cfg_name + " scope") == 2)
				ZenModLogger.Log(cfg_name, "CfgVehicles", false);
		}

		ZenModLogger.Log("", "CfgVehicles");
		ZenModLogger.Log("[ZENMODPACK CFGVEHICLES DUMP END]", "CfgVehicles");
	}

	override void EquipCharacter(MenuDefaultCharacterData char_data)
	{
		super.EquipCharacter(char_data);

		//! IMMERSIVE LOGIN
		if (!m_ZenDisableFireSpawn)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateFire, 10, false, m_player);
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) 
	{
		super.InvokeOnConnect(player, identity);

		//! SEND GENERAL CONFIG IMMEDIATELY
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenModPackConfig", new Param2<map<string, bool>, map<string, bool>>(GetZenModPackConfig().ModEnabled, GetZenModPackConfig().PersistentModEnabled), true, identity);

		//! UTILITIES 
		if (player && identity)
		{
			ZenModLogger.Log("Player logged in: " + player.GetCachedName() + " (" + player.GetCachedID() + ") (pos=" + player.GetPosition() + ")", "login");

			//! DISCORD WATCHLIST
			string reason;
			if (GetZenDiscordConfig().PlayerWatchlist.Find(identity.GetId(), reason))
			{
				ZenDiscordMessage playerWatchlistMsg = new ZenDiscordMessage("Watchlist");
				playerWatchlistMsg.SetTitle(identity.GetName() + " " + GetZenDiscordConfig().JustLoggedIn + " " + GetZenDiscordConfig().ServerName);
				playerWatchlistMsg.SetMessage(GetZenDiscordConfig().Explanation + "\n\n" + reason + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(player.GetPosition()) + "\n\n" + identity.GetId() + "\n\n[" + identity.GetPlainId() + "](http://steamcommunity.com/profiles/" + identity.GetPlainId() + ")");
				playerWatchlistMsg.SetColor(255, 255, 0);

				foreach(string s : GetZenDiscordConfig().AdminWebhooks)
				{
					playerWatchlistMsg.AddWebhook(s);
				}

				GetZenDiscordAPI().SendMessage(playerWatchlistMsg);
			}

			//! SERVER DIVERSION
			if (GetZenServerDiversionConfig().ServerIP != "")
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenServerDiversionMessage, 6969, false, player);
			}
		}

		//! SHARED 
		int playerUID;
		if (!m_ZenPlayerUIDs.Find(identity.GetId(), playerUID))
		{
			playerUID = ZEN_UNIQUE_PLAYER_ID_TRACKER;
			ZEN_UNIQUE_PLAYER_ID_TRACKER++;
			m_ZenPlayerUIDs.Insert(identity.GetId(), playerUID);
		}

		player.SetZenPlayerUID(playerUID);

		// Delay sending of client config to avoid spamming new client login along with data from other mods
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendNotificationConfig, 500 + Math.RandomInt(0, 500), false, player);

		if (ZenModEnabled("ZenMusic"))
			SendZenMusicConfig(player, identity);

		if (ZenModEnabled("ZenBasebuildingConfig"))
			SendZenBasebuildingConfig(player, identity);
	}

	override void InvokeOnDisconnect(PlayerBase player)
	{
		super.InvokeOnDisconnect(player);

		//! UTILITIES 
		if (player && player.GetIdentity())
		{
			ZenModLogger.Log("Player logged out: " + player.GetCachedName() + " (" + player.GetCachedID() + ")" + " (pos=" + player.GetPosition() + ")", "login");
		}
	}

	//! IMMERSIVE LOGIN 
	bool m_ZenDisableFireSpawn = false;

	// Create a fireplace near the player
	void CreateFire(PlayerBase player)
	{
		if (!player || !ZenModEnabled("ZenImmersiveLogin"))
			return;

		vector posXZ = player.GetPosition();
		float randX = Math.RandomFloatInclusive(2.5, 2.8);
		float randZ = Math.RandomFloatInclusive(2.5, 2.8);

		// Create fire at a safe distance and random location within 2.5m of player
		if (Math.RandomIntInclusive(1, 2) == 1)
			randX = randX * -1;

		if (Math.RandomIntInclusive(1, 2) == 1)
			randZ = randZ * -1;

		// Get fire surface pos
		float x = posXZ[0] + randX;
		float z = posXZ[2] + randZ;
		float y = GetGame().SurfaceY(x, z);
		vector firePos = { x,y,z };

		// Check surface type
		string surface_type;
		GetGame().SurfaceGetType(firePos[0], firePos[2], surface_type);
		surface_type.ToLower();

		// Don't start fires under water
		if (surface_type.Contains("water"))
			return;

		// Create fire
		FireplaceBase fire = FireplaceBase.Cast(GetGame().CreateObject("Fireplace", firePos));

		if (!fire)
			return;

		// If the fire is created under a roof, just delete it
		if (MiscGameplayFunctions.IsUnderRoof(fire))
		{
			fire.DeleteSafe();
			return;
		}

		// All checks passed - create fire and ignite it
		fire.PlaceOnSurface();
		fire.Synchronize();

		// Create tinder & fuel
		ItemBase wood = ItemBase.Cast(fire.GetInventory().CreateAttachment("Firewood"));
		ItemBase stick = ItemBase.Cast(fire.GetInventory().CreateAttachment("WoodenStick"));
		ItemBase rag = ItemBase.Cast(fire.GetInventory().CreateAttachment("Rag"));

		// Lock tinder & fuel
		if (wood)
			wood.LockToParent();

		if (stick)
			stick.LockToParent();
		
		if (rag)
			rag.LockToParent();

		// Ignite fire
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartFire, 100, false, fire);

		// Orient player towards fire 
		ZenFunctions.OrientObjectToPosition(player, fire.GetPosition());
	}

	// Ignite the fire
	void StartFire(FireplaceBase fire)
	{
		if (fire)
		{
			// Ignite fire
			fire.StartFire(true);
			// Put fire out after 12 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExtinguishFire, 12000, false, fire);
			GetGame().CreateObjectEx("ClutterCutterFireplace", fire.GetPosition(), ECE_PLACE_ON_SURFACE|ECE_NOLIFETIME);
		}
	}

	// Extinguish the fire so the player cannot suicide on it
	void ExtinguishFire(FireplaceBase fire)
	{
		if (fire)
		{
			// Set fire extinguish sound effect
			fire.SetExtinguishingState();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StopFire, 3000, false, fire);
		}
	}

	// Stop the fire and set lifetime short (1 minute) so that it gets deleted when player leaves area
	void StopFire(FireplaceBase fire)
	{
		if (fire)
		{
			// Stop fire burning
			fire.StopFire();
			fire.SetLifetime(60);
			int attachments_count = fire.GetInventory().AttachmentCount();

			// Delete all attachments so the player cannot re-ignite the fire
			for (int i = 0; i < attachments_count; i++)
			{
				ItemBase item = ItemBase.Cast(fire.GetInventory().GetAttachmentFromIndex(i));

				if (item)
					item.DeleteSafe();
			}
		}
	}

	//! PERSISTENT TREES 
	// Restore saved "health" of trees and cut down any that have 0 health
	void Zen_Deforestation()
	{
		ZMPrint("[ZenModPack] ZenPersistentTrees::Zen_Deforestation");

		for (int i = 0; i < GetZenTrees_Load().CutTrees.Count(); i++)
		{
			CutDownTree(GetZenTrees_Load().CutTrees.Get(i));
		}

		// Delete loaded trees from memory
		GetZenTrees_Load().Delete();
	}

	// Cut down the given tree based on config info
	void CutDownTree(ZenTreeState treeConfig)
	{
		// Get all objects within 10cm of the vector location
		array<Object> objectsNearTree = new array<Object>;
		GetGame().GetObjectsAtPosition3D(treeConfig.Location, 0.01, objectsNearTree, null);
		Object obj;

		// Cut down any trees/bushes within 0.01m of location
		for (int x = 0; x < objectsNearTree.Count(); x++)
		{
			obj = objectsNearTree.Get(x);

			if (!obj)
				continue;

			// Check if object is a "hard" tree and hard trees config is enabled
			if (GetZenTreesConfig().TreeHard && obj.IsInherited(TreeHard) && obj.IsCuttable())
			{
				obj.SetHealth(treeConfig.Health);

				if (treeConfig.Health <= 0)
					dBodyDestroy(obj);

				return;
			}

			// Check if object is a "soft" tree and soft trees config is enabled
			if (GetZenTreesConfig().TreeSoft && obj.IsInherited(TreeSoft) && obj.IsCuttable())
			{
				obj.SetHealth(treeConfig.Health);

				if (treeConfig.Health <= 0)
					dBodyDestroy(obj);

				return;
			}

			// Check if object is a "hard" bush and hard bush config is enabled
			if (GetZenTreesConfig().BushHard && obj.IsInherited(BushHard))
			{
				obj.SetHealth(treeConfig.Health);

				if (treeConfig.Health <= 0)
					dBodyDestroy(obj);

				return;
			}

			// Check if object is a "soft" bush and soft bush config is enabled
			if (GetZenTreesConfig().BushSoft && obj.IsInherited(BushSoft))
			{
				obj.SetHealth(treeConfig.Health);

				if (treeConfig.Health <= 0)
					dBodyDestroy(obj);

				return;
			}
		}
	}

	//! FIREWOOD 
	void ZenFireWoodInit()
	{
		ZMPrint("[ZenModPack] ZenFirewood::ZenFireWoodInit");

		if (GetZenFirewoodConfig().DumpObjectLocations)
		{
			// Wait 20 secs to ensure all vanilla + modded items are loaded in
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpFirewoodObjects, 20000, false);
		}
		else
		if (GetZenFirewoodConfig().SpawnFirewoodObjects || GetZenFirewoodConfig().DebugOn)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 20000, false);
		}
	}

	// Sets up wood piles across the map
	void SetupWoodPiles()
	{
		ZenFirewoodLogger.Log("Setting up wood piles...");

		string woodModel = "Zen_WoodReserve";
		int minWood;
		int maxWood;
		vector objOffset;
		vector objOrient;

		if (GetZenFirewoodConfig().DebugOn)
			woodModel = "Zen_WoodReserve_Debug";

		foreach (ZenFirewoodObject wood : GetZenFirewoodConfig().WoodPositions)
		{
			objOffset = GetZenFirewoodConfig().GetOffset(wood.TypeName);
			objOrient = GetZenFirewoodConfig().GetOrient(wood.TypeName);

			foreach (vector loc : wood.Locations)
			{
				if (Math.RandomFloat01() > GetZenFirewoodConfig().ChanceOfSpawn)
					continue;

				// Get objects within 1 meter of the config'd wood pile vector
				array<Object> objectsNearWoodpile = new array<Object>;
				GetGame().GetObjectsAtPosition3D(loc, 1, objectsNearWoodpile, null);

				// Prepare reused variables
				string debugName = "";
				string className = "";

				foreach (Object obj : objectsNearWoodpile)
				{
					// Get classname(s)
					debugName = obj.GetDebugNameNative();
					className = obj.ClassName();

					// Convert to lower
					debugName.ToLower();
					className.ToLower();

					// Skip any existing zen wood piles
					if (className.Contains("zen_wood"))
					{
						continue;
					}

					// If map object is what we're looking for, attach an interactive wood pile object to it
					if (debugName.Contains(wood.TypeName) || className.Contains(wood.TypeName))
					{
						CreateWoodPile(obj, woodModel, objOffset, objOrient);
					}
				}
			}
		}

		GetZenFirewoodConfig().CleanUp();
		ZenFirewoodLogger.Log("Done.");
	}

	// Creates an interactive object on the static wood piles
	static void CreateWoodPile(Object parentObj, string childObjType, vector offset, vector orient)
	{
		if (!parentObj)
			return;

		childObjType.ToLower();
		Object newObj = GetGame().CreateObject(childObjType, vector.Zero);
		newObj.SetPosition(parentObj.GetPosition() + offset);
		newObj.SetOrientation(parentObj.GetOrientation() + orient);
		newObj.Update();
	}

	// Dump all objects around the map that match our typenames.
	void DumpFirewoodObjects()
	{
		ZenFirewoodLogger.Log("Start object dump.");

		vector centerPos = GetGame().ConfigGetVector(string.Format("CfgWorlds %1 centerPosition", GetGame().GetWorldName()));

		// Get all objects on the map in a 20km radius from the center of that 20km radius (enough for most maps?)
		array<Object> objectsOnMap = new array<Object>;
		GetGame().GetObjectsAtPosition3D(centerPos, 20000, objectsOnMap, null);
		int objCount = 0;

		foreach(ZenFirewoodType woodType : GetZenFirewoodConfig().WoodTypes)
		{
			string type = woodType.TypeName;
			type.ToLower();

			// Prepare this object
			ZenFirewoodObject woodObj = new ZenFirewoodObject;
			woodObj.TypeName = type;
			array<vector> objLocations = new array<vector>;

			// Cycle through all map objects
			for (int x = 0; x < objectsOnMap.Count(); x++)
			{
				// Prepare classname(s)
				string debugName = "";
				string className = "";

				// Get classname(s)
				debugName = objectsOnMap[x].GetDebugNameNative();
				className = objectsOnMap[x].ClassName();

				// Convert to lower
				debugName.ToLower();
				className.ToLower();

				// If map object is what we're looking for, add it to the array
				if (debugName.Contains(type) || className.Contains(type))
				{
					objLocations.Insert(objectsOnMap[x].GetPosition());
					objCount++;
				}
			}

			// Save this wood object type & all its locations
			woodObj.Locations = objLocations;
			GetZenFirewoodConfig().WoodPositions.Insert(woodObj);
		}

		// Save objects and disable object dump for next restart
		ZenFirewoodLogger.Log("End object dump - found " + objCount + " relevant objects out of a total of " + objectsOnMap.Count() + " map objects.");
		GetZenFirewoodConfig().DumpObjectLocations = false;
		GetZenFirewoodConfig().Save();

		// Clear array & delete from memory
		objectsOnMap.Clear();
		objectsOnMap = NULL;

		// Setup wood piles
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 5000, false);
	}

	//! CHICKEN COOPS
	void ZenChickenCoopsInit()
	{
		ZMPrint("[ZenModPack] ZenChickenCoops::ZenChickenCoopsInit");

		if (GetZenChickenCoopsConfig().DumpObjectLocations)
		{
			// Wait 20 secs to ensure all vanilla + modded items are loaded in
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpChickenCoopObjects, 20000, false);
		}
		else
		if (GetZenChickenCoopsConfig().SpawnChickenCoops || GetZenChickenCoopsConfig().DebugOn)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupChickenCoops, 20000, false);
		}
	}

	// Sets up chicken coops across the map
	void SetupChickenCoops()
	{
		ZenChickenCoopsLogger.Log("Setting up chicken coops...");
		int minFeathers;
		int maxFeathers;
		vector objOffset;
		vector objOrient;

		foreach (ZenChickenCoopObject coop : GetZenChickenCoopsConfig().CoopPositions)
		{
			objOffset = GetZenChickenCoopsConfig().GetOffset(coop.TypeName);
			objOrient = GetZenChickenCoopsConfig().GetOrient(coop.TypeName);

			foreach (vector loc : coop.Locations)
			{
				if (Math.RandomFloat01() > GetZenChickenCoopsConfig().ChanceOfSpawn)
					continue;

				// Get objects within 1 meter of the config'd coop vector
				array<Object> objectsNearCoop = new array<Object>;
				GetGame().GetObjectsAtPosition3D(loc, 1, objectsNearCoop, null);

				// Prepare reused variables
				string debugName = "";
				string className = "";

				foreach (Object obj : objectsNearCoop)
				{
					// Get classname(s)
					debugName = obj.GetDebugNameNative();
					className = obj.ClassName();

					// Convert to lower
					debugName.ToLower();
					className.ToLower();

					// Skip any existing coops
					if (className.Contains("zen_chickencoop"))
					{
						continue;
					}

					// If map object is what we're looking for, attach an interactive chicken coop object to it
					if (debugName.Contains(coop.TypeName) || className.Contains(coop.TypeName))
					{
						string coopModel = GetZenChickenCoopsConfig().GetObjectName(coop.TypeName);

						if (GetZenChickenCoopsConfig().DebugOn)
							coopModel = coopModel + "_Debug";

						CreateZenChickenCoop(obj, coopModel, objOffset, objOrient);
					}
				}
			}
		}

		GetZenChickenCoopsConfig().CleanUp();
		ZenChickenCoopsLogger.Log("Done.");
	}

	// Creates an interactive object on the static chicken coops
	static void CreateZenChickenCoop(Object parentObj, string childObjType, vector offset, vector orient)
	{
		if (!parentObj)
			return;

		childObjType.ToLower();
		Object newObj = GetGame().CreateObject(childObjType, vector.Zero);
		newObj.SetPosition(parentObj.GetPosition() + offset);
		newObj.SetOrientation(parentObj.GetOrientation() + orient);
		newObj.Update();
	}

	// Dump all objects around the map that match our typenames.
	void DumpChickenCoopObjects()
	{
		ZenChickenCoopsLogger.Log("Start object dump.");

		vector centerPos = GetGame().ConfigGetVector(string.Format("CfgWorlds %1 centerPosition", GetGame().GetWorldName()));

		// Get all objects on the map in a 20km radius from the center of that 20km radius (enough for most maps?)
		array<Object> objectsOnMap = new array<Object>;
		GetGame().GetObjectsAtPosition3D(centerPos, 20000, objectsOnMap, null);
		int objCount = 0;

		foreach(ZenChickenCoopType coopType : GetZenChickenCoopsConfig().CoopTypes)
		{
			string type = coopType.TypeName;
			type.ToLower();

			// Prepare this object
			ZenChickenCoopObject coopObj = new ZenChickenCoopObject;
			coopObj.TypeName = type;
			array<vector> objLocations = new array<vector>;

			// Cycle through all map objects
			for (int x = 0; x < objectsOnMap.Count(); x++)
			{
				// Prepare classname(s)
				string debugName = "";
				string className = "";

				// Get classname(s)
				debugName = objectsOnMap[x].GetDebugNameNative();
				className = objectsOnMap[x].ClassName();

				// Convert to lower
				debugName.ToLower();
				className.ToLower();

				// If map object is what we're looking for, add it to the array
				if (debugName.Contains(type) || className.Contains(type))
				{
					objLocations.Insert(objectsOnMap[x].GetPosition());
					objCount++;
				}
			}

			// Save this chicken coop type & all its locations
			coopObj.Locations = objLocations;
			GetZenChickenCoopsConfig().CoopPositions.Insert(coopObj);
		}

		// Save objects and disable object dump for next restart
		ZenChickenCoopsLogger.Log("End object dump - found " + objCount + " relevant objects out of a total of " + objectsOnMap.Count() + " map objects.");
		GetZenChickenCoopsConfig().DumpObjectLocations = false;
		GetZenChickenCoopsConfig().Save();

		// Clear array & delete from memory
		objectsOnMap.Clear();
		objectsOnMap = NULL;

		// Setup chicken coops
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupChickenCoops, 5000, false);
	}

	//! GRAVES 
	void Zen_Save_Graves()
	{
		// Scan through corpses and save their gear before they get deleted on server restart
		if (GetZenGravesConfig().BurySkeleton && m_DeadPlayersArray && m_DeadPlayersArray.Count() > 0)
		{
			ZMPrint("[ZenModPack] ZenGraves::Zen_Save_Graves - Saving " + m_DeadPlayersArray.Count() + " dead player graves.");
			CorpseData corpse_data;

			for (int i = 0; i < m_DeadPlayersArray.Count(); i++)
			{
				corpse_data = m_DeadPlayersArray.Get(i);
				if (corpse_data && corpse_data.m_Player)
				{
					corpse_data.m_Player.SaveZenSkeletonItems();
				}
			}
		}

		// Save dead player config on server shutdown
		GetZenGravesConfig().Save();
	}

	// Spawn any currently active player graves
	void Zen_SpawnGraves()
	{
		ZMPrint("[ZenModPack] ZenGraves::Zen_SpawnGraves - Spawning " + GetZenGravesConfig().DeadPlayers.Count() + " dead player graves.");

		for (int i = GetZenGravesConfig().DeadPlayers.Count() - 1; i >= 0; i--)
		{
			ZenDeadPlayerData deadData = GetZenGravesConfig().DeadPlayers.Get(i);
			Zen_SpawnGrave(deadData);

			// Remove config after cross has been spawned, don't need it anymore.
			GetZenGravesConfig().DeadPlayers.Remove(i);
		}

		GetZenGravesConfig().Save();
	}

	// Spawns a crucifix
	void Zen_SpawnGrave(ZenDeadPlayerData deadData)
	{
		// Delete any duplicate objects nearby cross within 1m
		array<Object> objectsAtCross = new array<Object>;
		GetGame().GetObjectsAtPosition3D(deadData.Position, GetZenGravesConfig().MinDistanceBetweenCrosses, objectsAtCross, null);
		ZenGraves_DeadPlayerCross cross;

		int i;
		for (i = 0; i < objectsAtCross.Count(); i++)
		{
			cross = ZenGraves_DeadPlayerCross.Cast(objectsAtCross[i]);

			if (cross)
				break;
		}

		if (cross)
		{
			// Cross already exists
			GetZenGravesConfig().DebugMsg("Zen_SpawnGrave() - Cross already exists @ " + cross.GetPosition());
			return;
		}
		else
		{
			// Cross does not exist - spawn it in
			cross = ZenGraves_DeadPlayerCross.Cast(GetGame().CreateObject("ZenGraves_DeadPlayerCross", deadData.Position, false, true, true));
		}

		// If cross doesn't exist by now, either something has gone wrong or the cross has already been spawned. Delete the deadData and stop.
		if (!cross)
		{
			GetZenGravesConfig().DebugMsg("[ERROR!] Zen_SpawnGrave() - Grave could not be spawned for cross @ " + deadData.Position);
		}

		// Place cross
		cross.SetOrientation(deadData.Orientation);
		cross.PlaceOnSurface();
		cross.SetPosition(cross.GetPosition() + "0 -0.01 0");
		cross.SetDeathDate(deadData.Day, deadData.Month, deadData.Year, GetZenGravesConfig().DateFormat);
		
		// If server has set cross to not display player info
		if (!GetZenGravesConfig().ShowPlayerInfoOnCross)
		{
			GetZenGravesConfig().DebugMsg("Zen_SpawnGrave() - Don't spawn RPC trigger (ShowPlayerInfoOnCross is false)");
			return;
		}

		// Set cross info
		cross.m_PlayerName = GetZenGravesConfig().Prefix + " " + deadData.Name;
		string description = GetZenGravesConfig().Description;
		if (GetZenGravesConfig().ShowCauseOfDeath)
			description = description + " " + GetZenGravesConfig().CauseOfDeath + " " + deadData.CauseOfDeath;
		description.Replace("#name", deadData.Name);

		// Set player age in description if it's enabled.
		if (GetZenGravesConfig().ShowPlayerAge)
		{
			string ageText = GetZenGravesConfig().AgeText;

			if (deadData.AgeMinutes < 60) // < 1 hour
				ageText = ageText + " " + deadData.AgeMinutes.ToString() + " " + GetZenGravesConfig().Minutes;
			else
			if (deadData.AgeMinutes < 1440) // < 1 day
				ageText = ageText + " " + Math.Round(deadData.AgeMinutes / 60).ToString() + " " + GetZenGravesConfig().Hours;
			else
			if (deadData.AgeMinutes < 10080) // < 1 week
				ageText = ageText + " " + Math.Round(deadData.AgeMinutes / 1440).ToString() + " " + GetZenGravesConfig().Days;
			else
				ageText = ageText + " " + Math.Round(deadData.AgeMinutes / 10080).ToString() + " " + GetZenGravesConfig().Weeks;

			description.Replace("#age", ageText);
		}
		else
			description.Replace("#age", "");

		cross.m_PlayerDescription = description;
		cross.SpawnZenGravesRpcTrigger();
		GetZenGravesConfig().DebugMsg("\Zen_SpawnGrave() - Placed cross @ " + deadData.Position + " for " + deadData.SteamID + " (" + deadData.Name + ")");

		// Spawn skeleton stash
		if (GetZenGravesConfig().BurySkeleton)
			Zen_SpawnDeathSkeleton(deadData, cross);
	}

	// Spawn a buried skeleton
	void Zen_SpawnDeathSkeleton(ZenDeadPlayerData deadData, ZenGraves_DeadPlayerCross cross)
	{
		int i;

		// Check for any duplicate objects nearby cross within 1m
		array<Object> objectsAtCross = new array<Object>;
		GetGame().GetObjectsAtPosition3D(cross.GetPosition(), 1, objectsAtCross, null);

		for (i = 0; i < objectsAtCross.Count(); i++)
		{
			ZenGraves_UndergroundStash nearbyStash = ZenGraves_UndergroundStash.Cast(objectsAtCross[i]);

			// If nearby stash found, stop here and remove deadData inventory items and set bury flag to false
			if (nearbyStash)
			{
				deadData.SkeletonItems.Clear();
				return;
			}
		}

		// Create skeleton
		EntityAI skeletonBuried = EntityAI.Cast(GetGame().CreateObject("ZenGraves_DeadPlayerSkeleton", cross.GetPosition(), ECE_PLACE_ON_SURFACE));
		if (!skeletonBuried || !deadData.SkeletonItems)
		{
			GetZenGravesConfig().DebugMsg("SpawnZenDeathSkeleton() - Failed to spawn skeleton @ " + deadData.Position);
			return;
		}

		skeletonBuried.SetHealth(skeletonBuried.GetMaxHealth() * 0.1); // 10% health, badly damaged

		// Check what items we have to use as storage
		array<ItemBase> skeletonClothing = new array<ItemBase>;
		ZenGraves_InventoryItem skeletonItem;
		ItemBase spawnedItem;

		// Spawn clothing items on skeleton
		for (i = 0; i < deadData.SkeletonItems.Count(); i++)
		{
			skeletonItem = deadData.SkeletonItems.Get(i);

			// Skip cargo items that aren't clothes
			if (!skeletonItem.Attachment)
				continue;

			spawnedItem = ItemBase.Cast(skeletonBuried.GetInventory().CreateAttachment(skeletonItem.Type));

			if (spawnedItem)
			{
				spawnedItem.SetHealth(skeletonItem.Health * GetZenGravesConfig().SkeletonItemHealthModifier);

				if (!spawnedItem.IsRuined() && spawnedItem.GetInventory())
				{
					skeletonClothing.Insert(spawnedItem);
					GetZenGravesConfig().DebugMsg("    SpawnedItem: " + skeletonItem.Type + " x1");
				}
			}
		};

		// Spawn cargo items on skeleton
		if (skeletonClothing.Count() > 0)
		{
			for (i = 0; i < deadData.SkeletonItems.Count(); i++)
			{
				skeletonItem = deadData.SkeletonItems.Get(i);
				spawnedItem = null;

				// Skip clothes
				if (skeletonItem.Attachment)
					continue;

				// 50% chance to spawn top-down versus bottom-up in terms of clothing cargo position on skeleton (to randomize loot a little)
				if (Math.RandomFloat01() <= 0.5)
				{
					// Head -> toes
					for (int i2 = 0; i2 < skeletonClothing.Count(); i2++)
					{
						ItemBase checkClothing1 = skeletonClothing.Get(i2);
						if (SpawnZenCrossItemInItem(skeletonItem, checkClothing1))
						{
							GetZenGravesConfig().DebugMsg("    SpawnedItem: " + skeletonItem.Type + " x" + skeletonItem.Quantity);
							break;
						}
					}
				}
				else
				{
					// Toes -> head
					for (int i3 = skeletonClothing.Count() - 1; i3 >= 0; i3--)
					{
						ItemBase checkClothing = skeletonClothing.Get(i3);
						if (SpawnZenCrossItemInItem(skeletonItem, checkClothing))
						{
							GetZenGravesConfig().DebugMsg("    SpawnedItem: " + skeletonItem.Type + " x" + skeletonItem.Quantity);
							break;
						}
					}
				}
			}
		}

		// Generate slightly random position
		vector stashPos = cross.GetPosition();
		stashPos[0] = stashPos[0] + Math.RandomFloatInclusive(-0.2, 0.2);
		stashPos[2] = stashPos[2] + Math.RandomFloatInclusive(-0.2, 0.2);

		// Create stash
		ZenGraves_UndergroundStash stash = ZenGraves_UndergroundStash.Cast(GetGame().CreateObject("ZenGraves_UndergroundStash", stashPos, ECE_PLACE_ON_SURFACE));

		if (!stash)
		{
			GetZenGravesConfig().DebugMsg("SpawnZenDeathSkeleton() - Failed to spawn stash @ " + deadData.Position);
			return;
		}

		vector stashOrientation = cross.GetOrientation();
		stashOrientation[2] = 0;
		stash.SetOrientation(stashOrientation);
		stash.PlaceOnGround();
		stash.GetInventory().AddEntityToInventory(skeletonBuried);
		stash.Update();

		// Final check: if buried stash is > 2m from original cross position, just delete this grave (eg. this can happen if grave is spawned in Livonia bunker)
		if (vector.Distance(cross.GetPosition(), stash.GetPosition()) > 2)
		{
			GetZenGravesConfig().DebugMsg("SpawnZenDeathSkeleton() - Failed to spawn stash due to distance constraint @ " + deadData.Position);
			stash.DeleteSafe();
			cross.DeleteSafe();
		}
	}

	// Spawn an item inside the given item
	ItemBase SpawnZenCrossItemInItem(ZenGraves_InventoryItem skeletonItem, ItemBase insideItem)
	{
		if (insideItem.IsRuined() || !insideItem.GetInventory())
			return null;

		ItemBase spawnedItem = ItemBase.Cast(insideItem.GetInventory().CreateInInventory(skeletonItem.Type));

		if (!spawnedItem)
			return null;

		spawnedItem.SetHealth(skeletonItem.Health * GetZenGravesConfig().SkeletonItemHealthModifier);
		spawnedItem.SetQuantity(skeletonItem.Quantity);

		// Check for special item quantity types (ignore magazines as we don't know what ammo type is in them - spawn them empty)
		Ammunition_Base ammo = Ammunition_Base.Cast(spawnedItem);
		if (ammo)
		{
			ammo.ServerSetAmmoCount(skeletonItem.Quantity);
		}
		else
		if (spawnedItem.HasEnergyManager() && spawnedItem.GetCompEM())
		{
			spawnedItem.GetCompEM().SetEnergy(skeletonItem.Quantity);
		}

		Edible_Base edible = Edible_Base.Cast(spawnedItem);
		if (edible && edible.HasFoodStage())
		{
			edible.ChangeFoodStage(FoodStageType.ROTTEN);
		}

		return spawnedItem;
	}

	//! MUSIC 
	void SendZenMusicConfig(PlayerBase player, PlayerIdentity identity) 
	{
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenMusicCfgClient", new Param1<bool>(GetZenMusicConfig().AllowCarInventory), true, player.GetIdentity());
	}

	//! BASEBULDING CONFIG
	void SendZenBasebuildingConfig(PlayerBase player, PlayerIdentity identity) 
	{
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenBasebuildingCfgClient", new Param1<ref ZenBasebuildingConfig>(GetZenBasebuildingConfig()), true, player.GetIdentity());
	}

	//! STATIC BARBED WIRE 
	//! Init barbed wire 
	protected void ZenStaticBarbedWireInit()
	{
		if (GetZenStaticBarbedWireConfig().DumpObjects)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpStaticBarbedWireObjects, 20000, false);
		}
		else
		if (GetZenStaticBarbedWireConfig().TurnedOn)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupStaticBarbedWireDamageZones, 20000, false);
		}
	}

	// Sets up the barbed wire damage zones
	private void SetupStaticBarbedWireDamageZones()
	{
		ZenStaticBarbedWireType wireType;

		foreach (ZenStaticBarbedWireObject wirePos : GetZenStaticBarbedWireConfig().WirePositions)
		{
			// Get wire type
			wireType = GetZenStaticBarbedWireConfig().GetWireType(wirePos.TypeName);

			if (!wireType)
			{
				Print("[ZenStaticBarbedWire] Error: No wire type found for config'd wire object: " + wirePos.TypeName);
				continue;
			}

			// If this wire type is disabled, skip it.
			if (!wireType.TurnedOn)
				continue;

			foreach (vector loc : wirePos.Locations)
			{
				// Get objects within 0.5 meter of the config'd vector
				array<Object> objectsNearWire = new array<Object>;
				GetGame().GetObjectsAtPosition3D(loc, 0.5, objectsNearWire, null);

				for (int x = 0; x < objectsNearWire.Count(); x++)
				{
					// Prepare classname(s)
					string debugName = "";
					string className = "";

					// Get classname(s)
					debugName = objectsNearWire[x].GetDebugNameNative();
					className = objectsNearWire[x].ClassName();

					// Convert to lower
					debugName.ToLower();
					className.ToLower();

					// If found object is our wire type, create the damage zone
					if (debugName.Contains(wirePos.TypeName) || className.Contains(wirePos.TypeName))
					{
						// Create damage zone for barbed wire cutting players
						ZenStaticBarbedWire wireObj = ZenStaticBarbedWire.Cast(Object.Cast(GetGame().CreateObject("ZenStaticBarbedWire", "0 0 0")));

						if (wireObj)
						{
							wireObj.SetPosition(objectsNearWire[x].GetPosition());
							wireObj.SetOrientation(objectsNearWire[x].GetOrientation());
							wireObj.SetupDamageZone(objectsNearWire[x], wireType);
						}
					}
				}
			}
		}

		GetZenStaticBarbedWireConfig().Destroy();
	}

	// Scan all map objects and dump positions of relevant ones
	private void DumpStaticBarbedWireObjects()
	{
		// Get all objects on the map
		array<Object> objectsOnMap = new array<Object>;
		GetGame().GetObjectsAtPosition3D(Vector(0, 0, 0), 50000, objectsOnMap, null);
		int objCount = 0;

		foreach (ZenStaticBarbedWireType wireType : GetZenStaticBarbedWireConfig().WireTypes)
		{
			string type = wireType.TypeName;
			type.ToLower();

			// Prepare this object
			ZenStaticBarbedWireObject wireObj = new ZenStaticBarbedWireObject;
			wireObj.TypeName = type;
			array<vector> objLocations = new array<vector>;

			// Cycle through all map objects
			for (int x = 0; x < objectsOnMap.Count(); x++)
			{
				// Prepare classname(s)
				string debugName = "";
				string className = "";

				// Get classname(s)
				debugName = objectsOnMap[x].GetDebugNameNative();
				className = objectsOnMap[x].ClassName();

				// Convert to lower
				debugName.ToLower();
				className.ToLower();

				// If map object is the droid we're looking for, add it to the array
				if (debugName.Contains(type) || className.Contains(type))
				{
					objLocations.Insert(objectsOnMap[x].GetPosition());
					objCount++;
				}
			}

			// Save this object type & all its locations
			wireObj.Locations = objLocations;
			GetZenStaticBarbedWireConfig().WirePositions.Insert(wireObj);
		}

		// Save found objects and disable object dump for next restart
		GetZenStaticBarbedWireConfig().DumpObjects = false;
		GetZenStaticBarbedWireConfig().Save();

		// Clear array
		objectsOnMap.Clear();

		// Setup zones
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupStaticBarbedWireDamageZones, 5000, false);
	}

	//! NIGHT CONFIG 
	override void SyncGlobalLighting(PlayerBase player)
	{
		if (!ZenModEnabled("ZenNightConfig"))
		{
			super.SyncGlobalLighting(player);
			return;
		}

		if (player)
		{
			int id = m_ZenNightConfigID;

			if (GetZenNightConfig().OvercastToTriggerDarkNights > 0)
			{
				ZenNightLightingPlugin nightPlugin = ZenNightLightingPlugin.Cast(GetPlugin(ZenNightLightingPlugin));
				if (nightPlugin)
					id = nightPlugin.GetLightingConfigID();
			}

			Param1<int> lightID = new Param1<int>(id);
			GetGame().RPCSingleParam(player, ERPCs.RPC_SEND_LIGHTING_SETUP, lightID, true, player.GetIdentity());
		}
	}

	//! UTILITIES 
	private void ZenDeferredInit()
	{
		ZenItemStatsLogger.PrintStats();

		if (!GetZenUtilitiesConfig().ShouldDeleteGhostItems)
			return;

		// Delete any ghost items spawned at 0 0 0 (vector.Zero) and log them to debug it
		// this can happen on some mods where the hologram gets fucked up during placement
		// I found hundreds of objects placed at this location on my server once so this keeps an eye on that.
		array<Object> nearest_objects = new array<Object>;
        GetGame().GetObjectsAtPosition3D("0 0 0", 1, nearest_objects, NULL);
		foreach (Object obj : nearest_objects)
		{
			ZenModLogger.Log("Deleted " + obj.GetType() + " @ 0 0 0", "000ghosts");
		}
	}

	void SendZenServerDiversionMessage(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || player.IsPlayerDisconnected())
			return;

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", new Param1<string>(GetZenServerDiversionConfig().RedirectMessage), true, player.GetIdentity());
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenServerDiversionConfig, 10000, false, player);
	}

	void SendZenServerDiversionConfig(PlayerBase player)
	{
		if (!player || !player.GetIdentity() || player.IsPlayerDisconnected())
			return;

		// Send client config
		Param3<string, int, string> data = new Param3<string, int, string>(GetZenServerDiversionConfig().ServerIP, GetZenServerDiversionConfig().ServerPort, GetZenServerDiversionConfig().ServerPass);
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenServerDiversionConfigOnClient", data, true, player.GetIdentity());
	}

	// Send notification config to client
	void SendNotificationConfig(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;

		// Send client config
		Param1<ZenNotificationsConfig> configParams = new Param1<ZenNotificationsConfig>(GetZenNotificationsConfig());
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenNotificationConfigOnClient", configParams, true, player.GetIdentity());
	}
}