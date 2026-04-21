class ZenFlask extends Bottle_Base 
{
	override void OnQuantityChanged(float delta)
	{
		super.OnQuantityChanged(delta);
		
		if (GetQuantityNormalized() < 0.005)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenShapeshifterMorph, 1, false);
		}
	}
	
	void ZenShapeshifterMorph()
	{
		GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ZenReplaceAlcohol(this, "Empty_" + GetType()));
	}
};

class Empty_ZenFlask extends Bottle_Base {};