//! JAMESON: 
// This is the easiest way I could think of to add a custom liquid type 
// without overriding the static voids and breaking other mods.
modded class ItemManager 
{
	override void PrepareTooltip(EntityAI item, int x = 0, int y = 0)
	{
		super.PrepareTooltip(item, x, y);

		ItemBase ib = ItemBase.Cast(item);
		if (!ib)
			return;

		if (item.IsInherited(ZenJameson))
		{
			InspectMenuNew.ZenWhiskyUpdateItemInfo(m_TooltipWidget, item);
			return;
		}

		int liquid_type = ib.GetLiquidType();
		switch(liquid_type)
		{
			case LIQUID_WATER:	
			case LIQUID_RIVERWATER:	
			case LIQUID_VODKA:
			case LIQUID_BEER:
			case LIQUID_GASOLINE:
			case LIQUID_DIESEL:
			case LIQUID_DISINFECTANT:
			case LIQUID_SALINE:
				return;
		}
	}
}