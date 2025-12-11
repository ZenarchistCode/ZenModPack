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

		ZMPrint("[ZenModPack] MissionServer :: OnInit()");

		//! UTILITIES 
		ZEN_UNIQUE_PLAYER_ID_TRACKER = 0;
		m_ZenPlayerUIDs = new map<string, int>;
		GetZenUtilitiesConfig();
		GetZenPlayerMessageConfig();
		GetZenUpdateMessage();
		GetZenUpdateMessagePersistence();
		GetZenServerDiversionConfig();
		m_ModLogger = new ZenModLogger;
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenDeferredInit, 30000, false);

		//! GENERAL CONFIG
		ZMPrint("[ZenModPack] Loading config");
		GetZenModPackConfig();

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

		//! CAR COMPASS 
		GetZenCarCompassConfig();

		//! BASEBUILDING CONFIG 
		if (ZenModEnabled("ZenBasebuildingConfig"))
			GetZenBasebuildingConfig();

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
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

		ZMPrint("[ZenModPack] OnMissionStart");

		//! GENERAL SERVER-SIDE DEBUG 
		ZenServerDebugStartup();
	}

	void ~MissionServer()
	{
		SaveAllZenDBsShutdown();
	}

	void SaveAllZenDBsShutdown()
	{
		ZMPrint("[ZenModPack] OnMissionFinish");
		SaveAllZenDBs();
	}

	// If a player just logged out and they were the LAST player to logout, save all DBs.
	// This is a robust way to save databases at server restarts if server runs CFTools and kicks
	// players before a restart, which most high-pop DayZ servers typically do to prevent item duping.
	// It can also trigger if the server is simply quiet and the last player logs out - no harm there either.
	override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
	{
		super.PlayerDisconnected(player, identity, uid);
		
		array<Man> players = new array<Man>();
		g_Game.GetPlayers(players);
		
		if (players.Count() > 1)
			return;

		ZMPrint("Final active player disconnected: saving all skill databases.");
		SaveAllZenDBs();
	}

	void SaveAllZenDBs()
	{
		ZMPrint("[ZenModPack] SaveAllZenDBs");

		//! REPAIR WELLS 
		if (ZenModEnabled("ZenRepairWells"))
			GetZenWellsConfig().Save();

		//! REPAIR PUMPS
		if (ZenModEnabled("ZenRepairPumps"))
			GetZenPumpsConfig().Save();

		//! UTILITIES
		GetZenUpdateMessagePersistence().Save();

		//PrintZenPlayerDropCounts();

		ZMPrint("[ZenModPack] Saved all configs & DBs successfully.");
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
		if (g_Game.ConfigGetInt("CfgVehicles ZenModPackConfig dumpCfgVehicles") != 1)
			return;
		
		string cfg_name = "";
		bool foundZenModPack = false;

		ZenModLogger.Log("[ZENMODPACK CFGVEHICLES DUMP START]", "CfgVehicles");
		ZenModLogger.Log("", "CfgVehicles");

		for (int i = 0; i < g_Game.ConfigGetChildrenCount("CfgVehicles"); i++)
		{
			g_Game.ConfigGetChildName("CfgVehicles", i, cfg_name);

			if (cfg_name == "ZenModPackConfig")
			{
				foundZenModPack = true;
				continue;
			}

			if (cfg_name == "ZenModPack_EndDump")
				break;

			if (foundZenModPack && g_Game.ConfigGetInt("CfgVehicles " + cfg_name + " scope") == 2)
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
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateFire, 10, false, m_player);
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

			//! SERVER DIVERSION
			if (GetZenServerDiversionConfig().ServerIP != "")
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenServerDiversionMessage, 6969, false, player);
			}
		}

		//! SHARED
		/* 
		int playerUID;
		if (!m_ZenPlayerUIDs.Find(identity.GetId(), playerUID))
		{
			playerUID = ZEN_UNIQUE_PLAYER_ID_TRACKER;
			ZEN_UNIQUE_PLAYER_ID_TRACKER++;
			m_ZenPlayerUIDs.Insert(identity.GetId(), playerUID);
		}

		player.SetZenPlayerUID(playerUID);
		*/

		// Delay sending of client config to avoid spamming new client login along with data from other mods
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendNotificationConfig, 500 + Math.RandomInt(0, 500), false, player);

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
		float y = g_Game.SurfaceY(x, z);
		vector firePos = { x,y,z };

		// Check surface type
		string surface_type;
		g_Game.SurfaceGetType(firePos[0], firePos[2], surface_type);
		surface_type.ToLower();

		// Don't start fires under water
		if (surface_type.Contains("water"))
			return;

		// Create fire
		FireplaceBase fire = FireplaceBase.Cast(g_Game.CreateObject("Fireplace", firePos));

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
		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartFire, 100, false, fire);

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
			fire.SetTemperatureDirect(FireplaceBase.PARAM_MIN_FIRE_TEMPERATURE);
			// Put fire out after 12 seconds
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExtinguishFire, 12000, false, fire);
			g_Game.CreateObjectEx("ClutterCutterFireplace", fire.GetPosition(), ECE_PLACE_ON_SURFACE|ECE_NOLIFETIME);
		}
	}

	// Extinguish the fire so the player cannot suicide on it
	void ExtinguishFire(FireplaceBase fire)
	{
		if (fire)
		{
			// Set fire extinguish sound effect
			fire.SetExtinguishingState();
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StopFire, 3000, false, fire);
		}
	}

	// Stop the fire and set lifetime short (1 minute) so that it gets deleted when player leaves area
	void StopFire(FireplaceBase fire)
	{
		if (fire)
		{
			// Stop fire burning
			fire.StopFire(FireplaceFireState.EXTINGUISHED_FIRE);
			fire.SetLifetimeMax(60);
			fire.SetLifetime(60);
			int attachments_count = fire.GetInventory().AttachmentCount();

			// Delete all attachments so the player cannot re-ignite the fire
			for (int i = 0; i < attachments_count; i++)
			{
				ItemBase item = ItemBase.Cast(fire.GetInventory().GetAttachmentFromIndex(i));

				if (item)
				{
					item.SetHealth(0); //DeleteSafe();
				}
			}

			fire.SetHealth(0);
		}
	}

	//! FIREWOOD 
	void ZenFireWoodInit()
	{
		ZMPrint("[ZenModPack] ZenFirewood::ZenFireWoodInit");

		if (GetZenFirewoodConfig().DumpObjectLocations)
		{
			// Wait 20 secs to ensure all vanilla + modded items are loaded in
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpFirewoodObjects, 20000, false);
		}
		else
		if (GetZenFirewoodConfig().SpawnFirewoodObjects || GetZenFirewoodConfig().DebugOn)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 20000, false);
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

				// Get objects within 3 meter of the config'd wood pile vector
				array<Object> objectsNearWoodpile = new array<Object>;
				g_Game.GetObjectsAtPosition3D(loc, 3, objectsNearWoodpile, null);

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

					// Skip any existing zen wood piles within 3 meters
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
		Object newObj = g_Game.CreateObject(childObjType, vector.Zero);
		newObj.SetPosition(parentObj.GetPosition() + offset);
		newObj.SetOrientation(parentObj.GetOrientation() + orient);
		newObj.Update();
	}

	// Dump all objects around the map that match our typenames.
	void DumpFirewoodObjects()
	{
		ZenFirewoodLogger.Log("Start object dump.");

		vector centerPos = g_Game.ConfigGetVector(string.Format("CfgWorlds %1 centerPosition", g_Game.GetWorldName()));

		// Get all objects on the map in a 20km radius from the center of that 20km radius (enough for most maps?)
		array<Object> objectsOnMap = new array<Object>;
		g_Game.GetObjectsAtPosition3D(centerPos, 20000, objectsOnMap, null);
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupWoodPiles, 5000, false);
	}

	//! CHICKEN COOPS
	void ZenChickenCoopsInit()
	{
		ZMPrint("[ZenModPack] ZenChickenCoops::ZenChickenCoopsInit");

		if (GetZenChickenCoopsConfig().DumpObjectLocations)
		{
			// Wait 20 secs to ensure all vanilla + modded items are loaded in
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpChickenCoopObjects, 20000, false);
		}
		else
		if (GetZenChickenCoopsConfig().SpawnChickenCoops || GetZenChickenCoopsConfig().DebugOn)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupChickenCoops, 20000, false);
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
				g_Game.GetObjectsAtPosition3D(loc, 1, objectsNearCoop, null);

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
		Object newObj = g_Game.CreateObject(childObjType, vector.Zero);
		newObj.SetPosition(parentObj.GetPosition() + offset);
		newObj.SetOrientation(parentObj.GetOrientation() + orient);
		newObj.Update();
	}

	// Dump all objects around the map that match our typenames.
	void DumpChickenCoopObjects()
	{
		ZenChickenCoopsLogger.Log("Start object dump.");

		vector centerPos = g_Game.ConfigGetVector(string.Format("CfgWorlds %1 centerPosition", g_Game.GetWorldName()));

		// Get all objects on the map in a 20km radius from the center of that 20km radius (enough for most maps?)
		array<Object> objectsOnMap = new array<Object>;
		g_Game.GetObjectsAtPosition3D(centerPos, 20000, objectsOnMap, null);
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupChickenCoops, 5000, false);
	}

	//! BASEBUILDING CONFIG
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
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DumpStaticBarbedWireObjects, 20000, false);
		}
		else
		if (GetZenStaticBarbedWireConfig().TurnedOn)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupStaticBarbedWireDamageZones, 20000, false);
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
				g_Game.GetObjectsAtPosition3D(loc, 0.5, objectsNearWire, null);

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
						ZenStaticBarbedWire wireObj = ZenStaticBarbedWire.Cast(Object.Cast(g_Game.CreateObject("ZenStaticBarbedWire", "0 0 0")));

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
		g_Game.GetObjectsAtPosition3D(Vector(0, 0, 0), 50000, objectsOnMap, null);
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupStaticBarbedWireDamageZones, 5000, false);
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
			g_Game.RPCSingleParam(player, ERPCs.RPC_SEND_LIGHTING_SETUP, lightID, true, player.GetIdentity());
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
        g_Game.GetObjectsAtPosition3D("0 0 0", 1, nearest_objects, NULL);
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
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenServerDiversionConfig, 10000, false, player);
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