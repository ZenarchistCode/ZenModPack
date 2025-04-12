//! DISCORD API
modded class Grenade_Base
{
	protected PlayerBase m_ZenKillfeedUnpinPlayer = NULL;

	PlayerBase Zen_GetUnpinPlayer()
	{
		return m_ZenKillfeedUnpinPlayer;
	}

	// Detect by players unpins
	override void Unpin()
	{
		super.Unpin();

#ifdef SERVER
		if (GetGame().IsDedicatedServer() && GetHierarchyRootPlayer() != NULL)
		{
			PlayerBase pb = PlayerBase.Cast(GetHierarchyRootPlayer());
			if (pb)
			{
				#ifdef ENFUSION_AI_PROJECT
				if (pb.IsAI())
				{
					// Don't count expansion AI
					m_ZenKillfeedUnpinPlayer = NULL;
					return;
				}
				#endif

				m_ZenKillfeedUnpinPlayer = pb;
			}
		}
		else 
		{
			m_ZenKillfeedUnpinPlayer = NULL;
		}
#endif
	}
}
