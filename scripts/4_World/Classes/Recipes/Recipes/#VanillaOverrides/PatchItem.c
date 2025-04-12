modded class PatchItem extends RecipeBase
{	
	override void Init()
	{
		super.Init();

		InsertIngredient(0, "ZenSwissKnife_Scissors");
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)
	{
		super.Do(ingredients, player, results, specialty_weight);

		if (ingredients[0] && ingredients[0].IsInherited(ZenSwissKnife_Scissors))
		{
			// No quantity in scissors: do damage instead 
			ingredients[0].AddHealth(ingredients[0].ConfigGetFloat("sewingDamage") * -1);
		}
	}
}