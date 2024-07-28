class Zen_RaidAlarmStation extends ItemBase
{
	// Persistent data
	protected string m_ZenBaseName = "";
	protected string m_ZenWebhook1 = "";
	protected string m_ZenWebhook2 = "";
	protected string m_ZenWebhook3 = "";
	protected bool m_ZenHadValidRadar = false;

	// Session-specific data
	protected Zen_RaidAlarmRadar m_ZenAttachedRaidRadar;
	protected int m_ZenLastDiscordMessageTime = 0;
	protected bool m_ZenDontNotifyAboutPower = false;
	protected bool m_ZenNotifiedLowPower = false;
	protected bool m_ManualTurnOnOff = false;
	protected ref Timer m_ZenRoofCheckTimer;

	// Client and/or Server
	protected int m_ZenRaidAlarmStatus = ZEN_ALARM_STATUS_OFF; // todo: change to STATUS int. 0 = off, 1 = alarmed, 2 = green, 3 = yellow/orange
	protected int m_ZenRaidAlarmClientStatus = -1;
	protected EffectSound m_ZenSoundAlarmLoop;
	protected ZenRaidAlarmLight m_ZenBlinkingLight;

	// Constants 
	static const int ZEN_ALARM_STATUS_OFF = 0;
	static const int ZEN_ALARM_STATUS_ARMED = 1;
	static const int ZEN_ALARM_STATUS_TRIGGERED = 2;
	static const int ZEN_ALARM_STATUS_WASTRIGGERED = 3;

	///////////////////////////////////////////////////////////////////////
	//! CLIENT-ONLY 
	///////////////////////////////////////////////////////////////////////
#ifndef SERVER
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		// Check for new status sync
		if (m_ZenRaidAlarmClientStatus != m_ZenRaidAlarmStatus)
		{
			switch (m_ZenRaidAlarmStatus)
			{
				case ZEN_ALARM_STATUS_OFF:
					StopAlarmClient();
					break;
				case ZEN_ALARM_STATUS_ARMED:
					ArmAlarmClient();
					break;
				case ZEN_ALARM_STATUS_TRIGGERED:
					TriggerAlarmClient();
					break;
				case ZEN_ALARM_STATUS_WASTRIGGERED:
					StopAlarmSoundClient();
					break;
			}

			m_ZenRaidAlarmClientStatus = m_ZenRaidAlarmStatus;
		}
	}

	void CreateLight()
	{
		if (m_ZenBlinkingLight != NULL)
			return;

		m_ZenBlinkingLight = ZenRaidAlarmLight.Cast(ScriptedLightBase.CreateLightAtObjMemoryPoint(ZenRaidAlarmLight, this, "lightpos"));
		if (!m_ZenBlinkingLight)
			Error("[ZenDiscordAPI] Failed to create ZenRaidAlarmLight!");
	}

	void ArmAlarmClient()
	{
		CreateLight();
		m_ZenBlinkingLight.SetColor(0, 1, 0);
		m_ZenBlinkingLight.SetBlinkingSpeed(0);
	}

	void PlayAlarmSound()
	{
		if (!m_ZenSoundAlarmLoop)
		{
			if (GetMegaphone() != NULL && GetMegaphone().GetCompEM().CanWork())
			{
				PlaySoundSetLoop(m_ZenSoundAlarmLoop, "Zen_RaidAlarm_SoundSet", 0, 0);
			}
		}
	}

	void TriggerAlarmClient()
	{
		PlayAlarmSound();
		CreateLight();
		m_ZenBlinkingLight.SetColor(1, 0, 0);
		m_ZenBlinkingLight.SetBlinkingSpeed(10);
	}

	void StopAlarmSoundClient()
	{
		if (m_ZenSoundAlarmLoop)
			StopSoundSet(m_ZenSoundAlarmLoop);

		CreateLight();
		m_ZenBlinkingLight.SetColor(1, 0.6, 0);
		m_ZenBlinkingLight.SetBlinkingSpeed(0);
	}

	void StopAlarmClient()
	{
		if (m_ZenSoundAlarmLoop)
			StopSoundSet(m_ZenSoundAlarmLoop);

		CreateLight();
		m_ZenBlinkingLight.SetBlinkingSpeed(0);
		m_ZenBlinkingLight.SetBrightnessAdjusted(0);
	}
