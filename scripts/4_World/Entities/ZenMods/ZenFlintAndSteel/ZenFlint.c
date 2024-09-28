class ZenFlint extends ItemBase
{
	protected PlayerBase m_ZenIgniterPlayer;
	protected ItemBase m_ZenIgniterItem;

	void SetZenIgniterItem(ItemBase item, PlayerBase player)
	{
		m_ZenIgniterItem = item;
		m_ZenIgniterPlayer = player;
	}

	bool HasValidZenIgniterItem()
	{
		if (GetQuantity() <= 0)
			return false;

		if (!m_ZenIgniterItem)
			return false;

		if (m_ZenIgniterItem.IsRuined())
			return false;

		if (m_ZenIgniterItem.GetHierarchyRootPlayer() != m_ZenIgniterPlayer)
			return false;

		return true;
	}

	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		if (HasValidZenIgniterItem())
			return true;
		else
			return false;
	}

	override void OnIgnitedTarget(EntityAI target_item)
	{
		if (!HasValidZenIgniterItem() || !GetGame().IsDedicatedServer())
			return;

		AddQuantity(Math.RandomFloatInclusive(0.5, 1) * -1);
		m_ZenIgniterItem.AddHealth(-5);
	}

	override void OnIgnitedTargetFailed(EntityAI target_item)
	{
		if (!HasValidZenIgniterItem() || !GetGame().IsDedicatedServer())
			return;

		AddQuantity(Math.RandomFloatInclusive(0.5, 1) * -1);
		m_ZenIgniterItem.AddHealth(-5);
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionPrepareZenFlint);
		AddAction(ActionLightItemOnFire);
	}
};