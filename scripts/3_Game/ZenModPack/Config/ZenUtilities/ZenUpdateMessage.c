ref ZenUpdateMessage g_ZenUpdateMessage;

static ZenUpdateMessage GetZenUpdateMessage()
{
	if (!g_ZenUpdateMessage) GetZenConfigRegister().RegisterConfig(ZenUpdateMessage);
	return g_ZenUpdateMessage;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenUpdateMessage);
	}
}

class ZenUpdateMessage : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenUpdateMessage = this;
	}

	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.2"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenUpdateMessage>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenUpdateMessage>.SaveFile(path, this, err);
	}

	// -------------------------
	// CONFIG VARS
	// -------------------------
	string UPDATE_VERSION;
	string UPDATE_PREFIX;
	string UPDATE_SUFFIX;
	string UPDATE_MESSAGE;
	string LOGIN_MESSAGE;
	bool PopupInGame;
	bool NotifySurvivorOfNameChange;
	string SurvivorNotification;
	int RestartHours;
	
	override void SetDefaults()
	{
		UPDATE_VERSION = "-1";
		UPDATE_PREFIX = "Lastest Update: ";
		UPDATE_SUFFIX = " | For more information: HardcoreDayZ.com/changes";
		UPDATE_MESSAGE = "";
		LOGIN_MESSAGE = "";
		PopupInGame = false;
		NotifySurvivorOfNameChange = true;
		SurvivorNotification = "Please change your name to a custom name in the DayZ Launcher (tick Parameters -> Profile name)";
		RestartHours = 4;
	}
}

ref ZenUpdateMessagePersistence g_ZenUpdateMessagePersistence;

static ZenUpdateMessagePersistence GetZenUpdateMessagePersistence()
{
	if (!g_ZenUpdateMessagePersistence) GetZenConfigRegister().RegisterConfig(ZenUpdateMessagePersistence);
	return g_ZenUpdateMessagePersistence;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload(); 
		RegisterType(ZenUpdateMessagePersistence);
	}
}

class ZenUpdateMessagePersistence : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenUpdateMessagePersistence = this;
	}

	override string 	GetRootFolder() 			{ return ZenConstants.GetDbFolder(); }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenUpdateMessagePersistence>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenUpdateMessagePersistence>.SaveFile(path, this, err);
	}
	
	ref map<string, ref ZenPlayerUpdateMsg> PlayerUpdates = new map<string, ref ZenPlayerUpdateMsg>;

	// Load config file or create default file if config doesn't exist
	override void AfterLoad()
	{
		super.AfterLoad();
		
		int oldCount = PlayerUpdates.Count();

		// Scan player data and remove any that are out of date by 1 month or longer 
		int timestamp = CF_Date.Now().GetTimestamp();
		for (int i = PlayerUpdates.Count(); i >= 0; i--)
		{
			ZenPlayerUpdateMsg msg = PlayerUpdates.GetElement(i);
			if (msg != NULL)
			{
				if (timestamp - msg.lastLoginTimestamp >= 604800) // Keep records for 1 week
					PlayerUpdates.Remove(PlayerUpdates.GetKey(i));
			}
		}

		ZMPrint("[ZenUpdateMessagePersistence] Loaded " + PlayerUpdates.Count() + "/" + oldCount + " player update messages from DB file.");
	}
}

class ZenPlayerUpdateMsg
{
	string steamID;
	string updateID;
	int lastLoginTimestamp;

	void ZenPlayerUpdateMsg(string p_steamID, string p_updateID, int p_lastLoginTimestamp)
	{
		steamID = p_steamID;
		updateID = p_updateID;
		lastLoginTimestamp = p_lastLoginTimestamp;
	}
}