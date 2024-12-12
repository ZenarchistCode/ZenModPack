modded class ModItemRegisterCallbacks
{
	override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterTwoHanded(pType, pBehavior);

		//! CHESS
		pType.AddItemInHandsProfileIK("Zen_ChessBoard", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/BatteryCar.anm");
	
		//! MUSIC 
		pType.AddItemInHandsProfileIK("Zen_BoomBox", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
	
		//! RAID ALARM
		pType.AddItemInHandsProfileIK("Zen_RaidAlarmStationKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
	}

	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);

		//! FLASK
		pType.AddItemInHandsProfileIK("ZenFlask", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/carp_live.anm");
		pType.AddItemInHandsProfileIK("Empty_ZenFlask", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/carp_live.anm");

		//! SLEEPING BAGS
		pType.AddItemInHandsProfileIK("ZenSleepingBagPackedBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/BatteryCar.anm");

		//! SWISS KNIFE
		pType.AddItemInHandsProfileIK("ZenSwissKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/screwdriver.anm");
		pType.AddItemInHandsProfileIK("ZenSwissKnife_Lockpick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/screwdriver.anm");
		pType.AddItemInHandsProfileIK("ZenSwissKnife_CanOpener", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/screwdriver.anm");
	
		//! RUNES 
		pType.AddItemInHandsProfileIK("ZenRune_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/SmallStone.anm");

		//! RAYBANS
		pType.AddItemInHandsProfileIK("ZenRaybans_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/aviators_glasses.anm");

		//! MUSIC 
		pType.AddItemInHandsProfileIK("Zen_Cassette_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("Zen_CassetteCase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/cleaning_kit_wood.anm");

		//! ZIPPO
		pType.AddItemInHandsProfileIK("ZenZippoLighter", "dz/anims/workspaces/player/player_main/props/player_main_1h_lighter.asi", pBehavior,	"dz/anims/anm/player/ik/gear/petrol_lighter.anm");

		//! TREASURE 
		pType.AddItemInHandsProfileIK("ZenTreasure_PhotoBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");

		//! NOTES
		pType.AddItemInHandsProfileIK("ZenNote", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");

		//! CHESS 
		pType.AddItemInHandsProfileIK("Zen_ChessPieceBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/9v_battery.anm");

		//! CONCUSSION GRENADE
		pType.AddItemInHandsProfileIK("Zen_ConcussionGrenade", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", pBehavior, "dz/anims/anm/player/ik/gear/hammer_ik.anm");
	
		//! CAMO NET GHILLIE 
		pType.AddItemInHandsProfileIK("Zen_CamoNet_GhillieAtt", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/attachments/support/ghillie_wrap.anm");
        pType.AddItemInHandsProfileIK("Zen_CamoNet_Bushrag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/GhillieBushrag.anm");
        pType.AddItemInHandsProfileIK("Zen_CamoNet_Suit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/GhillieBushrag.anm");
        pType.AddItemInHandsProfileIK("Zen_CamoNet_Top", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/GhillieBushrag.anm");
        pType.AddItemInHandsProfileIK("Zen_CamoNet_Hood", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/dirtbike_visor.anm");
        pType.AddItemInHandsProfileIK("Zen_CamoNet_Strip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/burlap_strips.anm");
	
		//! RAID ALARM
		pType.AddItemInHandsProfileIK("Zen_RaidAlarmRadarKit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi",	pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");

		//! FLINT 
		pType.AddItemInHandsProfileIK("ZenFlint", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/WoodenStick.anm");

		//! LEFTOVERS
		// Food
        pType.AddItemInHandsProfileIK("Empty_BoxCerealCrunchin", "dz/anims/workspaces/player/player_main/props/player_main_1h_food_box.asi", pBehavior, "dz/anims/anm/player/ik/gear/box_cereal.anm");
        pType.AddItemInHandsProfileIK("Empty_Rice", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/rice.anm");
        pType.AddItemInHandsProfileIK("Empty_Marmalade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/marmalade.anm");
        pType.AddItemInHandsProfileIK("Empty_Marmalade_NoLiquid", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/marmalade.anm");
		pType.AddItemInHandsProfileIK("Empty_PowderedMilk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/powdered_milk.anm");
		pType.AddItemInHandsProfileIK("Empty_BakedBeansCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("Empty_PeachesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("Empty_TacticalBaconCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("Empty_SpaghettiCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("Empty_SardinesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/sardines.anm");
		pType.AddItemInHandsProfileIK("Empty_TunaCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("Empty_DogFoodCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan250g.anm");
		pType.AddItemInHandsProfileIK("Empty_CatFoodCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan250g.anm");
		pType.AddItemInHandsProfileIK("Empty_PorkCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan250g.anm");
		pType.AddItemInHandsProfileIK("Empty_Lunchmeat_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan250g.anm");
		pType.AddItemInHandsProfileIK("Empty_UnknownFoodCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/food_can_opened.anm");
		pType.AddItemInHandsProfileIK("Empty_Pajka_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan100g.anm");
		pType.AddItemInHandsProfileIK("Empty_Pate_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan100g.anm");
		pType.AddItemInHandsProfileIK("Empty_BrisketSpread_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/foodcan100g.anm");
		pType.AddItemInHandsProfileIK("Empty_Honey", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/marmalade.anm");
		pType.AddItemInHandsProfileIK("Empty_Honey_NoLiquid", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/marmalade.anm");
		pType.AddItemInHandsProfileIK("Empty_Zagorky_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/zagorky.anm");
		pType.AddItemInHandsProfileIK("Empty_Snack_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/BloodBag_Sealed.anm");
		pType.AddItemInHandsProfileIK("Empty_Crackers", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/saltycrackers.anm");
		// Meds
		pType.AddItemInHandsProfileIK("Used_Epinephrine", "dz/anims/workspaces/player/player_main/props/player_main_1h_morphine.asi", pBehavior, "dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Used_Morphine", "dz/anims/workspaces/player/player_main/props/player_main_1h_morphine.asi", pBehavior, "dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Used_AntiChemInjector", "dz/anims/workspaces/player/player_main/props/player_main_1h_morphine.asi", pBehavior, "dz/anims/anm/player/ik/gear/morphine.anm");
		pType.AddItemInHandsProfileIK("Used_BloodBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/BloodBag_Full_IV.anm");
		pType.AddItemInHandsProfileIK("Used_SalineBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/TransfusionKit.anm");
		// Drink cans
		pType.AddItemInHandsProfileIK("Empty_SodaCan_ColorBase", "dz/anims/workspaces/player/player_main/props/player_main_1h_sodacan.asi", pBehavior, "dz/anims/anm/player/ik/gear/soda_can.anm");	
	} 

	override void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterHeavy(pType, pBehavior);

		//! TIRE RACK
		pType.AddItemInHandsProfileIK("Zen_TireRack", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/car_tent_packed.anm");
	
		//! WORKBENCH 
		pType.AddItemInHandsProfileIK("Zen_CarWorkbenchKit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");
	}
};