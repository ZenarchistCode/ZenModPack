modded class SawWoodenLog extends RecipeBase
{
	//! SWISS KNIFE - Only makes firewood, not planks.
	override void Init()
	{
		super.Init();

		InsertIngredient(1,"ZenSwissKnife_SmallSaw");
	}

	//! CRAFTING SOUNDS
	override string GetSoundCategory(int ingredientIndex, ItemBase item)
	{
		return "Zen_SawWood";
	}
};
