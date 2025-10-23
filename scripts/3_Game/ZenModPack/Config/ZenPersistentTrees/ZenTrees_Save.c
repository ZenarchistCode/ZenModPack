// This config stores trees cut down during this play session in memory to save on shutdown (not saved to JSON)
class ZenTrees_Save
{
	// Config data
	ref array<ref ZenTreeState> CutTrees = new array<ref ZenTreeState>;

	// Save tree state
	void SaveTreeState(vector location, float hp)
	{
		// Update existing tree
		int ts = JMDate.Now(false).GetTimestamp() + GetZenTreesConfig().TimeToRespawn;
		foreach (ZenTreeState treeState : CutTrees)
		{
			if (treeState.Location == location)
			{
				treeState.Health = hp;
				treeState.RenewTime = ts;
				return; // Stop function here
			}
		}

		// Save new tree if not found
		ZenTreeState newTreeState = new ZenTreeState(location, ts, hp);
		CutTrees.Insert(newTreeState);
	}

	// Transfer all 'saved' trees to the next server startup's load config
	void TransferSavedTrees()
	{
		foreach (ZenTreeState treeState : CutTrees)
		{
			GetZenTrees_Load().CutTrees.Insert(treeState);
		}

		CutTrees.Clear();
		GetZenTrees_Load().Save();
	}
}

// Save config data
ref ZenTrees_Save m_ZenTrees_Save;

// Helper function to return Config data storage object
static ZenTrees_Save GetZenTrees_Save()
{
	if (!m_ZenTrees_Save)
	{
		ZMPrint("[ZenTrees_Save] Init");
		m_ZenTrees_Save = new ZenTrees_Save;
	}

	return m_ZenTrees_Save;
};