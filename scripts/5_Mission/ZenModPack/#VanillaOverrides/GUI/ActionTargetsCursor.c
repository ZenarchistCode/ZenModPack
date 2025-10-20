modded class ActionTargetsCursor
{
	// Hide action target cursor until player has completed lie-down emote
	override void Update()
	{
		if (m_Player && m_Player.GetSimulationTimeStamp() < 300)
			m_Hidden = true;

		super.Update();
	};
};
