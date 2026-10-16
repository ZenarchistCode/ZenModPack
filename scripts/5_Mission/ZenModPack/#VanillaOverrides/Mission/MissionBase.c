modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		// SERVER RECEIVE RPC

		//! UTILITIES
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageConfirmRead", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerUpdateMessageConfirmRead", this, SingleplayerExecutionType.Both);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenServerDiversionConfigOnClient", this, SingleplayerExecutionType.Both);
	}
	
	// NOTE: This is triggered in 3_game. It passes the ZenConfigBase param which was received on the CLIENT to 3_game's script invoker.
	// The script invoker then triggers any class which "subscribes" to this trigger via ZenConfigRegister.AfterSyncReceived.Insert()
	// OnZenCfgSynced() is triggered by ZenModCore - and any mods which override this function can perform post-received cfg adjustments.
	override void OnZenCfgSynced(notnull ZenConfigBase cfg)
	{
		super.OnZenCfgSynced(cfg);
		
		#ifdef SERVER 
		return;
		#endif
		
		// Fishing junk config received!
		// Note: Fishing is one of the only actions in-game where the Random Number Generator is sync'ed to the server, so they both
		// generate the same catch dicerolls etc -> so if there is a mismatch between client/server you get bugged AF when fishing if you
		// catch an item on the client which doesn't match the server's chances. So once cfg is received -> remap it on client too.
		ZenFishingConfig fishCfg = ZenFishingConfig.Cast(cfg);
	    if (!fishCfg)
	        return;

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DeferredApplyZenFishingYieldClient);
    	g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredApplyZenFishingYieldClient, 0, false);
		//ZenModInitWorldYield(g_Game.GetMission().GetWorldData().GetCatchYieldBank());
	}
	
	// Make sure game is ready before applying fishing yield config
	void DeferredApplyZenFishingYieldClient()
	{
		ZenFishingConfig cfg = GetZenFishingConfig();
		if (!cfg || !cfg.HasReceivedSync())
		{
		    g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredApplyZenFishingYieldClient, 100, false);
		    return;
		}
		
	    Mission mission = g_Game.GetMission();
	    if (!mission)
	    {
	        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredApplyZenFishingYieldClient, 100, false);
	        return;
	    }
	
	    WorldData worldData = mission.GetWorldData();
	    if (!worldData)
	    {
	        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredApplyZenFishingYieldClient, 100, false);
	        return;
	    }
	
	    CatchYieldBank bank = worldData.GetCatchYieldBank();
	    if (!bank)
	    {
	        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredApplyZenFishingYieldClient, 100, false);
	        return;
	    }
	
		ZMPrint("[ZenModPack] DeferredApplyZenFishingYieldClient::ZenModInitWorldYield()");
	    ZenModInitWorldYield(bank);
	}

	// Server sets it junk yields here - client sets it once received from server ^
	override void InitWorldYieldDataDefaults(CatchYieldBank bank)
	{
        super.InitWorldYieldDataDefaults(bank);

		// Client remaps fish yields upon receiving config from the server - only the server needs to do it here.
        if (!g_Game.IsDedicatedServer())
            return;
		
		if (!ZenModEnabled("ZenFishingConfig")) 
			return; // If this is disabled, the config will also not sync, so client never fires this code either.
		
		// Call my injection after a brief delay to allow vanilla and other mods to interact with the yieldbank first.
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ZenModInitWorldYield);
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenModInitWorldYield, 34, false, bank);
	}
	
	void ZenModInitWorldYield(CatchYieldBank bank)
	{
		if (!bank) 
			return;
	
		ZenFishingConfig cfg = GetZenFishingConfig();
	
		// Build a fast lookup of types we override (fish + junk)
		ref map<string, bool> overrideTypes = new map<string, bool>;
	
		// junk keys
		foreach (string jk, int jw : cfg.FishingJunk) 
		{      
			overrideTypes.Set(jk, true); 
		}
		
		foreach (string ek, int ew : cfg.FishingJunkEmpty) 
		{
			overrideTypes.Set(ek, true);
		}
	
		// fish keys
		foreach (ZenFishingFishEntry fe : cfg.Fish)
		{
			if (fe && fe.Type != "")
			{
				overrideTypes.Set(fe.Type, true);
			}
		}
		
		//==================================================================================
	
		// Step 1) Remove *any* existing yield whose spawn type matches ours (vanilla or modded)
		for (int i = bank.GetYieldsMap().Count() - 1; i >= 0; i--)
		{
			YieldItemBase yi = bank.GetYieldsMap().GetElement(i);
			if (!yi) 
				continue;
	
			// YieldItemBase.GetType() returns m_Type (spawn class), not script classname 
			if (overrideTypes.Contains(yi.GetType()))
			{
				bank.GetYieldsMap().RemoveElement(i);
			}
		}
	
		// Step 2) Re-register ours in deterministic order (sorting avoids any map-order weirdness)
		TStringArray keys;
		int weight;
	
		// junk
		keys = cfg.FishingJunk.GetKeyArray();
		keys.Sort();
		foreach (string k1 : keys)
		{
			weight = cfg.FishingJunk.Get(k1);
			if (weight <= 0)
				continue;
			
			//Print("[ZenModPack] Registering fishing junk: " + k1 + "=" + weight);
			bank.RegisterYieldItem(new YieldItemJunk(weight, k1));
		}
	
		// empty junk
		keys = cfg.FishingJunkEmpty.GetKeyArray();
		keys.Sort();
		foreach (string k2 : keys)
		{
			weight = cfg.FishingJunkEmpty.Get(k2);
			if (weight <= 0)
				continue;
			
			//Print("[ZenModPack] Registering empty fishing junk: " + k2 + "=" + weight);
			bank.RegisterYieldItem(new YieldItemJunkEmpty(weight, k2));
		}
	
		// fish
		// sort by Type for deterministic registration order
		map<string, ref ZenFishingFishEntry> fishByType = new map<string, ref ZenFishingFishEntry>;
		foreach (ZenFishingFishEntry e : cfg.Fish)
		{
			if (e && e.Type != "")
				fishByType.Set(e.Type, e); // last wins if duplicates
		}
	
		keys = fishByType.GetKeyArray();
		keys.Sort();
	
		foreach (string ft : keys)
		{
			ZenFishingFishEntry entry = fishByType.Get(ft);
			if (!entry) 
				continue;
	
			// allow "disable" by weight <= 0 (still removes duplicates, but won't add ours)
			if (entry.Weight <= 0) 
				continue;
	
			//Print("[ZenModPack] Registering fish: " + entry.Type + "=" + entry.Weight);
			bank.RegisterYieldItem(new ZenYieldItemFishConfigurable(entry.Weight, entry, cfg.DefaultFishHourlyCycleCoefs, cfg.DefaultFishQualityBase));
		}
	}

	override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);

        if (!menu)
        {
            switch (id)
            {
				//! UTILITIES 
				case ZenMenus.PLAYER_MESSAGE:
                {
                    menu = new ZenAdminMessageGUI();
					menu.SetID(ZenMenus.PLAYER_MESSAGE);
                    break;
                }
            }
        }

        return menu;
    }

    void AfterZenModPackConfigReceived()
    {
    }

	//! UTILITIES 

	// Client -> server :: confirm that the player has clicked OK and read the admin message
	void RPC_SendZenPlayerMessageConfirmRead(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && g_Game.IsDedicatedServer())
        {
            Param1<bool> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_SendZenPlayerMessageConfirmRead");
                return;
            }

            // Handle report confirmation server-side
            if (sender && data.param1)
            {
				PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
				if (!player)
				{
					Error("[ZenModPack Utilities] RPC_SendZenPlayerMessageConfirmRead :: Error converting player ID into player object: " + sender.GetId());
					return;
				}

                ZMPrint("[ZenUtilities] Player " + player.GetCachedName() + " (" + player.GetCachedID() + ") has confirmed they read your admin message.");
                player.SpawnZenAdminMessageItems();
            }
        }
    }

    // Client -> server :: confirm that the player has clicked OK and read the admin update message
    void RPC_SendZenPlayerUpdateMessageConfirmRead(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && g_Game.IsDedicatedServer())
        {
            Param1<bool> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_SendZenPlayerUpdateMessageConfirmRead");
                return;
            }

            // Handle report confirmation server-side
            if (sender && data.param1)
            {
                PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
                if (!player)
                {
                    Error("[ZenModPack Utilities] RPC_SendZenPlayerUpdateMessageConfirmRead :: Error converting player ID into player object: " + sender.GetId());
                    return;
                }

                ZMPrint("[ZenUtilities] Player " + player.GetCachedName() + " (" + player.GetCachedID() + ") has confirmed they read your update message.");
                player.MarkReadZenUpdateMessage();
            }
        }
    }

	// Server -> client :: receive admin text message and dialogue
    void RPC_ReceiveZenAdminMessage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && g_Game.IsClient())
        {
            Param3<string, bool, bool> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenAdminMessage");
                return;
            }
			
			MissionGameplay mission = MissionGameplay.Cast(g_Game.GetMission());
			if (mission)
			{
				mission.SHOW_ZEN_ADMIN_DIALOG = true;
				mission.SHOW_ZEN_ADMIN_DIALOG_TEXT = data.param1;
				mission.SHOW_ZEN_ADMIN_DIALOG_ISPOPUP = data.param2;
				mission.SHOW_ZEN_ADMIN_DIALOG_OKBTN = data.param3;
			}
        }
    }

    //! SERVER DIVERSION
    void RPC_ReceiveZenServerDiversionConfigOnClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !g_Game.IsClient())
            return;

        Param3<string, int, string> data;
        if (!ctx.Read(data))
            return;

        GetZenServerDiversionConfig().ServerIP = data.param1;
        GetZenServerDiversionConfig().ServerPort = data.param2;
        GetZenServerDiversionConfig().ServerPass = data.param3;

        string currentServerIP;
        int port;
        g_Game.GetHostAddress(currentServerIP, port);

        if (currentServerIP != GetZenServerDiversionConfig().ServerIP)
        {
            g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game.DisconnectSessionForce);
        }
    }
}