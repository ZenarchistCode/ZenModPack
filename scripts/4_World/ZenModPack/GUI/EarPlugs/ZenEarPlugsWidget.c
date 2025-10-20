class ZenEarPlugsWidget extends Managed // Extend Managed for auto-cleanup
{
	static const string LAYOUT_DIR = "ZenModPack/data/gui/earplugs/";

	// Prepare required variables
	ref Widget				m_layout;
	ref ImageWidget			m_icon;
	ref TextWidget			m_volume;
	ref WidgetFadeTimer		m_fadeTimer;
	float					m_iconAlpha = 0;
	float					m_volumeAlpha = 0;

	void ZenEarPlugsWidget()
	{
		m_layout	= GetGame().GetWorkspace().CreateWidgets(LAYOUT_DIR + "earplugs.layout");
		m_icon 		= ImageWidget.Cast(m_layout);
		m_volume	= TextWidget.Cast(m_layout.FindAnyWidget("volume"));
		m_fadeTimer = new WidgetFadeTimer();

		m_icon.LoadImageFile(0, LAYOUT_DIR + "earplug.edds");

		m_icon.Show(false);
		m_volume.Show(false);
	}

	void ~ZenEarPlugsWidget()
	{
		if (!GetGame())
			return;
		
		if (m_fadeTimer && m_fadeTimer.IsRunning())
			m_fadeTimer.Stop();

		delete m_icon;
		delete m_volume;
		delete m_fadeTimer;
		delete m_layout;
	}

	// Called whenever a menu is opened or HUD is hidden (inventory, maps, admin tools, ~ key etc)
	void HideAll()
	{
		if (m_icon)
		{
			m_icon.Show(false);
		}

		if (m_volume)
		{
			m_volume.Show(false);
		}
	}

	// Show ear plug icon instantly
	void ShowIcon()
	{
		m_icon.Show(true);
		m_iconAlpha = 1;
		m_icon.SetAlpha(m_iconAlpha);
	}

	// Start fade out for volume level text
	void SetVolume(int vol)
	{
		string s = " " + vol.ToString() + "%";
		m_volume.SetText(s);
		m_volume.Show(true);
		m_volumeAlpha = 1;
		m_volume.SetAlpha(m_iconAlpha);
		m_fadeTimer.FadeOut(m_volume, 2.0, true); // Fade out volume text over ~2 seconds.
	}

	// Hides both ear plug icon and volume level instantly.
	void HideIcon()
	{
		if (m_icon)
			m_icon.Show(false);

		if (m_volume)
			m_volume.Show(false);
	}
}