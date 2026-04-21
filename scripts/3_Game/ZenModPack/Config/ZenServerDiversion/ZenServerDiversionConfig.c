ref ZenServerDiversionConfig g_ZenServerDiversionConfig;

static ZenServerDiversionConfig GetZenServerDiversionConfig()
{
	if (!g_ZenServerDiversionConfig) GetZenConfigRegister().RegisterConfig(ZenServerDiversionConfig);
	return g_ZenServerDiversionConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenServerDiversionConfig);
	}
}

class ZenServerDiversionConfig: ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenServerDiversionConfig = this;
	}
	
	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool		ShouldSyncToClient() 		{ return true; }

	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenServerDiversionConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenServerDiversionConfig>.SaveFile(path, this, err);
	}
	
	override protected bool BuildSyncPayload(out string payload, out string err)
	{
		return JsonFileLoader<ZenServerDiversionConfig>.MakeData(this, payload, err, false);
	}

	override protected bool ApplySyncPayload(string payload, out string err)
	{
		return JsonFileLoader<ZenServerDiversionConfig>.LoadData(payload, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	string ServerIP = "";
	int ServerPort = -1;
	string ServerPass = "";
	string RedirectMessage = "";

	[NonSerialized()]
	bool m_ServerDiverted = false;

	override void SetDefaults()
	{
		RedirectMessage = "The server has moved to a new IP address (%ip:%port) - you will be automatically redirected shortly. Please update your server bookmarks in the launcher.";
	}
	
	override void AfterLoad()
	{
		super.AfterLoad();
		
		RedirectMessage.Replace("%ip", ServerIP);
		RedirectMessage.Replace("%port", ServerPort.ToString());
	}
}