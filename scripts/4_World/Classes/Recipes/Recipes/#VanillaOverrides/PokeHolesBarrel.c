modded class PokeHolesBarrel
{
	override bool CanDo(ItemBase ingredients[], PlayerBase player)
	{
		if (!super.CanDo(ingredients, player))
			return false;

		if (ingredients[0].GetType().Contains("Zen_Community"))
			return false; // Don't allow community barrels to be manipulated

		return true;
	}
}