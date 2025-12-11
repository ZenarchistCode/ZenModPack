modded class Land_FuelStation_Feed
{
	//! REPAIR PUMPS
	protected bool m_ZenIsRepaired = false;

	// This is called whenever a new Pump is created (ie. loaded in on server startup)
	void Land_FuelStation_Feed()
	{
		if (!ZenModEnabled("ZenRepairPumps"))
		{
			m_ZenIsRepaired = true;
			return;
		}

		#ifdef SERVER
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SetupZenRepairablePump, 2500, false);
		#endif
	}

	// Returns this Pump's repaired state
	bool IsZenRepaired()
	{
		return m_ZenIsRepaired;
	}

	// Sets this Pump's repaired state
	void SetZenRepaired(bool repaired)
	{
		m_ZenIsRepaired = repaired;
	}

	// Sets this Pump's repaired state.
	void SetupZenRepairablePump()
	{
		int ts = CF_Date.Now().GetTimestamp();
		int idx = GetZenPumpsDB().GetRepairablePumpIndex(GetPosition());
		ZenRepairablePump Pump = NULL;

		if (idx >= 0)
		{
			Pump = GetZenPumpsDB().RepairablePumps.Get(idx);

			if (!Pump.Invincible)
			{
				// If time stamp > Pump's repaired timestamp, set it back to broken
				if (ts > Pump.BreakTime)
				{
					GetZenPumpsDB().SetPumpRepaired(idx, GetPosition(), false, false);
				}
			}
			else
			{
				GetZenPumpsDB().SetPumpRepaired(idx, GetPosition(), true, false);
			}
		}

		if (Pump)
		{
			m_ZenIsRepaired = Pump.Repaired;
		}
		else
		{
			m_ZenIsRepaired = !GetZenPumpsConfig().DisablePumpsByDefault;
		}
	}

	// Checks if this can be used, and if not, informs player
	bool Zen_CanBeUsed(notnull PlayerBase player)
	{
		if (!ZenModEnabled("ZenRepairPumps"))
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
			player.Zen_SendMessage(GetZenPumpsConfig().MessagePumpNotWork);
			return false;
		}

		return true;
	}
}