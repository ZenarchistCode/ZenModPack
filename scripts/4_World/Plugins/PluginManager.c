modded class PluginManager
{
	override void Init()
	{
		super.Init();

		if (ZenModEnabled("ZenWeatherConfig"))
			RegisterPlugin("PluginZenWeatherConfig", false, true);
	}
}