#endif

	///////////////////////////////////////////////////////////////////////
	//! SHARED
	///////////////////////////////////////////////////////////////////////
	override void InitItemVariables()
	{
		super.InitItemVariables();

		RegisterNetSyncVariableInt("m_ZenRaidAlarmStatus");
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

#ifdef SERVER
		UnregisterRaidStation();
#else
		if (m_ZenSoundAlarmLoop)
			m_ZenSoundAlarmLoop.SoundStop();

		if (m_ZenBlinkingLight)
			m_ZenBlinkingLight.Destroy();
#endif
	}

	int GetAlarmStatus()
	{
		return m_ZenRaidAlarmStatus;
	}

	override bool IsRefresherSignalingViable()
	{
		return !IsRuined();
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		if (slotId == InventorySlots.GetSlotIdFromString("Back"))
			return attachment.IsInherited(BaseRadio);

		return true;
	}

	bool HasWorkingBaseRadio()
	{
		if (!GetBaseRadio() || !GetBaseRadio().IsLockedInSlot())
			return false;

		CarBattery battery = GetCarBattery();
		return battery.GetCompEM() && battery.GetCompEM().CanWork();
	}

	BaseRadio GetBaseRadio()
	{
		int slot = InventorySlots.GetSlotIdFromString("Back");
		BaseRadio radio = BaseRadio.Cast(GetInventory().FindAttachment(slot));
		if (!radio)
			return NULL;

		return radio;
	}

	CarBattery GetCarBattery()
	{
		BaseRadio radio = GetBaseRadio();
		if (!radio)
			return NULL;

		int slot = InventorySlots.GetSlotIdFromString("CarBattery");
		CarBattery battery = CarBattery.Cast(radio.GetInventory().FindAttachment(slot));
		if (!battery || battery.IsRuined())
			return NULL;

		return battery;
	}

	Megaphone GetMegaphone()
	{
		int slot = InventorySlots.GetSlotIdFromString("ZenMegaphone");
		Megaphone megaphone = Megaphone.Cast(GetInventory().FindAttachment(slot));
		if (!megaphone)
			return NULL;

		return megaphone;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTurnOffZenRaidAlarm);
		AddAction(ActionTurnOnZenRaidAlarm);
		AddAction(ActionViewRaidAlarmGUI);
		AddAction(ActionFoldZenRaidAlarmKit);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void OnWork(float consumed_energy)
	{
		super.OnWork(consumed_energy);

		//! CLIENT
#ifndef SERVER
		if (GetAlarmStatus() == ZEN_ALARM_STATUS_TRIGGERED)
		{
			// Do this here not on OnVarsSync() because otherwise upon login megaphone & battery is not detected in time.
			PlayAlarmSound();
		}

		return;
#endif

		//! SERVER ONWORK
		// If radar is required.... well, check for radar
		if (GetZenDiscordConfig().RaidDiscordAlertRequiresRadar)
		{
			if (!m_ZenRoofCheckTimer && GetZenDiscordConfig().RaidRadarDishCheckRoofTimerMinutes > 0)
			{
				m_ZenRoofCheckTimer = new Timer();
				m_ZenRoofCheckTimer.Run(GetZenDiscordConfig().RaidRadarDishCheckRoofTimerMinutes * 60, this, "GetRaidAlarmRadar", null, true);
			}

			// Check discord radar status
			if (!GetRaidAlarmRadar(false))
			{
				if (m_ZenHadValidRadar)
				{
					InformDisconnection("", true);
					m_ZenHadValidRadar = false;
				}
			}
			else
			{
				if (!m_ZenHadValidRadar)
				{
					InformConnection();
					m_ZenHadValidRadar = true;
				}
			}
		}

		if (!GetCarBattery())
		{
			GetCompEM().SwitchOff();
			return;
		}

		// Check power status
		if (!m_ZenNotifiedLowPower && GetCompEM().IsWorking() && GetCompEM().GetEnergy0To1() < 0.1)
		{
			m_ZenNotifiedLowPower = true;
			InformLowBattery();
		}

		// Sync our energy to battery
		GetCarBattery().GetCompEM().SetEnergy0To1(GetCompEM().GetEnergy0To1());
		SetSynchDirty();
	}

	static bool GetTerritoryPermission(string uid, vector pos)
	{
		if (!GetZenDiscordConfig().RaidAlarmRequiresTerritory)
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

	///////////////////////////////////////////////////////////////////////
	//! SERVER-ONLY 
	///////////////////////////////////////////////////////////////////////
	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

#ifdef SERVER
		if (HasWorkingBaseRadio())
		{
			RegisterRaidStation();
		}
#endif
	}

	array<string> GetWebhooks()
	{
		array<string> webhooks = new array<string>;
		webhooks.Insert(m_ZenWebhook1);
		webhooks.Insert(m_ZenWebhook2);
		webhooks.Insert(m_ZenWebhook3);
		return webhooks;
	}

	bool HasValidWebhook()
	{
		return m_ZenWebhook1 != "" || m_ZenWebhook2 != "" || m_ZenWebhook3 != "";
	}

	void SetWebhooks(array<string> p_hooks, string p_name = "")
	{
		InformWebhookUpdate(p_name);

		if (p_hooks.Count() >= 1)
			m_ZenWebhook1 = p_hooks.Get(0);

		if (p_hooks.Count() >= 2)
			m_ZenWebhook2 = p_hooks.Get(1);

		if (p_hooks.Count() >= 3)
			m_ZenWebhook3 = p_hooks.Get(2);

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(InformConnection, 1500, false, p_name);
	}

	void SetBaseName(string p_name)
	{
		m_ZenBaseName = p_name;
	}

	string GetBaseName()
	{
		return m_ZenBaseName;
	}

	void InitBaseName()
	{
		#ifdef EXPANSIONMODBASEBUILDING
		if (m_ZenBaseName == "")
		{
			TerritoryFlag theFlag = GetNearestTerritoryFlag(GetPosition());
			if (!theFlag)
				return;

			ExpansionTerritory territory = theFlag.GetTerritory();
			if (territory != NULL)
				m_ZenBaseName = territory.GetTerritoryName();
		}
		#endif
	}

	void OpenRaidAlarmGUI(notnull PlayerIdentity identity)
	{
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenRaidAlarmServerWebhooks", new Param3<array<string>, Zen_RaidAlarmStation, string>(GetWebhooks(), this, GetBaseName()), true, identity);
	}

	void CheckForValidRadar()
	{
		if (!GetRaidAlarmRadar() && m_ZenHadValidRadar)
		{
			InformDisconnection("", true);
			m_ZenHadValidRadar = false;
		}
	}

	bool GetRaidAlarmRadar(bool checkForRoof = true)
	{
		// GetPluggedDevice() returns the cable reel
		// Only one plug is allowed per raid station so we can just get the first item
		CableReel cable = CableReel.Cast(GetCompEM().GetPluggedDevice());
		if (!cable)
		{
			m_ZenAttachedRaidRadar = NULL;
			return false;
		}

		m_ZenAttachedRaidRadar = Zen_RaidAlarmRadar.Cast(cable.GetCompEM().GetPluggedDevice());

		// If radar is not first attached item to cable reel, check for daisy chain
		if (!m_ZenAttachedRaidRadar)
		{
			CableReel daisyChain = CableReel.Cast(cable.GetCompEM().GetPluggedDevice());
			if (daisyChain != NULL)
			{
				m_ZenAttachedRaidRadar = Zen_RaidAlarmRadar.Cast(daisyChain.GetCompEM().GetPluggedDevice());
				if (!m_ZenAttachedRaidRadar)
				{
					CableReel daisyChain2 = CableReel.Cast(daisyChain.GetCompEM().GetPluggedDevice());
					if (daisyChain2 != NULL)
						m_ZenAttachedRaidRadar = Zen_RaidAlarmRadar.Cast(daisyChain2.GetCompEM().GetPluggedDevice());
				}
			}
		}

		if (m_ZenAttachedRaidRadar)
		{
			if (checkForRoof && MiscGameplayFunctions.IsUnderRoof(m_ZenAttachedRaidRadar))
			{
				m_ZenAttachedRaidRadar = NULL;
				return false;
			}
		}

		return m_ZenAttachedRaidRadar != NULL;
	}

	void UpdateBatteryEnergy()
	{
		if (!GetCarBattery())
			return;

		GetCompEM().SetEnergy0To1(GetCarBattery().GetCompEM().GetEnergy0To1());
	}

	void ResetRaidAlarm()
	{
		SetAlarmStatus(ZEN_ALARM_STATUS_WASTRIGGERED);
		m_ZenDontNotifyAboutPower = true;
		GetCompEM().SwitchOff();
	}

	void TurnOffAlarm(string playerName)
	{
		m_ManualTurnOnOff = true;
		m_ZenDontNotifyAboutPower = true;
		InformDisconnection(playerName, m_ZenHadValidRadar);
		GetCompEM().SwitchOff();
		GetCompEM().InteractBranch(this);
		SetAlarmStatus(ZEN_ALARM_STATUS_OFF);
	}

	void TurnOnAlarm(string playerName)
	{
		m_ManualTurnOnOff = true;
		UpdateBatteryEnergy();
		InformConnection(playerName);
		GetCompEM().SwitchOn();
		GetCompEM().InteractBranch(this);
		SetAlarmStatus(ZEN_ALARM_STATUS_ARMED);
	}

	void TriggerRaidAlarm()
	{
		// If no power source or turned off, or we already triggered an alert this session, don't alert players
		if (!GetCompEM().IsWorking() || GetAlarmStatus() == ZEN_ALARM_STATUS_TRIGGERED)
			return;

		// Sync alarm state and schedule reset
		SetAlarmStatus(ZEN_ALARM_STATUS_TRIGGERED);
		
		// Alert any online players with notifications 
		NotifyAllTerritoryMembers();

		// Check if we have valid webhooks first
		if (!CanSendDiscordMessage())
			return;

		string footer = "" + GetCompEM().GetEnergy0To100() + "% " + GetZenDiscordConfig().RaidAlarmBatteryWarning;
		ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		msg.SetMessage(GetZenDiscordConfig().RaidAlarmAttackMessage + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(GetPosition(), m_ZenBaseName));
		msg.SetFooter(footer);
		msg.SetColor(255, 0, 0);
		msg.AddWebhooks(GetWebhooks());
		GetZenDiscordAPI().SendMessage(msg);
		m_ZenLastDiscordMessageTime = GetGame().GetTime();
	}

	//! TERRITORY STUFF 

	static TerritoryFlag GetNearestTerritoryFlag(vector pos)
	{
		#ifdef EXPANSIONMODBASEBUILDING
		ExpansionTerritoryModule expansionTerritoryModule = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		if (expansionTerritoryModule != NULL)
		{
			TerritoryFlag tflag = expansionTerritoryModule.GetFlagAtPosition3D(pos);
			if (tflag != NULL)
				return tflag;
		}
		#endif

		#ifdef BASICTERRITORIES
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;

		float theRadius = GameConstants.REFRESHER_RADIUS * 1.5;
		GetGame().GetObjectsAtPosition3D(pos, theRadius, objects, proxyCargos);
		TerritoryFlag theFlag;

		for (int i = 0; i < objects.Count(); i++ )
		{
			if (Class.CastTo(theFlag, objects.Get(i)))
			{
				if (theFlag.HasRaisedFlag())
				{
					return theFlag;
				}
			}
		}
		#endif

		return NULL;
	}

	void NotifyAllTerritoryMembers()
	{
		vector pos = GetPosition();
		string raidNotification = GetZenDiscordConfig().RaidAlarmAttackMessage + "\n(" + m_ZenBaseName + " @ " + pos[0] + " / " + pos[2] + ")";

		array<Man> onlinePlayers = new array<Man>;
		GetGame().GetPlayers(onlinePlayers);
		PlayerBase pb;

		TerritoryFlag theFlag = GetNearestTerritoryFlag(GetPosition());
		if (!theFlag)
			return;

		// Check Expansion
		#ifdef EXPANSIONMODBASEBUILDING
		ExpansionTerritory territory = theFlag.GetTerritory();
		if (!territory || territory.GetTerritoryMembers().Count() == 0)
			return;

		foreach (Man man1 : onlinePlayers)
		{
			foreach (string uid : territory.GetTerritoryMemberIDs())
			{
				pb = PlayerBase.Cast(man1);
				if (pb && pb.GetIdentity())
				{
					if (territory.IsMember(pb.GetCachedID()))
					{
						if (HasRequiredAlarmItem(pb))
						{
							NotificationSystem.SendNotificationToPlayerIdentityExtended(pb.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, raidNotification, "set:ccgui_enforce image:MapShieldBooster");
						}
					}
				}
			}
		}

		return;
		#endif

		#ifdef BASICTERRITORIES
		// Check basic territories by Daemonforge
		foreach (Man man2 : onlinePlayers)
		{
			pb = PlayerBase.Cast(man2);
			if (pb && pb.GetIdentity())
			{
				if (theFlag.IsTerritoryMember(pb.GetCachedID()))
				{
					if (HasRequiredAlarmItem(pb))
					{
						NotificationSystem.SendNotificationToPlayerIdentityExtended(pb.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, raidNotification, "set:ccgui_enforce image:MapShieldBooster");
					}
				}
			}
		}
		#endif
	}

	bool HasRequiredAlarmItem(notnull PlayerBase player)
	{
		if (!GetZenDiscordConfig().RequireAnItemForIngameAlert)
			return true;

		array<EntityAI> itemsArray = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
		
		ItemBase itemInHands = player.GetItemInHands();
		if (itemInHands != NULL)
		{
			itemsArray.Insert(itemInHands);
		}
		
		ItemBase item;		
		for (int i = 0; i < itemsArray.Count(); i++)
		{
			ItemBase.CastTo(item, itemsArray.Get(i));

			if (!item)
				continue;

			if (item.IsRuined())
				continue;

			string itemType = item.GetType();
			itemType.ToLower();

			// Check if item matches config types
			if (!GetZenDiscordConfig().ItemsRequiredForIngameAlert.Contains(itemType))
				continue;

			// Check if battery is required
			if (!GetZenDiscordConfig().ItemsRequiredForIngameAlert.Get(itemType))
				return true;

			if (!item.HasEnergyManager())
				continue;

			// CanWork() instead of IsWorking() - Syberia PDA isn't actually ever "on"... shouldn't really matter,
			// depends on immersive you wanna be I guess but I'll leave this here - feel free to edit & repack.
			if (!item.GetCompEM().CanWork())
				continue;

			return true;
		}
		
		return false;
	}

	void InformDisconnection(string playerName = "", bool overrideRadarCheck = false)
	{
		if (!CanSendDiscordMessage(overrideRadarCheck))
			return;

		string footer = playerName;
		ref ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		msg.SetMessage(GetZenDiscordConfig().RaidAlarmConnectionOffline + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(GetPosition(), m_ZenBaseName));
		msg.SetFooter(footer);
		msg.SetColor(255, 255, 0);
		msg.AddWebhooks(GetWebhooks());
		GetZenDiscordAPI().SendMessage(msg);
		m_ZenLastDiscordMessageTime = GetGame().GetTime();
	}

	void InformConnection(string playerName = "")
	{
		if (!CanSendDiscordMessage())
			return;

		string footer = playerName;
		if (footer != "")
			footer = footer + " | ";

		footer = footer + GetCompEM().GetEnergy0To100() + "% " + GetZenDiscordConfig().RaidAlarmBatteryWarning;

		ref ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		msg.SetMessage(GetZenDiscordConfig().RaidAlarmConnectionOnline + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(GetPosition(), m_ZenBaseName));
		msg.SetFooter(footer);
		msg.SetColor(0, 255, 0);
		msg.AddWebhooks(GetWebhooks());
		GetZenDiscordAPI().SendMessage(msg);
		m_ZenLastDiscordMessageTime = GetGame().GetTime();
	}

	void InformLowBattery()
	{
		if (!CanSendDiscordMessage())
			return;

		ref ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		msg.SetMessage("" + GetCompEM().GetEnergy0To100() + "% " + GetZenDiscordConfig().RaidAlarmBatteryWarning + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(GetPosition(), m_ZenBaseName));
		msg.SetColor(255, 165, 0);
		msg.AddWebhooks(GetWebhooks());
		GetZenDiscordAPI().SendMessage(msg);
		m_ZenLastDiscordMessageTime = GetGame().GetTime();
	}

	void InformWebhookUpdate(string playerName = "")
	{
		if (!CanSendDiscordMessage())
			return;

		string footer = playerName;
		if (footer != "")
			footer = footer + " | ";

		footer = footer + GetCompEM().GetEnergy0To100() + "% " + GetZenDiscordConfig().RaidAlarmBatteryWarning;

		ref ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		msg.SetMessage(GetZenDiscordConfig().RaidAlarmWebhookUpdate + "\n\n" + GetZenDiscordConfig().GetMapLinkPosition(GetPosition(), m_ZenBaseName));
		msg.SetFooter(footer);
		msg.SetColor(255, 255, 0);
		msg.AddWebhooks(GetWebhooks());
		GetZenDiscordAPI().SendMessage(msg);
		m_ZenLastDiscordMessageTime = GetGame().GetTime();
	}

	bool CanSendDiscordMessage(bool overrideRadarCheck = false)
	{
		if (!HasValidWebhook())
			return false;

		GetRaidAlarmRadar();

		// If no radar connected to this station and config disables alert, don't alert discord.
		if (!m_ZenAttachedRaidRadar && GetZenDiscordConfig().RaidDiscordAlertRequiresRadar && !overrideRadarCheck)
			return false;

		if (GetGame().GetTime() - m_ZenLastDiscordMessageTime < (GetZenDiscordConfig().RaidDiscordAlertCooldownSecs * 1000))
			return false;

		return true;
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		if (m_ZenRoofCheckTimer)
			m_ZenRoofCheckTimer.Stop();

		if (!m_ZenDontNotifyAboutPower)
			InformDisconnection();

		if (GetAlarmStatus() != ZEN_ALARM_STATUS_WASTRIGGERED)
			SetAlarmStatus(ZEN_ALARM_STATUS_OFF);

		m_ZenDontNotifyAboutPower = false;
	}

	void SetAlarmStatus(int status)
	{
		m_ZenRaidAlarmStatus = status;
		SetSynchDirty();

		if (m_ZenRaidAlarmStatus == ZEN_ALARM_STATUS_TRIGGERED)
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetRaidAlarm, GetZenDiscordConfig().RaidAlarmResetInMinutes * 60 * 1000, false);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
		{
			if (GetZenDiscordConfig().BaseDamageTriggersRaidAlert)
			{
				ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
				if (plugin)
					plugin.AlertNearestRaidStation(GetPosition());
			}
			
			return true;
		}

		return false;
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

