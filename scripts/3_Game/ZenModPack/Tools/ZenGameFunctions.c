class ZenGameFunctions
{
	static bool IsWinter(bool countLateOrEarlyWinter = true)
	{
		if (IsDeepWinter())
			return true;

		if (countLateOrEarlyWinter)
			return IsLateOrEarlyWinter();

		return false;
	}

	static bool IsDeepWinter()
	{
		#ifdef WinterChernarusV2
		return true;
		#endif

		#ifdef WinterLivoniaWorld
		return true;
		#endif

		return false;
	}

	static bool IsLateOrEarlyWinter()
	{
		#ifdef earlywinter_scripts
		return true;
		#endif 

		#ifdef earlywinter_livonia_scripts
		return true;
		#endif

		return false;
	}

	// For event-related auto-code.
	static bool IsChristmas()
	{
		int year, month, day;
		GetYearMonthDay(year, month, day);
		return month == 12 && day >= 24 && day <= 25;
	}

	static bool IsEaster()
	{
		int year, month, day;
		GetYearMonthDay(year, month, day);
		return month == 4 && day >= 5 && day <= 6;
	}

	static bool IsHalloween()
	{
		int year, month, day;
		GetYearMonthDay(year, month, day);
		return month == 10 && day >= 30 && day <= 31;
	}

	static bool IsAustraliaDay()
	{
		int year, month, day;
		GetYearMonthDay(year, month, day);
		return month == 1 && day >= 25 && day <= 27;
	}

	static void PrintMods() 
	{
		int mod_count = GetGame().ConfigGetChildrenCount("CfgMods");
		string mod_name;

		Print("[ZenModPack] Listing CfgMods:");

		for (int i = 0; i < mod_count; i++)
		{
			GetGame().ConfigGetChildName("CfgMods", i, mod_name);
			Print("#ifdef " + mod_name);
		}
	}
};