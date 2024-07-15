modded class OptionsMenu
{
	protected int m_ZenModPackTabIndex;
	protected ref OptionsMenuZenModPack m_ZenModPackTab;
	
	override Widget Init()
	{
		layoutRoot = super.Init();
		
		m_ZenModPackTabIndex = m_Tabber.AddTab("ZEN");
		m_ZenModPackTab = new OptionsMenuZenModPack(layoutRoot.FindAnyWidget("Tab_" + m_ZenModPackTabIndex), m_Details, m_Options, this);
		ColorNormal(m_Tabber.GetTab(m_ZenModPackTabIndex));

		return layoutRoot;
	}

	override void OnTabSwitch(int tab)
	{
		super.OnTabSwitch(tab);

		if (tab == m_ZenModPackTabIndex)
			m_ZenModPackTab.Focus();
		
		m_ActiveTabIdx = tab;
	}

	override void OnChanged()
	{
		super.OnChanged();

		if (m_ZenModPackTab && m_ZenModPackTab.IsChanged())
		{
			m_Apply.ClearFlags(WidgetFlags.IGNOREPOINTER);
			ColorNormal(m_Apply);
		}
	}
	
	override void Apply()
	{
		super.Apply();

		if (m_ZenModPackTab && m_ZenModPackTab.IsChanged())
			m_ZenModPackTab.Apply();
	}

	override bool IsAnyTabChanged()
	{
		if (super.IsAnyTabChanged())
			return true;

		return m_ZenModPackTab.IsChanged();
	}
}
