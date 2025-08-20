modded class Chat
{
	private int LAST_MESSAGE;

	static bool HasReceivedServerConfig;
	static bool EnableServerNotifications;
	static string ServerMessage;
	static string ServerIcon;

	override void Add(ChatMessageEventParams params)
	{
		// If game hasn't loaded yet, stop here
		if (!GetGame() || !GetGame().GetMission())
			return;

		// Get message parameters
		int channel = params.param1;
		string from = params.param2;
		string text = params.param3;

		// Check if it's a server message
		bool serverMessage = (channel & CCBattlEye) || (channel & CCAdmin) || (channel & CCSystem);
		if (serverMessage && HasReceivedServerConfig && EnableServerNotifications)
		{
			int timeDiff = GetGame().GetTime() - LAST_MESSAGE;
			if (timeDiff < 1000)
			{
				super.Add(params);
				return;
			}

			// TODO: This breaks private and global messaging
			ZenNotification notification = FindKeyword(text);
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
				title = ServerMessage;

			string icon = notification.Icon;
			if (notification.Icon == "#servericon")
				icon = ServerIcon;

			if (notification.HideKeyword)
				text.Replace(GetKeyword(text), "");

			ZenNotifications.NotifyLocal(title, text.Trim(), icon);
			LAST_MESSAGE = GetGame().GetTime();
		}
		else
		{
			super.Add(params);
		}
	}

	// Check if the given message contains a flagged keyword
	private ZenNotification FindKeyword(string msg)
	{
		if (msg.Contains("(Private) Admin:") || msg.Contains("(Global) Admin:") || msg.Contains("[N]") || msg.Contains("[DaRT]"))
			return new ZenNotification("#servermessage");

		msg.ToLower();
		foreach (ZenNotification n : NotificationUI.ZenKeywords)
		{
			string key = n.Keyword;
			key.ToLower();
			if (msg.Contains(key))
				return n;
		}

		return NULL;
	}

	// Returns the keyword that was found in the given text
	private string GetKeyword(string msg)
	{
		msg.ToLower();
		foreach (ZenNotification n : NotificationUI.ZenKeywords)
		{
			string key = n.Keyword;
			key.ToLower();
			if (msg.Contains(key))
				return n.Keyword;
		}

		return "";
	}
}