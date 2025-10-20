class ZenRadioBadgeWidget extends Managed
{
	static const string		LAYOUT_FILE = "ZenModPack/data/gui/layouts/radiobadge/radio.layout";

	ref Widget				m_layout;
	ref ImageWidget			m_icon;
	ref TextWidget			m_channel;
	ref WidgetFadeTimer		m_fadeTimer;

	void ZenRadioBadgeWidget()
	{
		m_layout	= GetGame().GetWorkspace().CreateWidgets(LAYOUT_FILE);
		m_icon 		= ImageWidget.Cast(m_layout);
		m_channel	= TextWidget.Cast(m_layout.FindAnyWidget("channel"));
		m_fadeTimer = new WidgetFadeTimer();

		//m_icon.LoadImageFile(0, "ZenModPack/data/gui/layouts/radio.edds");
		m_icon.Show(false);
		m_channel.Show(false);
	}

	void ~ZenRadioBadgeWidget()
	{
		if (!GetGame())
			return;
		
		if (m_fadeTimer && m_fadeTimer.IsRunning())
			m_fadeTimer.Stop();

		delete m_icon;
		delete m_channel;
		delete m_fadeTimer;
		delete m_layout;
	}

	void HideAll()
	{
		if (m_icon)
		{
			m_icon.Show(false);
		}

		if (m_channel)
		{
			m_channel.Show(false);
		}
	}

	void ShowIcon()
	{
		m_icon.Show(true);
		m_icon.SetAlpha(1);
		m_channel.Show(true);
		m_channel.SetAlpha(1);
	}

	void SetChannel(string ch)
	{
		m_channel.SetText(ch);
	}

	void HideIcon()
	{
		if (m_icon)
			m_icon.Show(false);

		if (m_channel)
			m_channel.Show(false);
	}
}