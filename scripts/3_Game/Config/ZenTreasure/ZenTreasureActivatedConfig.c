/*
On my server, players were repeatedly farming the treasure locations once they learned of each spot.
This config ensures the treasure only spawns when a player who has read a treasure photo enters the area.
*/
class ZenTreasureConfig_SpawnTriggers
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenTreasureConfig_SpawnTriggers.json";

	// Main config data
	string ConfigVersion = "";
	int TriggerPersistenceSecs = 604800 * 2; // 2 weeks before trigger config is deleted
	ref array<ref ZenTreasureTriggerConfig> TreasureTriggers = new array<ref ZenTreasureTriggerConfig>;

	void Load()
	{
		// Load JSON on server only
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenTreasureConfig_SpawnTriggers>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenTreasureConfig_SpawnTriggers>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				ClearOutdatedTriggers();
				return;
			}
		}

		ConfigVersion = CONFIG_VERSION;
		Save();
	}

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenTreasureConfig_SpawnTriggers>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	ZenTreasureTriggerConfig AddTreasureTrigger(vector position, string playerID, int stashType)
	{
		for (int i = 0; i < TreasureTriggers.Count(); i++)
		{
			ZenTreasureTriggerConfig cfg = TreasureTriggers.Get(i);
			if (vector.Distance(position, cfg.Position) < 1)
			{
				for (int x = 0; x < cfg.TreasureHunters.Count(); x++)
				{
					ZenTreasureHunter hunter = cfg.TreasureHunters.Get(x);
					if (hunter.SteamID == playerID)
					{
						cfg.TreasureHunters.Get(x).Timestamp = JMDate.Now(false).GetTimestamp();
						cfg.TreasureHunters.Get(x).StashType = stashType;
						Save();

						return cfg;
					}
				}

				cfg.TreasureHunters.Insert(new ZenTreasureHunter(stashType, playerID));
				Save();

				return cfg;
			}
		}

		// Trigger not found - create it
		ZenTreasureTriggerConfig newCfg = new ZenTreasureTriggerConfig(position, playerID, stashType);
		TreasureTriggers.Insert(newCfg);
		Save();

		return newCfg;
	}

	void ClearOutdatedTriggers()
	{
		int timestamp = JMDate.Now(false).GetTimestamp();
		ZenTreasureTriggerConfig cfg;

		for (int i = 0; i < TreasureTriggers.Count(); i++)
		{
			cfg = TreasureTriggers.Get(i);
			for (int x = cfg.TreasureHunters.Count() - 1; x >= 0; x--)
			{
				ZenTreasureHunter hunter = cfg.TreasureHunters.Get(x);
				if (hunter.Timestamp == 0 || (timestamp - hunter.Timestamp > TriggerPersistenceSecs))
				{
					cfg.TreasureHunters.Remove(x);
				}
			}
		}

		Save();
	}
}

// Define a stash trigger (save stash position, type, player ID and timestamp)
class ZenTreasureTriggerConfig
{
	vector Position;
	ref array<ref ZenTreasureHunter> TreasureHunters = new array<ref ZenTreasureHunter>;

	void ZenTreasureTriggerConfig(vector p_stashPos, string p_id, int p_type)
	{
		Position = p_stashPos;
		AddTreasureHunter(p_id, p_type);
	}

	void AddTreasureHunter(string p_id, int p_type)
	{
		int timestamp = JMDate.Now(false).GetTimestamp();

		for (int i = 0; i < TreasureHunters.Count(); i++)
		{
			ZenTreasureHunter hunter = TreasureHunters.Get(i);
			if (hunter.SteamID == p_id)
			{
				hunter.StashType = p_type;
				hunter.Timestamp = timestamp;
				return;
			}
		}

		TreasureHunters.Insert(new ZenTreasureHunter(p_type, p_id));
	}
};

class ZenTreasureHunter
{
	string SteamID;
	int StashType;
	int Timestamp;

	void ZenTreasureHunter(int p_type, string p_id)
	{
		StashType = p_type;
		SteamID = p_id;
		Timestamp = JMDate.Now(false).GetTimestamp();
	}
}

ref ZenTreasureConfig_SpawnTriggers m_ZenTreasureConfig_SpawnTriggers;

static ZenTreasureConfig_SpawnTriggers GetZenTreasureConfig_SpawnTriggers()
{
	if (!m_ZenTreasureConfig_SpawnTriggers)
	{
		ZMPrint("[ZenTreasureConfig_SpawnTriggers] Init");
		m_ZenTreasureConfig_SpawnTriggers = new ZenTreasureConfig_SpawnTriggers;
		m_ZenTreasureConfig_SpawnTriggers.Load();
	}

	return m_ZenTreasureConfig_SpawnTriggers;
}
