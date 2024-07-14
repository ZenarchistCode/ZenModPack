//! This is a lazy implementation of mod config that saves directly to JSON, but it does the trick.
// My mod settings are not very complex so I don't see any reason to bother improving this further.
class OptionsMenuZenModPack extends ScriptedWidgetEventHandler
{
	static const string	LAYOUT_FILE = "ZenModPack/data/gui/layouts/options/zen_tab.layout";

	protected bool							m_IsChanged;
	protected ref ZenModPackClientConfig	m_OldSettings;							

	protected ButtonWidget					m_CassetteMusic;
	protected ButtonWidget					m_RadioBadge;

	protected Widget						m_Root;
	protected OptionsMenu					m_Menu;
	
	void OptionsMenuZenModPack(Widget parent, Widget details_root, GameOptions options, OptionsMenu menu)
	{
		m_Root										= GetGame().GetWorkspace().CreateWidgets(LAYOUT_FILE, parent);
		m_Menu										= menu;
		
		// Custom settings
		m_CassetteMusic								= ButtonWidget.Cast(m_Root.FindAnyWidget("cassette_setting_option"));
		m_RadioBadge								= ButtonWidget.Cast(m_Root.FindAnyWidget("radiobadge_setting_option"));

		float x, y, y2;
		m_Root.FindAnyWidget("zenmodpack_settings_scroll").GetScreenSize(x, y);
		m_Root.FindAnyWidget("zenmodpack_settings_root").GetScreenSize(x, y2);
		int f = (y2 > y);
		m_Root.FindAnyWidget("zenmodpack_settings_scroll").SetAlpha(f);
		
		m_Root.Show(true);
		m_Root.SetHandler(this);
	}

	void Focus()
	{
		m_OldSettings = m_ZenModPackClientConfig;
		UpdateButtons(false);
	}

	ZenModPackClientConfig GetOldSettings()
	{
		return m_OldSettings;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		//! MUSIC
		if (w == m_CassetteMusic)
		{
			GetZenModPackClientConfig().CassetteMusicSetting++;

			if (GetZenModPackClientConfig().CassetteMusicSetting > 3)
				GetZenModPackClientConfig().CassetteMusicSetting = 1;

			UpdateButtons();
			return true;
		}

		//! RADIO BADGE
		if (w == m_RadioBadge)
		{
			GetZenModPackClientConfig().ShowRadioBadge = !GetZenModPackClientConfig().ShowRadioBadge;

			UpdateButtons();
			return true;
		}

		return super.OnClick(w, x, y, button);
	}

	void UpdateButtons(bool changed = true)
	{
		m_IsChanged = changed;

		if (GetZenModPackClientConfig().ShowRadioBadge)
		{
			m_RadioBadge.SetText("#menu_enabled");
			m_RadioBadge.SetColor(ARGB(255, 0, 128, 32));
		}
		else 
		{
			m_RadioBadge.SetText("#menu_disabled");
			m_RadioBadge.SetColor(ARGB(255, 128, 0, 0));
		}

		if (GetZenModPackClientConfig().CassetteMusicSetting == 1)
		{
			m_CassetteMusic.SetText("#menu_enabled");
			m_CassetteMusic.SetColor(ARGB(255, 0, 128, 32));
		} else
		if (GetZenModPackClientConfig().CassetteMusicSetting == 2)
		{
			m_CassetteMusic.SetText("No Copyright");
			m_CassetteMusic.SetColor(ARGB(255, 128, 0, 0));
		} else
		if (GetZenModPackClientConfig().CassetteMusicSetting == 3)
		{
			m_CassetteMusic.SetText("#menu_disabled");
			m_CassetteMusic.SetColor(ARGB(255, 128, 0, 0));
		}

		if (m_IsChanged)
			m_Menu.OnChanged();
	}

	bool IsChanged()
	{
		return m_IsChanged;
	}
	
	void Apply()
	{
		m_IsChanged = false;
		GetZenModPackClientConfig().Save();
	}
}