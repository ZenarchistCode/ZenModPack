modded class ActionBase
{
	// CLIENT ACTION DISPLAY PRIORITY
	int Zen_GetDisplayPriority()
	{
		return 0;
	}

	//! UTILITIES
	protected static ref ZenPerformanceMonitorPlugin m_PerformancePlugin;

#ifdef SERVER
	void ActionBase()
	{
		if (GetZenUtilitiesConfig().ServerActionFPSDropWarningPct == 0)
			return;

		if (!m_PerformancePlugin)
			m_PerformancePlugin = ZenPerformanceMonitorPlugin.Cast(GetPlugin(ZenPerformanceMonitorPlugin));
	}

	override void Start(ActionData action_data)
	{
		super.Start(action_data);

		if (!m_PerformancePlugin)
			return;

		m_PerformancePlugin.ActionBase_Start(action_data);
	}

	override void OnUpdateServer(ActionData action_data)
	{
		super.OnUpdateServer(action_data);

		if (!m_PerformancePlugin)
			return;

		m_PerformancePlugin.ActionBase_OnUpdate(action_data);
	}

	override void ActionCleanup(ActionData action_data)
	{
		super.ActionCleanup(action_data);

		if (!m_PerformancePlugin)
			return;

		m_PerformancePlugin.ActionBase_End(action_data);
	}
#endif
}