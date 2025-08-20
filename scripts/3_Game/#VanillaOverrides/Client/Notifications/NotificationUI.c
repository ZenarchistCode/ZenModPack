modded class NotificationUI
{
	static ref array<ref ZenNotification> ZenKeywords;

	protected static ref EffectSound m_ZenStaticLoop;
	protected static ref map<string, string> ZEN_NOTIFICATION_SOUNDSETS;

	// Pre-load any notification soundsets
	void NotificationUI()
	{
		LoadZenNotificationSoundsets();
	}

	// There are 6000+ soundsets to cycle through, so we should 
	// pre-load the few that we need on game startup to save 
	// cycling though all of them each time a notification plays
	static void LoadZenNotificationSoundsets()
	{
		if (ZEN_NOTIFICATION_SOUNDSETS != NULL)
			return;

		ZEN_NOTIFICATION_SOUNDSETS = new map<string, string>;
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgSoundSets"); i++)
		{
			string soundset_name = "";
			GetGame().ConfigGetChildName("CfgSoundSets", i, soundset_name);
			if (soundset_name == "")
				continue;

			string keywords = "";
			GetGame().ConfigGetText("CfgSoundSets " + soundset_name + " soundZenKeywords", keywords);
			if (keywords == "")
				continue;

			// Valid mission keyword found - add to soundset map
			ZEN_NOTIFICATION_SOUNDSETS.Insert(keywords, soundset_name);
		}
	}

	override void AddNotification(NotificationRuntimeData data)
	{
		super.AddNotification(data);

		if (!GetGame().IsClient())
			return;

		// Hide icon image background if there is no image
		Widget notification = m_Notifications.Get(data);
		if (notification)
		{
			ImageWidget icon = ImageWidget.Cast(notification.FindAnyWidget("Image"));
			if (icon)
			{
				if (data.GetIcon() == "")
					icon.SetAlpha(0);
				else 
					icon.SetAlpha(1);
			}
		}

		if (!CheckPlayZenNotificationSoundset(data.GetDetailText()))
		{
			// Play notification sound on local player (not synced; other players cannot hear this)
			if (CheckPlayZenNotificationNormal(data.GetDetailText()) && GetGame().GetPlayer())
			{
				SEffectManager.PlaySoundOnObject("Zen_NotifySoundset", GetGame().GetPlayer());
			}
		}
	}

	static bool CheckPlayZenNotificationNormal(string keywords)
	{
		keywords = Widget.TranslateString(keywords);

		if (!ZenKeywords)
		{
			return true;
		}

		string loweredMsg = keywords;
		loweredMsg.ToLower();

		foreach (ZenNotification n : ZenKeywords)
		{
			string loweredKeyword = n.Keyword;
			loweredKeyword.ToLower();

			if (loweredMsg.Contains(loweredKeyword))
			{
				return n.PlaySound; // Only true if PlaySound was set to true in config
			}
		}

		return true; // No keyword match or none with PlaySound = true
	}

	static bool CheckPlayZenNotificationSoundset(string keywords)
	{
		keywords = Widget.TranslateString(keywords);
		//keywords = "Can anybody hear me? Over!"; // For testing.

		string soundset_name = GetZenSoundSetForZenKeywords(keywords);
		if (soundset_name == "")
			return false;

		int playOnRadio = GetGame().ConfigGetInt("CfgSoundSets " + soundset_name + " playOnRadio");
		if (playOnRadio == 1)
			return true; // Don't play soundset OR notification osund here - use ZenRadioPlugin to play it on the radios.

		// Play voice effect
		EffectSound voiceEffect = SEffectManager.PlaySoundOnObject(soundset_name, GetGame().GetPlayer(), 0, 0.15, false);
					
		// Check if we should play static effect for radio messages etc
		int playStatic = GetGame().ConfigGetInt("CfgSoundSets " + soundset_name + " playStatic");
					
		if (playStatic == 1 && voiceEffect && voiceEffect.Zen_GetAbstractWave())
		{
			//voiceEffect.SetSoundWaveKind(WaveKind.WAVESPEECH); // TODO: Find out how to auto-apply a "radio" tinny high-pass filter sound 
	
			// Get voice effect duration
			int voiceAudioDuration = voiceEffect.Zen_GetAbstractWave().GetLength() + 2;

			// Play static loop
			GetGame().GetPlayer().PlaySoundSetLoop(m_ZenStaticLoop, "personalradio_staticnoise_SoundSet", 0, 0);
			EffectSound staticEffect = SEffectManager.PlaySoundOnObject("tuneRadio_SoundSet", GetGame().GetPlayer(), 0, 0.15, false);
			if (staticEffect)
				staticEffect.SetAutodestroy(true);

			// Schedule end of static loop
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StopStaticSoundLoop, voiceAudioDuration * 1000, false);
		}

		if (voiceEffect)
			voiceEffect.SetAutodestroy(true);

		return true;
	}

	static string GetZenSoundSetForZenKeywords(string keywords)
	{
		if (!ZEN_NOTIFICATION_SOUNDSETS)
			return "";

		for (int i = 0; i < ZEN_NOTIFICATION_SOUNDSETS.Count(); i++)
		{
			string key = ZEN_NOTIFICATION_SOUNDSETS.GetKey(i);
			if (keywords.Contains(key))
			{
				string soundset_name = ZEN_NOTIFICATION_SOUNDSETS.GetElement(i);
				if (GetGame().ConfigIsExisting("CfgSoundSets " + soundset_name))
				{
					return soundset_name;
				}
			}
		}

		return "";
	}

	static void StopStaticSoundLoop()
	{
		if (m_ZenStaticLoop)
		{
			EffectSound staticEffect = SEffectManager.PlaySoundOnObject("tuneRadio_SoundSet", GetGame().GetPlayer(), 0, 0.15, false);
			if (staticEffect)
				staticEffect.SetAutodestroy(true);

			m_ZenStaticLoop.SetAutodestroy(true);
			m_ZenStaticLoop.Stop();
		}
	}
}