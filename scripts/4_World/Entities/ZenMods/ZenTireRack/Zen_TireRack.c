class Zen_TireRack extends ItemBase
{
	void Zen_TireRack()
	{
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsPlaceSound");
	}

	override bool IsRefresherSignalingViable()
	{
		return !IsRuined();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return GetInventory().AttachmentCount() == 0;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return GetInventory().AttachmentCount() == 0;
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (!GetGame().IsDedicatedServer())
			return;

		if (newLevel == GameConstants.STATE_RUINED)
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, vector.Zero);
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
};
