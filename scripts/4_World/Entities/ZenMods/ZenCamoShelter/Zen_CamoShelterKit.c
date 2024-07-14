class Zen_CamoShelterKit extends KitBase
{
	string GetHologram()
	{
		return "Zen_ImprovisedShelter";
	}

	//================================================================
	// ADVANCED PLACEMENT
	//================================================================		
	
	vector GetHoloPlacementOffset()
	{
		return "0 1 0";
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (GetGame().IsDedicatedServer())
		{
			GetGame().ObjectDelete(this); // Delete kit when shelter is placed.
			SetIsDeploySound(true);
		}
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		if (GetMaterialCount("CamoNet") == 0 && GetMaterialCount("Material_Shelter_FrameSticks") == 0)
		{
			return true;
		}

		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanReleaseCargo(EntityAI attachment)
	{
		return true;
	}

	override bool CanBePlaced(Man player, vector pos)
	{
		return true;
	}

	override bool IsDeployable()
	{
		return true;
	}

	override string GetDeploySoundset()
	{
		return "putDown_FenceKit_SoundSet";
	}

	override string GetLoopDeploySoundset()
	{
		return "BarbedWire_Deploy_loop_SoundSet";
	}

	override bool IsContainer()
	{
		return true;
	}

	int GetMaterialCount(string slot)
	{
		int slot_id = InventorySlots.GetSlotIdFromString(slot);
		ItemBase slotCast = ItemBase.Cast(GetInventory().FindAttachment(slot_id));

		// If item exists - get quantity
		if (slotCast)
		{
			if (slotCast.ConfigGetInt("varQuantityMax") > 0)
				return slotCast.GetQuantity();
			else
				return 1;
		}

		return 0;
	}

	bool HasAllMaterials()
	{
		if (GetMaterialCount("Rope") == 0 || GetMaterialCount("CamoNet") == 0)
			return false;

		if (GetMaterialCount("Material_Shelter_FrameSticks") != 4)
			return false;

		return true;
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		if (item.IsInherited(Rope)) // Detaching rope destroys the kit, drop all items
		{
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, vector.Zero);
		}

		super.EEItemDetached(item, slot_name);
	}
}
