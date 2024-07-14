modded class IngameHud
{
	//! BROKEN GLASSES
	bool HideZenBrokenGlasses()
	{
		return !m_HudState || m_HudHidePlayer || m_HudHideUI;
	}

	//! EAR PLUGS
	bool HideEarPlugs()
	{
		return !m_HudState || m_HudHidePlayer || m_HudHideUI || m_HudInventory;
	}

	//! RADIO BADGE 
	bool HideZenRadioBadge()
	{
		return !m_HudState || m_HudHidePlayer || m_HudHideUI;
	}
}