modded class PluginManager
{
	override void Init()
	{
		super.Init();

		// Gotta check if this is server before we check for server-side mods because plugins init on client too and server-side mod configs aren't sync'd
		// so checking ZenModEnabled for server mods prints a nullpointer to error logs on client otherwise.
#ifdef SERVER
		if (GetZenUtilitiesConfig().CountServerFPS)
			RegisterPlugin("ZenPerformanceMonitorPlugin", false, true);

		if (ZenModEnabled("ZenWeatherConfig"))
			RegisterPlugin("ZenWeatherPlugin", false, true);

		if (ZenModEnabled("ZenRaidAlarm"))
			RegisterPlugin("ZenRaidAlarmPlugin", false, true);

		if (ZenModEnabled("ZenCampSites"))
			RegisterPlugin("ZenCampSitesPlugin", false, true);

		if (ZenModEnabled("ZenZombieDoors"))
			RegisterPlugin("ZenZombieDoorsPlugin", false, true);

		if (ZenModEnabled("ZenDynamicZoneLoot"))
		{
			if (GetZenContaminatedZoneLootConfig().ItemLifetime < 0)
				RegisterPlugin("ZenContaminatedItemsPlugin", false, true);
		}

		RegisterPlugin("ZenAdminMessagePlugin", false, true);
#endif
	}
}