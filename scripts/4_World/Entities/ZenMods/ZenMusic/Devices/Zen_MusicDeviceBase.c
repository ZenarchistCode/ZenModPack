class Zen_MusicDeviceBase extends ItemBase
{
	const static float SONG_UPDATE_TIMER_SECS = 1.0;

	 // Zombie sound variables
    private ref NoiseParams		m_NoisePar = null;
    private NoiseSystem			m_NoiseSystem = null;
    protected float				m_NoiseDuration = 1.0;
    protected int				m_MaxNoiseCount = 60;
    protected int				m_NoiseCount = 0;

	// Server & client
	protected float				m_MusicVolumeClient = 1.0;
	protected float				m_MusicVolume = 1.0;
	protected int				m_MusicPlaySecs = -1;

	// Client
	protected ref EffectSound	m_Song;

	// Server
	int							m_MusicLength = -1;

	void Zen_MusicDeviceBase()
	{
		if (m_MusicVolume > GetInitVolume())
		{
			m_MusicVolumeClient = GetInitVolume();
			m_MusicVolume = GetInitVolume();
		}

		#ifdef SERVER
		if (CanAlertZombies())
		{
			if (!m_NoiseSystem)
				m_NoiseSystem = GetGame().GetNoiseSystem();

			// Prepare zombie noise caller
			if (m_NoiseSystem && !m_NoisePar)
			{
				m_NoisePar = new NoiseParams();
				m_NoisePar.LoadFromPath("CfgVehicles Zen_MusicNoise " + ConfigGetString("noiseType"));
			}
		}
		#endif

		RegisterNetSyncVariableFloat("m_MusicVolume");
		RegisterNetSyncVariableInt("m_MusicPlaySecs");
	};

	void ~Zen_MusicDeviceBase()
	{
		if (!GetGame())
			return;

		if (GetGame().IsDedicatedServer())
			StopSongServer();

		if (m_Song)
			m_Song.Stop();
	}

	bool IsPlayingSong()
    {
        return m_MusicPlaySecs >= 0;
    }

	float GetVolume()
	{
		if (GetGame().IsClient())
			return m_MusicVolumeClient;

		return m_MusicVolume;
	}

	float GetInitVolume()
	{
		return ConfigGetFloat("initVolume");
	}

	float GetMaxVolume()
	{
		return ConfigGetFloat("maxVolume");
	}

	ItemBase GetCassette()
    {
        return ItemBase.Cast(FindAttachmentBySlotName("ZenCassette"));
    }

    ItemBase GetBattery()
    {
        return ItemBase.Cast(FindAttachmentBySlotName("BatteryD"));
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
		return GetCassette() && !GetCassette().IsRuined() && Zen_Cassette_Base.GetSongSoundSet(GetCassette().GetType()) != "" && Zen_Cassette_Base.GetSongDuration(GetCassette().GetType()) > 0;
	}

	bool CanAlertZombies()
	{
		return ConfigGetBool("attractsZombies");
	}

	void CallZombieDancers()
    {
		if (!CanAlertZombies())
			return;

        if (!m_NoisePar)
			return;

        if (!m_NoiseSystem)
			return;

        m_NoiseSystem.AddNoiseTarget(GetPosition(), m_NoiseDuration, m_NoisePar, GetVolume());
    }

	void TurnVolumeUpServer()
	{
		m_MusicVolume = m_MusicVolume + 0.25;
		if (m_MusicVolume > 4.0)
			m_MusicVolume = 4.0;

		SetSynchDirty();
	}

	void TurnVolumeDownServer()
	{
		m_MusicVolume = m_MusicVolume - 0.25;
		if (m_MusicVolume < 0)
			m_MusicVolume = 0;

		SetSynchDirty();
	}

	int GetSongPlaySecs()
	{
		return m_MusicPlaySecs;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		// Check music setting (for twitch streamers/youtubers etc)
		if (GetCassette())
		{
			// Don't allow copyrighted music
			if (GetZenModPackClientConfig().CassetteMusicSetting == 2)
			{
				if (Zen_Cassette_Base.GetCopyrighted(GetCassette().GetType()))
				{
					StopSongClient();
					return;
				}
			}

			// Don't allow any music
			if (GetZenModPackClientConfig().CassetteMusicSetting == 3)
			{
				if (Zen_Cassette_Base.IsMusicTape(GetCassette().GetType()))
				{
					StopSongClient();
					return;
				}
			}
		}

		if (m_MusicPlaySecs >= 0)
		{
			if (m_MusicPlaySecs < GetSongDuration() && !m_Song)
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
			if (m_Song)
			{
				if (m_Song.Zen_GetAbstractWave().IsHeaderLoaded())
				{
					m_MusicVolumeClient = m_MusicVolume;
					m_Song.Zen_SetMaxVolume(m_MusicVolumeClient);
					m_Song.SetSoundVolume(m_MusicVolumeClient);
				}
			}
		}
	}

	// Called every X seconds. No need for SynchDirty because that will be called when car is loaded on client anyway and this just tracks current song time
	void UpdateSongDuration()
	{
		// If song has been stopped, don't update music play secs and stop here
		if (m_MusicPlaySecs < 0 || m_MusicPlaySecs >= m_MusicLength)
		{
			StopSongServer();
			return;
		}

		// All is well - continue to rock out with your small stones out.
		m_MusicPlaySecs += SONG_UPDATE_TIMER_SECS;
		SetSynchDirty();

		CallZombieDancers();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateSongDuration, SONG_UPDATE_TIMER_SECS * 1000.0, false);
	}

	bool PlaySongServer()
	{
		if (!GetCompEM() || !GetCompEM().CanWork())
			return false;

		if (!HasValidCassette())
		{
			//ZenFunctions.SendGlobalMessage("[ZenMusic] Cassette malfunctioned: " + GetCassette().GetType() + " - someone please tell Zenarchist he fucked up! (HardcoreDayZ.com@gmail.com)");
			return false;
		}

		m_MusicPlaySecs = 0;
		m_MusicLength = Zen_Cassette_Base.GetSongDuration(GetCassette().GetType()) + 10; // Add 10 secs as a safe buffer for server/client sync

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateSongDuration, 1000, false);
		SetSynchDirty();

		return true;
	}

	void StopSongServer()
	{
		m_MusicPlaySecs = -1;

		if (GetCompEM())
			GetCompEM().SwitchOff();

		SetSynchDirty();
	}

	void PlaySongClient()
	{
		if (!HasValidCassette())
			return;

		int songLength = Zen_Cassette_Base.GetSongDuration(GetCassette().GetType());
		if (!songLength || songLength <= 0)
			return;

		m_Song = SEffectManager.CreateSound(Zen_Cassette_Base.GetSongSoundSet(GetCassette().GetType()), GetPosition(), 0, 0, false);
		if (!m_Song)
			return;

		m_Song.SetParent(this);
		m_Song.SetLocalPosition(vector.Zero);
		m_Song.SetSoundAutodestroy(true);
		m_Song.Zen_SetMaxVolume(m_MusicVolumeClient);
		m_Song.SoundPlay();

		// If song has already been playing a while, skip ahead
		if (m_MusicPlaySecs > 0 && m_Song.Zen_GetAbstractWave())
		{
			float skip = (m_MusicPlaySecs / songLength);
			m_Song.Zen_GetAbstractWave().SetStartOffset(skip);
		}
	}

	void StopSongClient()
	{
		if (m_Song)
			m_Song.SoundStop();

		m_Song = NULL;
	}

    override void OnWorkStart()
    {
        super.OnWorkStart();

        #ifdef SERVER
        if (GetCompEM() && !PlaySongServer())
            GetCompEM().SwitchOff();
		#endif
    }

    override void OnWorkStop()
    {
        super.OnWorkStop();

		#ifdef SERVER
        GetCompEM().SwitchOff();
        StopSongServer();
		#endif
    }

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		#ifdef SERVER
		if (slot_name == "Battery9V" || slot_name == "ZenCassette")
			StopSongServer();
		#endif
	};

	override void SetActions()
    {
        super.SetActions();
        AddAction(ActionTurnOnWhileInHands);
        AddAction(ActionTurnOffWhileInHands);
        AddAction(ActionTurnOnWhileOnGround);
        AddAction(ActionTurnOffWhileOnGround);
    }
};
