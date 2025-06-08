modded class Clothing
{
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		#ifdef SERVER
		if (!ZenModEnabled("ZenDropRuinedClothing"))
			return;

		// If ruined, drop item on ground
		if (newLevel == GameConstants.STATE_RUINED && GetHierarchyRootPlayer())
		{
			DropRuinedItem();
		}
		#endif
	}

	void DropRuinedItem()
	{
		if (GetHierarchyParent().IsInherited(PlayerBase))
		{
			PlayerBase pb = PlayerBase.Cast(GetHierarchyParent());

			#ifdef ENFUSION_AI_PROJECT
			if (pb.IsAI())
				return;
			#endif
			
			GetInventory().DropEntity(InventoryMode.SERVER, pb, this);
		}
	}
}