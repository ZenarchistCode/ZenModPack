modded class ExplosivesBase
{
	// The player who "armed" this explosive
	protected PlayerBase m_ZenArmedPlayer;

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (GetGame().IsDedicatedServer())
		{
			m_ZenArmedPlayer = PlayerBase.Cast(player);
		}
	}

	PlayerBase Zen_GetArmedPlayer()
	{
		return m_ZenArmedPlayer;
	}

	override void OnExplode()
	{
		super.OnExplode();

		#ifdef SERVER
		//! TREESPLOSIONS
		if (ZenModEnabled("ZenTreesplosions"))
			TreeKiller.KillTree(GetPosition(), GetType());
		#endif
	}
}