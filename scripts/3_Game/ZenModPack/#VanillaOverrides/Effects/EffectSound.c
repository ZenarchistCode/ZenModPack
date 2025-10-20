modded class EffectSound
{
	void Zen_SetMaxVolume(float v)
	{
		m_SoundWaveVolumeMax = v;
	}

	AbstractWave Zen_GetAbstractWave()
	{
		return m_SoundWaveObject;
	}
}