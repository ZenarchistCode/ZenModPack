modded class Canteen
{
	override bool CanDisplayAttachmentCategory(string category_name)
	{
		string catName = category_name;
		catName.ToLower();

		if (catName == "zenchlorinetablets")
			return super.CanDisplayAttachmentCategory(category_name) && ZenModEnabled("ZenCanteenTablets");

		return super.CanDisplayAttachmentCategory(category_name);
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (InventorySlots.GetSlotName(slot_id) == "Material_ZenChlorine")
		{
			return ZenModEnabled("ZenCanteenTablets") && super.CanDisplayAttachmentSlot(slot_id);
		}

		return super.CanDisplayAttachmentSlot(slot_id);
	}
}