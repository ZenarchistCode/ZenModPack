#ifdef EXPANSIONMODCORE
// Expansion compatibility
modded class ExpansionNotificationView
{
	override void ShowNotification() 
	{
		super.ShowNotification();

		if (CheckZenSoundSet())
			NotificationUI.CheckPlayZenNotificationSoundset(m_Data.GetDetailText());
	}

	// Override this if you have any reason to disable soundsets on Expansion notifications
	bool CheckZenSoundSet()
	{
		return true;
	}
}
#endif