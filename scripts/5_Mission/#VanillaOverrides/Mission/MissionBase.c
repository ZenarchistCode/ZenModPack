modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPC

		//! UTILITIES
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageConfirmRead", this, SingeplayerExecutionType.Server);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerUpdateMessageConfirmRead", this, SingeplayerExecutionType.Server);
		#else
		// CLIENT RECEIVE RPC

		//! GENERAL CONFIG
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenModPackConfig", this, SingeplayerExecutionType.Client);

		//! ARTILLERY
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveAirstrikeData", this, SingeplayerExecutionType.Client);

		//! UTILITIES 
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenNotificationConfigOnClient", this, SingeplayerExecutionType.Client);
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenServerDiversionConfigOnClient", this, SingeplayerExecutionType.Client);

        //! BASEBUILDING CONFIG 
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenBasebuildingCfgClient", this, SingeplayerExecutionType.Client);
		#endif
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
                    menu = new ZenAdminMessageGUI;
                    break;
                }

				//! SPLIT GUI
                case ZenMenus.SPLIT_GUI:
                {
                    menu = new ZenSplitItemUI;
                    break;
                }
            }
        }

        return menu;
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
		Param2<map<string, bool>, map<string, bool>> data;
		if (!ctx.Read(data))
		{
			Error("[ZenModPack] RPC_ReceiveZenModPackConfig: sync data read error");
			return;
		}

        int i;

		GetZenModPackConfig().ModEnabled.Clear();
		for (i = 0; i < data.param1.Count(); i++)
		{
			GetZenModPackConfig().ModEnabled.Insert(data.param1.GetKey(i), data.param1.GetElement(i));
		}

        GetZenModPackConfig().PersistentModEnabled.Clear();
		for (i = 0; i < data.param2.Count(); i++)
		{
			GetZenModPackConfig().PersistentModEnabled.Insert(data.param2.GetKey(i), data.param2.GetElement(i));
		}

        GetZenModPackClientConfig().BetterChatFont = ZenModEnabled("ZenBetterChatFont");
        GetZenModPackClientConfig().ImmersiveLogin = ZenModEnabled("ZenImmersiveLogin");
        GetZenModPackClientConfig().Save();
	}

    //! BASEBUILDING CONFIG 
	void RPC_ReceiveZenBasebuildingCfgClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<ref ZenBasebuildingConfig> data;
        if (!ctx.Read(data))
        {
            Error("[ZenMod] Error sync'ing server-side data to client - RPC_ReceiveZenBasebuildingCfgClient");
            return;
        }

        GetZenBasebuildingConfig(); // Create config class before setting its client values
		GetZenBasebuildingConfig().SetConfig(data.param1);
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
			
			if (!NotificationUI.ZenKeywords)
			{
				NotificationUI.ZenKeywords = new array<ref ZenNotification>;
			}
            else 
            {
                NotificationUI.ZenKeywords.Clear();
            }
			
            foreach (ZenNotification n : data.param1.Keywords)
			{
                NotificationUI.ZenKeywords.Insert(n);
			}
			
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
                Error("Error receiving data - RPC_SendZenPlayerMessageConfirmRead");
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

                ZenModLogger.Log("Player " + player.GetCachedName() + " (" + player.GetCachedID() + ") has confirmed they read your admin message.");
                player.SpawnZenAdminMessageItems();
            }
        }
    }

    // Client -> server :: confirm that the player has clicked OK and read the admin update message
    void RPC_SendZenPlayerUpdateMessageConfirmRead(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
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
                int highBits, lowBits;
                GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
                PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
                if (!player)
                {
                    Error("[ZenModPack Utilities] RPC_SendZenPlayerUpdateMessageConfirmRead :: Error converting player ID into player object: " + sender.GetId());
                    return;
                }

                ZenModLogger.Log("Player " + player.GetCachedName() + " (" + player.GetCachedID() + ") has confirmed they read your update message.");
                player.MarkReadZenUpdateMessage();
            }
        }
    }

	// Server -> client :: receive admin text message and dialogue
    void RPC_ReceiveZenAdminMessage(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param2<string, bool> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_ReceiveZenAdminMessage");
                return;
            }

            if (GetGame().GetUIManager() != NULL)
            {
                ZenAdminMessageGUI gui = ZenAdminMessageGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.PLAYER_MESSAGE, NULL));
                if (gui)
                    gui.SetAdminMessage(data.param1, data.param2);
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