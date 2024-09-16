modded class ActionFishingNew
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		//! ICE SHEETS
		Object targetObject = target.GetObject();

		if (targetObject != NULL)
		{
			if (targetObject.Type() == Land_ZenIceSheet_4x4_Hole)
				return true;

			if (targetObject.GetType().Contains("Land_ZenIceSheet") && !targetObject.GetType().Contains("Hole"))
				return false;
		}

		array<Object> objects = ZenFunctions.GetObjectsRayCastCamera(UAMaxDistances.BASEBUILDING * 2);
		array<Object> objects2 = ZenFunctions.GetObjectsRayCastBeneath();
		foreach(Object obj : objects2)
		{
			objects.Insert(obj);
		}

		for (int i = 0; i < objects.Count(); i++)
		{
			if (objects.Get(i).GetType().Contains("Land_ZenIceSheet") && !objects.Get(i).GetType().Contains("Hole"))
			{
				return false;
			}
		}

		return true;
	}
};