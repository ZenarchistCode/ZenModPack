modded class RecipeBase
{
	//! CRAFTING SOUNDS 

	// Hi-jack ingredient sound here whenever appropriate to cover multiple recipes
	// and to allow compatability with future vanilla updates (InsertIngredientEx() will be used directly to add sounds and this would be ignored)
	override void InsertIngredient(int index, string ingredient)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
		{
			super.InsertIngredient(index, ingredient);
			return;
		}

		switch (ingredient)
		{
			case "SewingKit":
				InsertIngredientEx(index, ingredient, "Zen_SewingKit");
				return;
			case "LeatherSewingKit":
				InsertIngredientEx(index, ingredient, "Zen_LeatherSewingKit");
				return;
			case "Whetstone":
				InsertIngredientEx(index, ingredient, "Zen_SharpenMelee");
				return;
			case "WeaponCleaningKit":
				InsertIngredientEx(index, ingredient, "Zen_CleanWeapon");
				return;
			case "DisinfectantAlcohol":
			case "IodineTincture":
			case "Syringe":
			case "SyringeEmpty":
			case "SyringeFull":
			case "AmpouleAlcohol":
			case "SyberiaMedicineAmpoule":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectBottle");
				return;
			case "DisinfectantSpray":
				InsertIngredientEx(index, ingredient, "Zen_DisinfectSpray");
				return;
			case "DuctTape":
				InsertIngredientEx(index, ingredient, "Zen_DuctTape");
				return;
			case "Spraycan_ColorBase":
			case "Zen_Dye_Black":
			case "Zen_Dye_Green":
				InsertIngredientEx(index, ingredient, "Zen_SprayPaint");
				return;
			case "TireRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_Rubber");
				return;
			case "SmallStone":
			case "Stone":
				InsertIngredientEx(index, ingredient, "Zen_BreakStone");
				return;
			case "BarbedWire":
				InsertIngredientEx(index, ingredient, "Zen_MetalWire");
				return;
			case "WoodenPlank":
				InsertIngredientEx(index, ingredient, "Zen_Wood");
				return;
			case "Nail":
				InsertIngredientEx(index, ingredient, "Zen_Nails");
				return;
			case "Flag_Base":
			case "Armband_ColorBase":
			case "Raincoat_ColorBase":
				InsertIngredientEx(index, ingredient, "Rags_craft");
				return;
			case "ElectronicRepairKit":
				InsertIngredientEx(index, ingredient, "Zen_ElectronicRepair");
				return;
			case "Paper":
				InsertIngredientEx(index, ingredient, "Zen_Paper");
				return;
			default:
				break;
		}

		InsertIngredientEx(index, ingredient, "");
	};
}