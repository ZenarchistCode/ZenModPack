//! DISCORD API
modded class TrapBase
{
	// This will only be tracked until server restarts.
	protected PlayerBase m_ZenPlayerTrapper = NULL;

	PlayerBase Zen_GetPlayerTrapper()
	{
		return m_ZenPlayerTrapper;
	}

	override void StartActivate(PlayerBase player)
	{
		super.StartActivate(player);

#ifdef SERVER
		if (GetGame().IsDedicatedServer())
		{
			m_ZenPlayerTrapper = player;
		}
#endif
	}
}
