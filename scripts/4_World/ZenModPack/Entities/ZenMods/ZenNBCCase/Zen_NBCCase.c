class Zen_NBCCase extends ItemBase
{
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		if (!attachment)
			return false;

		if (!attachment.GetInventory() || !attachment.GetInventory().GetCargo())
			return true;

		return attachment.GetInventory().GetCargo().GetItemCount() == 0;
	}
}