modded class ActionSawPlanks
{
	override protected void SpawnNewPlankPile(SawPlanksActionData data, float quantity)
	{
		if (!ZenModEnabled("ZenWoodHealth") || !data.m_MainItem)
		{
			super.SpawnNewPlankPile(data, quantity);
			return;
		}

		float toolHealth = data.m_MainItem.GetHealth01();
		ItemBase planksNewResult = ItemBase.Cast(data.m_Player.SpawnEntityOnGroundRaycastDispersed("WoodenPlank",0.3,UAItemsSpreadRadius.VERY_NARROW));
		planksNewResult.SetQuantity(quantity);
		planksNewResult.SetHealth01("", "", toolHealth);
		data.m_LastPlanksPile = planksNewResult;
	}
}