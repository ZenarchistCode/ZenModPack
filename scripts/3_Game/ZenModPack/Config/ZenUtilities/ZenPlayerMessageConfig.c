ref ZenPlayerMessageConfig g_ZenPlayerMessageConfig;

static ZenPlayerMessageConfig GetZenPlayerMessageConfig()
{
	if (!g_ZenPlayerMessageConfig) GetZenConfigRegister().RegisterConfig(ZenPlayerMessageConfig);
	return g_ZenPlayerMessageConfig;
}

modded class ZenConfigRegister
{
	override void RegisterPreload()
	{
		super.RegisterPreload();
		RegisterType(ZenPlayerMessageConfig);
	}
}

class ZenPlayerMessageConfig : ZenConfigBase
{
	// -------------------------
	// CONFIG SETTINGS
	// -------------------------
	override void OnRegistered()
	{
		g_ZenPlayerMessageConfig = this;
	}
	
	override string 	GetFolderName()       		{ return "Utilities"; }
	override string    	GetCurrentVersion()   		{ return "1.29.1"; }
	override bool		ShouldLoadOnServer() 		{ return true; }
	override bool 		IsServerOnlyConfig()		{ return true; }
	
	override bool ReadJson(string path, out string err)
	{
		return JsonFileLoader<ZenPlayerMessageConfig>.LoadFile(path, this, err);
	}

	override bool WriteJson(string path, out string err)
	{
		return JsonFileLoader<ZenPlayerMessageConfig>.SaveFile(path, this, err);
	}
	
	// -------------------------
	// CONFIG VARS
	// -------------------------
	ref array<ref ZenPlayerMessage> PlayerReplies;

	// Set up default config 
	override void SetDefaults()
	{
		PlayerReplies = new array<ref ZenPlayerMessage>;
		ZenPlayerMessage exampleReply = new ZenPlayerMessage("bohemiaUID", "exampleMsg");
		exampleReply.SpawnItems.Insert("exampleItem1(quantity)");
		exampleReply.SpawnItems.Insert("exampleItem2(quantity/health)");
		exampleReply.SpawnItems.Insert("Matchbox(23/1)");
		PlayerReplies.Insert(exampleReply);
	}

	// Removes the given report from the list and re-saves config
	bool DeleteMessage(notnull PlayerIdentity id)
	{
		bool deletedCfg = false;

		for (int i = 0; i < PlayerReplies.Count(); i++)
		{
			ZenPlayerMessage msg = PlayerReplies.Get(i);
			if (msg.BohemiaID == id.GetId() || msg.BohemiaID == id.GetPlainId())
			{
				PlayerReplies.Remove(i);
				deletedCfg = true;
			}
		}

		// Recreate the template message if all messages have been deleted
		if (PlayerReplies.Count() == 0) 
		{
			ZenPlayerMessage exampleReply = new ZenPlayerMessage("example123", "exampleMsg");
			exampleReply.SpawnItems.Insert("exampleItem1(0)");
			exampleReply.SpawnItems.Insert("exampleItem2(100)");
			PlayerReplies.Insert(exampleReply);
		}

		Save();
		return deletedCfg;
	}

	// Get an item from config list
	ZenPlayerMessage GetPlayerMessage(notnull PlayerIdentity id)
	{
		for (int i = 0; i < PlayerReplies.Count(); i++)
		{
			string toFind = PlayerReplies.Get(i).BohemiaID;

			if (id.GetId() == toFind || id.GetPlainId() == toFind)
			{
				return PlayerReplies.Get(i);
			}
		}

		return NULL;
	}
}

class ZenPlayerMessage
{
	string BohemiaID = ""; // Player Bohemia ID
	string Message = ""; // Reply message
	ref array<string> SpawnItems = new array<string>;
	vector TeleportLocation = vector.Zero;

	void ZenPlayerMessage(string p_BohemiaID, string p_message)
	{
		BohemiaID = p_BohemiaID;
		Message = p_message;
		SpawnItems.Clear();
		TeleportLocation = vector.Zero;
	}
}