class ZenNotifications
{
	static void Notify(PlayerBase player, string title, string message, string icon = "", float time = 5)
	{
#ifdef SERVER
		if (player && player.GetIdentity())
		{
			NotificationSystem.SendNotificationToPlayerExtended(player, time, title, message, icon);
		}
#endif
	}

	static void NotifyLocal(string title, string message, string icon = "", float p_time = 5)
	{
		if (GetGame().IsClient() && GetGame().GetPlayer())
		{
			float time = message.Length() / 10;
			if (time < p_time)
				time = p_time;

			NotificationSystem.AddNotificationExtended(time, title, message, icon);
		}
	}
}