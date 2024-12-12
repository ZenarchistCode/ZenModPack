modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPC

		//! UTILITIES
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageConfirmRead", this, SingeplayerExecutionType.Server);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPollChoice", this, SingeplayerExecutionType.Server);

        //! RAID ALARM
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenRaidAlarmClientWebhooks", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessage", this, SingeplayerExecutionType.Server);
		#else
		// CLIENT RECEIVE RPC

		//! GENERAL CONFIG
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenModPackConfig", this, SingeplayerExecutionType.Client);

		//! ARTILLERY
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveAirstrikeData", this, SingeplayerExecutionType.Client);

		//! TREASURE 
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenTreasureTextClient", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenTreasurePhotoReadClient", this, SingeplayerExecutionType.Client);

		//! MUSIC
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenMusicCfgClient", this, SingeplayerExecutionType.Client);

		//! UTILITIES 
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenNotificationConfigOnClient", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenPollOptions", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenServerDiversionConfigOnClient", this, SingeplayerExecutionType.Client);

        //! RAID ALARM
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenRaidAlarmServerWebhooks", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessageFailed", this, SingeplayerExecutionType.Client);
		#endif

		//! TREASURE 
		// Load how many photos exist for RandomPhoto spawner
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgVehicles"); i++)
		{
			string cfg_name = "";
			GetGame().ConfigGetChildName("CfgVehicles", i, cfg_name);
			if (cfg_name.Contains("ZenTreasure_Photo") && !cfg_name.Contains("Base"))
				ZenTreasure_RandomPhoto.PHOTO_COUNT++;
		}
	}

	override UIScriptedMenu CreateScriptedMenu(int id)
    {
        UIScriptedMenu menu = super.CreateScriptedMenu(id);

        if (!menu)
        {
            switch (id)
            {
				//! NOTES
                case ZenMenus.NOTE_GUI:
                {
                    menu = new ZenNoteGUI;
                    break;
                }

				//! UTILITIES 
				case ZenMenus.PLAYER_MESSAGE:
                {
                    menu = new ZenAdminMessageGUI;
                    break;
                }

                case ZenMenus.POLL_GUI:
                {
                    menu = new ZenPollGUI;
                    break;
                }

				//! SPLIT GUI
                case ZenMenus.SPLIT_GUI:
                {
                    menu = new ZenSplitItemUI;
                    break;
                }

                //! RAID ALARM
                case ZenMenus.RAID_ALARM_GUI:
                {
                    menu = new ZenRaidAlarmGUI;
                    break;
                }
            }
        }

        return menu;
    }

    //! RAID ALARM
    // Client -> server :: receive potential admin command
    void RPC_ReceiveZenAdminCommandMessage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenAdminCommandMessage");
                return;
            }

            if (sender && data.param1)
            {
                ZenAdminMessagePlugin plugin = ZenAdminMessagePlugin.Cast(GetPlugin(ZenAdminMessagePlugin));
                if (plugin)
                {
                    if (plugin.HandleAdminCommand(sender, data.param1))
                        return;
                }

                // If we failed to stop @ plugin.HandleAdminCommand, relay this message back to client
                GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessageFailed", new Param1<string>(data.param1), true, sender);
            }
        }
    }

    // Client -> server :: receive webhooks on server
    void RPC_ReceiveZenRaidAlarmClientWebhooks(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param3<array<string>, Zen_RaidAlarmStation, string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenRaidAlarmClientWebhooks");
                return;
            }

            if (data.param1.Count() != 3)
            {
                Error("Error sync'ing client-side RaidStation data to server - RPC_ReceiveZenRaidAlarmClientWebhooks - count=" + data.param1.Count());
                return;
            }

            if (!data.param2)
            {
                Error("Error sync'ing client-side RaidStation data to server - RPC_ReceiveZenRaidAlarmClientWebhooks");
                return;
            }

            if (sender && data.param1 && data.param2)
            {
                data.param2.SetWebhooks(data.param1, sender.GetName());
                data.param2.SetBaseName(data.param3);
            }
        }
    }

    // Server -> client :: receive failed admin command and relay it back to chat
    void RPC_ReceiveZenAdminCommandMessageFailed(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_ReceiveZenAdminCommandMessageFailed");
                return;
            }

            if (data.param1)
            {
                GetGame().ChatPlayer(data.param1);
            }
        }
    }

    // Server -> client :: receive webhooks on client
    void RPC_ReceiveZenRaidAlarmServerWebhooks(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param3<array<string>, Zen_RaidAlarmStation, string> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenRaidAlarmServerWebhooks");
                return;
            }

            if (data.param1.Count() != 3)
            {
                Error("Error sync'ing server-side webhook data to client - RPC_ReceiveZenRaidAlarmServerWebhooks - count=" + data.param1.Count());
                return;
            }

            if (!data.param2)
            {
                Error("Error sync'ing server-side Raid Station data to client - RPC_ReceiveZenRaidAlarmServerWebhooks");
                return;
            }

            if (GetGame().GetUIManager() != NULL)
            {
                ZenRaidAlarmGUI gui = ZenRaidAlarmGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.RAID_ALARM_GUI, NULL));
                if (gui)
                    gui.OnReceivedServerData(data.param1.Get(0), data.param1.Get(1), data.param1.Get(2), data.param2, data.param3);
            }
        }
    }

	//! ARTILLERY
	private ref EffectSound m_ZenArtySound;

	// SERVER->CLIENT RPCS /////////////////////////////////////////////////////////////////////////////
	void RPC_ReceiveAirstrikeData(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<vector> data;
		if (!ctx.Read(data))
		{
			Error("[ZenModPack] RPC_ReceiveAirstrikeData: sync data read error");
			return;
		}

		vector soundPosition = data.param1;

		if (soundPosition)
		{
			m_ZenArtySound = SEffectManager.PlaySound("Artillery_Distant_Barrage_SoundSet", soundPosition, 0.1, 0.1);
			m_ZenArtySound.SetAutodestroy(true);
		}
	}

	//! GENERAL CONFIG
	void RPC_ReceiveZenModPackConfig(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		Param1<map<string, bool>> data;
		if (!ctx.Read(data))
		{
			Error("[ZenModPack] RPC_ReceiveZenModPackConfig: sync data read error");
			return;
		}

		GetZenModPackConfig().ModEnabled.Clear();
		for (int i = 0; i < data.param1.Count(); i++)
		{
			GetZenModPackConfig().ModEnabled.Insert(data.param1.GetKey(i), data.param1.GetElement(i));
		}

        GetZenModPackClientConfig().ImmersiveLogin = ZenModEnabled("ZenImmersiveLogin");
        GetZenModPackClientConfig().Save();
	}

	//! TREASURE 
	// Server -> inform player of treasure descriptions
	void RPC_ReceiveZenTreasureTextClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<array<string>> data;
        if (!ctx.Read(data))
        {
            Error("[ZenTreasure] Error sync'ing server-side data to client - RPC_ReceiveZenTreasureTextClient");
            return;
        }

		// Copy config over from received map
		for (int i = 0; i < data.param1.Count(); i++)
		{
			ZenTreasureConfig.TreasureDescriptions.Insert(data.param1.Get(i));
		}
    }

	// Server -> player reads photo for first time -> tells client to open inspect menu via RPC along with photo data before SynchDirty happens
	void RPC_ReceiveZenTreasurePhotoReadClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param2<ItemBase, int> data;
        if (!ctx.Read(data))
        {
            Error("[ZenTreasure] Error sync'ing server-side data to client - RPC_ReceiveZenTreasurePhotoReadClient");
            return;
        }

		ZenTreasure_PhotoBase photo = ZenTreasure_PhotoBase.Cast(data.param1);
		if (!photo)
			return;

		if (data.param2 != -1) // First time reading if this is -1
			photo.SetStashType(data.param2);
		
		ZenOpenPhotoInspect(photo);
    }

	// TODO: Find a way to move this code to item OnRPC instead of MissionBase?
	void ZenOpenPhotoInspect(notnull ZenTreasure_PhotoBase photo)
	{
		if (GetGame().GetUIManager().GetMenu() != NULL)
			return;

		MissionGameplay missionGP = MissionGameplay.Cast(GetGame().GetMission());
		if (!missionGP)
			return;

		missionGP.ShowInventory();

		InventoryMenu inventory_menu = InventoryMenu.Cast(GetGame().GetUIManager().FindMenu(MENU_INVENTORY));
		if (!inventory_menu)
			return;

		InspectMenuNew inspect_menu = InspectMenuNew.Cast(inventory_menu.EnterScriptedMenu(MENU_INSPECT));
		if (inspect_menu)
		{
			GetGame().GetMission().GetHud().ShowHudUI(false);
			GetGame().GetMission().GetHud().ShowQuickbarUI(false);
			inspect_menu.SetItem(photo);
		}
	}

	//! MUSIC 
	// Server -> tell player if they can access inventory while in car
	void RPC_ReceiveZenMusicCfgClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<bool> data;
        if (!ctx.Read(data))
        {
            Error("[ZenMusic] Error sync'ing server-side data to client - RPC_ReceiveZenMusicCfgClient");
            return;
        }

		GetZenMusicConfig().AllowCarInventory = data.param1;
    }

	//! UTILITIES 

	// (Server -> Client, runs on client)
    void RPC_ReceiveZenNotificationConfigOnClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && !GetGame().IsDedicatedServer())
        {
            Param1<ZenNotificationsConfig> data;

            // If data fails to read, stop here.
            if (!ctx.Read(data) || !data.param1)
            {
                Error("IMPORTANT ERROR: RPC_ReceiveZenNotificationConfigOnClient failed to be read on client! Please tell Zenarchist he fucked up!");
                return;
            }

            // Data received - sync to client config
            Chat.EnableServerNotifications = data.param1.EnableServerNotifications;
            Chat.ServerMessage = data.param1.ServerMessage;
            Chat.ServerIcon = data.param1.ServerIcon;
            foreach (ZenNotification n : data.param1.Keywords)
                Chat.Keywords.Insert(n);
            Chat.HasReceivedServerConfig = true;
        }
    }

	// Client -> server :: confirm that the player has clicked OK and read the admin message
	void RPC_SendZenPlayerMessageConfirmRead(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param1<bool> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_SendReadReportToServer");
                return;
            }

            // Handle report confirmation server-side
            if (sender && data.param1)
            {
				int highBits, lowBits;
				GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
				PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
				if (!player)
				{
					Error("[ZenModPack Utilities] RPC_SendZenPlayerMessageConfirmRead :: Error converting player ID into player object: " + sender.GetId());
					return;
				}

                ZenModLogger.Log("Player " + player.GetCachedName() + " (" + player.GetCachedID() + ") has confirmed they read your reply.");
                player.SpawnZenAdminMessageItems();
            }
        }
    }
	
	// Client -> server :: receive poll vote
	void RPC_SendZenPollChoice(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param1<array<bool>> data;
            if (!ctx.Read(data))
            {
                Error("Error receiving data - RPC_SendZenPollChoice");
                return;
            }

            if (sender && data.param1)
            {
				GetZenPollConfig().RegisterVote(sender.GetId(), data.param1);

                int highBits, lowBits;
				GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
				PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
				if (player)
                {
                    player.Zen_SendMessage("[SERVER] Vote received!");
                }
            }
        }
    }

	// Server -> client :: receive admin text message and dialogue
    void RPC_ReceiveZenAdminMessage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenAdminMessage");
                return;
            }

            if (GetGame().GetUIManager() != NULL)
            {
                ZenAdminMessageGUI gui = ZenAdminMessageGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.PLAYER_MESSAGE, NULL));
                if (gui)
                    gui.SetAdminMessage(data.param1);
            }
        }
    }

	// Server -> client :: receive poll data
    void RPC_ReceiveZenPollOptions(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param4<string, string, bool, array<string>> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenPollOptions");
                return;
            }

            if (GetGame().GetUIManager() != NULL)
            {
                ZenPollGUI gui = ZenPollGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.POLL_GUI, NULL));
                if (gui)
                    gui.SetPollSettings(data.param1, data.param2, data.param3, data.param4);
            }
        }
    }

    //! SERVER DIVERSION
    void RPC_ReceiveZenServerDiversionConfigOnClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type != CallType.Client || !GetGame().IsClient())
            return;

        Param3<string, int, string> data;
        if (!ctx.Read(data))
            return;

        GetZenServerDiversionConfig().ServerIP = data.param1;
        GetZenServerDiversionConfig().ServerPort = data.param2;
        GetZenServerDiversionConfig().ServerPass = data.param3;

        string currentServerIP;
        int port;
        GetGame().GetHostAddress(currentServerIP, port);

        if (currentServerIP != GetZenServerDiversionConfig().ServerIP)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().DisconnectSessionForce);
        }
    }
}