class ZenJameson extends Bottle_Base 
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

class Empty_ZenJameson extends Bottle_Base {};

class ZenReplaceAlcohol : ReplaceItemWithNewLambdaBase
{
	override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		ItemBase newItem;
		Class.CastTo(newItem, new_item);
		
		ItemBase ingredient;
		Class.CastTo(ingredient, old_item);
		
		MiscGameplayFunctions.TransferItemProperties(ingredient, newItem);
		
		newItem.SetQuantity(0);
	}
}