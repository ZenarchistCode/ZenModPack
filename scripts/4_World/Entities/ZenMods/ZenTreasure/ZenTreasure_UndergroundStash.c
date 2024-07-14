class ZenTreasure_UndergroundStash extends UndergroundStash
{
	protected Object m_ClutterCutter;

	void ~ZenTreasure_UndergroundStash()
	{
		if (GetGame() && m_ClutterCutter)
			GetGame().ObjectDelete(m_ClutterCutter);
	}

	// If winter, convert to WinterStash
	override void EEInit()
	{
		super.EEInit();

		if (!m_ClutterCutter && !ZenTreasure_PhotoBase.IsWinterPhoto())
			m_ClutterCutter = GetGame().CreateObjectEx("MediumTentClutterCutter", GetPosition(), ECE_PLACE_ON_SURFACE);

		if (!ZenTreasure_PhotoBase.IsWinterPhoto() || !GetGame().IsDedicatedServer() || GetType() == "ZenTreasure_UndergroundStashWinter")
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShapeshifterMorphToWinter, 1000, false);
	}

	void ShapeshifterMorphToWinter()
	{
		ZenTreasure_UndergroundStashWinter stash = ZenTreasure_UndergroundStashWinter.Cast(GetGame().CreateObject("ZenTreasure_UndergroundStashWinter", GetPosition(), ECE_PLACE_ON_SURFACE));
		if (!stash)
			return;
		
		stash.SetPosition(GetPosition());
		stash.SetOrientation(GetOrientation());
		stash.PlaceOnGround();

		stash.SetLifetimeMax(GetLifetimeMax());
		stash.SetLifetime(GetLifetime());

		ZenFunctions.MoveInventory(this, stash);

		Print("[ZenTreasure] Converted " + GetType() + " into ZenTreasure_UndergroundStashWinter");
	}
}

class ZenTreasure_UndergroundStashWinter extends ZenTreasure_UndergroundStash
{
	// If summer, convert to SummerStash
	override void EEInit()
	{
		super.EEInit();

		if (ZenTreasure_PhotoBase.IsWinterPhoto() || !GetGame().IsDedicatedServer())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShapeshifterMorphToSummer, 1000, false);
	}

	void ShapeshifterMorphToSummer()
	{
		ZenTreasure_UndergroundStash stash = ZenTreasure_UndergroundStash.Cast(GetGame().CreateObject("ZenTreasure_UndergroundStash", GetPosition(), ECE_PLACE_ON_SURFACE));
		if (!stash)
			return;

		stash.SetPosition(GetPosition());
		stash.SetOrientation(GetOrientation());
		stash.PlaceOnGround();

		stash.SetLifetimeMax(GetLifetimeMax());
		stash.SetLifetime(GetLifetime());

		ZenFunctions.MoveInventory(this, stash);

		Print("[ZenTreasure] Converted " + GetType() + " into ZenTreasure_UndergroundStash");
	}
}