class ZenPollConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\Utilities\\";
	private const static string zenConfigName = "Poll.json";

	static const int MAX_OPTIONS = 6;
	static const string CURRENT_VERSION = "1";

	string CONFIG_VERSION = "1";

	int PollID = -1;
	string PollTitle = "This is an example poll";
	string PollSubtitle = "More explanation in the smaller subtitle";
	bool OnlyOneOption = true;
	string Option1 = "";
	string Option2 = "";
	string Option3 = "";
	string Option4 = "";
	string Option5 = "";
	string Option6 = "";
	string XXXXXXX = "POLL RESULTS:";
	int TotalVotes = 0;
	ref array<ref ZenPollResults> RESULTS = new array<ref ZenPollResults>;
	ref map<string, ref ZenPollVote> PlayerChoices; // playeruid, votes
	int LastPollID = -1;

	// Load config file or create default file if config doesn't exist
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			// If config exists, load file
			JsonFileLoader<ZenPollConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				CONFIG_VERSION = CURRENT_VERSION;
			}
			else
			{
				// If we have a new poll, reset the votes
				if (PollID != LastPollID)
				{
					RESULTS.Clear();
					PlayerChoices.Clear();
					LastPollID = PollID;
					TotalVotes = 0;
					Save();
				}

				return;
			}
		}

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

		ZenPollConfig OldFile;
		JsonFileLoader<ZenPollConfig>.JsonLoadFile(zenModFolder + zenConfigName, OldFile);

		// Save JSON config
		if (OldFile)
		{
			// This allows editing the poll while server is running, because the poll is saved on server shutdown
			PollID = OldFile.PollID;
			PollTitle = OldFile.PollTitle;
			PollSubtitle = OldFile.PollSubtitle;
		}

		XXXXXXX = "POLL RESULTS:";
		JsonFileLoader<ZenPollConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	void UpdateResults()
	{
		if (PollID == -1)
			return;

		int i;
		int j;
		TotalVotes = PlayerChoices.Count();
		RESULTS.Clear();

		array<string> Options = new array<string>;
		Options.Insert(Option1);
		Options.Insert(Option2);
		Options.Insert(Option3);
		Options.Insert(Option4);
		Options.Insert(Option5);
		Options.Insert(Option6);

		// Tally votes 
		for (i = 0; i < MAX_OPTIONS; i++)
		{
			ZenPollResults result = new ZenPollResults;
			result.Option = Options.Get(i);
			result.VoteCount = 0;

			for (j = 0; j < PlayerChoices.Count(); j++)
			{
				result.VoteCount += PlayerChoices.GetElement(j).Choice.Get(i);
			}

			RESULTS.Insert(result);
		}

		// Anaylze win rate
		float percent;
		for (i = 0; i < RESULTS.Count(); i++)
		{
			percent = 0;
			if (RESULTS.Get(i).VoteCount > 0)
				percent = RESULTS.Get(i).VoteCount / TotalVotes;

			RESULTS.Get(i).PercentOfVote = percent * 100;
		}

		// Sort votes by winning
		int min_idx;
		for (i = 0; i < RESULTS.Count() - 1; i++) 
		{
			min_idx = i;
			for (j = i + 1; j < RESULTS.Count(); j++) 
			{
				ZenPollResults record1 = RESULTS.Get(min_idx);
				ZenPollResults record2 = RESULTS.Get(j);

				if (record2.PercentOfVote > record1.PercentOfVote)
					min_idx = j;
			}

			RESULTS.SwapItems(min_idx, i);
		}

		// Delete any blank results for blank options
		for (i = RESULTS.Count() - 1; i >= 0; i--)
		{
			if (RESULTS.Get(i).Option == "")
				RESULTS.Remove(i);
		}

		Save();
	}

	void RegisterVote(string puid, array<bool> votes)
	{
		PlayerChoices.Set(puid, new ZenPollVote(votes));
	}
}

class ZenPollVote
{
	ref array<bool> Choice;

	void ZenPollVote(array<bool> choices)
	{
		Choice = new array<bool>;

		for (int i = 0; i < ZenPollConfig.MAX_OPTIONS; i++)
		{
			Choice.Insert(choices.Get(i));
		}
	}
}

class ZenPollResults
{
	string Option;
	int VoteCount;
	int PercentOfVote;
}

// Save config data
static ref ZenPollConfig m_ZenPollConfig;

// Helper function to return Config data storage object
static ZenPollConfig GetZenPollConfig()
{
	if (!m_ZenPollConfig)
	{
		m_ZenPollConfig = new ZenPollConfig;
		m_ZenPollConfig.Load();
	}

	return m_ZenPollConfig;
};