#ifdef SERVER
		if (item.IsInherited(BaseRadio))
		{
			// Turn battery off when attached - we now interact with the battery through raid mod.
			BaseRadio radio = BaseRadio.Cast(item);
			if (radio.GetCompEM() && radio.GetCompEM().IsWorking())
			{
				radio.GetCompEM().SwitchOff();
				UpdateBatteryEnergy();
			}
		}
#endif
	}

	void LockBaseRadio(bool lock)
	{
		BaseRadio radio = GetBaseRadio();

		if (lock)
		{
			if (!radio.IsLockedInSlot())
			{
				radio.LockToParent();
				RegisterRaidStation();
			}
		}
		else 
		{
			if (radio.IsLockedInSlot())
			{
				radio.UnlockFromParent();
				UnregisterRaidStation();
			}
		}
	}

	void RegisterRaidStation()
	{
		ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
		if (plugin)
			plugin.RegisterRaidStation(this);
	}

	void UnregisterRaidStation()
	{
		ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
		if (plugin)
			plugin.UnregisterRaidStation(this);
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

#ifdef SERVER
		if (newLevel == GameConstants.STATE_RUINED)
		{
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, vector.Zero);
			UnregisterRaidStation();
		}
#endif
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		// Save webhook array data
		ctx.Write(m_ZenBaseName);
		ctx.Write(m_ZenWebhook1);
		ctx.Write(m_ZenWebhook2);
		ctx.Write(m_ZenWebhook3);
		ctx.Write(m_ZenRaidAlarmStatus);
		ctx.Write(m_ZenHadValidRadar);
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		// Get name 
		if (!ctx.Read(m_ZenBaseName))
			return false;

		// Get webhooks
		if (!ctx.Read(m_ZenWebhook1))
			return false;

		if (!ctx.Read(m_ZenWebhook2))
			return false;

		if (!ctx.Read(m_ZenWebhook3))
			return false;

		if (!ctx.Read(m_ZenRaidAlarmStatus))
			return false;

		if (!ctx.Read(m_ZenHadValidRadar))
			return false;

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		SetAlarmStatus(m_ZenRaidAlarmStatus);
	}
}