class Zen_BoomBox_Static extends Zen_MusicDeviceBase
{
	private bool m_IsPlaying;
	private int m_CassetteSpawnFails;
	private bool m_MusicPausedServer;
	private int m_SongIndex;

	// THIS IS ALL SERVER-SIDE: SPAWNS A CASSETTE AND TELLS CLIENTS TO SYNC TO SONG
	override void EEInit()
	{
		super.EEInit();

		SetAllowDamage(false);
		SetTakeable(false);

		if (GetType() == "Zen_BoomBox_Static")
			m_MusicPausedServer = !GetZenMusicConfig().StaticBoomboxAutoPlay;
		else
		if (GetType() == "Zen_BoomBox_Invisible_Static")
			m_MusicPausedServer = !GetZenMusicConfig().StaticInvisibleBoomboxAutoPlay;

		m_SongIndex = 0;
		m_CassetteSpawnFails = 0;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PrepareForNewMusic, 1500.0, false, false);
	}

	void PrepareForNewMusic(bool forceUnpause = false)
	{
		if (forceUnpause)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateSongDuration);
			m_MusicPausedServer = false;
		}

		if (m_MusicPausedServer)
			return;

		if (GetCassette())
		{
			GetCassette().UnlockFromParent();
			GetCassette().DeleteSafe();
		}

		if (!GetBattery())
		{
			ItemBase battery = ItemBase.Cast(GetInventory().CreateAttachment("Battery9VLithium"));
			if (battery)
				battery.LockToParent();
		}

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnRandomCassette, 1000.0, false);
	}

	override void UpdateSongDuration()
	{
		if (m_MusicPausedServer)
			return;

		// If song has been stopped, don't update music play secs and stop here
		if (m_MusicPlaySecs < 0 || m_MusicPlaySecs > GetSongDuration() + 10)
		{
			StopSongServer();
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PrepareForNewMusic, 1500, false, false);
			return;
		}

		// All is well - continue to rock out with your small stones out.
		m_MusicPlaySecs += SONG_UPDATE_TIMER_SECS;
		SetSynchDirty();

		CallZombieDancers();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateSongDuration, SONG_UPDATE_TIMER_SECS * 1000, false);
	}

	void SpawnRandomCassette()
	{
		if (m_CassetteSpawnFails >= 10)
			return;

		string tape;
		if (GetType() == "Zen_BoomBox_Static")
		{
			tape = GetZenMusicConfig().StaticBoomboxSongs.Get(m_SongIndex);
			m_SongIndex++;
			if (m_SongIndex >= GetZenMusicConfig().StaticBoomboxSongs.Count())
			{
				GetZenMusicConfig().ShuffleArrays();
				m_SongIndex = 0;
			}
		} else
		if (GetType() == "Zen_BoomBox_Invisible_Static")
		{
			tape = GetZenMusicConfig().StaticInvisibleBoomboxSongs.Get(m_SongIndex);
			m_SongIndex++;
			if (m_SongIndex >= GetZenMusicConfig().StaticInvisibleBoomboxSongs.Count())
			{
				GetZenMusicConfig().ShuffleArrays();
				m_SongIndex = 0;
			}
		}

		if (tape == "")
		{
			ZMPrint("[ZenMusic] " + GetType() + " ERROR: Tape could not be loaded!");
			return;
		}

		GetInventory().CreateAttachment(tape);

		if (GetCassette())
		{
			GetCassette().LockToParent();
			PlaySongServer();
			//ZenFunctions.SendGlobalMessage("[ZenMusic] StaticBoomBox Playing Tape: " + tape);
		}
		else
		{
			ZMPrint("[ZenMusic] COULD NOT SPAWN STATIC MUSIC CASSETTE: " + tape);
			ZenFunctions.SendGlobalMessage("[ZenMusic] COULD NOT SPAWN STATIC MUSIC CASSETTE: " + tape);
			SpawnRandomCassette();
			m_CassetteSpawnFails++;
		}
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	// No need for CompEM() on static boombox
	override bool PlaySongServer()
	{
		if (!HasValidCassette())
		{
			ZenFunctions.SendGlobalMessage("[ZenMusic] Cassette malfunctioned: " + GetCassette().GetType() + " - someone please tell Zenarchist he fucked up! (admin@HardcoreDayZ.com)");
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(PrepareForNewMusic, 1500, false);
			m_CassetteSpawnFails++;
			return false;
		}

		m_MusicPlaySecs = 0;
		m_MusicPausedServer = false;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateSongDuration, 1000, false);
		SetSynchDirty();

		return true;
	}

	override void StopSongServer()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateSongDuration);
		m_MusicPlaySecs = -1;
		SetSynchDirty();
	}

	void PauseMusicServer()
	{
		m_MusicPausedServer = true;
		StopSongServer();
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionStaticBoomboxTurnOn);
		AddAction(ActionStaticBoomboxTurnOff);
		AddAction(ActionBoomboxVolDown);
		AddAction(ActionBoomboxVolUp);
		AddAction(ActionStaticBoomboxNextSong);
	}
};
