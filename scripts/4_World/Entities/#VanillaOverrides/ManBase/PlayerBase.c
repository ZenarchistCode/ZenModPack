modded class PlayerBase
{
	//! SHARED
	static int ZENMOD_PLAYER_COUNT = 0;
	static int ZENMOD_DRIVER_COUNT = 0;

	private int m_ZenPlayerUID;

	override void Init()
	{
		super.Init();

		//! IMMERSIVE LOGIN
		if (GetGame())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ImmersiveLoginFailsafe, 10000);
		RegisterNetSyncVariableBool("m_ZenLoginHasFinishedServer");

		//! SHARED
		RegisterNetSyncVariableInt("m_ZenPlayerUID");

		//! REPAIR WELLS
		RegisterNetSyncVariableBool("m_ZenPreventWellUsage");

		//! REPAIR PUMPS
		RegisterNetSyncVariableBool("m_ZenPreventPumpUsage");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		//! IMMERSIVE LOGIN
		ImmersiveLoginSync();
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
		AddAction(ActionMineCoopHands, InputActionMap);

		//! FIREWOOD 
		AddAction(ActionMineZenWoodStackHand, InputActionMap);

		//! CATCH RAIN 
		AddAction(ActionWashHandsRain, InputActionMap);

		//! SLEEPING BAGS
		AddAction(ActionZenPackSleepingBag, InputActionMap);

		//! SHOVE PLAYER 
		AddAction(ActionZenShovePlayer, InputActionMap);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

		//! AUTO RUN
        if (rpc_type == ZenRPCs.AUTORUN)
        {
            Param3<bool, int, int> autoRunParams;

            if(!ctx.Read(autoRunParams)) 
                return;

            bool isRunning  = autoRunParams.param1;
            int movementIdx = autoRunParams.param2;
            int angle       = autoRunParams.param3;

            GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
            GetInputController().OverrideMovementAngle(isRunning, angle);
        }

		//! ANTI-COMBAT LOG
		// Client-side (client receives logout message info from server)
		if (rpc_type == ZenRPCs.ANTI_COMBAT_LOG_MSG_RPC)
		{
			Param2<int, int> antiCombatLog_ClientParams;

			if (!ctx.Read(antiCombatLog_ClientParams))
				return;

			m_WillBePunishedForCombatLogging = antiCombatLog_ClientParams.param1;
			m_DisableExitButtonSecs = antiCombatLog_ClientParams.param2;
		}

		// Server-side (server receives notification that we shot at someone)
		if (rpc_type == ZenRPCs.ANTI_COMBAT_LOG_RPC)
		{
			Param1<int> antiCombatLog_ServerParams;

			if (!ctx.Read(antiCombatLog_ServerParams))
				return;

			// Reset our combat logout timer too.
			int highBits, lowBits;
			GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
			PlayerBase shooter = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
			PlayerBase victim = GetPlayerByZenUID(antiCombatLog_ServerParams.param1);

			// If victim exists and they're not dead, reset their combat logout timer.
			if (victim && victim.GetIdentity() && victim.IsAlive())
			{
				victim.SetCombatLogTimer(shooter, victim);
			}

			if (shooter && shooter.IsAlive())
			{
				shooter.SetCombatLogTimer(shooter, victim);
			}
		}
    }

	override void OnUnconsciousStart()
	{
		super.OnUnconsciousStart();

        #ifndef SERVER
		//! NOTES
        UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
        if (!menu)
			return;

        ZenNoteGUI noteMenu = ZenNoteGUI.Cast(menu);
        if (noteMenu)
        {
            noteMenu.OnExitBtnClick();
        }
        #endif

		//! IMMERSIVE LOGIN 
		if (!ZenLoginHasFinished())
			ZenImmersiveLoginFinished();
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! ANTI-COMBAT LOG 
		HandleZenAntiCombatLog(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		//! CAUSE OF DEATH 
		HandleZenCauseOfDeath(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

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

	void SetZenPlayerUID(int id)
	{
		m_ZenPlayerUID = id;
		SetSynchDirty();
	}

	int GetZenPlayerUID()
	{
		return m_ZenPlayerUID;
	}

	PlayerBase GetPlayerByZenUID(int id)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		
		foreach (Man playerMan : players)
		{
			PlayerBase player = PlayerBase.Cast(playerMan);
			if (player != NULL && player.GetZenPlayerUID() == id)
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
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenConcussionGrenadeManager, 1000, false);
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
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenConcussionGrenadeManager, 1000, false);
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
	protected bool m_AutoRunning;

	void SetAutoRunning(bool isRunning)
    {
        m_AutoRunning = isRunning;
		if (m_AutoRunning)
        {
			OnSprintStart();
		} else 
        {
			OnSprintEnd();
		}
    }

    bool IsAutoRunning()
    {
        return m_AutoRunning;
    }

	//! REPAIR WELLS 
	protected bool m_ZenPreventWellUsage = false; // If true, then the player is stopped from using wells client-side.

	// Set whether or not the player is to be prevented from using the well.
	void SetPreventWellUsage(bool stop)
	{
		m_ZenPreventWellUsage = stop;
		SetSynchDirty();
	}

	// Returns whether or not the player is currently prevented from using the well.
	bool IsPreventedWellUsage()
	{
		return m_ZenPreventWellUsage;
	}

	//! REPAIR PUMPS 
	protected bool m_ZenPreventPumpUsage = false; // If true, then the player is stopped from using pumps client-side.

	// Set whether or not the player is to be prevented from using the fuel pump.
	void SetPreventPumpUsage(bool stop)
	{
		m_ZenPreventPumpUsage = stop;
		SetSynchDirty();
	}

	// Returns whether or not the player is currently prevented from using the fuel pump.
	bool IsPreventedPumpUsage()
	{
		return m_ZenPreventPumpUsage;
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

	//! CAUSE OF DEATH
	string m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("unknown").CauseMessage;

	// Registers the check COD action
	override void SetActionsRemoteTarget(out TInputActionMap InputActionMap)
	{
		AddAction(ActionCheckCauseOfDeath, InputActionMap);
		super.SetActionsRemoteTarget(InputActionMap);
	}

	// Tracks last known damage source
	void HandleZenCauseOfDeath(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		//! CAUSE OF DEATH 
		if (GetHealth() <= 0 && (ZenModEnabled("ZenCauseOfDeath") || ZenModEnabled("ZenGraves")))
		{
			string cause = "unknown";
			ammo.ToLower();

			if (ammo.Contains("falldamage"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("falldamage").CauseMessage;
			}
			else
			if (ammo.Contains("melee"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("melee").CauseMessage;
			}
			else
			if (ammo.Contains("zombie"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("zombie").CauseMessage;
			}
			else
			if (ammo.Contains("bullet"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("bullet").CauseMessage;
			}
			else
			if (ammo.Contains("explosion"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("explosion").CauseMessage;
			}
			else
			if (ammo.Contains("grenade"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("grenade").CauseMessage;
			}
			else
			if (ammo.Contains("landmine"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("landmine").CauseMessage;
			}
			else
			if (ammo.Contains("transport"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("transport").CauseMessage;
			}
			else
			if (ammo.Contains("wolf"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("wolf").CauseMessage;
			}
			else
			if (ammo.Contains("bear"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("bear").CauseMessage;
			}
			else
			if (ammo.Contains("firedamage"))
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("fire").CauseMessage;
			}

			// Loop through causes of death and see if one is found
			if (cause == "unknown")
			{
				foreach(CauseOfDeathDef cod : GetCauseOfDeathConfig().CauseOfDeathDefs)
				{
					string codKey = cod.AmmoType;
					codKey.ToLower();
					if (ammo.Contains(codKey))
					{
						cause = GetCauseOfDeathConfig().GetCauseOfDeath(codKey).CauseMessage;
					}
				}
			}

			// No cause found in config.
			if (cause == "unknown")
			{
				cause = GetCauseOfDeathConfig().GetCauseOfDeath("unknown").CauseMessage;
			}

			// For debugging / adding new ammo types
			if (GetCauseOfDeathConfig().DebugOn)
			{
				cause = cause + " [ammo=" + ammo + "]";
			}

			m_CauseOfDeath = cause;
		}
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
		if (!ZenModEnabled("ZenImmersiveLogin"))
		{
			m_ZenLoginHasFinishedClient = true;
			m_ZenLoginHasFinishedServer = true;
			SetSynchDirty();
			return;
		}

		if (GetGame().IsClient())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLoginBegin);
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

		// Schedule "wake up" 6 seconds after player loads
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLoginOpenEyesClient, 6000, false);
	}

	// Trigger eye-open fx and enable sound
	protected void ZenLoginOpenEyesClient()
	{
		// Start opening eyes fx, and slowly fade-in volume over 5 secs
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLoginUpdateSpawnDarknessLevel, 1, false);
		GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound, 5);
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
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLoginUpdateSpawnDarknessLevel, 1, false);
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

	//! ANTI-COMBAT LOG 
	private float m_CombatLogTimer = 0; // This is how many ms to wait before we can logout after engaging in combat
	private bool m_KillPlayerForCombatLogging = false; // Server-side flag - if true, player will be killed if they disconnect before timeout
	private int m_WillBePunishedForCombatLogging = 0; // Server & client flag - if true, logout message on client will reflect the severity of their situation
	private int m_DisableExitButtonSecs = 5; // How long to disable the exit button (client-side)

	void HandleZenAntiCombatLog(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!ZenModEnabled("ZenAntiCombatLogout"))
			return;

		PlayerBase attacker;

		// If this player got hit by vehicle, trigger combat log timer for driver
		CarScript vehicle = CarScript.Cast(source);
		if (vehicle)
		{
			attacker = PlayerBase.Cast(vehicle.CrewMember(0));
			if (attacker)
			{
				float speed = 0.0;

				Car car;
				if (Class.CastTo(car, vehicle))
					speed = car.GetSpeedometer();
				else
					speed = GetVelocity(vehicle).Length() * 3.6;

				if (speed > 1 && damageResult && damageResult.GetDamage(dmgZone, "Health") > 1)
				{
					attacker.SetCombatLogTimer(attacker, this);
					return; // No need to continue.
				}
			}
		}

		// Don't trigger combat log timer for dead players
		if (!IsAlive() || !source)
			return;

		attacker = PlayerBase.Cast(source.GetHierarchyRootPlayer());

		// If we were not attacked by another player, don't set our combat timer
		if (!attacker || attacker == this)
			return;

		attacker.SetCombatLogTimer(attacker, this);
		SetCombatLogTimer(attacker, this);
	}

	// Get exit button disabled secs
	int GetDisableExitButtonSecs()
	{
		return m_DisableExitButtonSecs;
	}

	// Inform player they will be killed for combat logging
	void InformPlayerOfCombatLogout(int willBeKilled)
	{
		auto params = new Param2<int, int>(willBeKilled, GetZenAntiCombatLogoutConfig().DisableExitButtonSecs);
		GetGame().RPCSingleParam(this, ZenRPCs.ANTI_COMBAT_LOG_MSG_RPC, params, true, GetIdentity());
		m_WillBePunishedForCombatLogging = willBeKilled;
	}

	// Check if player will be killed or a flare dropped for combat logging
	int WillBePunishedForCombatLogging()
	{
		return m_WillBePunishedForCombatLogging;
	}

	// Stores who shot at who first to track the aggressor in combat <SteamID, AggressorStatus>
	ref map<string, bool> m_ShotAtUsFirst = new ref map<string, bool>;

	// Reset combat log timer
	void SetCombatLogTimer(PlayerBase attacker = NULL, PlayerBase victim = NULL)
	{
		// Do we have both a victim and an attacker in this altercation?
		if (attacker != victim && victim != NULL && attacker != NULL)
		{
			//! On first instance of assault, attacker is presumed the aggressor, and victim is presumed innocent. 
			//! Not always the case but whatever. Not gonna track who aimed at who first or who talked shit first.
			//! Mainly used for situations where 'estimating' who started the fight is ok, like integrations with ExpansionAI guards.
			if (!victim.m_ShotAtUsFirst.Contains(attacker.GetCachedID())) 
			{
				attacker.m_ShotAtUsFirst.Set(victim.GetCachedID(), false);
				victim.m_ShotAtUsFirst.Set(attacker.GetCachedID(), true);
			}
		}

		m_CombatLogTimer = GetGame().GetTime() + (GetZenAntiCombatLogoutConfig().CombatLogoutSecs * 1000);
	}

	// Check if we started combat with the given player
	bool DidWeStartCombatWith(notnull PlayerBase enemy)
	{
		bool weShotFirst = false;
		enemy.m_ShotAtUsFirst.Find(GetCachedID(), weShotFirst);
		return weShotFirst;
	}

	// Resets our combat log timer
	void ResetCombatLogTimer()
	{
		m_CombatLogTimer = 0;
	}

	// Get the current combat logout timer (resets whenever damage is dealt or a shot is fired at us)
	float GetCombatLogTimer()
	{
		return m_CombatLogTimer;
	}

	// (Client-side) Sends an RPC to the server notifying it that we shot at some poor fucker
	void InformServerThatWeShotAt(notnull PlayerBase player)
	{
		auto params = new Param1<int>(player.GetZenPlayerUID());
		GetGame().RPCSingleParam(this, ZenRPCs.ANTI_COMBAT_LOG_RPC, params, true);
	}

	//! GRAVES

	// @override this to change player name for certain mods
	string GetZenGravePlayerName()
	{
		if (GetZenGravesConfig().ShowPlayerName)
			return GetCachedName();

		return GetZenGravesConfig().UnknownPerson;
	}
	
	/* 
	// This is included for compatibility with mods that change the player's name using something other than their steam profile
	// eg. SyberiaServer mod should override this to return CharProfile name, don't forget to add ZenGraves to requiredAddons in config.cpp when overriding 
	override string GetZenGravePlayerName()
	{
		if (m_charProfile)
			return m_charProfile.m_name;

		return GetZenGravesConfig().UnknownPerson;
	}
	*/

	bool HasGraveIdentity()
	{
		#ifdef ENFUSION_AI_PROJECT
		if (IsAI())
			return false;
		#endif

		return GetCachedID() && GetCachedID() != string.Empty;
	}

	string GetZenGravesCauseOfDeath()
	{
		if (!m_CauseOfDeath || m_CauseOfDeath == string.Empty)
			return GetZenGravesConfig().GetCauseOfDeath("unknown").CauseMessage;

		return m_CauseOfDeath;
	}

	void SetZenGravesCauseOfDeath(string msg)
	{
		m_CauseOfDeath = msg;
	}

	int GetZenGravesAgeMinutes()
	{
		return StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
	}

	// Generate ZenDeadPlayerData object
	ZenDeadPlayerData GenerateZenDeadPlayerData()
	{
		if (!HasGraveIdentity())
		{
			GetZenGravesConfig().DebugMsg("GenerateZenDeadPlayerData() - No steam ID found!");
			return null;
		}

		// Don't spawn grave for admin account etc
		foreach (string s : GetZenGravesConfig().IgnoreSteamIDs)
		{
			if (s == GetCachedID())
				return null;
		}

		// Don't spawn grave in safezones etc
		foreach (ZenNoSpawnCrossZone zone : GetZenGravesConfig().NoCrossZones)
		{
			if (vector.Distance(zone.Position, GetPosition()) < zone.Distance)
				return null;
		}

		// Don't spawn a cross within X meters of another cross config
		for (int i = GetZenGravesConfig().DeadPlayers.Count() - 1; i >= 0; i--)
		{
			if (vector.Distance(GetZenGravesConfig().DeadPlayers.Get(i).Position, GetPosition()) <= GetZenGravesConfig().MinDistanceBetweenCrosses)
			{
				GetZenGravesConfig().DebugMsg("GenerateZenDeadPlayerData() - Distance to existing cross too close!");
				return null;
			}

			// Remove any duplicates stored in JSON config - only spawn 1 cross per server restart per player
			if (GetZenGravesConfig().DeadPlayers.Get(i).SteamID == GetCachedID())
			{
				GetZenGravesConfig().DeadPlayers.Remove(i);
			}
		}

		int year, month, day;
		GetYearMonthDay(year, month, day);

		// Save new cross info
		return new ZenDeadPlayerData(
			GetCachedID(),
			GetZenGravePlayerName(),
			GetZenGravesCauseOfDeath(),
			GetPosition(),
			GetOrientation(),
			GetZenGravesAgeMinutes(),
			day,
			month,
			year);
	}

	// Save all items to JSON to be spawned as a stash when corpse despawns
	void SaveZenSkeletonItems()
	{
		if (!ZenModEnabled("ZenGraves"))
			return;

		// Check that we have an identity
		if (!HasGraveIdentity())
		{
			GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - Don't save: No steam identity found!");
			return;
		}

		// Roll dice for ChanceOfGrave
		if (Math.RandomFloat01() > GetZenGravesConfig().ChanceOfGrave)
		{
			GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - Don't save: Failed dice roll for ChanceOfGrave");
			return;
		}

		// Check player age
		if (GetZenGravesAgeMinutes() < GetZenGravesConfig().MinPlayerAgeMinutes)
		{
			GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - Don't save: Player underage! (Age=" + GetZenGravesAgeMinutes() + " minutes)");
			return;
		}

		// If server should not spawn a cross for this cause of death, stop here.
		int i;
		for (i = 0; i < GetZenGravesConfig().CrossCauseOfDeath.Count(); i++)
		{
			ZenGravesCauseOfDeath cod = GetZenGravesConfig().CrossCauseOfDeath.Get(i);
			if (cod.CauseMessage == GetZenGravesCauseOfDeath() && cod.SpawnCross == false)
			{
				GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - Do not spawn a cross for death cause: " + GetZenGravesCauseOfDeath());
				return;
			}
		}

		string surface_type;
		vector position = GetPosition();

		// Syberia compatibility - (Syberia spawns characters 10km in the sky during character creation screen)
		if (position[1] > 9000)
			return;

		GetGame().SurfaceGetType(position[0], position[2], surface_type);

		// Check that we can actually bury stuff at this location
		if (!GetGame().IsSurfaceDigable(surface_type))
		{
			GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - Don't save: Surface is not diggable, don't spawn a cross.");
			return;
		}

		// Generate new cross info
		ZenDeadPlayerData deadData = GenerateZenDeadPlayerData();
		if (!deadData)
			return;

		// Store clothing & shoulder slots first
		ItemBase Shoulder = ItemBase.Cast(FindAttachmentBySlotName("Shoulder"));
		if (Shoulder && !Shoulder.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Shoulder.GetType(), 1, Shoulder.GetHealth(), true));
		}

		ItemBase Melee = ItemBase.Cast(FindAttachmentBySlotName("Melee"));
		if (Melee && !Melee.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Melee.GetType(), 1, Melee.GetHealth(), true));
		}

		ItemBase Headgear = ItemBase.Cast(FindAttachmentBySlotName("Headgear"));
		if (Headgear && !Headgear.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Headgear.GetType(), 1, Headgear.GetHealth(), true));
		}

		ItemBase Eyewear = ItemBase.Cast(FindAttachmentBySlotName("Eyewear"));
		if (Eyewear && !Eyewear.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Eyewear.GetType(), 1, Eyewear.GetHealth(), true));
		}

		ItemBase Mask = ItemBase.Cast(FindAttachmentBySlotName("Mask"));
		if (Mask && !Mask.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Mask.GetType(), 1, Mask.GetHealth(), true));
		}

		ItemBase Body = ItemBase.Cast(FindAttachmentBySlotName("Body"));
		if (Body && !Body.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Body.GetType(), 1, Body.GetHealth(), true));
		}

		ItemBase Gloves = ItemBase.Cast(FindAttachmentBySlotName("Gloves"));
		if (Gloves && !Gloves.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Gloves.GetType(), 1, Gloves.GetHealth(), true));
		}

		ItemBase Vest = ItemBase.Cast(FindAttachmentBySlotName("Vest"));
		if (Vest && !Vest.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Vest.GetType(), 1, Vest.GetHealth(), true));
		}

		ItemBase Hips = ItemBase.Cast(FindAttachmentBySlotName("Hips"));
		if (Hips && !Hips.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Hips.GetType(), 1, Hips.GetHealth(), true));
		}

		ItemBase Legs = ItemBase.Cast(FindAttachmentBySlotName("Legs"));
		if (Legs && !Legs.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Legs.GetType(), 1, Legs.GetHealth(), true));
		}

		ItemBase Feet = ItemBase.Cast(FindAttachmentBySlotName("Feet"));
		if (Feet && !Feet.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Feet.GetType(), 1, Feet.GetHealth(), true));
		}

		ItemBase Back = ItemBase.Cast(FindAttachmentBySlotName("Back"));
		if (Back && !Back.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Back.GetType(), 1, Back.GetHealth(), true));
		}

		ItemBase Armband = ItemBase.Cast(FindAttachmentBySlotName("Armband"));
		if (Armband && !Armband.IsRuined() && Math.RandomFloat01() <= GetZenGravesConfig().ChanceOfSkeletonClothing)
		{
			deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(Armband.GetType(), 1, Armband.GetHealth(), true));
		}

		Ammunition_Base ammo;
		Magazine magazine;
		float quantity;
		array<EntityAI> itemsArray = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, itemsArray);

		// Player has no items to store - stop here.
		if (itemsArray.Count() == 0)
		{
			GetZenGravesConfig().DebugMsg("SaveZenSkeletonItems() - No inventory items found!");
			return;
		}

		for (i = 0; i < itemsArray.Count(); i++)
		{
			EntityAI item = itemsArray.Get(i);
			if (item && !item.IsMan())
			{
				// Skip all clothing items as they're saved separately above
				if (item == Shoulder)
					continue;
				if (item == Melee)
					continue;
				if (item == Headgear)
					continue;
				if (item == Eyewear)
					continue;
				if (item == Mask)
					continue;
				if (item == Body)
					continue;
				if (item == Gloves)
					continue;
				if (item == Vest)
					continue;
				if (item == Hips)
					continue;
				if (item == Legs)
					continue;
				if (item == Feet)
					continue;
				if (item == Back)
					continue;
				if (item == Armband)
					continue;

				// Skip if item is "dummy" item (eg. Magnum_Cylinder)
				if (item.IsInherited(DummyItem))
					continue;

				// Skip if dice% fails
				if (Math.RandomFloat01() > GetZenGravesConfig().ChanceOfSkeletonItems)
					continue;

				quantity = item.GetQuantity();

				// Ignore magazines on purpose. CBF checking what ammo is in them, spawn the mags, but assume that the ammo was pilfered by whoever buried the body
				ammo = Ammunition_Base.Cast(item);
				if (ammo)
				{
					quantity = ammo.GetAmmoCount();
				}
				else
				if (item.HasEnergyManager() && item.GetCompEM())
				{
					quantity = item.GetCompEM().GetEnergy();
				}

				deadData.SkeletonItems.Insert(new ZenGraves_InventoryItem(item.GetType(), quantity, item.GetHealth()));
			}
		}

		// Add player data to JSON
		GetZenGravesConfig().DeadPlayers.Insert(deadData);
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
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Zen_SendMessage, timeDelay, false, message);
	}

	// Print a client-side white text message
	void Zen_DisplayClientMessage(string message)
	{
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
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

		if (ZenModEnabled("ZenEngraveWeapon"))
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

		// Check player melee kill & guns through HierarchyRootPlayer
		PlayerBase playerKiller = PlayerBase.Cast(killerAI);
		if (!playerKiller)
			playerKiller = PlayerBase.Cast(killerAI.GetHierarchyRootPlayer());

		if (!playerKiller)
		{
			// Check for grenades 
			Grenade_Base grenade = Grenade_Base.Cast(killerAI);
			if (grenade != NULL)
				playerKiller = PlayerBase.Cast(grenade.Zen_GetUnpinPlayer());

			// Check for traps 
			if (!playerKiller)
			{
				TrapBase trap = TrapBase.Cast(killerAI);
				if (trap != NULL)
					playerKiller = PlayerBase.Cast(trap.Zen_GetPlayerTrapper());
			}

			// Check for vehicles
			if (!playerKiller)
			{
				CarScript vehicle = CarScript.Cast(killerAI);
				if (vehicle != NULL)
				{
					for (int index = 0; index < vehicle.CrewSize(); index++)
					{
						if (vehicle.CrewMember(index) != NULL && vehicle.GetSeatAnimationType(index) == DayZPlayerConstants.VEHICLESEAT_DRIVER)
							playerKiller = PlayerBase.Cast(vehicle.CrewMember(index));
					}
				}
			}

			//! TODO: ChemGas and projectile explosions/crossbow...
		}

		if (!playerKiller || !playerKiller.GetIdentity() || playerKiller == this)
			return;

		ZenKillFeed(playerKiller, killer);
		ZenLogger(killer);
	}

	// Override this for things like Syberia mod to add character name
	string GetZenKillFeedName()
	{
		return GetCachedName();
	}

	// Send kill feed info
	void ZenKillFeed(notnull PlayerBase playerKiller, Object object)
	{
		if (!GetZenDiscordConfig().EnableDiscordKillFeed || !object)
			return;

		vector myPos = GetPosition();
		string killerText = playerKiller.GetZenKillFeedName();
		string victimText = GetZenKillFeedName();
		string location = GetZenDiscordConfig().GetMapLinkPosition(myPos);
		string distance = MiscGameplayFunctions.TruncateToS(vector.Distance(myPos, playerKiller.GetPosition()));

		if (GetZenDiscordConfig().DisplayPlayerSteamID)
		{
			killerText = "[" + playerKiller.GetZenKillFeedName() + "](http://steamcommunity.com/profiles/" + playerKiller.GetIdentity().GetPlainId() + ")";
			victimText = "[" + GetZenKillFeedName() + "](http://steamcommunity.com/profiles/" + GetIdentity().GetPlainId() + ")";
		}

		string weaponText = "";
		if (object.GetType() == playerKiller.GetType())
			weaponText = GetZenDiscordConfig().Melee;
		weaponText = object.GetDisplayName();

		int victimPlayMinutes = StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		int killerPlayerMinutes = playerKiller.StatGet(AnalyticsManagerServer.STAT_PLAYTIME) / 60;
		string victimAge = GetZenDiscordConfig().VictimAge + ": ";
		string killerAge = GetZenDiscordConfig().KillerAge + ": ";
		if (victimPlayMinutes > 60)
			victimAge = victimAge + (victimPlayMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else
			victimAge = victimAge + (victimPlayMinutes)+" " + GetZenDiscordConfig().Minutes;

		if (killerPlayerMinutes > 60)
			killerAge = killerAge + (killerPlayerMinutes / 60) + " " + GetZenDiscordConfig().Hours;
		else
			killerAge = killerAge + (killerPlayerMinutes)+" " + GetZenDiscordConfig().Minutes;

		string discordMsg = killerText + " " + GetZenDiscordConfig().Killed + " ";
		discordMsg = discordMsg + victimText + " " + GetZenDiscordConfig().With + " " + weaponText + " " + distance + "m ";
		if (GetZenDiscordConfig().DisplayKillLocation)
			discordMsg = discordMsg + location + " ";
		discordMsg = discordMsg + "\n\n" + victimAge + "\n" + killerAge;

		if (GetZenDiscordConfig().DisplayPlayerSteamID)
		{
			discordMsg = discordMsg + "\n\nVictim: " + GetCachedID();
			discordMsg = discordMsg + "\nKiller: " + playerKiller.GetCachedID();
		}

		// Send discord webhook message
		ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().KillFeed);
		msg.SetTitle(GetZenDiscordConfig().KillFeed);
		msg.SetMessage(discordMsg);
		msg.SetColor(255, 255, 255);
		msg.AddWebhooks(GetZenDiscordConfig().KillFeedWebhooks);
		GetZenDiscordAPI().SendMessage(msg);
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

				ZenModLogger.Log("PLAYER PVP KILL: " + identity + " killed " + GetCachedName() + " (" + GetCachedID() + ") with " + killer.GetType() + " (pos=" + GetPosition() + ") (killerHealth=" + killer.GetHealth().ToString() + ")", "pvp");
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
		if (GetIdentity() == NULL || GetType().Contains("_Ghost"))
			return;

		// Load last update ID 
		int timestamp = JMDate.Now(false).GetTimestamp();
		ZenPlayerUpdateMsg updateConfig;
		GetZenUpdateMessage().PlayerUpdates.Find(GetCachedID(), updateConfig);
		if (!updateConfig)
			updateConfig = new ZenPlayerUpdateMsg(GetCachedID(), "-1", timestamp);

		// Send latest update message
		if (GetZenUpdateMessage().UPDATE_MESSAGE != "" && updateConfig.updateID != GetZenUpdateMessage().UPDATE_VERSION)
		{
			Zen_SendMessageDelayed(GetZenUpdateMessage().UPDATE_PREFIX + GetZenUpdateMessage().UPDATE_MESSAGE + GetZenUpdateMessage().UPDATE_SUFFIX, 5000);
			updateConfig.updateID = GetZenUpdateMessage().UPDATE_VERSION;
			updateConfig.lastLoginTimestamp = timestamp;
			GetZenUpdateMessage().PlayerUpdates.Set(GetCachedID(), updateConfig);
		}

		// Check for admin -> player replies
		ZenPlayerMessage playerMsg = GetZenPlayerMessageConfig().GetPlayerMessage(GetCachedID());
		if (!playerMsg)
			GetZenPlayerMessageConfig().GetPlayerMessage(GetIdentity().GetPlainId());

		if (playerMsg && playerMsg.Message != "")
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenPlayerMessageReply, 1000, false, playerMsg);
			return;
		}

		// Check for Poll on login only if there was no admin message
		if (GetZenPollConfig().PollID != -1 && !GetZenPollConfig().PlayerChoices.Contains(GetIdentity().GetId()))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendZenPollData, 1000, false);
			return;
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

		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", new Param1<string>(message.Message), true, GetIdentity());
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

	// Send poll data to player and open screen 
	protected void SendZenPollData()
	{
		if (GetIdentity() != NULL)
		{
			array<string> options = new array<string>;
			options.Insert(GetZenPollConfig().Option1);
			options.Insert(GetZenPollConfig().Option2);
			options.Insert(GetZenPollConfig().Option3);
			options.Insert(GetZenPollConfig().Option4);
			options.Insert(GetZenPollConfig().Option5);
			options.Insert(GetZenPollConfig().Option6);
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenPollOptions", new Param4<string, string, bool, array<string>>(GetZenPollConfig().PollTitle, GetZenPollConfig().PollSubtitle, GetZenPollConfig().OnlyOneOption, options), true, GetIdentity());
		}
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

	//! MUSIC 
	// Allows viewing inventory while inside cars - VicinityItemManager.c handles not being able to pick up items outside of the car
	override void OnCommandVehicleStart()
	{
		super.OnCommandVehicleStart();

		if (!GetInventory())
			return;

		if ((ZenModEnabled("ZenMusic") && GetZenMusicConfig().AllowCarInventory) || ZenModEnabled("ZenGlovebox"))
		{
			GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}
	}

	//! SPLIT ITEM UI 
	override bool HandleRemoteItemManipulation(int userDataType, ParamsReadContext ctx)
	{
		// Custom item manipulation type: split a stackable item based on client-requested quantity instead of 50% stack.
		if (userDataType == ZenRPCs.ZEN_SPLIT_ITEM)
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

	//! SHOVE 
	void Zen_StunFromAngle(vector stunPos)
	{
		if (!stunPos || IsUnconscious() || !IsAlive())
			return;

		vector playerAttackerDirection = vector.Direction(GetPosition(), stunPos);
		vector playerDirection = GetDirection();
		float hitDirection = Math.DiffAngle(playerAttackerDirection.VectorToAngles()[0], playerDirection.VectorToAngles()[0]);

		string hitComponent = GetHitComponentForAI();
		DayZPlayerSyncJunctures.SendDamageHitEx(this, 0, hitDirection, true, null, DT_CLOSE_COMBAT, this, hitComponent, "MeleeZombie", playerDirection);
	}
}
