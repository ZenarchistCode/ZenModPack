modded class RecipeBase
{
	//! CRAFTING SOUNDS 

	// Hi-jack ingredient sound here whenever appropriate to cover multiple recipes
	// and to allow compatability with future vanilla updates (InsertIngredientEx() will be used directly to add sounds and this would be ignored)
	override void InsertIngredient(int index, string ingredient, DayZPlayerConstants uid = BASE_CRAFT_ANIMATION_ID, bool showItem = false)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
		{
			super.InsertIngredient(index, ingredient, uid, showItem);
			return;
		}

		switch (ingredient)
		{
			case "SewingKit":
				InsertIngredientEx(index, ingredient, "Zen_SewingKit", uid, showItem);
				return;
			case "LeatherSewingKit":
				InsertIngredientEx(index, ingredient, "Zen_LeatherSewingKit", uid, showItem);
				return;
			case "Whetstone":
				InsertIngredientEx(index, ingredient, "Zen_SharpenMelee", uid, showItem);
				return;
			case "WeaponCleaningKit":
				InsertIngredientEx(index, ingredient, "Zen_CleanWeapon", uid, showItem);
				return;
			case "DisinfectantAlcohol":
			case "IodineTincture":
			case "Syringe":
			case "SyringeEmpty":
			case "SyringeFull":
			case "AmpouleAlcohol":
			case "SyberiaMedicineAmpoule":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectBottle", uid, showItem);
				return;
			case "DisinfectantSpray":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectSpray", uid, showItem);
				return;
			case "DuctTape":
				InsertIngredientEx(index, ingredient, "Zen_DuctTape", uid, showItem);
				return;
			case "Spraycan_ColorBase":
			case "Zen_Dye_Black":
			case "Zen_Dye_Green":
				InsertIngredientEx(index, ingredient, "Zen_SprayPaint", uid, showItem);
				return;
			case "TireRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_Rubber", uid, showItem);
				return;
			case "SmallStone":
			case "Stone":
				InsertIngredientEx(index, ingredient, "Zen_BreakStone", uid, showItem);
				return;
			case "BarbedWire":
				InsertIngredientEx(index, ingredient, "Zen_MetalWire", uid, showItem);
				return;
			case "WoodenPlank":
				InsertIngredientEx(index, ingredient, "Zen_Wood", uid, showItem);
				return;
			case "Nail":
				InsertIngredientEx(index, ingredient, "Zen_Nails", uid, showItem);
				return;
			case "Flag_Base":
			case "Armband_ColorBase":
			case "Raincoat_ColorBase":
				InsertIngredientEx(index, ingredient, "Rags_craft", uid, showItem);
				return;
			case "ElectronicRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_ElectronicRepair", uid, showItem);
				return;
			case "Paper":
				InsertIngredientEx(index, ingredient, "Zen_Paper", uid, showItem);
				return;
			default:
				break;
		}

		InsertIngredientEx(index, ingredient, "", uid, showItem);
	}
}
