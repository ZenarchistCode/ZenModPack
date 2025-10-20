modded class Well
{
	//! REPAIR WELLS
	protected bool m_ZenIsRepaired = false;

	// This is called whenever a new well is created (ie. loaded in on server startup)
	void Well()
	{
		if (!ZenModEnabled("ZenRepairWells"))
		{
			m_ZenIsRepaired = true;
			return;
		}

		#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SetupZenRepairableWell, 2500, false);
		#endif
	}

	// Returns this well's repaired state
	bool IsZenRepaired()
	{
		return m_ZenIsRepaired;
	}

	// Sets this well's repaired state
	void SetZenRepaired(bool repaired)
	{
		m_ZenIsRepaired = repaired;
	}

	// Sets this well's repaired state.
	void SetupZenRepairableWell()
	{
		int ts = JMDate.Now(false).GetTimestamp();
		int idx = GetZenWellsDB().GetRepairableWellIndex(GetPosition());
		ZenRepairableWell well = NULL;

		if (idx >= 0)
		{
			well = GetZenWellsDB().RepairableWells.Get(idx);

			if (!well.Invincible)
			{
				// If time stamp > well's repaired timestamp, set it back to broken
				if (ts > well.BreakTime)
				{
					GetZenWellsDB().SetWellRepaired(idx, GetPosition(), false, false);
				}
			}
			else
			{
				GetZenWellsDB().SetWellRepaired(idx, GetPosition(), true, false);
			}
		}

		if (well)
		{
			m_ZenIsRepaired = well.Repaired;
		}
		else
		{
			m_ZenIsRepaired = !GetZenWellsConfig().DisableWellsByDefault;
		}
	}

	// Checks if this can be used, and if not, informs player
	bool Zen_CanBeUsed(notnull PlayerBase player)
	{
		if (!ZenModEnabled("ZenRepairWells"))
		{
			return true;
		}

		#ifdef EXPANSIONMODCORE
		if (player.Expansion_IsInSafeZone())
		{
			return true;
		}
		#endif

		if (GetGame().IsDedicatedServer() && !IsZenRepaired())
		{
			player.Zen_SendMessage(GetZenWellsConfig().MessageNotDrink);
			return false;
		}

		return true;
	}
}