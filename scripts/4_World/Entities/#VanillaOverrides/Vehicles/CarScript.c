modded class CarScript
{
	//! CAR BATTERY ICON 
	protected int m_ZenBatteryEnergy;

	//! PIMP
	Chemlight_ColorBase		m_ZenChemlightAttached;
	ChemlightLight			m_ZenPimpLight;

	//! MUSIC
	const static float SONG_UPDATE_TIMER_SECS = 1.0;

	// Server & client
	protected float m_MusicVolumeClient = 1.0;
	protected float m_MusicVolume = 1.0;

	// Client
	protected ref EffectSound m_Song;
	protected bool m_IsPlayingSong = false;

	// Server
	int m_MusicPlaySecs = -1;

	void CarScript()
	{
		RegisterNetSyncVariableFloat("m_MusicVolume");
		RegisterNetSyncVariableInt("m_MusicPlaySecs");
		RegisterNetSyncVariableInt("m_ZenBatteryEnergy");
	}

	void ~CarScript()
	{
		if (!GetGame())
			return;

		if (GetGame().IsDedicatedServer())
			StopSongServer();
		else
			StopSongClient();

		delete m_Song;
	}

	//! CAR BATTERY ICON 
	override void OnUpdate(float dt)
    {
        super.OnUpdate(dt);

		#ifdef SERVER 
		if (ZenModEnabled("ZenCarBatteryIcon"))
		{
			ItemBase battery;

			if (EngineIsOn() || IsScriptedLightsOn())
			{
				battery = GetBattery();

				if (battery)
				{
					UpdateZenBatteryEnergy(battery.GetCompEM().GetEnergy0To100());
				}
			}
		}
		#endif
    }

	int GetZenBatteryEnergy0To100()
	{
		return m_ZenBatteryEnergy;
	}

	void UpdateZenBatteryEnergy(int percent)
	{
        if (!GetGame().IsDedicatedServer())
			return;

		if (!ZenModEnabled("ZenCarBatteryIcon"))
			return;

		if (m_ZenBatteryEnergy == percent)
		    return;

		m_ZenBatteryEnergy = percent;
		SetSynchDirty();
	}

	//! GLOVEBOX 
	override void EEInit()
	{
		super.EEInit();

		if (ZenModEnabled("ZenGlovebox"))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnZenGlovebox, 2500, false);
		}
	}

	private void SpawnZenGlovebox()
	{
		if (!GetInventory())
			return;

		array<string> slot_names = new array<string>;
		ConfigGetTextArray("Attachments", slot_names);

		if (!slot_names)
			return;

		bool slotFound = false;
		foreach (string s : slot_names)
		{
			if (s == "ZenCarGlovebox")
			{
				slotFound = true;
				break;
			}
		}

		if (!slotFound)
			return;

		ZenGlovebox glovebox = ZenGlovebox.Cast(GetAttachmentByType(ZenGlovebox));
		if (!glovebox)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("ZenCarGlovebox"), false);
			ItemBase ib = ItemBase.Cast(GetInventory().CreateInInventory("ZenGlovebox"));
			if (ib)
				ib.LockToParent();
		}
	}


	//! MUSIC
	ItemBase GetCassette()
    {
        return ItemBase.Cast(FindAttachmentBySlotName("ZenCassette"));
    }

	int GetSongDuration()
	{
		if (GetCassette() && !GetCassette().IsRuined())
			return Zen_Cassette_Base.GetSongDuration(GetCassette().GetType());

		return -1;
	}

	void ToggleMusic()
	{
		if (m_MusicPlaySecs == -1)
			PlaySongServer();
		else
			StopSongServer();
	}

	bool HasValidCassette()
	{
		if (!GetCassette())
			return false;

		return GetCassette() && !GetCassette().IsRuined() && Zen_Cassette_Base.GetSongSoundSet(GetCassette().GetType()) != "" && Zen_Cassette_Base.GetSongDuration(GetCassette().GetType()) > 0;
	}

	void PlaySongServer()
	{
		if (!HasValidCassette())
			return;

		m_MusicPlaySecs = 0;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateSongDuration, 1000, false);
		SetSynchDirty();
	}

	void StopSongServer()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(UpdateSongDuration);
		m_MusicPlaySecs = -1;
		SetSynchDirty();
	}

	// Called every X seconds. No need for SynchDirty because that will be called when car is loaded on client anyway and this just tracks current song time
	void UpdateSongDuration()
	{
		// If song has been stopped, don't update music play secs and stop here
		if (m_MusicPlaySecs < 0 || m_MusicPlaySecs > GetSongDuration() + 10)
		{
			StopSongServer();
			return;
		}

		// If battery is dead, stop music
		if (IsVitalCarBattery())
		{
			CarBattery carBattery = CarBattery.Cast(FindAttachmentBySlotName("CarBattery"));
			if (!carBattery || !carBattery.GetCompEM() || !carBattery.GetCompEM().GetEnergy() > 0 || carBattery.IsRuined())
			{
				StopSongServer();
				return;
			}
		}
		else
		if (IsVitalTruckBattery())
		{
			TruckBattery truckBattery = TruckBattery.Cast(FindAttachmentBySlotName("TruckBattery"));
			if (!truckBattery || !truckBattery.GetCompEM() || !truckBattery.GetCompEM().GetEnergy() > 0 || truckBattery.IsRuined())
			{
				StopSongServer();
				return;
			}
		}

		// All is well - continue to rock out with your small stones out.
		m_MusicPlaySecs += SONG_UPDATE_TIMER_SECS;
		SetSynchDirty();

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateSongDuration, SONG_UPDATE_TIMER_SECS * 1000, false);
	}

	void TurnVolumeUp()
	{
		m_MusicVolume = m_MusicVolume + 0.25;
		if (m_MusicVolume > 4.0)
			m_MusicVolume = 4.0;

		SetSynchDirty();
	}

	void TurnVolumeDown()
	{
		m_MusicVolume = m_MusicVolume - 0.25;
		if (m_MusicVolume < 0)
			m_MusicVolume = 0;

		SetSynchDirty();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		CheckMusicSync();
	}

	protected void CheckMusicSync()
	{
		if (m_MusicPlaySecs >= 0)
		{
			if (m_MusicPlaySecs < GetSongDuration() && (!m_Song || !m_Song.IsSoundPlaying()))
			{
				PlaySongClient();
			}
		}
		else
		{
			StopSongClient();
		}

		if (m_MusicVolumeClient != m_MusicVolume)
		{
			m_MusicVolumeClient = m_MusicVolume;
			if (m_Song)
			{
				if (m_Song.Zen_GetAbstractWave().IsHeaderLoaded())
				{
					m_Song.Zen_SetMaxVolume(m_MusicVolumeClient);
					m_Song.SetSoundVolume(m_MusicVolumeClient);
				}
			}
		}
	}

	void PlaySongClient()
	{
		if (!HasValidCassette() || m_IsPlayingSong)
			return;

		m_Song = SEffectManager.CreateSound(Zen_Cassette_Base.GetSongSoundSet(GetCassette().GetType()), GetPosition(), 0, 0, false);
		int playSecs = Zen_Cassette_Base.GetSongDuration(GetCassette().GetType());

		if (!m_Song || !playSecs || playSecs <= 0)
			return;

		m_Song.SetParent(this);
		m_Song.SetLocalPosition(vector.Zero);
		m_Song.SetSoundAutodestroy(true);
		m_Song.Zen_SetMaxVolume(m_MusicVolumeClient);
		m_Song.SoundPlay();
		m_IsPlayingSong = true;

		// If song has already been playing a while, skip ahead
		if (m_MusicPlaySecs > 0 && m_Song.Zen_GetAbstractWave())
		{
			float skip = (m_MusicPlaySecs / playSecs);
			m_Song.Zen_GetAbstractWave().SetStartOffset(skip);
		}
	}

	void StopSongClient()
	{
		if (m_Song)
		{
			m_Song.SoundStop();
			m_IsPlayingSong = false;
		}
	}

	//! PIMP
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (GetGame().IsClient())
		{
			if (slot_name == "Chemlight")
				CreateZenPimpLight(item);

			return;
		}

		if (slot_name == "CarBattery" || slot_name == "TruckBattery")
		{
			if (item.GetCompEM())
			{
				UpdateZenBatteryEnergy(item.GetCompEM().GetEnergy0To100());
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (GetGame().IsClient())
		{
			if (slot_name == "Chemlight")
				DestroyZenPimpLight();

			return;
		}

		if (slot_name == "ZenCassette")
		{
			StopSongServer();
		}
		else
		if (slot_name == "CarBattery" || slot_name == "TruckBattery")
		{
			StopSongServer();
			UpdateZenBatteryEnergy(0);
		}
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionToggleMusic);
		AddAction(ActionMusicVolUp);
		AddAction(ActionMusicVolDn);
	}

	override bool CanDisplayAttachmentSlot(string slot_name)
	{
		//! MUSIC
		if (slot_name == "ZenCassette")
			return ZenModEnabled("ZenMusic");

		//! PIMP
		if (slot_name == "Chemlight")
			return ZenModEnabled("ZenPimpMyRide");

		return super.CanDisplayAttachmentSlot(slot_name);
	}

	override bool CanDisplayCargo()
	{
		if (ZenModEnabled("ZenMusic") && GetZenMusicConfig().AllowCarInventory)
		{
			if (GetGame().GetPlayer()) // Override since I've enabled the player inventory to be seen in cars
			{
				PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
				if (player && player.IsInVehicle())
				{
					return false;
				}
			}
		}

		return super.CanDisplayCargo();
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
		category_name.ToLower();

		if (category_name == "zencarglovebox")
			return ZenModEnabled("ZenGlovebox") && Zen_PlayerInFrontSeat();

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return false;

		bool inCar = player.IsInVehicle();

		if (category_name == "zenpimpmyride" && !ZenModEnabled("ZenPimpMyRide"))
			return false;

		if (ZenModEnabled("ZenMusic") && GetZenMusicConfig().AllowCarInventory && inCar)
		{
			if (category_name == "zenmusiccarradio")
				return true;
		}

		return super.CanDisplayAttachmentCategory(category_name) && !inCar;
	}

	//! GLOVEBOX / CLIENT-ONLY
	static bool Zen_PlayerInFrontSeat()
	{
		if (!GetGame().IsClient())
			return false;

		PlayerBase clientPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (clientPlayer)
		{
			if (!clientPlayer.IsInVehicle())
				return false;

			CarScript car = null;
			if (clientPlayer.GetCommand_Vehicle() && Class.CastTo(car, clientPlayer.GetCommand_Vehicle().GetTransport()))
			{
				int crewIdx = car.CrewMemberIndex(clientPlayer);
				if (crewIdx >= 0 && crewIdx <= 1)
				{
					return true;
				}
			}
		}

		return false;
	}

	//! PIMP / CLIENT-ONLY
	vector GetZenChemlightHeightOffset() { return "0 -0.01 0"; }

	void CreateZenPimpLight(EntityAI item)
	{
		m_ZenChemlightAttached = Chemlight_ColorBase.Cast(item);
		if (m_ZenChemlightAttached == NULL)
			return;

		if (!m_ZenChemlightAttached.GetCompEM() || !m_ZenChemlightAttached.GetCompEM().IsWorking())
		{
			DestroyZenPimpLight();
			return;
		}

		if (!m_ZenPimpLight)
		{
			m_ZenPimpLight = ChemlightLight.Cast(ScriptedLightBase.CreateLight(ChemlightLight, "0 -1 0"));
			m_ZenPimpLight.AttachOnObject(this, GetZenChemlightHeightOffset());

			switch(item.GetType())
			{
				case "Chemlight_White": 
					m_ZenPimpLight.SetColorToWhite();
					break;
				case "Chemlight_Red": 
					m_ZenPimpLight.SetColorToRed();
					break;
				case "Chemlight_Green": 
					m_ZenPimpLight.SetColorToGreen();
					break;
				case "Chemlight_Blue": 
					m_ZenPimpLight.SetColorToBlue();
					break;
				case "Chemlight_Yellow": 
					m_ZenPimpLight.SetColorToYellow();
					break;
					
				default: { m_ZenPimpLight.SetColorToWhite(); };
			}
		}
	}

	void DestroyZenPimpLight()
	{
		m_ZenChemlightAttached = NULL;

		if (m_ZenPimpLight)
		{
			m_ZenPimpLight.FadeOut();
		}
	}

	void UpdateZenPimpLight()
	{
		if (m_ZenChemlightAttached != NULL)
		{
			if (!m_ZenChemlightAttached.GetCompEM() || !m_ZenChemlightAttached.GetCompEM().IsWorking())
			{
				DestroyZenPimpLight();
			} else 
			if (m_ZenChemlightAttached.GetCompEM() != NULL)
			{
				float efficiency = m_ZenChemlightAttached.GetEfficiency0To1();
				if (efficiency < 1)
				{
					m_ZenPimpLight.SetIntensity(efficiency, m_ZenChemlightAttached.GetCompEM().GetUpdateInterval());
				}
			}
		}
	}

	override void UpdateLightsClient(int newGear = -1)
	{
		super.UpdateLightsClient(newGear);

		UpdateZenPimpLight();
	}
}
