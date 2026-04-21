#ifdef DZ_Expansion_Core
// Expansion compatibility
modded class ExpansionNotificationView
{
	override void ShowNotification() 
	{
		super.ShowNotification();
		
		if (!m_Data)
			return;

		if (CheckZenSoundSet())
			NotificationUI.CheckPlayZenNotificationSoundset(m_Data.GetTitleText() + " " + m_Data.GetDetailText());
	}

	// Override this if you have any reason to disable soundsets on Expansion notifications
	bool CheckZenSoundSet()
	{
		return true;
	}
}
#endif