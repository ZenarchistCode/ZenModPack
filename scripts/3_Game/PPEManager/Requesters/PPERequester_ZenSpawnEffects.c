//! IMMERSIVE LOGIN
class PPERequester_ZenSpawnEffects extends PPERequester_GameplayBase
{
	static const int LOGIN_ID = 99699;
	protected ref array<float> m_OverlayColor;

	void SetEffectValues(float percentage)
	{
		if (!IsRequesterRunning())
			Start();

		m_OverlayColor = { 0.0, 0.0, 0.0, 1.0 };
		if (percentage > 0.0)
			m_OverlayColor = { 0.1, 0.1, 0.1, 1.0 };

		// Set blur intensity
		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, true, percentage * 0.2, LOGIN_ID, PPOperators.ADD_RELATIVE);
		// Set vignette intensity
		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_VIGNETTE, false, percentage, LOGIN_ID, PPOperators.ADD);
		// Set black screen overlay color and intensity
		SetTargetValueFloat(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYFACTOR, true, percentage * 0.16, LOGIN_ID, PPOperators.HIGHEST);
		SetTargetValueColor(PostProcessEffectType.Glow, PPEGlow.PARAM_OVERLAYCOLOR, m_OverlayColor, LOGIN_ID, PPOperators.SET);
	}
}