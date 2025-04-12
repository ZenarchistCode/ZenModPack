class ZenNotificationsConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "NotificationsConfig.json";

	// Define notification keywords
	static bool HasReceivedServerConfig = false;
	bool EnableServerNotifications;
	string ServerMessage;
	string ServerIcon;
	ref array<ref ZenNotification> Keywords;

	// Load config file or create default file if config doesn't exist
	void Load()
	{
		if (GetGame().IsDedicatedServer())
		{
			if (FileExist(zenModFolder + zenConfigName))
			{	// If config exists, load file
				JsonFileLoader<ZenNotificationsConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
			}
			else // Config file does not exist, create default file
			{
				// Notification keywords
				EnableServerNotifications = true;
				ServerIcon = "ZenModPack/data/gui/Notifications/notify.edds";
				ServerMessage = "Server Message";
				Keywords = new array<ref ZenNotification>;
				Keywords.Insert(new ZenNotification("[N]", "#servermessage", "#servericon", true));
				Keywords.Insert(new ZenNotification("[Dart]", "#servermessage", "#servericon", true));
				Keywords.Insert(new ZenNotification("(Private) Admin:", "Admin Private Message", "#servericon", true));
				Keywords.Insert(new ZenNotification("(Global) Admin:", "Admin Global Message", "#servericon", true));
				Keywords.Insert(new ZenNotification("Restart"));
				Keywords.Insert(new ZenNotification("Shutdown"));
				Keywords.Insert(new ZenNotification("Update:"));
				Keywords.Insert(new ZenNotification("Quest:", "Quest Update:", "ZenModPack/data/gui/Notifications/quest.edds", true));
				// Save config
				Save();
			}
		}
	}

	// Save config
	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist("$profile:\\Zenarchist"))
		{
			MakeDirectory(zenModFolder);
		}

		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenNotificationsConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Define a notification
class ZenNotification
{
	string Keyword;
	string Title;
	string Icon;
	bool HideKeyword;

	void ZenNotification(string k, string t = "#servermessage", string i = "#servericon", bool hide = false)
	{
		Keyword = k;
		Title = t;
		Icon = i;
		HideKeyword = hide;
	}
};

// Save config data
static ref ZenNotificationsConfig m_ZenNotificationsConfig;

// Helper function to return Config data storage object
static ZenNotificationsConfig GetZenNotificationsConfig()
{
	if (!m_ZenNotificationsConfig)
	{
		m_ZenNotificationsConfig = new ZenNotificationsConfig;
		m_ZenNotificationsConfig.Load();
	}

	return m_ZenNotificationsConfig;
};