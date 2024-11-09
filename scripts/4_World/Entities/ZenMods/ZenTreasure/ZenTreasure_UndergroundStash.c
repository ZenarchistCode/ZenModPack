class ZenTreasure_UndergroundStash extends UndergroundStash
{
	protected Object m_ClutterCutter;

	void ~ZenTreasure_UndergroundStash()
	{
		if (GetGame() && m_ClutterCutter)
			GetGame().ObjectDelete(m_ClutterCutter);
	}

	override void EEInit()
	{
		super.EEInit();

		if (!m_ClutterCutter && !ZenTreasure_PhotoBase.IsWinterPhoto())
			m_ClutterCutter = GetGame().CreateObjectEx("MediumTentClutterCutter", GetPosition(), ECE_PLACE_ON_SURFACE);
	}
}

// For legacy purposes when WinterChernarusV2 had a different stash type. No longer used.
class ZenTreasure_UndergroundStashWinter extends ZenTreasure_UndergroundStash {};