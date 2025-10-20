modded class InspectMenuNew
{
	// I made my alcohol items inherit from Vodka to make it much easier to be compatible with any alcohol mods out there
	// but the whisky items just change the name from VODKA to WHISKY visually (liquid type is still vodka)
	override void SetItem(EntityAI item)
	{
		super.SetItem(item);

		// Override vanilla after call to super.
		InspectMenuNew.ZenUpdateItemInfo(layoutRoot, item);
		InspectMenuNew.ZenUpdateItemInfoQuantity(layoutRoot, item);
		InspectMenuNew.ZenUpdateAmmoInfo(layoutRoot, item);
	}

	//! Unfortunately because these are static functions that can't call super() this complicated workaround is the only way to not break future vanilla changes
	static void ZenUpdateItemInfo(Widget root_widget, EntityAI item)
	{
		if (!root_widget || !item) 
			return;

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
			if (item.ConfigGetString("stackedUnit") == "time")
			{
				string time = ZenFunctions.GetTimeToString(item_quantity, true, true, false);
				WidgetTrySetText(root_widget, "ItemQuantityWidget", time, Colors.COLOR_DEFAULT);
			}
		}
	}

	static void ZenUpdateAmmoInfo(Widget root_widget, EntityAI item)
	{
		if (!GetZenModPackClientConfig().MagObfuscation)
		{
			return;
		}

		Magazine magazine_item = Magazine.Cast(item);
		if (magazine_item)
		{
			float item_quantity = magazine_item.GetAmmoCount();
			int max_quantity = magazine_item.GetAmmoMax();
			float quantity_ratio = Math.Round((item_quantity / max_quantity) * 100);
			string quantity_str = "#inv_inspect_remaining " + quantity_ratio.ToString() + "#inv_inspect_percent";
			WidgetTrySetText(root_widget, "ItemQuantityWidget", quantity_str, Colors.COLOR_DEFAULT);
		}
	}
}