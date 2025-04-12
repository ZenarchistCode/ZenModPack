modded class ActionDrinkPondContinuous : ActionContinuousBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		//! ICE SHEETS
		array<Object> objects = ZenFunctions.GetObjectsRayCastBeneath();
		for (int i = 0; i < objects.Count(); i++)
		{
			if (objects.Get(i).GetType().Contains("Land_ZenIceSheet") && !objects.Get(i).GetType().Contains("Hole"))
			{
				return false;
			}
		}

		return true;
	}
}