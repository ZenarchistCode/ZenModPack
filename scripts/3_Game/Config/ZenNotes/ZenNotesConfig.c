class ZenNotesConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "2";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenNotesConfig.json";

	// Main config data
	string ConfigVersion = "";
	bool AllowChangingFonts;
	int PenConsumeQuantity;
	ref ZenNoteDateFormat NoteDateFormat;
	ref array<string> WordBlacklist = new array<string>;
	string SendPlayerWarning;

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (FileExist(zenModFolder + zenConfigName))
		{
			JsonFileLoader<ZenNotesConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenNotesConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Clear old config
		WordBlacklist.Clear();

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Set default config
		AllowChangingFonts = true;
		SendPlayerWarning = "We don't tolerate that sort of language on our server. Your note has been logged and any repeat offenses may result in a permanent ban.";
		PenConsumeQuantity = 3;

		// Set default date format
		NoteDateFormat = new ZenNoteDateFormat;
		NoteDateFormat.Format = 6;
		NoteDateFormat.DaySuffix = new array<string>;
		NoteDateFormat.DaySuffix.Insert("st");
		NoteDateFormat.DaySuffix.Insert("nd");
		NoteDateFormat.DaySuffix.Insert("rd");
		NoteDateFormat.DaySuffix.Insert("th");
		NoteDateFormat.MonthName = new array<string>;
		NoteDateFormat.MonthName.Insert("January");
		NoteDateFormat.MonthName.Insert("February");
		NoteDateFormat.MonthName.Insert("March");
		NoteDateFormat.MonthName.Insert("April");
		NoteDateFormat.MonthName.Insert("May");
		NoteDateFormat.MonthName.Insert("June");
		NoteDateFormat.MonthName.Insert("July");
		NoteDateFormat.MonthName.Insert("August");
		NoteDateFormat.MonthName.Insert("September");
		NoteDateFormat.MonthName.Insert("October");
		NoteDateFormat.MonthName.Insert("November");
		NoteDateFormat.MonthName.Insert("December");

		// Set default blacklisted words
		WordBlacklist.Clear();
		WordBlacklist.Insert("REALLY_NASTY_WORDS_THAT");
		WordBlacklist.Insert("YOU_DONT_WANT_WRITTEN");

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{	// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenNotesConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}

	// Check if the given message contains a blacklisted word
	static const string STRIP_CHARS = " /~|!@#$%^&*()_-+=[{]};:'\",<.>/?`~\\";
	bool IsBlacklisted(string msg)
	{
		// Trim whitespace & set msg to lower case
		msg = msg.Trim();
		msg.ToLower();

		// Cycle through all symbol chars and delete them
		for (int i = 0; i < STRIP_CHARS.Length(); i++)
		{
			msg.Replace(STRIP_CHARS.Get(i), "");
		}

		// Cycle through all blacklisted words and check for match
		foreach (string s : WordBlacklist)
		{
			s.ToLower();
			if (msg.Contains(s))
				return true;
		}

		return false;
	}

	// Get readable date
	string GetDate()
	{
		// 0 = no date.
		if (NoteDateFormat.Format == 0)
		{
			return "";
		}

		// Get date as numbers
		int year, month, day;
		GetYearMonthDay(year, month, day);

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

		if (NoteDateFormat.Format == 1) // eg. 23rd September, 2022
		{
			// Get formatted date 
			dayStr = GetNumberDateyThingy(day);
			monthStr = GetMonth(month);
			date = dayNumber + dayStr + " " + monthStr + ", " + year.ToStringLen(4);
		}
		else
		if (NoteDateFormat.Format == 2) // eg. 23/09/2022
		{
			date = day.ToStringLen(2) + "/" + month.ToStringLen(2) + "/" + year.ToStringLen(4);
		}
		else
		if (NoteDateFormat.Format == 3) // eg. 3/6/2022
		{
			date = dayNumber + "/" + monthNumber + "/" + year.ToStringLen(4);
		}
		else
		if (NoteDateFormat.Format == 4) // eg. 09/23/22
		{
			date = month.ToStringLen(2) + "/" + day.ToStringLen(2) + "/" + year.ToStringLen(4);
		}
		else
		if (NoteDateFormat.Format == 5) // eg. 12/9/22
		{
			date = monthNumber + "/" + dayNumber + "/" + year.ToStringLen(4);
		}
		else
		if (NoteDateFormat.Format == 6) // eg. 23rd September
		{
			// Get formatted date 
			dayStr = GetNumberDateyThingy(day);
			monthStr = GetMonth(month);
			date = dayNumber + dayStr + " " + monthStr ;
		}

		return date;
	}

	// Returns the suffix of a date number (for lack of a more educated term)
	string GetNumberDateyThingy(int number)
	{
		// I tried this fancy solution I found on stack overflow but it made 12 look like 12nd instead of 12th... 
		// And I'm not smart enough to think of a better solution than to just hard-code the fucker.
		
		/*switch (number % 10)
		{
			case 1:  return NoteDateFormat.DaySuffix.Get(0);
			case 2:  return NoteDateFormat.DaySuffix.Get(1);
			case 3:  return NoteDateFormat.DaySuffix.Get(2);
			default: return NoteDateFormat.DaySuffix.Get(3);
		}*/

		switch (number)
		{
			case 1:
			case 21:
			case 31:
				return NoteDateFormat.DaySuffix.Get(0); // 1st
			case 2:
			case 22:
				return NoteDateFormat.DaySuffix.Get(1); // 2nd
			case 3:
			case 23:
				return NoteDateFormat.DaySuffix.Get(2); // 3rd
			default:
				return NoteDateFormat.DaySuffix.Get(3); // 4th
		}

		return "";
	}

	// Gets the month as a string
	string GetMonth(int month)
	{
		return NoteDateFormat.MonthName.Get(month - 1);
	}
}

// Define the date format
class ZenNoteDateFormat
{
	int Format;
	ref array<string> DaySuffix;
	ref array<string> MonthName;
}

// Save config data
ref ZenNotesConfig m_ZenNotesConfig;

// Helper function to return Config data storage object
static ZenNotesConfig GetZenNotesConfig()
{
	if (!m_ZenNotesConfig)
	{
		ZMPrint("[ZenNotesConfig] Init");
		m_ZenNotesConfig = new ZenNotesConfig;
		m_ZenNotesConfig.Load();
	}

	return m_ZenNotesConfig;
}