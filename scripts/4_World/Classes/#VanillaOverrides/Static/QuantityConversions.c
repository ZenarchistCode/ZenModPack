modded class QuantityConversions
{
	override static int HasItemQuantity(notnull EntityAI item)
	{
		if (!GetZenModPackClientConfig().MagObfuscation)
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

	// Last copied from vanilla scripts on 8th July 2025
	static int Vanilla_HasItemQuantity( notnull EntityAI item )
	{
		ItemBase ib;
		if ( Class.CastTo(ib, item) )
		{
			if ( item.IsMagazine() )
				return QUANTITY_COUNT;
			
			if ( !ib.m_CanShowQuantity )
				return QUANTITY_HIDDEN;
			
			int max = item.GetQuantityMax();
			if ( max > 0 )
			{
				if ( ib.m_HasQuantityBar )
				{
					return QUANTITY_PROGRESS;
				}
				else
				{
					if (max == 1)
					{
						return QUANTITY_HIDDEN;
					}
					else
					{
						return QUANTITY_COUNT;
					}
				}
			}
		}
		return QUANTITY_HIDDEN;
	}
}
