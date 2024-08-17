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
			InspectMenuNew.ZenUpdateItemInfo(m_TooltipWidget, item);
		}

		InspectMenuNew.ZenUpdateItemInfoQuantity(m_TooltipWidget, item);
	}
}