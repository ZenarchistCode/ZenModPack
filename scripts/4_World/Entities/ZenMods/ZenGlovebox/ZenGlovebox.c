class ZenGlovebox : WoodenCrate
{
	void ZenGlovebox()
	{
		SetAllowDamage(false);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return parent.IsInherited(CarScript);
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return CarScript.Zen_PlayerInFrontSeat();
	}

	override bool CanDisplayCargo()
	{
		return CarScript.Zen_PlayerInFrontSeat();
	}

	// If this item moves location and it isn't attached to a car, delete it. Shouldn't happen, but failsafe.
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{ 
		super.OnItemLocationChanged(old_owner, new_owner);

		if (!new_owner || !new_owner.IsInherited(CarScript))
		{
			DeleteSafe();
		}
	}
}