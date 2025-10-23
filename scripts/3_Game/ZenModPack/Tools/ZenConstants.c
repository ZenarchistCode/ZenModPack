enum ZenRPCs
{
	RECEIVE_GRAVE_RPC = 33761990,
	AUTORUN,
	PLAY_SOUND,
	SPLIT_ITEM
};

enum ZenMenus
{
	PLAYER_MESSAGE = 43761990,
	SPLIT_GUI
};

class Zen 
{
	static bool CHECKED_WINTER = false;
	static bool IS_WINTER = false;
	
	static bool CHECKED_LATE_WINTER = false;
	static bool IS_LATE_WINTER = false;
	
	static bool IsWinter(bool countFall = true)
	{
		if (!CHECKED_WINTER)
		{
			if (!GetGame())
				return false;
			
			int mod_count = GetGame().ConfigGetChildrenCount("CfgMods");

	        for (int i = 0; i < mod_count; i++)
	        {
	            string mod_name;
	            GetGame().ConfigGetChildName("CfgMods", i, mod_name);
				
				Print("MODNAME: " + mod_name);
	
	            if (mod_name == "WinterChernarusV2" || mod_name == "WinterLivoniaWorld")
	            {
	                IS_WINTER = true;
					break;
	            }
	        }
	
	        ZMPrint("[WinterCheck] IS_WINTER=" + IS_WINTER);
			CHECKED_WINTER = true;
		}
		
		if (!CHECKED_LATE_WINTER && countFall)
		{
			IsLateWinter();
		}

		return IS_WINTER || (countFall && IS_LATE_WINTER);
	}
	
	static bool IsLateWinter()
	{
		if (!CHECKED_LATE_WINTER)
		{
			if (!GetGame())
				return false;
			
			int mod_count = GetGame().ConfigGetChildrenCount("CfgMods");

	        for (int i = 0; i < mod_count; i++)
	        {
	            string mod_name;
	            GetGame().ConfigGetChildName("CfgMods", i, mod_name);

				if (mod_name.Contains("earlywinter"))
				{
					IS_LATE_WINTER = true;
					break;
				}
	        }
	
	        ZMPrint("[WinterCheck] IS_LATE_WINTER=" + IS_LATE_WINTER);
			CHECKED_LATE_WINTER = true;
		}
		
		return IS_LATE_WINTER;
	}
	
	static bool IsWinterOrFall()
	{
		return IsWinter() || IsLateWinter();
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
}