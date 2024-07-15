modded class MissionBase
{
	//! SHARED 
	void MissionBase()
	{
		#ifdef SERVER
		// SERVER RECEIVE RPC

		//! UTILITIES
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageConfirmRead", this, SingeplayerExecutionType.Server);
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
        GetRPCManager().AddRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageReply", this, SingeplayerExecutionType.Client);
		GetRPCManager().AddRPC("ZenMod_RPC", "RPC_ReceiveZenNotificationConfigOnClient", this, SingeplayerExecutionType.Client);
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
		Param1< vector > data;
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
		Param1<ref map<string, bool>> data;
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
	}

	//! TREASURE 
	// Server -> inform player of treasure descriptions
	void RPC_ReceiveZenTreasureTextClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
    {
        Param1<autoptr array<string>> data;
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
    void RPC_ReceiveZenNotificationConfigOnClient(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
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
	void RPC_SendZenPlayerMessageConfirmRead(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type == CallType.Server && GetGame().IsDedicatedServer())
        {
            Param1<bool> data;
            if (!ctx.Read(data))
            {
                Error("Error sending data - RPC_SendReadReportToServer");
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

	// Server -> client :: send admin text message and dialogue
    void RPC_SendZenPlayerMessageReply(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        if (type == CallType.Client && GetGame().IsClient())
        {
            Param1<string> data;
            if (!ctx.Read(data))
            {
                Error("Error sync'ing server-side data to client - RPC_SendZenPlayerMessageReply");
                return;
            }

            if (data.param1)
            {
                ZenAdminMessageGUI.REPLY = data.param1;
                GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OpenZenPlayerMessageReplyDialogue, 100, false);
            }
        }
    }

	// Client :: Open reply dialogue
    void OpenZenPlayerMessageReplyDialogue()
    {
        if (ZenAdminMessageGUI.REPLY != "")
        {
            GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.PLAYER_MESSAGE, NULL);
        }
    }
}