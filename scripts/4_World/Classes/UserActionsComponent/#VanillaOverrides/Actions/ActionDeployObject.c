modded class ActionDeployObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		//! CAMO SHELTER
		if (item.IsKindOf("Zen_CamoShelterKit"))
		{
			Zen_CamoShelterKit shelter = Zen_CamoShelterKit.Cast(item);
			return shelter && shelter.HasAllMaterials();
		}

		//! NOTES 
		#ifndef SERVER
		if (player.IsPlacingLocal() && item.IsKindOf("Paper"))
			return true;
		#endif

		return super.ActionCondition(player, target, item);
	}

	override void SetupAnimation(ItemBase item)
    {
		//! NOTES
        if (item.IsKindOf("Paper"))
        {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
            return;
        }

        super.SetupAnimation(item);
    }
};
