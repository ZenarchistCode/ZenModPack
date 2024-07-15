modded class Cooking
{
	//! LEFTOVERS
	override protected void AddTemperatureToItem(ItemBase cooked_item, ItemBase cooking_equipment, float min_temperature)
	{
		if (HandleEmptyCanHeat(cooked_item, cooking_equipment, min_temperature))
			return;

		super.AddTemperatureToItem(cooked_item, cooking_equipment, min_temperature);
	}

	// Since empty can is not an item we cook in like a Pot etc, it needs to be handled differently when adding temperature or it doesn't work properly
	bool HandleEmptyCanHeat(ItemBase cooked_item, ItemBase cooking_equipment, float min_temperature)
	{
		if (cooked_item.IsInherited(Empty_Can_Opened))
		{
			float item_temperature = cooked_item.GetTemperature() * 1.05;
			if (item_temperature > cooked_item.GetTemperatureMax())
				item_temperature = cooked_item.GetTemperatureMax();

			cooked_item.SetTemperature(item_temperature);

			if (item_temperature > 100) // vanilla is 150 degrees but 100 degrees is hot enough to kill most bacteria, and cans damage much faster than other items if left on fire
			{
				cooked_item.RemoveAllAgents();
				
				if (cooked_item.GetQuantity() > 0)
				{
					cooked_item.AddQuantity(-LIQUID_VAPOR_QUANTITY);
				};
			}

			return true;
		}

		return false;
	}
}