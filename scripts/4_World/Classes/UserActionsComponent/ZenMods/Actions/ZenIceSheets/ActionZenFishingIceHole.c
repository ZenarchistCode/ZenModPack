class ActionZenFishingIceHole extends ActionFishingNew
{
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined();
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ItemBase bait;
		FishingRod_Base_New rod = FishingRod_Base_New.Cast(item);
		Object targetObject = target.GetObject();

		if (!targetObject || targetObject.Type() != Land_ZenIceSheet_4x4_Hole)
			return false;

		if (rod)
			bait = ItemBase.Cast(rod.FindAttachmentBySlotName("Hook"));

		if (bait && !bait.IsRuined())
			return true;

		return false;
	}
}