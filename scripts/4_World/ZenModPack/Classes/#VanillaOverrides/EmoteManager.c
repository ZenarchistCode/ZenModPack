modded class EmoteManager
{
	override bool InterruptGestureCheck()
	{
		//! IMMERSIVE LOGIN
		if (m_Player && !m_Player.ZenLoginHasFinished())
			return false;

		return super.InterruptGestureCheck();
	}

	override void Update(float deltaT)
	{
		super.Update(deltaT);

		//! IMMERSIVE LOGIN 
		UpdateImmersiveLogin();
	}

	//! IMMERSIVE LOGIN 
	void UpdateImmersiveLogin()
	{
		if (m_Player.ZenLoginHasFinished())
			return;

		if (m_Callback && m_CurrentGestureID == EmoteConstants.ID_EMOTE_LYINGDOWN)
		{
			if (m_Callback.GetState() == 2) // Lying down on ground part of animation
			{
				m_Player.ZenImmersiveLoginFinished();
			}
		}
		else
		if (m_Player.GetSimulationTimeStamp() >= 150) // 110 is how long it should take to lie down server-side
		{
			// Failsafe, no emote has been performed within reasonable period of time - set player as 'logged in'
			m_Player.ZenImmersiveLoginFinished();
		}
	}
}