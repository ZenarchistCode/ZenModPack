modded class ModItemRegisterCallbacks
{
	override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterTwoHanded(pType, pBehavior);

		//! CHESS
		pType.AddItemInHandsProfileIK("Zen_ChessBoard", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/BatteryCar.anm");
	}

	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);

		//! DEXIES 
		pType.AddItemInHandsProfileIK("Zen_DexBottle", "dz/anims/workspaces/player/player_main/props/player_main_1h_pills.asi", pBehavior, "dz/anims/anm/player/ik/gear/VitaminBottle.anm");

		//! NBC CASE
		pType.AddItemInHandsProfileIK("Zen_NBCCase",  "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/protector_case.anm");	

		//! JOURNAL
		pType.AddItemInHandsProfileIK("ZenBook_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/book.anm");

		//! FLASK
		pType.AddItemInHandsProfileIK("ZenFlask", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/carp_live.anm");
		pType.AddItemInHandsProfileIK("Empty_ZenFlask", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/carp_live.anm");

		//! RUNES 
		pType.AddItemInHandsProfileIK("ZenRune_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/SmallStone.anm");

		//! RAYBANS
		pType.AddItemInHandsProfileIK("ZenRaybans_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/aviators_glasses.anm");

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

		//! FLINT 
		pType.AddItemInHandsProfileIK("ZenFlint", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/WoodenStick.anm");
	} 

	override void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterHeavy(pType, pBehavior);

		//! TIRE RACK
		pType.AddItemInHandsProfileIK("Zen_TireRack", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/car_tent_packed.anm");
	}
}