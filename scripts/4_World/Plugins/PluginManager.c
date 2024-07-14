modded class PluginManager
{
	override void Init()
	{
		super.Init();

		#ifdef SERVER
		if (ZenModEnabled("ZenWeatherConfig"))
			RegisterPlugin("PluginZenWeatherConfig", false, true);
		#endif
	}
}