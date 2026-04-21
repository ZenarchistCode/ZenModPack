modded class MissionServer
{
	//! SHARED
	ref map<string, int> m_ZenPlayerUIDs;
	static int ZEN_UNIQUE_PLAYER_ID_TRACKER;

	//! NIGHT CONFIG 
	protected int m_ZenNightConfigID;

	override void AfterLoadZenConfig() 
	{
		//! UTILITIES 
		ZEN_UNIQUE_PLAYER_ID_TRACKER = 0;
		m_ZenPlayerUIDs = new map<string, int>;

		//! CUSTOM DYNAMIC CONTAMINATED ZONE LOOT
		if (ZenModEnabled("ZenDynamicZoneLoot"))
			GetZenContaminatedZoneLootConfig();

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

		//! PAS Battery Persistence 
		if (ZenModEnabled("ZenBatteryPASPersistence"))
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CreateZenBatteryPAS, 21000);
		}

		//! GENERAL SERVER-SIDE DEBUG 
		ZenServerDebugStartup();
	}
	
	override void LoadZenConfig()
	{
		super.LoadZenConfig();
		
		GetZenUpdateMessage();
		GetZenUpdateMessagePersistence();
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

		//! PAS BATTERY PERSISTENCE 
		if (ZenModEnabled("ZenBatteryPASPersistence"))
			SaveZenPAS();

		//! UTILITIES
		GetZenUpdateMessagePersistence().Save();

		//PrintZenPlayerDropCounts();

		ZMPrint("[ZenModPack] Saved all configs & DBs successfully.");
	}

	void SaveZenPAS()
	{
		GetZenPAS_Config().PAS_Positions.Clear();

		for (int i = 0; i < Land_Radio_PanelPAS.Zen_PAS_List.Count(); i++)
		{
			Land_Radio_PanelPAS pas = Land_Radio_PanelPAS.Zen_PAS_List.Get(i);

			if (!pas)
				continue;

			ItemBase battery = ItemBase.Cast(pas.FindAttachmentBySlotName("LargeBattery"));

			if (battery && battery.GetHealth() > 0)
			{
				GetZenPAS_Config().PAS_Positions.Insert(new ZenPASObject(pas.GetPosition(), battery.GetType(), battery.GetHealth(), battery.GetCompEM().GetEnergy()));
			}
		}

		GetZenPAS_Config().Save();
		Land_Radio_PanelPAS.Zen_PAS_List.Clear();
	}

	void CreateZenBatteryPAS()
	{
		for (int i = 0; i < GetZenPAS_Config().PAS_Positions.Count(); i++)
		{
			ZenPASObject pas = GetZenPAS_Config().PAS_Positions.Get(i);
			array<Object> objectsNearPAS = new array<Object>;
			g_Game.GetObjectsAtPosition3D(pas.Location, 3, objectsNearPAS, null);

			for (int x = 0; x < objectsNearPAS.Count(); x++)
			{
				if (objectsNearPAS[x].IsInherited(Land_Radio_PanelPAS))
				{
					Land_Radio_PanelPAS pas_obj = Land_Radio_PanelPAS.Cast(objectsNearPAS[x]);

					if (pas_obj)
					{
						pas_obj.ZenCreateBattery(pas.BatteryType, pas.BatteryHealth, pas.BatteryCharge);
					}
				}
			}
		}
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

		ZMPrint("[CFGVEHICLES DUMP START]");
		Print("");

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
				Print(cfg_name);
		}

		Print("");
		ZMPrint("[CFGVEHICLES DUMP END]");
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) 
	{
		super.InvokeOnConnect(player, identity);

		//! UTILITIES 
		if (player && identity)
		{
			//! SERVER DIVERSION
			if (GetZenServerDiversionConfig().ServerIP != "")
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenServerDiversionMessage, 6969, false, player);
			}
		}
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
	override void ZenDeferredInit()
	{
		super.ZenDeferredInit();
		
		ZenItemStatsLogger.PrintStats();
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
}