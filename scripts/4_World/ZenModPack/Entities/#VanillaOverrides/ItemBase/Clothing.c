modded class Clothing
{
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (GetGame().IsDedicatedServer())
		{
			if (ZenModEnabled("ZenDropRuinedClothing"))
			{
				// If ruined, drop item on ground
				if (newLevel == GameConstants.STATE_RUINED && GetHierarchyRootPlayer())
				{
					DropRuinedItem();
				}
			}			
		}
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

	override bool CanDisplayCargo()
	{
		if (GetHierarchyParent() != NULL)
		{
			CarScript car = CarScript.Cast(GetHierarchyParent());
			if (car)
			{
				return false;
			}
		}

		return super.CanDisplayCargo();
	}
}