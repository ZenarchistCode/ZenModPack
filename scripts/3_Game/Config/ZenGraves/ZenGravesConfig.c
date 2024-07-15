class ZenGravesConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Static month array
	static const ref array<string> MONTH =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	// Config location
	const static string PROFILE_FOLDER = "$profile:\\Zenarchist\\";
	const static string CONFIG_FILE = "ZenGravesConfig.json";
	static const string LOG_FOLDER = "$profile:\\Zenarchist\\Logs\\";
	static const string LOG_FILE = "ZenGraves.log";
	static const string LOG_FILEPATH = LOG_FOLDER + LOG_FILE;

	// Main config data
	string ConfigVersion = "";
	bool DebugOn;
	int DateFormat;
	int MinPlayerAgeMinutes;
	float MinDistanceBetweenCrosses;
	bool ShowPlayerInfoOnCross;
	bool ShowPlayerName;
	bool ShowPlayerAge;
	bool BurySkeleton;
	bool DeleteCrossOnDig;
	float ChanceOfGrave;
	float ChanceOfSkeletonClothing;
	float ChanceOfSkeletonItems;
	float SkeletonItemHealthModifier;
	string Prefix;
	string UnknownPerson;
	string Description;
	string AgeText;
	string Minutes;
	string Days;
	string Weeks;
	string Hours;
	string CauseOfDeath;
	bool ShowCauseOfDeath = true;
	ref array<string> IgnoreSteamIDs;
	ref array<ref ZenNoSpawnCrossZone> NoCrossZones;
	ref array<ref ZenGravesCauseOfDeath> CrossCauseOfDeath;
	ref array<ref ZenDeadPlayerData> DeadPlayers;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(PROFILE_FOLDER + CONFIG_FILE))
		{ 
			JsonFileLoader<ZenGravesConfig>.JsonLoadFile(PROFILE_FOLDER + CONFIG_FILE, this);

			if (ConfigVersion != CONFIG_VERSION)
			{
				// If version mismatch, backup old version of json before replacing it
				JsonFileLoader<ZenGravesConfig>.JsonSaveFile(PROFILE_FOLDER + CONFIG_FILE + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		ConfigVersion = CONFIG_VERSION;
		IgnoreSteamIDs = new array<string>;
		IgnoreSteamIDs.Clear();
		IgnoreSteamIDs.Insert("xxx");
		DeadPlayers = new array <ref ZenDeadPlayerData>;
		DeadPlayers.Clear();

		// Set default text values
		Prefix = "RIP";
		UnknownPerson = "unknown";
		Description = "Here lies #name, who died #date #age";
		AgeText = "aged";
		Minutes = "minutes,";
		Days = "days,";
		Weeks = "weeks,";
		Hours = "hours,";
		CauseOfDeath = "from";

		// Set default number values
		DebugOn = false;
		DateFormat = 6;
		MinPlayerAgeMinutes = 240; // 4 hour minimum life
		MinDistanceBetweenCrosses = 10; // 10 meters
		ShowPlayerInfoOnCross = true;
		ShowPlayerName = true;
		ShowPlayerAge = true;
		BurySkeleton = true;
		DeleteCrossOnDig = true;
		ChanceOfGrave = 0.9; // 90% chance of grave spawning if dead player meets all the requirements to spawn a grave
		ChanceOfSkeletonClothing = 0.9; // 90% chance of any non-ruined clothing being kept on skeleton
		ChanceOfSkeletonItems = 0.33; // 33% chance of an item being kept in the grave
		SkeletonItemHealthModifier = 0.5; // 50% of current item health

		// Set default cause of death messages
		CrossCauseOfDeath = new array<ref ZenGravesCauseOfDeath>;
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("unknown", "an unknown cause."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("falldamage", "falling from a great height."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("melee", "some kind of blunt force trauma."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("zombie", "injuries inflicted by the infected."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("bullet", "a gunshot wound."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("explosion", "an explosion."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("grenade", "an explosion."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("landmine", "an explosion."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("transport", "a vehicle collision."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("wolf", "an animal attack."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("bear", "an animal attack."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("fire", "third-degree burns."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("bleeding", "bleeding out."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("hunger", "starvation."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("thirst", "dehydration."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("woundinfection", "wound infection."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("contamination", "toxic contamination."));
		CrossCauseOfDeath.Insert(new ZenGravesCauseOfDeath("suicide", "suicide."));

		// Set default no cross spawn zones
		NoCrossZones = new array<ref ZenNoSpawnCrossZone>;
		NoCrossZones.Insert(new ZenNoSpawnCrossZone("Example", "0 0 0", 1));

		// Save default config.
		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(PROFILE_FOLDER))
			MakeDirectory(PROFILE_FOLDER);

		JsonFileLoader<ZenGravesConfig>.JsonSaveFile(PROFILE_FOLDER + CONFIG_FILE, this);
	}

	// Get readable date (taken from ZenNotes)
	string GetDate(int p_dateFormat = -1, int day = 1, int month = 1, int year = 1)
	{
		if (p_dateFormat == -1)
			p_dateFormat = DateFormat;

		// 0 = no date.
		if (p_dateFormat == 0)
		{
			return "";
		}

		// Prepare date strings
		string date;
		string dayNumber;
		string monthNumber;
		string dayStr;
		string monthStr;

		// Get day number (eg. convert 07 -> 7)
		dayNumber = day.ToStringLen(2);
		if (day <= 9)
			dayNumber = day.ToStringLen(1);

		// Get month number (eg. convert 07 -> 7)
		monthNumber = month.ToStringLen(2);
		if (month <= 9)
			monthNumber = month.ToStringLen(1);

		if (p_dateFormat == 1) // eg. 23rd September, 2022
		{
			// Get formatted date 
			dayStr = GetNumberDateyThingy(day);
			monthStr = GetMonth(month);
			date = dayNumber + dayStr + " " + monthStr + ", " + year.ToStringLen(4);
		}
		else
		if (p_dateFormat == 2) // eg. 23/09/2022
		{
			date = day.ToStringLen(2) + "/" + month.ToStringLen(2) + "/" + year.ToStringLen(4);
		}
		else
		if (p_dateFormat == 3) // eg. 3/6/2022
		{
			date = dayNumber + "/" + monthNumber + "/" + year.ToStringLen(4);
		}
		else
		if (p_dateFormat == 4) // eg. 09/23/22
		{
			date = month.ToStringLen(2) + "/" + day.ToStringLen(2) + "/" + year.ToStringLen(4);
		}
		else
		if (p_dateFormat == 5) // eg. 12/9/22
		{
			date = monthNumber + "/" + dayNumber + "/" + year.ToStringLen(4);
		}
		else
		if (p_dateFormat == 6) // eg. 23rd September
		{
			// Get formatted date 
			dayStr = GetNumberDateyThingy(day);
			monthStr = GetMonth(month);
			date = dayNumber + dayStr + " " + monthStr;
		}

		return date;
	}

	// Returns the suffix of a date number (for lack of a more educated term)
	string GetNumberDateyThingy(int number)
	{
		switch (number)
		{
			case 1:
			case 21:
			case 31:
				return "st"; // 1st
			case 2:
			case 22:
				return "nd"; // 2nd
			case 3:
			case 23:
				return "rd"; // 3rd
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
				return "th"; // 4th
		}

		return "";
	}

	// Gets the month as a string
	string GetMonth(int month)
	{
		if (month < 1 || month > 12)
		{
			int year, monthToday, day;
			GetYearMonthDay(year, monthToday, day);
			return MONTH[monthToday - 1];
		}

		return MONTH[month - 1];
	}

	// Get a cause of death from config list
	ZenGravesCauseOfDeath GetCauseOfDeath(string ammoType)
	{
		ammoType.ToLower();
		for (int i = 0; i < CrossCauseOfDeath.Count(); i++)
		{
			string toFind = CrossCauseOfDeath.Get(i).AmmoType;
			toFind.ToLower();
			if (ammoType == toFind)
			{
				return CrossCauseOfDeath.Get(i);
			}
		}

		return new ZenGravesCauseOfDeath();
	}

	// Debug print logs
	void DebugMsg(string s)
	{
		ZenModLogger.Log(s, "ZenGraves");
	}
}

// Define a cause of death
class ZenGravesCauseOfDeath
{
	string AmmoType = "";
	string CauseMessage = "";
	bool SpawnCross = true;

	void ZenGravesCauseOfDeath(string pAmmoType = "", string pCauseMessage = "")
	{
		AmmoType = pAmmoType;
		CauseMessage = pCauseMessage;
	}
};

// Define the date format
class ZenGravesDateFormat
{
	int Format;
	ref array<string> DaySuffix;
	ref array<string> MonthName;
};

// Define a dead player cross
class ZenDeadPlayerData
{
	string SteamID;
	string Name;
	string CauseOfDeath;
	vector Position;
	vector Orientation;
	int AgeMinutes;
	int Day;
	int Month;
	int Year;
	ref array<ref ZenGraves_InventoryItem> SkeletonItems;

	void ZenDeadPlayerData(string p_id, string p_name, string p_cod, vector p_position, vector p_orientation, int p_age, int p_day, int p_month, int p_year)
	{
		SteamID = p_id;
		Name = p_name;
		CauseOfDeath = p_cod;
		Position = p_position;
		Orientation = p_orientation;
		AgeMinutes = p_age;
		Day = p_day;
		Month = p_month;
		Year = p_year;
		SkeletonItems = new array<ref ZenGraves_InventoryItem>;
	}
};

class ZenGraves_InventoryItem
{
	string Type;
	int Quantity;
	float Health;
	bool Attachment;

	void ZenGraves_InventoryItem(string p_type, int p_quantity, float p_health, bool p_attachment = false)
	{
		Type = p_type;
		Quantity = p_quantity;
		Health = p_health;
		Attachment = p_attachment;
	}
};

class ZenNoSpawnCrossZone
{
	string Name;
	vector Position;
	int Distance;

	void ZenNoSpawnCrossZone(string p_name, vector p_pos, int p_dist)
	{
		Name = p_name;
		Position = p_pos;
		Distance = p_dist;
	}
}

ref ZenGravesConfig m_ZenGravesConfig;

static ZenGravesConfig GetZenGravesConfig()
{
	if (!m_ZenGravesConfig)
	{
		Print("[ZenGravesConfig] Init");
		m_ZenGravesConfig = new ZenGravesConfig;
		m_ZenGravesConfig.Load();
	}

	return m_ZenGravesConfig;
};