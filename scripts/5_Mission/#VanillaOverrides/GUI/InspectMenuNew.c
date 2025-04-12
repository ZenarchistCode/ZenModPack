modded class InspectMenuNew
{
	// I made my alcohol items inherit from Vodka to make it much easier to be compatible with any alcohol mods out there
	// but the whisky items just change the name from VODKA to WHISKY visually (liquid type is still vodka)
	override void SetItem(EntityAI item)
	{
		super.SetItem(item);

		InspectMenuNew.ZenUpdateItemInfo(layoutRoot, item);
	}

	//! Unfortunately because these are static functions that can't call super() this complicated workaround is the only way to not break future vanilla changes
	static void ZenUpdateItemInfo(Widget root_widget, EntityAI item)
	{
		if (!root_widget || !item) 
			return;

		UpdateItemInfo(root_widget, item);

		string zenLiquid = "CfgVehicles " + item.GetType() + " zenLiquidName";
		if (GetGame().ConfigIsExisting(zenLiquid))
		{
			string zenLiquidText = "";
			GetGame().ConfigGetText(zenLiquid, zenLiquidText);

			if (zenLiquidText != "")
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", zenLiquidText, Colors.COLOR_LIQUID);
		}
	}

	static void ZenUpdateItemInfoQuantity(Widget root_widget, EntityAI item)
	{
		ItemBase item_base = ItemBase.Cast(item);
		if (!item_base)
			return;

		float item_quantity = item_base.GetQuantity();
		int max_quantity = item.GetQuantityMax();

		if (max_quantity > 0 && !item.IsInherited(ClothingBase))
		{
			//! ZEN RAID ALARM QTY
			if (item.ConfigGetString("stackedUnit") == "time")
			{
				string time = ZenFunctions.GetTimeToString(item_quantity, true, true, false);
				WidgetTrySetText(root_widget, "ItemQuantityWidget", time, Colors.COLOR_DEFAULT);
			}
		}
	}
}