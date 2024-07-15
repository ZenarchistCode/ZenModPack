class Zen_Cassette_Base extends ItemBase
{
	static int GetSongDuration(string type)
	{
		if (!GetGame())
			return -1;

		string cfgPath = "CfgVehicles " + type;
		return GetGame().ConfigGetInt(cfgPath + " playSeconds");
	}

	static string GetSongSoundSet(string type)
	{
		 return type + "_SoundSet";
	}

	static bool GetCopyrighted(string type)
	{
		if (!GetGame())
			return false;

		string cfgPath = "CfgVehicles " + type;
		return GetGame().ConfigGetInt(cfgPath + " copyrighted") == 1;
	}

	static bool IsMusicTape(string type)
	{
		if (!GetGame())
			return false;

		string cfgPath = "CfgVehicles " + type;
		return GetGame().ConfigGetInt(cfgPath + " isMusic") == 1;
	}
};
