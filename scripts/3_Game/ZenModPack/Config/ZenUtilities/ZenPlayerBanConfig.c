ref ZenPlayerBanConfig g_ZenPlayerBanConfig;

static ZenPlayerBanConfig GetZenPlayerBanConfig()
{
	if (!g_ZenPlayerBanConfig) GetZenConfigRegister().RegisterConfig(ZenPlayerBanConfig);
	return g_ZenPlayerBanConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenPlayerBanConfig);
	}
}

class ZenPlayerBanConfig : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenPlayerBanConfig = this;
	}
	
	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.2"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenPlayerBanConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenPlayerBanConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	int KickBannedPlayerSecs
	float BanTeleportX;
	float BanTeleportY;
	float BanTeleportZ;
	float UnbanTeleportX;
	float UnbanTeleportY;
	float UnbanTeleportZ;
	string BanExpiresText;
	string BanYearText;
	string BanMonthText;
	string BanDayText;
	string UnbanMessage;
	ref array<ref ZenPlayerBan> PlayerBans;
	
	override void SetDefaults()
	{
		KickBannedPlayerSecs = 60;
		BanTeleportX = 0;
		BanTeleportY = 0;
		BanTeleportZ = 0;
		UnbanTeleportX = 0;
		UnbanTeleportY = 0;
		UnbanTeleportZ = 0;
		BanExpiresText = "Ban Expires:";
		BanYearText = "Year:";
		BanMonthText = "Month:";
		BanDayText = "Day:";
		UnbanMessage = "Your ban has been lifted. Please review our rules, as any further offences could result in a permanent ban - not just from our servers, but potentially all regional servers.";
		PlayerBans = new array<ref ZenPlayerBan>;
	}
	
	override void AfterLoad()
	{
		super.AfterLoad();
		
		UpdateDefaultConfig();
	}

	// Get an item from config list
	ZenPlayerBan GetPlayerBan(notnull PlayerIdentity id)
	{
		for (int i = 0; i < PlayerBans.Count(); i++)
		{
			string toFind = PlayerBans.Get(i).BohemiaID;
			if (id.GetId() == toFind || id.GetPlainId() == toFind)
			{
				return PlayerBans.Get(i);
			}
		}

		return NULL;
	}
	
	void UpdateDefaultConfig()
	{
		if (PlayerBans.Count() == 0)
		{
			PlayerBans.Insert(new ZenPlayerBan("playerID", "Ban message."));
			Save();
		}
	}
}

class ZenPlayerBan
{
	string BohemiaID = ""; // Player BohemiaID
	string Message = ""; // Ban message
	int UnbanYear;
	int UnbanMonth;
	int UnbanDay;
	float BanTeleportX;
	float BanTeleportY;
	float BanTeleportZ;
	float UnbanTeleportX = 0;
	float UnbanTeleportY = 0;
	float UnbanTeleportZ = 0;
	bool KillOnUnban = false;
	string UnbanMessage = "";
	bool Unbanned = false;

	void ZenPlayerBan(string p_id, string p_message)
	{
		BohemiaID = p_id;
		Message = p_message;

		GetYearMonthDay(UnbanYear, UnbanMonth, UnbanDay);
		
		UnbanMonth = UnbanMonth + UnbanMonth + 1;

		if (UnbanMonth > 12)
		{
			UnbanMonth = 1;
			UnbanYear = UnbanYear + 1;
		}
	}
}
