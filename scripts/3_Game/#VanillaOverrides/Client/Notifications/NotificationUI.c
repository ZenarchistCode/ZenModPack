modded class NotificationUI
{
	private ref EffectSound m_Sound;

	override void AddNotification(NotificationRuntimeData data)
	{
		super.AddNotification(data);

		// Hide icon image background if there is no image
		/*Widget notification = m_Notifications.Get(data);
		if (notification)
		{
			ImageWidget icon = ImageWidget.Cast(notification.FindAnyWidget("Image"));
			if (icon && data.GetIcon() == "")
			{
				icon.SetAlpha(0);
			}
		}*/

		// Play notification sound
		if (!GetGame().IsDedicatedServer())
		{
			if (GetGame().GetPlayer())
			{
				SEffectManager.PlaySoundOnObject("Zen_NotifySoundset", GetGame().GetPlayer());
			}
		}
	}
};