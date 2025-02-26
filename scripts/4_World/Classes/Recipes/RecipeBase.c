modded class RecipeBase
{
	//! CRAFTING SOUNDS 

	// Hi-jack ingredient sound here whenever appropriate to cover multiple recipes
	// and to allow compatability with future vanilla updates (InsertIngredientEx() will be used directly to add sounds and this would be ignored)
	override void InsertIngredient(int index, string ingredient, DayZPlayerConstants uid = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
		{
			super.InsertIngredient(index, ingredient, uid);
			return;
		}

		switch (ingredient)
		{
			case "SewingKit":
				InsertIngredientEx(index, ingredient, "Zen_SewingKit", uid);
				return;
			case "LeatherSewingKit":
				InsertIngredientEx(index, ingredient, "Zen_LeatherSewingKit", uid);
				return;
			case "Whetstone":
				InsertIngredientEx(index, ingredient, "Zen_SharpenMelee", uid);
				return;
			case "WeaponCleaningKit":
				InsertIngredientEx(index, ingredient, "Zen_CleanWeapon", uid);
				return;
			case "DisinfectantAlcohol":
			case "IodineTincture":
			case "Syringe":
			case "SyringeEmpty":
			case "SyringeFull":
			case "AmpouleAlcohol":
			case "SyberiaMedicineAmpoule":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectBottle", uid);
				return;
			case "DisinfectantSpray":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectSpray", uid);
				return;
			case "DuctTape":
				InsertIngredientEx(index, ingredient, "Zen_DuctTape", uid);
				return;
			case "Spraycan_ColorBase":
			case "Zen_Dye_Black":
			case "Zen_Dye_Green":
				InsertIngredientEx(index, ingredient, "Zen_SprayPaint", uid);
				return;
			case "TireRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_Rubber", uid);
				return;
			case "SmallStone":
			case "Stone":
				InsertIngredientEx(index, ingredient, "Zen_BreakStone", uid);
				return;
			case "BarbedWire":
				InsertIngredientEx(index, ingredient, "Zen_MetalWire", uid);
				return;
			case "WoodenPlank":
				InsertIngredientEx(index, ingredient, "Zen_Wood", uid);
				return;
			case "Nail":
				InsertIngredientEx(index, ingredient, "Zen_Nails", uid);
				return;
			case "Flag_Base":
			case "Armband_ColorBase":
			case "Raincoat_ColorBase":
				InsertIngredientEx(index, ingredient, "Rags_craft", uid);
				return;
			case "ElectronicRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_ElectronicRepair", uid);
				return;
			case "Paper":
				InsertIngredientEx(index, ingredient, "Zen_Paper", uid);
				return;
			default:
				break;
		}

		InsertIngredientEx(index, ingredient, "", uid);
	};
}