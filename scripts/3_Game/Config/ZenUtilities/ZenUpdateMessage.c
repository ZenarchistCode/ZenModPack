class ZenUpdateMessage
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "UpdateMessage.json";

	string UPDATE_VERSION = "-1";
	string UPDATE_PREFIX = "Lastest Update: ";
	string UPDATE_SUFFIX = " | For more information: HardcoreDayZ.com/changes";
	string UPDATE_MESSAGE = "";
	bool PopupInGame = false;
	ref map<string, ref ZenPlayerUpdateMsg> PlayerUpdates;

	// Load config file or create default file if config doesn't exist
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{ 
			// If config exists, load file
			JsonFileLoader<ZenUpdateMessage>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// Scan player data and remove any that are out of date by 1 month or longer 
			int timestamp = JMDate.Now(false).GetTimestamp();
			for (int i = PlayerUpdates.Count(); i >= 0; i--)
			{
				ZenPlayerUpdateMsg msg = PlayerUpdates.GetElement(i);
				if (msg != NULL)
				{
					if (timestamp - msg.lastLoginTimestamp >= 604800) // Keep records for 1 week
						PlayerUpdates.Remove(PlayerUpdates.GetKey(i));
				}
			}
		}

		// Config file does not exist, create default file
		Save();
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

		autoptr ZenUpdateMessage OldFile;
		JsonFileLoader<ZenUpdateMessage>.JsonLoadFile(zenModFolder + zenConfigName, OldFile);

		// Save JSON config
		if (OldFile)
		{
			UPDATE_VERSION = OldFile.UPDATE_VERSION;
			UPDATE_MESSAGE = OldFile.UPDATE_MESSAGE; // This allows editing the message and ID between 'saves'
		}

		JsonFileLoader<ZenUpdateMessage>.JsonSaveFile(zenModFolder + zenConfigName, this);
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

// Save config data
static ref ZenUpdateMessage m_ZenUpdateMessage;

// Helper function to return Config data storage object
static ZenUpdateMessage GetZenUpdateMessage()
{
	if (!m_ZenUpdateMessage)
	{
		m_ZenUpdateMessage = new ZenUpdateMessage;
		m_ZenUpdateMessage.Load();
	}

	return m_ZenUpdateMessage;
};
