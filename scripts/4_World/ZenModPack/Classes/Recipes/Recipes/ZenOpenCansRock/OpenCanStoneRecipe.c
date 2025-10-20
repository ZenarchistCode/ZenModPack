class OpenCanStoneRecipe extends RecipeBase
{
	override void Init()
	{
		m_Name = "#STR_OpenCan0";
		m_IsInstaRecipe = false;//should this recipe be performed instantly without animation
		m_AnimationLength = 1;//animation length in relative time units
		m_Specialty = 0.01;// value > 0 for roughness, value < 0 for precision

		m_AnywhereInInventory = false;//is this recipe valid even when neither of the items is in hands
		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = 3;//-1 = disable check

		m_MinQuantityIngredient[0] = -1;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check

		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = 3;//-1 = disable check

		m_MinQuantityIngredient[1] = -1;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------

		//INGREDIENTS
		//ingredient 1
		InsertIngredientEx(0, "BakedBeansCan", "openBeanCan");//you can insert multiple ingredients this way
		InsertIngredientEx(0, "PeachesCan", "openBeanCan");//you can insert multiple ingredients this way
		InsertIngredientEx(0, "TacticalBaconCan", "openBeanCan");//you can insert multiple ingredients this way
		InsertIngredientEx(0, "SpaghettiCan", "openBeanCan");//you can insert multiple ingredients this way
		InsertIngredientEx(0, "SardinesCan", "openBeanCan");//you can insert multiple ingredients this way
		InsertIngredientEx(0, "UnknownFoodCan", "openBeanCan");//you can insert multiple ingredients this way

		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = 1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		m_IngredientUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this ingredient

		//ingredient 2
		InsertIngredient(1, "Stone");
		InsertIngredient(1, "SmallStone");

		m_IngredientAddHealth[1] = -0.5;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		m_IngredientUseSoftSkills[1] = true;// set 'true' to allow modification of the values by softskills on this ingredient
		//----------------------------------------------------------------------------------------------------------------------

		//result1
		//AddResult("");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = 0;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
		m_ResultUseSoftSkills[0] = false;// set 'true' to allow modification of the values by softskills on this result
		m_ResultReplacesIngredient[0] = -1;// value == -1 means do nothing; a value >= 0 means this result will transfer item propertiesvariables, attachments etc.. from an ingredient value

		//----------------------------------------------------------------------------------------------------------------------
	}

	override bool CanDo(ItemBase ingredients[], PlayerBase player)//final check for recipe's validity
	{
		if ( player.IsPlacingLocal() )
			return false;
		
		return true;
	}

	override void Do(ItemBase ingredients[], PlayerBase player,array<ItemBase> results, float specialty_weight)//gets called upon recipe's completion
	{
		ItemBase can;
		Class.CastTo(can, ingredients[0]);
		ItemBase tool;
		Class.CastTo(tool, ingredients[1]);
		PlayerBase playerPB;
		Class.CastTo(playerPB, player);
		can.SetHealth(1);
		OpenItem.OpenAndSwitch(tool, can, playerPB, specialty_weight);
	}
};