class Zen_FireFuel extends ItemBase
{
	override bool HasFlammableMaterial()
	{
		return true;
	}

	override bool CanBeIgnitedBy(EntityAI igniter = NULL)
	{
		if (GetHierarchyParent()) return false;

		return true;
	}

	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		return false;
	}

	override void OnIgnitedTarget(EntityAI target_item)
	{
	}

	override void OnIgnitedThis(EntityAI fire_source)
	{
		Fireplace.IgniteEntityAsFireplace(this, fire_source);
	}

	override bool IsThisIgnitionSuccessful(EntityAI item_source = NULL)
	{
		return Fireplace.CanIgniteEntityAsFireplace(this);
	}
}