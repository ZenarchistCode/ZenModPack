modded class QuantityConversions
{
	override static int HasItemQuantity(notnull EntityAI item)
	{
		if (!ZenModEnabled("ZenMagObfuscation"))
		{
			return Vanilla_HasItemQuantity(item);
		}

		if (item.IsInherited(InventoryItem))
		{
			if (item.IsInherited(Magazine))
			{
				if (item.ConfigIsExisting("iconCartridge"))
				{
					return QUANTITY_COUNT;
				}
				else
				{
					return QUANTITY_PROGRESS;
				}
			}

			bool show_quantity = item.ConfigGetBool("quantityShow");
			if (!show_quantity)
			{
				return QUANTITY_HIDDEN;
			}

			int max = item.ConfigGetInt("varQuantityMax");
			bool bar = item.ConfigGetBool("quantityBar");
			if (max > 0)
			{
				if (bar)
				{
					return QUANTITY_PROGRESS;
				}
				else
				{
					return QUANTITY_COUNT;
				}
			}
		}

		return QUANTITY_HIDDEN;
	}

	// Last copied from 1.30 vanilla scripts on 18th September 2026
	static int Vanilla_HasItemQuantity(notnull EntityAI item)
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item))
		{
			if (item.IsMagazine())
				return QUANTITY_COUNT;

			ItemBaseType itemBaseType = ItemBaseType.Cast(itemBase.GetEntityType());

			if (!itemBaseType.m_CanShowQuantity)
				return QUANTITY_HIDDEN;

			int maxQuantity = item.GetQuantityMax();
			if (maxQuantity > 0)
			{
				if (itemBaseType.m_HasQuantityBar)
				{
					return QUANTITY_PROGRESS;
				}

				if (maxQuantity == 1)
					return QUANTITY_HIDDEN;

				return QUANTITY_COUNT;
			}
		}

		return QUANTITY_HIDDEN;
	}
}
