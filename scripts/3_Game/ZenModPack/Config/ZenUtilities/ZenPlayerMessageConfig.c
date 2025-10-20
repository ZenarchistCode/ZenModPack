class ZenPlayerMessageConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "PlayerMessageConfig.json";

	ref array<ref ZenPlayerMessage> PlayerReplies = new array<ref ZenPlayerMessage>;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenPlayerMessageConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);
		}
		else // Config file does not exist, create default file
		{
			SetupDefaultCfg();
		}

		Save();
	}

	// Set up default config 
	void SetupDefaultCfg()
	{
		ZenPlayerMessage exampleReply = new ZenPlayerMessage("bohemiaUID", "exampleMsg");
		exampleReply.SpawnItems.Insert("exampleItem1(quantity)");
		exampleReply.SpawnItems.Insert("exampleItem2(quantity/health)");
		exampleReply.SpawnItems.Insert("Matchbox(23/1)");
		PlayerReplies.Insert(exampleReply);
	}

	// Save config
	void Save()
	{
		if (!FileExist("$profile:\\Zenarchist"))
			MakeDirectory("$profile:\\Zenarchist");

		if (!FileExist("$profile:\\Zenarchist\\Utilities"))
			MakeDirectory("$profile:\\Zenarchist\\Utilities");

		if (!FileExist(zenModFolder))
			MakeDirectory(zenModFolder);

		// Save JSON config
		JsonFileLoader<ZenPlayerMessageConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Removes the given report from the list and re-saves config
	bool DeleteMessage(string uid)
	{
		bool deletedCfg = false;

		for (int i = 0; i < PlayerReplies.Count(); i++)
		{
			ZenPlayerMessage msg = PlayerReplies.Get(i);
			if (msg.BohemiaID == uid)
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
	ZenPlayerMessage GetPlayerMessage(string BohemiaID)
	{
		for (int i = 0; i < PlayerReplies.Count(); i++)
		{
			string toFind = PlayerReplies.Get(i).BohemiaID;
			if (BohemiaID == toFind)
			{
				return PlayerReplies.Get(i);
			}
		}

		return NULL;
	}
};

class ZenPlayerMessage
{
	string BohemiaID = ""; // Player Bohemia ID
	string Message = ""; // Reply message
	ref array<string> SpawnItems = new array<string>;

	void ZenPlayerMessage(string p_BohemiaID, string p_message)
	{
		BohemiaID = p_BohemiaID;
		Message = p_message;
		SpawnItems.Clear();
	}
};

// Save config data
static ref ZenPlayerMessageConfig m_ZenPlayerMessageConfig;

// Helper function to return Config data storage object
static ZenPlayerMessageConfig GetZenPlayerMessageConfig()
{
	if (!m_ZenPlayerMessageConfig)
	{
		m_ZenPlayerMessageConfig = new ZenPlayerMessageConfig;
		m_ZenPlayerMessageConfig.Load();
	}

	return m_ZenPlayerMessageConfig;
};