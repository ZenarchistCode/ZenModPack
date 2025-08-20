modded class CraftWoodenPlank extends RecipeBase
{	
	override void Init()
	{
		super.Init();

		#ifdef SERVER
		if (ZenModEnabled("ZenWoodHealth"))
		{
			m_ResultInheritsHealth[0] = -2; // -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		}
		#endif
	}
}