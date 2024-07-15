modded class Well
{
	//! REPAIR WELLS
	protected bool m_IsRepaired = false;

	// This is called whenever a new well is created (ie. loaded in on server startup)
	void Well()
	{
		if (!ZenModEnabled("ZenRepairPumps"))
		{
			m_IsRepaired = true;
			return;
		}

		#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SetupRepairableWell, 2500, false);
		#endif
	}

	// Returns this well's repaired state
	bool IsRepaired()
	{
		return m_IsRepaired;
	}

	// Sets this well's repaired state
	void SetRepaired(bool repaired)
	{
		m_IsRepaired = repaired;
	}

	// Sets this well's repaired state.
	void SetupRepairableWell()
	{
		int ts = JMDate.Now(false).GetTimestamp();
		int idx = GetZenWellsConfig().GetRepairableWellIndex(GetPosition());
		RepairableWell well = NULL;

		if (idx >= 0)
		{
			well = GetZenWellsConfig().RepairableWells.Get(idx);

			if (!well.Invincible)
			{
				// If time stamp > well's repaired timestamp, set it back to broken
				if (ts > well.BreakTime)
				{
					GetZenWellsConfig().SetWellRepaired(idx, GetPosition(), false, false);
				}
			}
			else
			{
				GetZenWellsConfig().SetWellRepaired(idx, GetPosition(), true, false);
			}
		}

		if (well)
		{
			m_IsRepaired = well.Repaired;
		}
		else
		{
			m_IsRepaired = !GetZenWellsConfig().DisableWellsByDefault;
		}
	}
}