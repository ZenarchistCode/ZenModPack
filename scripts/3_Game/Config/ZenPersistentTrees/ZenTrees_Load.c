// This class loads all cut down trees on server startup, then deletes itself so they're not stored in memory
class ZenTrees_Load
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenTrees_Load.json";

	// Config data
	autoptr array<ref ZenTreeState> CutTrees = new array<ref ZenTreeState>;

	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			// If config exists, load file
			JsonFileLoader<ZenTrees_Load>.JsonLoadFile(zenModFolder + zenConfigName, this);
			return;
		}

		// Create config
		Save();
	};

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenTrees_Load>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Clears any "respawned" trees (ie. trees that have exceeded their timestamp)
	void ClearRespawnedTrees()
	{
		int timestamp = JMDate.Now(false).GetTimestamp();
		ZenTreeState treeState;

		for (int i = CutTrees.Count() - 1; i >= 0; i--)
		{
			treeState = CutTrees.Get(i);

			// Check if today's timestamp is greater than future renew timestamp, if so then respawn tree by removing it
			if (timestamp > treeState.RenewTime)
				CutTrees.Remove(i);
		}

		// Save any changes to config file
		Save();
	}

	// Deletes this config from memory as it's no longer needed
	void Delete()
	{
		CutTrees.Clear();
		delete CutTrees;
		delete m_ZenTrees_Load;
	}
}

// Save config data
ref ZenTrees_Load m_ZenTrees_Load;

// Helper function to return Config data storage object
static ZenTrees_Load GetZenTrees_Load()
{
	if (!m_ZenTrees_Load)
	{
		ZMPrint("[ZenTrees_Load] Init");
		m_ZenTrees_Load = new ZenTrees_Load;
		m_ZenTrees_Load.Load();
	}

	return m_ZenTrees_Load;
};