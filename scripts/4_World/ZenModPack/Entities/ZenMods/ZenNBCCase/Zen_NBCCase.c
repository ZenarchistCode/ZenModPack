class Zen_NBCCase extends Container_Base
{
	//! FIX FOR GAME RANDOMLY DROPPING ATTACHED ITEMS TO THIS NBC CASE AFTER LOGOUT/LOG BACK IN/HIERARCHY RACE CONDITIONS WHILE LOADING ITEMS
	override protected void AdjustExclusionAccessCondition(int occupiedSlot, int testedSlot, set<int> value, inout set<int> adjustedValue)
	{
		string slotName = InventorySlots.GetSlotName(testedSlot);

		//! NBC case attachment slots are storage slots, not wearable equipment slots.
		//! Do not allow clothing attachment exclusions to propagate through the case.
		if (slotName.Contains("ZenNBC_"))
		{
			adjustedValue = new set<int>;
			return;
		}

		super.AdjustExclusionAccessCondition(occupiedSlot, testedSlot, value, adjustedValue);
	}

	//! PREVENT ATTACHING CLOTHING ITEMS WITH CARGO IN THEM TO THE NBC CASE
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!attachment)
			return false;

		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		if (!attachment.GetInventory() || !attachment.GetInventory().GetCargo())
			return true;

		return attachment.GetInventory().GetCargo().GetItemCount() == 0;
	}
}
