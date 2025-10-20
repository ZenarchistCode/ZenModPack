class ZenGraves_DeadPlayerSkeleton extends WoodenCrate
{
	bool m_CanInteractWithCargo = true;

	override bool IsRefresherSignalingViable()
	{
		return false;
	}

	// Only allow skeleton 
	override bool CanPutInCargo(EntityAI parent)
	{
		if (!parent)
			return m_CanInteractWithCargo;

		return parent.IsInherited(UndergroundStash);
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return m_CanInteractWithCargo;
	}

	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return m_CanInteractWithCargo;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return m_CanInteractWithCargo;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return m_CanInteractWithCargo;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (attachment)
			return m_CanInteractWithCargo;

		return true;
	}

	override void EEInit()
	{
		super.EEInit();

		SetAllowDamage(false);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.RestrictCargoDelayed, 3000);
	}

	// Delaying cargo access 3 secs after creating skeleton is necessary otherwise sometimes the stash 'ejects' the skeleton 
	// because it's not allowed to be placed in its cargo
	void RestrictCargoDelayed()
	{
		m_CanInteractWithCargo = false;
	}
};