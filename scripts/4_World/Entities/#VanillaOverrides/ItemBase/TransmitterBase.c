modded class TransmitterBase
{
	const static int RPC_PLAY_ZEN_RADIO_SOUNDSET = -60123;

	bool m_ZenPlayingSoundEffect;
	EffectSound m_ZenVoiceEffect;
	EffectSound m_ZenStaticLoopEffect;

	void ~TransmitterBase()
	{
		SEffectManager.DestroyEffect(m_ZenVoiceEffect);
		SEffectManager.DestroyEffect(m_ZenStaticLoopEffect);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        if (GetGame().IsClient() && rpc_type == RPC_PLAY_ZEN_RADIO_SOUNDSET)
        {
            Param1<string> data;
            if (ctx.Read(data))
            {
                string soundset_name = data.param1;

				if (soundset_name == "" || !GetGame().ConfigIsExisting("CfgSoundSets " + soundset_name))
				{
					Error("[ZenModPack] Tried to play non-existing soundset: '" + soundset_name + "'");
					return;
				}

				ZenRadioStopSounds(false);
				m_ZenVoiceEffect = ZenPlaySoundOnThisItem(soundset_name);

				if (m_ZenVoiceEffect && m_ZenVoiceEffect.Zen_GetAbstractWave())
				{
					// Get voice audio duration
					int voiceAudioDuration = m_ZenVoiceEffect.Zen_GetAbstractWave().GetLength() + 2;

					if (voiceAudioDuration > 0)
					{
						// Play static loop
						m_ZenStaticLoopEffect = ZenPlaySoundOnThisItem("personalradio_staticnoise_SoundSet", true);
						EffectSound staticEffect = ZenPlaySoundOnThisItem("tuneRadio_SoundSet", false);

						// Schedule end of static loop
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StopZenStaticSoundLoop);
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StopZenStaticSoundLoop, voiceAudioDuration * 1000, false, true);
						//Print("[ZenModPack] " + GetType() + " - playing soundset " + soundset_name + " - stopping static in " + voiceAudioDuration + " seconds.");
					}
				}
			}
		}
    }

	void StopZenStaticSoundLoop(bool playStatic = true)
	{
		if (m_ZenStaticLoopEffect)
		{
			if (playStatic)
			{
				EffectSound staticEffect = ZenPlaySoundOnThisItem("tuneRadio_SoundSet", false);
			}

			m_ZenStaticLoopEffect.SoundStop();
			//Print("[ZenModPack] " + GetType() + " - stop static.");
		}
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		if (GetGame().IsDedicatedServer())
		{
			ZenRadioPlugin plugin = ZenRadioPlugin.Cast(GetPlugin(ZenRadioPlugin));
			if (plugin)
			{
				plugin.AddRadio(this);
			}
		}
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		ZenRadioStopSounds(true, false);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZenRadioStopSounds();
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (newLevel == GameConstants.STATE_RUINED)
		{
			ZenRadioStopSounds();
		}
	}

	void ZenRadioStopSounds(bool remove = true, bool playStaticSound = true)
	{
		if (GetGame().IsClient())
		{
			if (m_ZenVoiceEffect)
			{
				m_ZenVoiceEffect.SoundStop();
			}

			StopZenStaticSoundLoop(playStaticSound);
		}

		if (GetGame().IsDedicatedServer())
		{
			if (remove)
			{
				ZenRadioPlugin plugin = ZenRadioPlugin.Cast(GetPlugin(ZenRadioPlugin));
				if (plugin)
					plugin.RemoveRadio(this);
			}
		}
	}

	EffectSound ZenPlaySoundOnThisItem(string soundset, bool loop = false)
	{
		EffectSound sound = SEffectManager.CreateSound(soundset, GetPosition(), 0, 0.15, loop);
		if (!sound)
		{
			return NULL;
		}

		if (!loop)
		{
			sound.SetAutodestroy(true);
		}
		
		sound.SetParent(this);
		sound.SetLocalPosition(vector.Zero);
		sound.SoundPlay();

		return sound;
	}
}