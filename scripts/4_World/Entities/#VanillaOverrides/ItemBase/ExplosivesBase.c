modded class ExplosivesBase
{
	override void OnExplode()
	{
		super.OnExplode();

		#ifdef SERVER
		//! TREESPLOSIONS
		if (ZenModEnabled("ZenTreesplosions"))
			TreeKiller.KillTree(GetPosition(), GetType());

		//! ANTI-COMBAT LOG
		if (!ZenModEnabled("ZenAntiCombatLogout") || !TriggerCombatLogTimer())
			return;

		TriggerCombatPlayersInRadius(GetZenAntiCombatLogoutConfig().TriggerOnExplosiveRadius);
		#endif
	}

	//! ANTI-COMBAT LOG 
	private bool m_TriggeredPlayersCombat = false;

	void TriggerCombatPlayersInRadius(int radius)
	{
		if (m_TriggeredPlayersCombat)
			return;

		// Only trigger once per grenade explosion
		m_TriggeredPlayersCombat = true;

		// If radius <= 0 then explosive combat log feature is disabled
		if (radius <= 0)
			return;

		// Scan player list for nearby players.
		array<Man> players = new array<Man>;
		g_Game.GetWorld().GetPlayerList(players);
		for (int x = 0; x < players.Count(); x++)
		{
			// If player is within config'd distance for an explosive attack, set their combat log timer.
			if (vector.Distance(players.Get(x).GetPosition(), GetPosition()) <= radius)
			{
				// Do this for the thrower too, even if no one is around. 
				// Prevents meta-gaming by throwing a grenade and checking your logout timer to see if anyone is alive inside a building etc.
				PlayerBase pb = PlayerBase.Cast(players.Get(x));
				pb.SetCombatLogTimer();
			}
		}
	}

	bool TriggerCombatLogTimer()
	{
		return true;
	}
};
