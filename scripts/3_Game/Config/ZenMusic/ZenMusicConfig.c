class ZenMusicConfig
{
	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenMusicConfig.json";
	private const static string CURRENT_VERSION = "1";
	string CONFIG_VERSION = "";

	// Cassettes to play on static boombox
	bool AllowCarInventory = true;
	bool StaticBoomboxAutoPlay;
	ref array<string> StaticBoomboxSongs;
	int StaticBoomboxTurnsOffInvisibleRange;
	int StaticBoomboxTurnsOnInvisibleRange;
	bool StaticInvisibleBoomboxAutoPlay;
	ref array<string> StaticInvisibleBoomboxSongs;

	void Load()
	{
		StaticBoomboxSongs = new array<string>;
		StaticInvisibleBoomboxSongs = new array<string>;

		// Don't load JSON on server
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenMusicConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			if (CONFIG_VERSION != CURRENT_VERSION)
			{
				// Mismatch, back up old version
				JsonFileLoader<ZenMusicConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, shuffle array and stop here.
				ShuffleArrays();
				return;
			}
		}

		CONFIG_VERSION = CURRENT_VERSION;

		StaticBoomboxAutoPlay = true;
		StaticInvisibleBoomboxAutoPlay = false;
		StaticBoomboxTurnsOffInvisibleRange = 10;
		StaticBoomboxTurnsOnInvisibleRange = 10;

		StaticBoomboxSongs.Clear();
		StaticBoomboxSongs.Insert("Zen_Cassette_Zen1");

		StaticInvisibleBoomboxSongs.Clear();
		StaticInvisibleBoomboxSongs.Insert("Zen_Cassette_Zen2");

		Save();
	};

	void Save()
	{
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		JsonFileLoader<ZenMusicConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	void ShuffleArrays()
	{
		int i;
		int randomIndexToSwap;
		string temp;

		for (i = 0; i < StaticBoomboxSongs.Count(); i++) 
		{
			randomIndexToSwap = Math.RandomIntInclusive(0, StaticBoomboxSongs.Count() - 1);
			temp = StaticBoomboxSongs[randomIndexToSwap];
			StaticBoomboxSongs[randomIndexToSwap] = StaticBoomboxSongs[i];
			StaticBoomboxSongs[i] = temp;
		}

		//Print("[ZenMusic] Shuffled array: StaticBoomboxSongs - count=" + StaticBoomboxSongs.Count());
		//foreach (string s : StaticBoomboxSongs)
		//	Print(s);

		for (i = 0; i < StaticInvisibleBoomboxSongs.Count(); i++) 
		{
			randomIndexToSwap = Math.RandomIntInclusive(0, StaticInvisibleBoomboxSongs.Count() - 1);
			temp = StaticInvisibleBoomboxSongs[randomIndexToSwap];
			StaticInvisibleBoomboxSongs[randomIndexToSwap] = StaticInvisibleBoomboxSongs[i];
			StaticInvisibleBoomboxSongs[i] = temp;
		}

		//Print("[ZenMusic] Shuffled array: StaticInvisibleBoomboxSongs - count=" + StaticInvisibleBoomboxSongs.Count());
		//foreach (string s2 : StaticInvisibleBoomboxSongs)
		//	Print(s2);
	}
}

ref ZenMusicConfig m_ZenMusicConfig;

static ZenMusicConfig GetZenMusicConfig()
{
	if (!m_ZenMusicConfig)
	{
		m_ZenMusicConfig = new ZenMusicConfig;
		m_ZenMusicConfig.Load();
	}

	return m_ZenMusicConfig;
};