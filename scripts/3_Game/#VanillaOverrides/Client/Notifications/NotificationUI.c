modded class NotificationUI
{
	private ref EffectSound m_Sound;

	static ref map<string, string> ZEN_NOTIFICATION_SOUNDSETS;

	// Pre-load any notification soundsets
	void NotificationUI()
	{
		LoadZenNotificationSoundsets();
	}

	// There are 6000+ soundsets to cycle through, so we should pre-load the few that we need
	void LoadZenNotificationSoundsets()
	{
		ZEN_NOTIFICATION_SOUNDSETS = new map<string, string>;
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgSoundSets"); i++)
		{
			string soundset_name = "";
			GetGame().ConfigGetChildName("CfgSoundSets", i, soundset_name);
			if (soundset_name == "")
				continue;

			string keywords = "";
			GetGame().ConfigGetText("CfgSoundSets " + soundset_name + " soundKeywords", keywords);
			if (keywords == "")
				continue;

			// Valid mission keyword found - add to soundset map
			ZEN_NOTIFICATION_SOUNDSETS.Insert(keywords, soundset_name);
		}
	}

	override void AddNotification(NotificationRuntimeData data)
	{
		super.AddNotification(data);

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
			// Play notification sound
			if (!GetGame().IsDedicatedServer())
			{
				if (GetGame().GetPlayer())
				{
					SEffectManager.PlaySoundOnObject("Zen_NotifySoundset", GetGame().GetPlayer());
				}
			}
		}
	}

	static bool CheckPlayZenNotificationSoundset(string keywords)
	{
		//keywords = "Can anybody hear me? Over!"; // For testing.

		for (int i = 0; i < ZEN_NOTIFICATION_SOUNDSETS.Count(); i++)
		{
			string key = ZEN_NOTIFICATION_SOUNDSETS.GetKey(i);
			if (keywords.Contains(key))
			{
				string soundset_name = ZEN_NOTIFICATION_SOUNDSETS.GetElement(i);
				if (GetGame().ConfigIsExisting("CfgSoundSets " + soundset_name))
				{
					// Play voice effect
					EffectSound voiceEffect = SEffectManager.PlaySoundOnObject(soundset_name, GetGame().GetPlayer(), 0, 0.15, false);
					if (voiceEffect && voiceEffect.Zen_GetAbstractWave())
					{
						//voiceEffect.SetSoundWaveKind(WaveKind.WAVESPEECH); // TODO: Find out how to auto-apply a "radio" tinny high-pass filter sound 
	
						// Get voice effect duration
						int voiceAudioDuration = voiceEffect.Zen_GetAbstractWave().GetLength() + 2;

						// Play static loop
						EffectSound m_StaticLoop;
						GetGame().GetPlayer().PlaySoundSetLoop(m_StaticLoop, "Zen_MissionRadioStatic_SoundSet", 0, 0);

						// Schedule end of static loop
						GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StopSoundLoop, voiceAudioDuration * 1000, false, m_StaticLoop);
						return true;
					}
				}
			}
		}

		return false;
	}

	static void StopSoundLoop(EffectSound loopEffect)
	{
		if (loopEffect)
		{
			loopEffect.Stop();
		}
	}
};