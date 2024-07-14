modded class Land_FuelStation_Feed
{
	//! REPAIR PUMPS
	protected bool m_IsRepaired = false;

	// This is called whenever a new Pump is created (ie. loaded in on server startup)
	void Land_FuelStation_Feed()
	{
		if (!ZenModEnabled("ZenRepairPumps"))
		{
			m_IsRepaired = true;
			return;
		}

		#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SetupRepairablePump, 2500, false);
		#endif
	}

	// Returns this Pump's repaired state
	bool IsRepaired()
	{
		return m_IsRepaired;
	}

	// Sets this Pump's repaired state
	void SetRepaired(bool repaired)
	{
		m_IsRepaired = repaired;
	}

	// Sets this Pump's repaired state.
	void SetupRepairablePump()
	{
		int ts = JMDate.Now(false).GetTimestamp();
		int idx = GetZenPumpsConfig().GetRepairablePumpIndex(GetPosition());
		RepairablePump Pump = NULL;

		if (idx >= 0)
		{
			Pump = GetZenPumpsConfig().RepairablePumps.Get(idx);

			if (!Pump.Invincible)
			{
				// If time stamp > Pump's repaired timestamp, set it back to broken
				if (ts > Pump.BreakTime)
				{
					GetZenPumpsConfig().SetPumpRepaired(idx, GetPosition(), false, false);
				}
			}
			else
			{
				GetZenPumpsConfig().SetPumpRepaired(idx, GetPosition(), true, false);
			}
		}

		if (Pump)
		{
			m_IsRepaired = Pump.Repaired;
		}
		else
		{
			m_IsRepaired = !GetZenPumpsConfig().DisablePumpsByDefault;
		}
	}
}