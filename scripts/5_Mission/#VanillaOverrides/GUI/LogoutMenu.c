//! ANTI-COMBAT LOG
modded class LogoutMenu
{
	int m_KilledOnLogoutCB = -1;

	// Update description text based on info sent from server
	override void UpdateInfo()
	{
		super.UpdateInfo();

		if (!GetGame())
			return;

		// Get player
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;

		// If player has not shot at anyone or been shot at, enable exit button in 1 sec
		if (m_bLogoutNow)
		{
			m_bLogoutNow.Show(false);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(EnableLogoutButton, 1000, false);
		}

		// Set logout message
		if (player.WillBePunishedForCombatLogging() >= 1)
		{
			if (player.WillBePunishedForCombatLogging() == 3)
				m_DescriptionText.SetText("#zen_layout_logout_dialog_note_cb"); // No penalty but extended timer
			else
			if (player.WillBePunishedForCombatLogging() == 1)
				m_DescriptionText.SetText("#zen_layout_logout_dialog_note_killed_cb"); // Dead message
			else
				m_DescriptionText.SetText("#zen_layout_logout_dialog_note_killed_cb2"); // Flare message

			m_DescriptionText.SetColor(ARGB(255, 255, 0, 0)); // Set text color to red

			// If player has shot at someone or been shot at, enable exit button in X secs from server config
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(EnableLogoutButton);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(EnableLogoutButton, player.GetDisableExitButtonSecs() * 1000, false);
		}
		else
		{
			// Reset text color to default (vanilla = white)
			m_DescriptionText.SetColor(ARGB(255, 255, 255, 255));
		}
	}

	// UpdateInfo() is only called when dialog opens, so we need to constantly check on Update() if server has informed player they'll be killed after dialog opens
	override void Update(float timeslice)
	{
		super.Update(timeslice);

		// Check player combat log status
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player && m_KilledOnLogoutCB != player.WillBePunishedForCombatLogging())
		{
			UpdateInfo();
			m_KilledOnLogoutCB = player.WillBePunishedForCombatLogging();
		}
	}

	// Enable the Exit button
	private void EnableLogoutButton()
	{
		if (m_bLogoutNow)
			m_bLogoutNow.Show(true);
	}
};