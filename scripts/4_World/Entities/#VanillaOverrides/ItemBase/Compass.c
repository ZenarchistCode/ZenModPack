modded class Compass
{
	protected ZenCarCompass m_ZenCompassDashboard;

	void SetZenCarCompass(ZenCarCompass carCompass)
	{
		m_ZenCompassDashboard = carCompass;
	}

	ZenCarCompass GetZenCarCompass()
	{
		return m_ZenCompassDashboard;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetGame().IsDedicatedServer())
		{
			if (m_ZenCompassDashboard)
			{
				m_ZenCompassDashboard.DeleteSafe();
			}
		}
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (GetGame().IsDedicatedServer())
		{
			if (m_ZenCompassDashboard)
			{
				m_ZenCompassDashboard.SetHealth(GetHealth());
			}
		}
	}
}