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

#ifdef GAMELABS
		_registeredInstanceZenGL = new _Event(GetType(), "poo", this);
		GetGameLabs().RegisterEvent(_registeredInstanceZenGL);
#endif
	}

#ifdef GAMELABS
	ref _Event _registeredInstanceZenGL;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGameLabs())
			return;

		if (_registeredInstanceZenGL)
		{
			GetGameLabs().RemoveEvent(_registeredInstanceZenGL);
		}
	}
#endif
}

// For legacy purposes when WinterChernarusV2 had a different stash type. No longer used.
class ZenTreasure_UndergroundStashWinter extends ZenTreasure_UndergroundStash {};

/*
modded class DigOutStashLambda : DropEquipAndDestroyRootLambda
{
	void DigOutStashLambda(EntityAI old_item, string new_item_type, PlayerBase player) 
	{
		if (player && old_item.IsInherited(ZenTreasure_UndergroundStash))
		{
			// Remove stash trigger for player

		}
	}
}
*/
