ref ZenNotificationsConfig g_ZenNotificationsConfig;

static ZenNotificationsConfig GetZenNotificationsConfig()
{
	if (!g_ZenNotificationsConfig) GetZenConfigRegister().RegisterConfig(ZenNotificationsConfig);
	return g_ZenNotificationsConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenNotificationsConfig);
	}
}

class ZenNotificationsConfig : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenNotificationsConfig = this;
	}
	
	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool		ShouldLoadOnClient() 		{ return true; }
	override bool		ShouldSyncToClient()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenNotificationsConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenNotificationsConfig>.SaveFile(path, this, err);
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenNotificationsConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenNotificationsConfig>.LoadData(payload, this, err);
	}

	// Define notification keywords
	bool EnableServerNotifications;
	bool DebugMode;
	string ServerMessage;
	string ServerIcon;
	ref array<ref ZenNotification> Keywords;

	override void SetDefaults()
	{
		// Notification keywords
		EnableServerNotifications = true;
		DebugMode = false;
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
	}
}

// Define a notification
class ZenNotification
{
	string Keyword;
	string Title;
	string Icon;
	bool HideKeyword;
	bool PlaySound;

	void ZenNotification(string k, string t = "#servermessage", string i = "#servericon", bool hide = false, bool playSound = true)
	{
		Keyword = k;
		Title = t;
		Icon = i;
		HideKeyword = hide;
		PlaySound = playSound;
	}
}