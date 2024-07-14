modded class CraftWoodenPlank
{
	//! SWISS KNIFE
	// Don't allow swiss saw to make planks, just firewood etc
	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		if (ingredients[1] != NULL && ingredients[1].IsInherited(ZenSwissKnife_SmallSaw))
			return false;

		return super.CanDo(ingredients, player);
	}

	//! CRAFTING SOUNDS
	override string GetSoundCategory(int ingredientIndex, ItemBase item)
	{
		return "Zen_SawWood";
	}
};
