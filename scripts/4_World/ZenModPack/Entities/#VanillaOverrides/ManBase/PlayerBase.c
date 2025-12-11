modded class PlayerBase
{
	//! SHARED
	static int ZENMOD_PLAYER_COUNT = 0;
	static int ZENMOD_DRIVER_COUNT = 0;

	//private int m_ZenPlayerUID;

	override void Init()
	{
		super.Init();

		//! IMMERSIVE LOGIN
		RegisterNetSyncVariableBool("m_ZenLoginHasFinishedServer");

		//! SHARED
		//RegisterNetSyncVariableInt("m_ZenPlayerUID");
	}

	void ~PlayerBase()
	{
		if (!GetGame())
			return;

		if (!GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
			return; 

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ImmersiveLoginFailsafe);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ZenConcussionGrenadeManager);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ZenLoginOpenEyesClient);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Zen_SendMessage);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckZenPlayerMsgConfig);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SendZenPlayerMessageReply);
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//! IMMERSIVE LOGIN
		ImmersiveLoginSync();
	}
	
	override bool GetInColdArea()
	{
		if (super.GetInColdArea())
			return true;
		
		if (ZenModEnabled("ZenAutoWinterDetect"))
			return ZenGameFunctions.IsWinter();
		
		return false;
	}

	override void EOnPostFrame(IEntity other, int extra)
	{
		super.EOnPostFrame(other, extra);

		#ifdef SERVER
		return;
		#endif

		//! IMMERSIVE LOGIN
		ImmersiveLogin_OnPostFrame();
	}

	override void OnPlayerLoaded()
	{
		super.OnPlayerLoaded();

		/*if (GetIdentity() != null)
		{
			string loginMsg = "Player login @ " + GetPosition();
			loginMsg = loginMsg + ": uid=" + GetIdentity().GetId();
			loginMsg = loginMsg + " name1=" + GetIdentity().GetName();
			loginMsg = loginMsg + "/2=" + GetIdentity().GetPlainName();
			loginMsg = loginMsg + "/3=" + GetIdentity().GetFullName();
			loginMsg = loginMsg + "/cached=" + GetCachedName();
			loginMsg = loginMsg + "/ netid=" + GetIdentity().GetPlayerId();
			ZMPrint(loginMsg);
		}*/

		//! IMMERSIVE LOGIN
		ZenImmersiveLogin_OnPlayerLoaded();

		//! UTILITIES
		ZenUtilities_OnPlayerLoaded();
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

		//! LOOT CYCLING DETECTION
		/*if (!GetZenUtilitiesConfig().ShouldLogLootCyclers)
			return;

		if (!ZEN_LOOT_CYCLING_PLAYERAGE)
			ZEN_LOOT_CYCLING_PLAYERAGE = new map<string, int>;

		int secondsPlayed = GetGame().GetTickTime() - m_ZenTimeStarted;
		ZEN_LOOT_CYCLING_PLAYERAGE.Set(GetCachedID(), secondsPlayed);*/
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		//! CHICKEN COOPS
		AddAction(ActionZenMineCoopHands, InputActionMap);

		//! FIREWOOD 
		AddAction(ActionMineZenWoodStackHand, InputActionMap);

		//! CATCH RAIN 
		AddAction(ActionZenWashHandsRain, InputActionMap);

		//! SLEEPING BAGS
		//AddAction(ActionZenPackSleepingBag, InputActionMap);
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
			Mission mission = GetGame().GetMission();
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

	override void OnUnconsciousStart()
	{
		super.OnUnconsciousStart();

		//! IMMERSIVE LOGIN 
		if (!ZenLoginHasFinished())
			ZenImmersiveLoginFinished();
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (GetGame().IsClient())
			return;

		//! ARTILLERY
		if (ammo.Contains("Zen_ArtilleryBomb_Ammo"))
			HandleZenArtillery(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! CONCUSSION GRENADE 
		if (ammo == "Zen_ConcussionGrenade_Ammo")
		{
			HandleZenConcussionGrenade();
		}

		//! UTILITIES 
		HandleZenUtilities(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
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
		GetGame().GetPlayers(players);
		
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
#ifdef EXPANSIONMODBASEBUILDING
		ExpansionTerritoryModule expansionTerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		if (expansionTerritoryModule != NULL)
			return expansionTerritoryModule.IsInsideOwnTerritoryOrPerimeter(pos, -1, -1, uid);
#endif

		// Check basic territories by Daemonforge
#ifdef BASICTERRITORIES
		TerritoryFlag tflag = GetNearestTerritoryFlag(pos);
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
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenConcussionGrenadeManager, 1000, false);
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
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenConcussionGrenadeManager, 1000, false);
		}
	}

	//! ARTILLERY
	void HandleZenArtillery(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		// If it should kill players, kill them
		if (GetZenArtilleryConfig().Player_Kill)
		{
			SetHealth(0);
		}
		else
		{
			// Deal damage if specified in config
			if (GetZenArtilleryConfig().Player_ExtraDamage > 0)
			{
				SetHealth(GetHealth() - GetZenArtilleryConfig().Player_ExtraDamage);
			}
		}

		// Force-break player legs if enabled in config
		if (GetZenArtilleryConfig().Player_BreakLegs && GetBrokenLegs() != eBrokenLegs.BROKEN_LEGS)
		{
			DamageAllLegs(GetMaxHealth("RightLeg", "") + 1);
			ProcessDirectDamage(DamageType.CUSTOM, this, "", "FallDamageHealth", WorldToModel(GetPosition()), 0.01);
		}

		// Make player bleed if enabled in config
		if (GetZenArtilleryConfig().Player_CauseBleeds)
		{
			bool sendInjuredSound = false;

			if (GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("RightFoot"))
				sendInjuredSound = true;

			if (!sendInjuredSound && GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("RightForeArm"))
				sendInjuredSound = true;

			// Send injured sound if player is injured
			if (sendInjuredSound)
				SendSoundEvent(EPlayerSoundEventID.INJURED_LIGHT);
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

	//! REPAIR WELLS & PUMPS HELPER FUNCTION (ZenModEnabled check is in the well/pump object itself)
	bool IsAllowedZenWellOrPumpUsage(ActionData action_data)
	{
		if (!action_data || !action_data.m_Target || !action_data.m_Target.GetObject() || !GetGame().IsDedicatedServer())
			return true;
		
		Well wellObj = NULL;
		Land_FuelStation_Feed pumpObj = NULL;

		if (action_data.m_Target.GetObject().IsInherited(Well))
		{
			wellObj = Well.Cast(action_data.m_Target.GetObject());
		}
		else 
		if (action_data.m_Target.GetObject().IsInherited(Land_FuelStation_Feed))
		{
			pumpObj = Land_FuelStation_Feed.Cast(action_data.m_Target.GetObject());
		}

		if (wellObj)
		{
			return wellObj.Zen_CanBeUsed(this);
		}
		else 
		if (pumpObj)
		{
			return pumpObj.Zen_CanBeUsed(this);
		}
		
		return true;
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

	//! IMMERSIVE LOGIN
	// (Client Only) Used to determine if client player should lie down this login, or just sit down.
	static bool HAS_LOGGED_IN_THIS_SESSION = false;

	// Visual FX variables
	private static const float DEFAULT_SPAWNFX_DURATION = 500;
	private float m_IL_m_SpawnDarkeningCurrentTime;
	private ref PPERequester_ZenSpawnEffects m_ZenLoginPPEEffect;

	// Login sync variables
	private bool m_ZenLoginHasFinishedClient;
	private bool m_ZenLoginHasFinishedServer;

	void ZenImmersiveLogin_OnPlayerLoaded()
	{
		if (!ZenModEnabled("ZenImmersiveLogin") || !(GetGame().IsClient() && GetZenModPackClientConfig().ImmersiveLogin))
		{
			m_ZenLoginHasFinishedClient = true;
			m_ZenLoginHasFinishedServer = true;
			SetSynchDirty();
			return;
		}

		if (GetGame().IsClient())
		{
			if (GetGame())
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ImmersiveLoginFailsafe, 10000);

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenLoginBegin);
			return;
		}

		m_ZenLoginHasFinishedServer = ZenLoginOverrideAI();
	}

	void ImmersiveLogin_OnPostFrame()
	{
		if (!ZenModEnabled("ZenImmersiveLogin"))
			return;

		// Don't apply to client player or if this player has sync'd as logged in
		if (m_ZenLoginHasFinishedClient || GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_REMOTE)
			return;

		// Check that this isn't a loading screen character before setting it invisible
		if (GetDayZGame().GetMissionState() == DayZGame.MISSION_STATE_MAINMENU)
			return;

		// Set invisible by clearing Visible entity flag
		ClearFlags(EntityFlags.VISIBLE, true);
	}

	void ImmersiveLoginSync()
	{
		if (m_ZenLoginHasFinishedClient == m_ZenLoginHasFinishedServer)
			return;

		m_ZenLoginHasFinishedClient = m_ZenLoginHasFinishedServer;
		if (m_ZenLoginHasFinishedClient && !IsFlagSet(EntityFlags.VISIBLE))
		{
			// Player who is not us has sync'd as logged in, set them visible
			if (!IsControlledPlayer())
			{
				SetFlags(EntityFlags.VISIBLE, true);
				Update();
			}
		}
	}

	// Failsafe for invisibility (eg. if you're using Admin tools to spawn a Survivor NPC in, which will stay invisible without this)
	protected void ImmersiveLoginFailsafe()
	{
		m_ZenLoginHasFinishedServer = true;
		m_ZenLoginHasFinishedClient = true;
	}

	// eAI = Expansion AI compatibility (if true then they are not invisible on spawn - override if needed for other mods)
	bool ZenLoginOverrideAI()
	{
		return GetType().Contains("eAI"); 
	}

	protected void ZenLoginBegin()
	{
		if (!ZenModEnabled("ZenImmersiveLogin"))
			return;

		if (!IsControlledPlayer() || GetType().Contains("_Ghost")) // Ghost = Syberia mod compatibility
			return;

		if (ZenImmersiveLoginCancelFX())
		{
			ZenImmersiveLoginFinished();
			return;
		}

		// This is our player object and we're fine, close eyes & lie down
		ZenImmersiveLogin();
	}

	protected bool ZenImmersiveLoginCancelFX()
	{
		return IsRestrained() || IsUnconscious() || IsFalling() || !IsAlive() || IsSwimming() || !ZenModEnabled("ZenImmersiveLogin");
	}

	protected void ZenImmersiveLogin()
	{
		if (!GetEmoteManager() || GetEmoteManager().IsEmotePlaying())
		{
			ZenImmersiveLoginFinished();
			return;
		}

		// Don't use immersive login for AI
		#ifdef ENFUSION_AI_PROJECT
		if (IsAI())
		{
			ZenImmersiveLoginFinished();
			return;
		}
		#endif

		if (!HAS_LOGGED_IN_THIS_SESSION)
		{
			// Lie down or sit if lie-down emote is unavailable (eg. player is in shallow water or a tight space)
			if (GetEmoteManager().CanPlayEmote(EmoteConstants.ID_EMOTE_LYINGDOWN))
			{
				GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_LYINGDOWN, true);
			}
			else
			{
				GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_SITA, true);
			}
		}
		else
		{
			GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_SITA, true);
		}

		GetGame().GetMission().GetHud().ShowQuickbarUI(false);

		// Create blackout screen effect
		m_IL_m_SpawnDarkeningCurrentTime = DEFAULT_SPAWNFX_DURATION;
		m_ZenLoginPPEEffect = PPERequester_ZenSpawnEffects.Cast(PPERequesterBank.GetRequester(PPERequester_ZenSpawnEffects));

		// Set login flags
		m_ZenLoginHasFinishedClient = false;

		if (!m_ZenLoginPPEEffect)
			return;

		m_ZenLoginPPEEffect.SetEffectValues(200);
		GetGame().GetSoundScene().SetSoundVolume(0, 0);
		//ZenFunctions.SetPlayerControl(false);

		// Schedule "wake up" 6 seconds after player loads
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenLoginOpenEyesClient, 6000, false);
	}

	// Trigger eye-open fx and enable sound
	protected void ZenLoginOpenEyesClient()
	{
		// Start opening eyes fx, and slowly fade-in volume over 5 secs
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenLoginUpdateSpawnDarknessLevel, 1, false);
		GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound, 5);
		//ZenFunctions.SetPlayerControl(true);
		HAS_LOGGED_IN_THIS_SESSION = true;
	}

	// Simulate Spawn visual effect
	protected void ZenLoginUpdateSpawnDarknessLevel()
	{
		// Check fx PPE
		if (!m_ZenLoginPPEEffect)
			return;

		// Calculate vignette percentage
		float percentage = Math.Lerp(0, 2, 1 - ((DEFAULT_SPAWNFX_DURATION - m_IL_m_SpawnDarkeningCurrentTime) / DEFAULT_SPAWNFX_DURATION));

		// Decrease fx timer
		m_IL_m_SpawnDarkeningCurrentTime -= 1;
		m_ZenLoginPPEEffect.SetEffectValues(percentage);

		// If animation of opening eyes is over, stop updating.
		if (percentage <= 0 || ZenImmersiveLoginCancelFX())
		{
			// If our eyes are no longer shut, remove the Spawn requester effect altogether
			if (m_ZenLoginPPEEffect)
			{
				m_ZenLoginPPEEffect.SetEffectValues(0);
				m_ZenLoginPPEEffect.Stop();
				m_ZenLoginPPEEffect = null;
			}

			// Stop update loop
			return;
		}

		// Re-schedule update loop
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenLoginUpdateSpawnDarknessLevel, 1, false);
	}

	void ZenImmersiveLoginFinished()
	{
		m_ZenLoginHasFinishedClient = true;

#ifdef SERVER
		m_ZenLoginHasFinishedServer = true;
		SetSynchDirty();
#endif
	}

	bool ZenLoginHasFinished()
	{
		return !ZenModEnabled("ZenImmersiveLogin") || m_ZenLoginHasFinishedClient || m_ZenLoginHasFinishedServer;
	}

	//! UTILITIES
	private int m_ZenTimeStarted;
	protected ref ZenPlayerMessage m_CachedZenPlayerMessage; // Cached admin message, allows admin to spawn items on player via JSON

	// Sends a text message to the client (Zen_ to prevent conflicts with other mods that might use the same method name)
	void Zen_SendMessage(string message)
	{
#ifdef SERVER
		Param1<string> m_MessageParam = new Param1<string>("");
		if (message != "" && m_MessageParam != NULL && !IsPlayerDisconnected())
		{
			m_MessageParam.param1 = message;
			GetGame().RPCSingleParam(this, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, GetIdentity());
		}
#endif
	}

	// Sends a text message with X miliseconds delay
	void Zen_SendMessageDelayed(string message, float timeDelay)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Zen_SendMessage, timeDelay, false, message);
	}

	// Print a client-side white text message
	void Zen_DisplayClientMessage(string message)
	{
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
		}
	}

	void Zen_PutItemInHandsAway()
	{
		//if (GetGame().IsDedicatedServer())
		//	return;

		EntityAI itemInHands = GetHumanInventory().GetEntityInHands();
		
		if (!itemInHands.GetInventory().CanRemoveEntity())
			return;

		if (GetInventory().CanAddEntityToInventory(itemInHands) && GetHumanInventory().CanRemoveEntityInHands())
		{
			PredictiveTakeEntityToInventory(FindInventoryLocationType.ANY, itemInHands);
		}
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

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

		ZenLogger(killer);
	}

	// Log player's items on death
	void ZenLogger(Object killer)
	{
		if (!GetZenUtilitiesConfig().ShouldLogDeathGear || !killer)
			return;

		LogPlayerDeathItems(this);

		// Check if log PVP is enabled
		if (!GetZenUtilitiesConfig().ShouldLogPVP)
			return;

		if (!killer)
		{
			ZenModLogger.Log("Player death: " + GetCachedName() + " (" + GetCachedID() + ") was killed by a null object / unknown (pos=" + GetPosition() + ")", "pvp");
			return;
		}

		// Player melee kill
		PlayerBase player = PlayerBase.Cast(killer);
		if (player && player.GetIdentity())
		{
			ZenModLogger.Log("PLAYER PVP KILL: " + player.GetCachedName() + " (" + player.GetCachedID() + ") killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
			return;
		}
		else
		if (player && player.GetType().Contains("eAI")) // For Expansion AI compatibility
		{
			ZenModLogger.Log("PLAYER AI KILL: " + player.GetType() + " killed " + GetCachedName() + " (" + GetCachedID() + ") pos=" + GetPosition() + " (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
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

				ZenModLogger.Log("Player death: " + identity + " killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos = " + GetPosition() + ") (killerHealth = " + killer.GetHealth().ToString() + ")", "pvp");
				return;
			}

			player = PlayerBase.Cast(man);
			if (player && player.GetIdentity())
			{
				if (player.GetIdentity())
				{
					identity = player.GetCachedName() + " (" + player.GetCachedID() + ")";
				}

				ZenModLogger.Log("PLAYER PVP KILL: " + identity + " killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + player.GetHealth().ToString() + ")", "pvp");
				return;
			}
			else
			if (player && player.GetType().Contains("eAI")) // For Expansion AI compatibility
			{
				ZenModLogger.Log("PLAYER AI KILL: " + player.GetType() + " killed " + GetCachedName() + " (" + GetCachedID() + ") pos=" + GetPosition() + " (killerHealth=" + killer.GetHealth().ToString() + ")", "pvp");
				return;
			}
		}

		ZenModLogger.Log("Player death: " + GetCachedName() + " (" + GetCachedID() + ") was killed by " + killer.GetType() + " (pos=" + GetPosition() + ")", "pvp");
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

		ZenModLogger.Log("PlayerInventory (" + player.GetCachedID() + "/" + player.GetCachedName() + ")\n" + logMsg, "pvp");
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
			ZenModLogger.Log("PLAYER PVP AI HIT: " + GetCachedName() + " (" + GetCachedID() + ") was hit by " + source.GetType() + " by " + ammo + " (dmgZone=" + dmgZone + " health=" + GetHealth().ToString() + ") @ " + GetPosition(), "pvp");
			return;
		}

		if (player && player.GetIdentity())
		{
			if (player.GetCachedID() != GetCachedID())
				ZenModLogger.Log("PLAYER PVP HIT: " + player.GetCachedName() + " (" + player.GetCachedID() + ") hit " + GetCachedName() + " (" + GetCachedID() + ") with " + ammo + " (dmgZone=" + dmgZone + " health=" + GetHealth().ToString() + ")", "pvp");
		}
	}

	void ZenUtilities_OnPlayerLoaded()
	{
		// Save player login timestamp 
		m_ZenTimeStarted = GetGame().GetTickTime();

		// Reload admin->player msg JSON in case it has changed during session
		GetZenPlayerMessageConfig().Load();

		// Reset player message
		m_CachedZenPlayerMessage = NULL;

		// Check config on delay
#ifdef SERVER
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckZenPlayerMsgConfig, 5000, false);
#endif
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
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenPlayerMessageReply, 1000, false, playerMsg);
			return;
		}

		string loginMsg = GetZenUpdateMessage().LOGIN_MESSAGE;
		loginMsg.Replace("#survivorname#", GetCachedName());
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

		ZenModLogger.Log("Player " + GetCachedName() + " (" + GetCachedID() + ") has received your admin message:");
		ZenModLogger.Log(message.Message);

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

				item = EntityAI.Cast(GetGame().CreateObject(className, GetPosition(), false));
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

		if (ZenModEnabled("ZenGlovebox") || ZenModEnabled("ZenCarCompass"))
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

	//! SPLIT ITEM UI 
	override bool HandleRemoteItemManipulation(int userDataType, ParamsReadContext ctx)
	{
		// Custom item manipulation type: split a stackable item based on client-requested quantity instead of 50% stack.
		if (userDataType == ZenRPCs.SPLIT_ITEM)
		{
			ItemBase item1	= null;
			int quantity	= 1;

			if (!ctx.Read(quantity))
				return false;

			if (!ctx.Read(item1))
				return false;

			InventoryLocation dst = new InventoryLocation;
			if (dst.ReadFromContext(ctx))
			{
				item1.SplitItemToInventoryLocation_ZenSplit(dst, quantity);
				return true;
			}

			return false;
		}

		return super.HandleRemoteItemManipulation(userDataType, ctx);
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
					StartZenAlcoholTimer();
					m_ZenAlcoholConsumed += data.m_Amount;
					GiveShock(data.m_Amount * -0.1);
				}
			}
        }

		return result;
	}

	void StartZenAlcoholTimer()
	{
		#ifdef TerjeMedicine
		return; // let terje mod handle it.
		#endif

		if (!m_ZenAlcoholUpdateTimer || !m_ZenAlcoholUpdateTimer.IsRunning())
		{
			m_ZenAlcoholUpdateTimer = new Timer();
			m_ZenAlcoholUpdateTimer.Run(60, this, "UpdateZenAlcohol", NULL, true);
		}
	}

	//! PERSISTENCE
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		if (ZenModEnabled("ZenAlcohol"))
		{
			ctx.Write(m_ZenAlcoholConsumed);
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
		{
			return false;
		}

		if (ZenModEnabled("ZenAlcohol"))
		{
			if (!ctx.Read(m_ZenAlcoholConsumed))
			{
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