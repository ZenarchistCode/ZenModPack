modded class DayZGame
{
	void DayZGame()
	{
		Print("[ZenModPack::DayZGame] Init");
	}
	
	override void DeferredInit()
	{
		super.DeferredInit();
		
		Print("[ZenModPack::DayZGame] Loading master config.");
		GetZenModPackConfig();
	}
}