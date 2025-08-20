modded class PluginRecipesManagerBase
{
    override void RegisterRecipies()
    {
        super.RegisterRecipies();

        //! CAMO SHELTER
        RegisterRecipe(new CraftZenCamoShelterKit);

        //! CAMO NET GHILLIE
        RegisterRecipe(new Craft_Zen_CamoNet_Strip);
		RegisterRecipe(new Craft_Zen_CamoNet_Bushrag);
		RegisterRecipe(new Craft_Zen_CamoNet_GhillieAtt);
		RegisterRecipe(new Craft_Zen_CamoNet_Hood);
		RegisterRecipe(new Craft_Zen_CamoNet_Suit);
		RegisterRecipe(new Craft_Zen_CamoNet_Top);

        //! OPEN CANS ON ROCKS 
        RegisterRecipe(new OpenCanStoneRecipe);

        //! LEFTOVERS 
        RegisterRecipe(new CraftJunkHookRecipe);

        //! TIRE RACK 
        RegisterRecipe(new Zen_CraftTireRack);

        //! WORKBENCH 
        RegisterRecipe(new Zen_CraftCarWorkbenchKit);
		RegisterRecipe(new Zen_DeCraftCarWorkbenchKit);

        //! CHESS 
        RegisterRecipe(new Zen_CraftChessBoard);
		RegisterRecipe(new Zen_CraftPawn);
		RegisterRecipe(new Zen_CraftRook);
		RegisterRecipe(new Zen_CraftKnight);
		RegisterRecipe(new Zen_CraftBishop);
		RegisterRecipe(new Zen_CraftKing);
		RegisterRecipe(new Zen_CraftQueen);
    }
}