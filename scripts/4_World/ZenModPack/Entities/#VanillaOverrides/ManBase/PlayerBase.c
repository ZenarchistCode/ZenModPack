modded class PlayerBase
{
	//! SHARED
	static int ZENMOD_PLAYER_COUNT = 0;
	static int ZENMOD_DRIVER_COUNT = 0;

	void ~PlayerBase()
	{
		if (!g_Game)
			return;

		if (!g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM))
			return; 

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ZenConcussionGrenadeManager);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Zen_SendMessage);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckZenPlayerMsgConfig);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SendZenPlayerMessageReply);
	}
	
	override bool GetInColdArea()
	{
		if (super.GetInColdArea())
			return true;
		
		if (ZenModEnabled("ZenAutoWinterDetect"))
			return ZenGameFunctions.IsWinter();
		
		return false;
	}

	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();

		//! UTILITIES
		if (g_Game.IsDedicatedServer())
		{
			ZenModPack_OnPlayerLoaded();
		}
	}

	override void OnConnect()
	{
		super.OnConnect();

		ZENMOD_PLAYER_COUNT++;
	}

	override void OnDisconnect()
	{
		super.OnDisconnect();

		ZENMOD_PLAYER_COUNT--;
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		//! CATCH RAIN 
		AddAction(ActionZenWashHandsRain, InputActionMap);

		//! CRAFT RAG HANDS
		AddAction(ActionZenCraftRagsWithHands, InputActionMap);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

		//! AUTO RUN
        if (rpc_type == ZenRPCs.AUTORUN)
        {
            Param3<bool, int, int> autoRunParams;

            if(!ctx.Read(autoRunParams)) 
			{
				Error("Failed to read AUTORUN param data.");
                return;
			}

            bool isRunning  = autoRunParams.param1;
            int movementIdx = autoRunParams.param2;
            int angle       = autoRunParams.param3;

            GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
            GetInputController().OverrideMovementAngle(isRunning, angle);
			return;
        }

		// Client-side lighting config (unused vanilla since they added JSON cfg for this - repurposed for my config)
		if (rpc_type == ERPCs.RPC_SEND_LIGHTING_SETUP)
		{
			Param1<int> lighting_ClientParams;

			if (!ctx.Read(lighting_ClientParams))
			{
				Error("Failed to read LIGHTING param data.");
                return;
			}

			m_ZenLightingConfigID = lighting_ClientParams.param1;
			UpdateLighting();
			return;
		}
    }

	protected int m_ZenLightingConfigID = -1;

	override void UpdateLighting()
	{
		super.UpdateLighting();

		if (m_ZenLightingConfigID != -1)
		{
			Mission mission = g_Game.GetMission();
			if (mission)
			{
				WorldLighting wLighting = mission.GetWorldLighting();
				if (wLighting)
				{
					wLighting.SetGlobalLighting(m_ZenLightingConfigID);
					ZMPrint("[ZenNightConfig] Set lighting override to " + m_ZenLightingConfigID);
				}
			}
		}
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (g_Game.IsClient())
			return;

		//! CONCUSSION GRENADE 
		if (ammo == "Zen_ConcussionGrenade_Ammo")
		{
			HandleZenConcussionGrenade();
		}

		//! UTILITIES 
		HandleZenUtilities(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		//! WEAPON ENGRAVING
		if (ZenWasAlive() && source && source.IsWeapon())
		{
			Weapon_Base weapon = Weapon_Base.Cast(source);
			if (weapon && weapon.IsZenEngraved())
			{
				weapon.UpdateZenLongestShot(vector.Distance(source.GetPosition(), GetPosition()));
			}
		}
	}

	/*
	void SetZenPlayerUID(int id)
	{
		m_ZenPlayerUID = id;
		SetSynchDirty();
	}

	int GetZenPlayerUID()
	{
		return m_ZenPlayerUID;
	}
	*/

	PlayerBase GetPlayerByZenUID(int id)
	{
		array<Man> players = new array<Man>;
		g_Game.GetPlayers(players);
		
		foreach (Man playerMan : players)
		{
			PlayerBase player = PlayerBase.Cast(playerMan);
			if (player != NULL && player.GetIdentity().GetPlayerId() == id)
			{
				return player;
			}
		}

		return NULL;
	}

	//! LOOT CYCLING DETECTION
	/*static ref map<string, int> ZEN_LOOT_CYCLING_COUNTER;
	static ref map<string, int> ZEN_LOOT_CYCLING_PLAYERAGE;

	void IncreaseZenLootCyclingCounter()
	{
		if (!GetIdentity() || !GetZenUtilitiesConfig().ShouldLogLootCyclers)
			return;

		// IF NOT TRACK LOOT CYCLE STOP HERE

		if (!ZEN_LOOT_CYCLING_COUNTER)
			ZEN_LOOT_CYCLING_COUNTER = new map<string, int>;

		int lootCounter;
		if (!ZEN_LOOT_CYCLING_COUNTER.Find(GetCachedID(), lootCounter))
			lootCounter = 0;

		ZEN_LOOT_CYCLING_COUNTER.Set(GetCachedID(), lootCounter + 1);
	}*/

	static bool GetZenTerritoryPermission(notnull PlayerBase player, bool someOverride = false)
	{
		return GetZenTerritoryPermission(player.GetIdentity().GetId(), player.GetPosition(), someOverride);
	}

	static bool GetZenTerritoryPermission(string uid, vector pos, bool someOverride = false)
	{
		if (someOverride)
			return true;

		// Check Expansion
#ifdef DZ_Expansion_BaseBuilding
		ExpansionTerritoryModule expansionTerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		if (expansionTerritoryModule != NULL)
			return expansionTerritoryModule.IsInsideOwnTerritoryOrPerimeter(pos, -1, -1, uid);
#endif

		// Check basic territories by Daemonforge
#ifdef BasicTerritories
		TerritoryFlag tflag = ZenFunctions.GetNearestTerritoryFlag(pos);
		if (tflag != NULL && tflag.HasRaisedFlag())
		{
			if (tflag.IsTerritoryMember(uid)
				return true;
		}
#endif

		return false;
	}

	//! CONCUSSION GRENADE 
	float m_ZenConcussionGrenadeUnconTime;
	float m_ZenConcussionGrenadeUnconSecs;

	void HandleZenConcussionGrenade()
	{
		SetHealth("", "Shock", 0);

		// Check delay time
		m_ZenConcussionGrenadeUnconSecs = Math.RandomFloatInclusive(GetZenConcussionGrenadeConfig().UnconMinTime, GetZenConcussionGrenadeConfig().UnconMaxTime) * 1000;
		m_ZenConcussionGrenadeUnconTime = 0;

		// Schedule uncon manager
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenConcussionGrenadeManager, 1000, false);
	}

	// This 'keeps' the player uncon until the duration expires, then wakes them up.
	void ZenConcussionGrenadeManager()
	{
		m_ZenConcussionGrenadeUnconTime += 1000;

		// Player uncon timer has expired - wake em up
		if (m_ZenConcussionGrenadeUnconTime >= m_ZenConcussionGrenadeUnconSecs)
		{
			SetHealth("", "Shock", 50);
		}
		else 
		{
			SetHealth("", "Shock", 0);

			// Player needs to remain uncon - re-schedule check every 1 secs
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenConcussionGrenadeManager, 1000, false);
		}
	}

	//! AUTO RUN 
	protected bool m_ZenAutoRunning;

	void SetZenAutoRunning(bool isRunning)
    {
        m_ZenAutoRunning = isRunning;
		if (m_ZenAutoRunning)
        {
			OnSprintStart();
		} else 
        {
			OnSprintEnd();
		}
    }

    bool IsZenAutoRunning()
    {
        return m_ZenAutoRunning;
    }

	//! INVENTORY ANIMATION
	protected ref ViewInventoryHandlerBase m_ViewInventoryHandler;

    override void OnSelectPlayer()
    {
        super.OnSelectPlayer();

        if (GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_SERVER)
        {
            m_ViewInventoryHandler = new ViewInventoryHandlerServer(this);
        }
        else
        {
            m_ViewInventoryHandler = new ViewInventoryHandlerClient(this);
        }
    }

    override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
    {
        super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);

        if ( m_ViewInventoryHandler )
        {
            m_ViewInventoryHandler.OnUpdate(pCurrentCommandID, pDt);
        }
    }

    override bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
    {
        if (super.OnInputUserDataProcess(userDataType, ctx))
        {
            return true;
        }

        if (userDataType == INPUT_UDT_VIEWINVENTORY && m_ViewInventoryHandler)
        {
            return m_ViewInventoryHandler.OnInputUserDataProcess(ctx);
        }

        return false;
    }

	//! UTILITIES
	protected ref ZenPlayerMessage m_CachedZenPlayerMessage; // Cached admin message, allows admin to spawn items on player via JSON

	void Zen_PutItemInHandsAway()
	{
		//if (g_Game.IsDedicatedServer())
		//	return;

		EntityAI itemInHands = GetHumanInventory().GetEntityInHands();
		
		if (!itemInHands.GetInventory().CanRemoveEntity())
			return;

		if (GetInventory().CanAddEntityToInventory(itemInHands) && GetHumanInventory().CanRemoveEntityInHands())
		{
			PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, itemInHands);
		}
	}

	override void EEKilledZen(notnull Object killer)
	{
		super.EEKilledZen(killer);

		// Expansion & Syberia compatibility
		if (GetType().Contains("eAI") || GetType().Contains("_Ghost"))
			return;

		if (!GetIdentity())
			return;

		EntityAI killerAI = EntityAI.Cast(killer);
		if (!killerAI)
			return;

		if (killerAI.GetType().Contains("eAI") || killerAI.IsZombie())
			return;

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			if (killer)
			{
				Weapon_Base gun = Weapon_Base.Cast(killer);
				if (gun && gun.GetZenEngravedPlayerName() != "")
				{
					gun.IncreaseZenPlayerKills();
				}
			}
		}

		ZenLogItems(killer);
	}

	// Log player's items on death
	void ZenLogItems(Object killer)
	{
		if (!GetZenUtilitiesConfig().ShouldLogDeathGear || !killer)
			return;

		LogPlayerDeathItems(this);

		// Check if log PVP is enabled
		if (!GetZenUtilitiesConfig().ShouldLogPVP)
			return;

		if (!killer)
		{
			ZenLogger.Log("PVP", "pvp", "Player death: " + GetCachedName() + " (" + GetCachedID() + ") was killed by a null object / unknown (pos=" + GetPosition() + ")");
			return;
		}

		// Player melee kill
		PlayerBase player = PlayerBase.Cast(killer);
		if (player && player.GetIdentity())
		{
			ZenLogger.Log("PVP", "pvp", "PLAYER PVP KILL: " + player.GetCachedName() + " (" + player.GetCachedID() + ") killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")");
			return;
		}
		else
		if (player && player.GetType().Contains("eAI")) // For Expansion AI compatibility
		{
			ZenLogger.Log("PVP", "pvp", "PLAYER AI KILL: " + player.GetType() + " killed " + GetCachedName() + " (" + GetCachedID() + ") pos=" + GetPosition() + " (killerHealth=" + player.GetHealth().ToString() + ")");
			return;
		}

		EntityAI killerAI = EntityAI.Cast(killer);
		string identity = "UnknownIdentity";
		if (killerAI)
		{
			if (killerAI.IsZombie())
				identity = "Zombie";

			Man man = killerAI.GetHierarchyRootPlayer();

			if (!man)
			{
				if (player && player.GetIdentity())
				{
					identity = GetCachedName() + " (" + player.GetCachedID() + ")";
				}

				ZenLogger.Log("PVP", "pvp", "Player death: " + identity + " killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos = " + GetPosition() + ") (killerHealth = " + killer.GetHealth().ToString() + ")");
				return;
			}

			player = PlayerBase.Cast(man);
			if (player && player.GetIdentity())
			{
				if (player.GetIdentity())
				{
					identity = player.GetCachedName() + " (" + player.GetCachedID() + ")";
				}

				ZenLogger.Log("PVP", "pvp", "PLAYER PVP KILL: " + identity + " killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")");
				return;
			}
			else
			if (player && player.GetType().Contains("eAI")) // For Expansion AI compatibility
			{
				ZenLogger.Log("PVP", "pvp", "PLAYER AI KILL: " + player.GetType() + " killed " + GetCachedName() + " (" + GetCachedID() + ") pos=" + GetPosition() + " (killerHealth=" + killer.GetHealth().ToString() + ")");
				return;
			}
		}

		ZenLogger.Log("PVP", "pvp", "Player death: " + GetCachedName() + " (" + GetCachedID() + ") was killed by " + killer.GetType() + " (pos=" + GetPosition() + ")");
	}

	void LogPlayerDeathItems(PlayerBase player)
	{
		if (!player || !player.GetIdentity())
			return;

		string logMsg = "items=";
		array<EntityAI> itemsArray = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		if (itemsArray.Count() == 0)
		{
			logMsg = "No items detected";
		}
		else
		{
			for (int i = 0; i < itemsArray.Count(); i++)
			{
				EntityAI item = itemsArray.Get(i);
				if (item && !item.GetType().Contains("SurvivorM") && !item.GetType().Contains("SurvivorF"))
				{
					float quant = item.GetQuantity();

					Ammunition_Base ammo = Ammunition_Base.Cast(item);
					if (ammo) // Item is ammunition, treat it differently to itembase
					{
						quant = ammo.GetAmmoCount();
					}

					logMsg = logMsg + "\"" + item.GetType() + "(" + quant + "/" + item.GetHealth() + ")\",";
				}
			}
		}

		ZenLogger.Log("PVP", "pvp", "PlayerInventory (" + player.GetCachedID() + "/" + player.GetCachedName() + ")\n" + logMsg);
	}

	void HandleZenUtilities(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!GetZenUtilitiesConfig().ShouldLogPVP)
			return;

		if (!source || !GetIdentity())
			return;

		PlayerBase player = PlayerBase.Cast(source);
		if (!player)
		{
			player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		}

		if (player && player.GetType().Contains("eAI"))
		{
			ZenLogger.Log("PVP", "pvp", "PLAYER PVP AI HIT: " + GetCachedName() + " (" + GetCachedID() + ") was hit by " + source.GetType() + " by " + ammo + " (dmgZone=" + dmgZone + " health=" + GetHealth().ToString() + ") @ " + GetPosition());
			return;
		}

		if (player && player.GetIdentity())
		{
			if (player.GetCachedID() != GetCachedID())
			{
				ZenLogger.Log("PVP", "pvp", "PLAYER PVP HIT: " + player.GetCachedName() + " (" + player.GetCachedID() + ") hit " + GetCachedName() + " (" + GetCachedID() + ") with " + ammo + " (dmgZone=" + dmgZone + " health=" + GetHealth().ToString() + ")");
		
			}
		}
	}

	void ZenModPack_OnPlayerLoaded()
	{
		// Reload admin->player msg JSON in case it has changed during session
		GetZenPlayerMessageConfig().Load();

		// Reset player message
		m_CachedZenPlayerMessage = NULL;

		// Check config on delay
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckZenPlayerMsgConfig, 5000, false);
	}

	// Check player message cfg
	protected void CheckZenPlayerMsgConfig()
	{
		if (!GetIdentity() || GetType().Contains("_Ghost"))
			return;

		if (GetZenUpdateMessage().NotifySurvivorOfNameChange && GetCachedName() == "Survivor")
		{
			Zen_SendMessageDelayed(GetZenUpdateMessage().SurvivorNotification, 10000);
		}

		string cachedID = GetCachedID();

		// Check for admin -> player replies
		ZenPlayerMessage playerMsg = GetZenPlayerMessageConfig().GetPlayerMessage(cachedID);

		if (playerMsg && playerMsg.Message != "")
		{
			if (playerMsg.TeleportLocation != vector.Zero)
			{
				SetPosition(playerMsg.TeleportLocation);
			}

			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenPlayerMessageReply, 1000, false, playerMsg);
			return;
		}

		string loginMsg = GetZenUpdateMessage().LOGIN_MESSAGE;

		#ifdef TerjeStartScreen
		loginMsg.Replace("#survivorname#", GetTerjeCharacterName());
		#else
		loginMsg.Replace("#survivorname#", GetCachedName());
		#endif

		if (loginMsg.Contains("#restarttime#"))
		{
			const int RESTART_EVERY_MS = 4 * 60 * 60 * 1000; // 4h

			int uptimeMs 		= g_Game.GetTime(); // server uptime in ms
			int remMs    		= uptimeMs % RESTART_EVERY_MS;
			int msLeft 			= (RESTART_EVERY_MS - remMs);
			int totalMinLeft 	= (msLeft + 59999) / 60000; // round up to next minute

			string timeStr;
			if (totalMinLeft < 60)
			{
				timeStr = string.Format("%1 #STR_ZenGui_Minutes", totalMinLeft);
			}
			else
			{
				// round to nearest 0.5 hour for readability
				float hours = totalMinLeft / 60.0;
				float rounded = Math.Round(hours * 2.0) / 2.0;

				// show .0 only if needed
				if (Math.AbsFloat(rounded - Math.Floor(rounded)) < 0.01)
					timeStr = string.Format("%1 #STR_ZenGui_Hours", Math.Floor(rounded));
				else
					timeStr = string.Format("%1 #STR_ZenGui_Hours", rounded);
			}

			loginMsg.Replace("#restarttime#", timeStr);
		}

		ZenFunctions.SendPlayerMessage(this, loginMsg);

		// Load last update ID 
		int timestamp = CF_Date.Now().GetTimestamp();
		ZenPlayerUpdateMsg updateConfig;
		GetZenUpdateMessagePersistence().PlayerUpdates.Find(cachedID, updateConfig);
		if (!updateConfig)
			updateConfig = new ZenPlayerUpdateMsg(cachedID, "-1", timestamp);

		// Send latest update message
		if (GetZenUpdateMessage().UPDATE_MESSAGE != "" && updateConfig.updateID != GetZenUpdateMessage().UPDATE_VERSION)
		{
			string updateMsg = GetZenUpdateMessage().UPDATE_MESSAGE;
			updateMsg.Replace("#survivorname#", GetCachedName());

			if (GetZenUpdateMessage().PopupInGame)
			{
				// Popup confirm dialog
				GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", new Param2<string, bool>(updateMsg + GetZenUpdateMessage().UPDATE_SUFFIX, true), true, GetIdentity());
				return;
			}
			else
			{
				// Regular in-game notification
				Zen_SendMessageDelayed(GetZenUpdateMessage().UPDATE_PREFIX + updateMsg + GetZenUpdateMessage().UPDATE_SUFFIX, 5000);
			}

			if (!GetZenUpdateMessage().PopupInGame)
			{
				updateConfig.updateID = GetZenUpdateMessage().UPDATE_VERSION;
				updateConfig.lastLoginTimestamp = timestamp;
				GetZenUpdateMessagePersistence().PlayerUpdates.Set(cachedID, updateConfig);
			}
		}
	}

	// Marks the update message pop-up as read
	void MarkReadZenUpdateMessage()
	{
		// Load last update ID 
		int timestamp = CF_Date.Now().GetTimestamp();

		ZenPlayerUpdateMsg updateConfig;
		GetZenUpdateMessagePersistence().PlayerUpdates.Find(GetCachedID(), updateConfig);
		if (!updateConfig)
			updateConfig = new ZenPlayerUpdateMsg(GetCachedID(), "-1", timestamp);

		if (GetZenUpdateMessage().PopupInGame && updateConfig)
		{
			updateConfig.updateID = GetZenUpdateMessage().UPDATE_VERSION;
			updateConfig.lastLoginTimestamp = timestamp;
			GetZenUpdateMessagePersistence().PlayerUpdates.Set(GetCachedID(), updateConfig);
		}
	}

	// Sends a reply to the given player cfg
	protected void SendZenPlayerMessageReply(ZenPlayerMessage message)
	{
		if (!IsAlive() || IsUnconscious())
			return;

		if (IsPlayerDisconnected() || GetIdentity() == NULL)
			return;

		
		ZMPrint("[ZenUtilities] Player " + GetCachedName() + " (" + GetCachedID() + ") has received your admin message:");
		ZMPrint(message.Message);

		// Cache message
		m_CachedZenPlayerMessage = message;

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", new Param2<string, bool>(message.Message, false), true, GetIdentity());
	}

	// Called only when player confirms they read the admin text message (via RPC MissionBase)
	void SpawnZenAdminMessageItems()
	{
		if (m_CachedZenPlayerMessage == NULL)
			return;

		string classNameFull;
		string className;
		string itemQuantityStr = "";
		string itemHealthStr = "-1";
		float itemQuantity;
		EntityAI item;

		if (m_CachedZenPlayerMessage.SpawnItems.Count() > 0)
		{
			for (int i = 0; i < m_CachedZenPlayerMessage.SpawnItems.Count(); i++)
			{
				classNameFull = m_CachedZenPlayerMessage.SpawnItems.Get(i);
				className = classNameFull;

				// Get quantity
				if (classNameFull.Contains("("))
				{
					itemQuantityStr = classNameFull.SubstringInverted(classNameFull, 0, classNameFull.IndexOf("("));
						
					if (itemQuantityStr.Contains("/"))
					{
						itemHealthStr = itemQuantityStr.SubstringInverted(itemQuantityStr, 0, itemQuantityStr.IndexOf("/") + 1);
						itemHealthStr = itemHealthStr.Substring(0, itemHealthStr.IndexOf(")"));
						itemQuantityStr = itemQuantityStr.Substring(1, itemQuantityStr.IndexOf("/") - 1);
					}
					else
					{
						itemQuantityStr = itemQuantityStr.Substring(1, itemQuantityStr.IndexOf(")") - 1);

					}

					className = classNameFull.Substring(0, classNameFull.IndexOf("("));
				}

				item = EntityAI.Cast(g_Game.CreateObject(className, GetPosition(), false));
				if (item == NULL)
				{
					Error("[ZenModPack ZenUtilities] SpawnZenAdminMessageItems :: FAILED TO SPAWN " + className + " ON PLAYER " + GetCachedID());
					continue;
				}
				else 
				{
					ZMPrint("SpawnZenAdminMessageItems :: Spawned " + className + " for " + GetCachedID());
				}

				item.PlaceOnSurface();
						
				if (itemHealthStr != "-1")
					item.SetHealth(itemHealthStr.ToFloat());

				if (itemQuantityStr != "")
				{
					// If item was successfully created, set its health & quantity
					int quantity = itemQuantityStr.ToFloat();

					if (quantity > 0)
					{
						Ammunition_Base ammo = Ammunition_Base.Cast(item);
						if (ammo) // Item is ammunition, treat it differently to itembase
						{
							ammo.ServerSetAmmoCount(quantity);

							if (itemHealthStr != "-1")
								ammo.SetHealth(itemHealthStr.ToFloat());
						}
						else // Item is ItemBase?
						{
							ItemBase ib = ItemBase.Cast(item);
							if (ib)
							{
								if (ib.GetCompEM())
									ib.GetCompEM().SetEnergy(quantity);
								else
									ib.SetQuantity(quantity);
							}
						}
					}
				}
			}
		}

		if (!GetZenPlayerMessageConfig().DeleteMessage(GetCachedID()))
			GetZenPlayerMessageConfig().GetPlayerMessage(GetIdentity().GetPlainId());

		m_CachedZenPlayerMessage.SpawnItems.Clear();
		m_CachedZenPlayerMessage = NULL;
	}

	override void OnVehicleSeatDriverEnter()
	{
		super.OnVehicleSeatDriverEnter();

		ZENMOD_DRIVER_COUNT++;
	}

	override void OnVehicleSeatDriverLeft()
	{
		super.OnVehicleSeatDriverLeft();

		ZENMOD_DRIVER_COUNT--;
	}

	//! GLOVEBOX 
	// Allows viewing inventory while inside cars - VicinityItemManager.c handles not being able to pick up items outside of the car
	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();

		if (!GetInventory())
			return;

		if (ZenModEnabled("ZenCarGlovebox") || ZenModEnabled("ZenCarCompass"))
		{
			GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}

		if (ZenModEnabled("ZenHideGearInCars"))
		{
			Zen_HideBackItems(true);
		}
	}

	override void OnCommandVehicleFinish()
	{
		super.OnCommandVehicleFinish();

		if (ZenModEnabled("ZenHideGearInCars"))
		{
			Zen_HideBackItems(false);
		}
	}

	void Zen_HideBackItems(bool hideGear)
	{
		EntityAI shoulderItem	= GetItemOnSlot("Shoulder");
		EntityAI meleeItem		= GetItemOnSlot("Melee");
		
		if (meleeItem)
		{
			if (hideGear)
			{
				SetSimpleHiddenSelectionState(SIMPLE_SELECTION_MELEE_RIFLE, false);
				SetSimpleHiddenSelectionState(SIMPLE_SELECTION_MELEE_MELEE, false);
			}
			else
			{
				UpdateShoulderProxyVisibility(meleeItem, "Melee");
			}
		}
		
		if (shoulderItem)
		{
			if (hideGear)
			{
				SetSimpleHiddenSelectionState(SIMPLE_SELECTION_SHOULDER_RIFLE, false);
				SetSimpleHiddenSelectionState(SIMPLE_SELECTION_SHOULDER_MELEE, false);
			}
			else
			{
				UpdateShoulderProxyVisibility(shoulderItem, "Shoulder");
			}
		}
		
		/*
		// Doesn't work - guessing because the item on player is a proxy, not a regular item in this context?

		ItemBase backItem = ItemBase.Cast(GetItemOnSlot("Back"));
		if (backItem)
		{
			if (hideGear)
			{
				backItem.SetZenHologrammed(true, "#(argb,8,8,3)color(1,1,1,0,ca)");
			}
			else
			{
				backItem.SetZenHologrammed(false);
			}
		}
		*/
	}

	//! NIGHT LIGHTING CONFIG 
	protected int m_ZenNightLightingConfigID = -1;

	void SetZenNightConfigID(int id)
	{
		m_ZenNightLightingConfigID = id;
	}

	int GetZenNightConfigID()
	{
		return m_ZenNightLightingConfigID;
	}

	//! ALCOHOL
	protected float m_ZenAlcoholConsumed = 0;
	protected ref Timer m_ZenAlcoholUpdateTimer;

	// Reduce alcohol in system 25 per minute (750 = drank a whole bottle of whisky = 30 mins to zero, > 500 = sickness and random uncon)
	protected void UpdateZenAlcohol()
	{
		m_ZenAlcoholConsumed -= 25;

		if (m_ZenAlcoholConsumed <= 0)
		{
			m_ZenAlcoholConsumed = 0;
			m_ZenAlcoholUpdateTimer.Stop();
			m_ZenAlcoholUpdateTimer = NULL;
			return;
		}

		// Painkiller effect
		if (m_ZenAlcoholConsumed > 100)
		{
			if (GetModifiersManager().IsModifierActive(eModifiers.MDF_PAINKILLERS)) // effectively resets the timer
			{
				GetModifiersManager().DeactivateModifier(eModifiers.MDF_PAINKILLERS);
			}

			GetModifiersManager().ActivateModifier(eModifiers.MDF_PAINKILLERS);
		}

		// You took too much man, too much, too much.
		if (m_ZenAlcoholConsumed > 500)
		{
			if (Math.RandomFloat01() < 0.5)
			{
				GiveShock(Math.RandomFloatInclusive(-50, -100));
			}

			if (m_AgentPool.GetSingleAgentCount(eAgents.FOOD_POISON) < 100)
			{
				InsertAgent(eAgents.FOOD_POISON, 100);
			}
		}

		//ZenFunctions.DebugMessage("m_ZenAlcoholConsumed=" + m_ZenAlcoholConsumed + " - foodPoison=" + m_AgentPool.GetSingleAgentCount(eAgents.FOOD_POISON));
	}

	override bool Consume(PlayerConsumeData data)
	{
		bool result = super.Consume(data);

		if (result && data && data.m_Source)
		{
			#ifdef TerjeMedicine
            if (data.m_Source.IsKindOf("ZenJameson") || data.m_Source.IsKindOf("ZenFlask"))
            {
                TerjeConsumableEffects medEffects = new TerjeConsumableEffects();
                medEffects.Apply(data.m_Source, "CfgVehicles " + data.m_Source.GetType(), this, data.m_Amount);
				return result;
            }
			#endif
	
			ItemBase source = ItemBase.Cast(data.m_Source);
			if (source)
			{
				if (source.IsInherited(ZenFlask) || source.IsInherited(ZenJameson))
				{
					if (ZenModEnabled("ZenAlcohol"))
					{
						StartZenAlcoholTimer();
						m_ZenAlcoholConsumed += data.m_Amount;
						GiveShock(data.m_Amount * -0.1);
					}
				}
			}
        }

		return result;
	}

	void StartZenAlcoholTimer()
	{
		if (!m_ZenAlcoholUpdateTimer || !m_ZenAlcoholUpdateTimer.IsRunning())
		{
			m_ZenAlcoholUpdateTimer = new Timer();
			m_ZenAlcoholUpdateTimer.Run(60, this, "UpdateZenAlcohol", NULL, true);
		}
	}

	//! PERSISTENCE
	// Use CF_Load/Save because this will NOT break server persistence if the mod is added/removed mid-wipe. 
	// NOTE: storage[] must refer to this mod's CfgMods classname EXACTLY or this won't work.
	
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage["ZenModPack"];
		if (!ctx) return;
		
		ctx.Write(m_ZenAlcoholConsumed);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage)) return false;

		auto ctx = storage["ZenModPack"];
		if (!ctx) return true;

		if (ctx.GetVersion() >= 1)
		{
			if (!ctx.Read(m_ZenAlcoholConsumed))
			{
				m_ZenAlcoholConsumed = 0;
				return false;
			}

			if (m_ZenAlcoholConsumed > 0)
			{
				StartZenAlcoholTimer();
			}
		}

		return true;
	}
}