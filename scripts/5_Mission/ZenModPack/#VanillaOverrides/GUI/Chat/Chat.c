modded class Chat
{
	private int ZEN_LAST_MESSAGE_TIME;

	override void Add(ChatMessageEventParams params)
	{
		// If game hasn't loaded yet, stop here
		if (!g_Game || !g_Game.GetMission())
			return;
		
		if (!ZenModEnabled("ZenNotifications"))
		{
			super.Add(params);
			return;
		}

		// Get message parameters
		int channel = params.param1;
		string from = params.param2;
		string text = params.param3;
		
		if (GetZenNotificationsConfig().DebugMode)
		{
			Print("chat_text=" + text);
			ZenNotification notificationTest = FindZenKeyword(text);
			if (notificationTest)
			{
				Print("Notification cfg found: PlaySound=" + notificationTest.PlaySound + " " + notificationTest.Title + " HasReceivedServerConfig=" + GetZenNotificationsConfig().HasReceivedSync() + " EnableServerNotifications=" + GetZenNotificationsConfig().EnableServerNotifications);
			}
		}

		// Check if it's a server message
		bool serverMessage = GetZenNotificationsConfig().DebugMode || (channel & CCBattlEye) || (channel & CCAdmin) || (channel & CCSystem);
		if (serverMessage && GetZenNotificationsConfig().HasReceivedSync() && GetZenNotificationsConfig().EnableServerNotifications)
		{
			int timeDiff = g_Game.GetTime() - ZEN_LAST_MESSAGE_TIME;
			if (timeDiff < 1000)
			{
				super.Add(params);
				return;
			}

			ZenNotification notification = FindZenKeyword(text);
			if (!notification)
			{
				super.Add(params);
				return;
			}

			// Display text + notification for private messages (useful for longer PMs that don't stay on the notification screen for very long)
			if (text.Contains("(Private) Admin:"))
				super.Add(params);

			string title = notification.Title;
			if (notification.Title == "#servermessage")
				title = GetZenNotificationsConfig().ServerMessage;

			string icon = notification.Icon;
			if (notification.Icon == "#servericon")
				icon = GetZenNotificationsConfig().ServerIcon;

			if (notification.HideKeyword)
				text.Replace(GetZenKeyword(text), "");

			ZenNotifications.NotifyLocal(title, text.Trim(), icon);
			ZEN_LAST_MESSAGE_TIME = g_Game.GetTime();
		}
		else
		{
			super.Add(params);
		}
	}

	// Check if the given message contains a flagged keyword
	private ZenNotification FindZenKeyword(string msg)
	{
		if (msg.Contains("(Private) Admin:") || msg.Contains("(Global) Admin:") || msg.Contains("[N]") || msg.Contains("[DaRT]"))
			return new ZenNotification("#servermessage");

		msg.ToLower();
		foreach (ZenNotification n : GetZenNotificationsConfig().Keywords)
		{
			string key = n.Keyword;
			key.ToLower();
			if (msg.Contains(key))
				return n;
		}

		return NULL;
	}

	// Returns the keyword that was found in the given text
	private string GetZenKeyword(string msg)
	{
		msg.ToLower();
		foreach (ZenNotification n : GetZenNotificationsConfig().Keywords)
		{
			string key = n.Keyword;
			key.ToLower();
			if (msg.Contains(key))
				return n.Keyword;
		}

		return "";
	}
}