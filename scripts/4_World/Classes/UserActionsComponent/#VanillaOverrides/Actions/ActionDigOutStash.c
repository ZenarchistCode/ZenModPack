modded class ActionDigOutStash : ActionContinuousBase
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		if (action_data.m_Target && action_data.m_Target.GetObject())
		{
			EntityAI targetEntity = EntityAI.Cast(action_data.m_Target.GetObject());
			ZenGraves_UndergroundStash stash;
			if (targetEntity && Class.CastTo(stash, targetEntity))
			{
				array<Object> objectsAtCross = new array<Object>;
				GetGame().GetObjectsAtPosition3D(stash.GetPosition(), 2, objectsAtCross, null);

				for (int i = 0; i < objectsAtCross.Count(); i++)
				{
					ZenGraves_DeadPlayerCross nearbyCross = ZenGraves_DeadPlayerCross.Cast(objectsAtCross[i]);

					if (nearbyCross)
					{
						nearbyCross.DeleteSafe();
					}
				}

				// Spawn a worm
				EntityAI worm = EntityAI.Cast(GetGame().CreateObject("Worm", stash.GetPosition(), ECE_PLACE_ON_SURFACE));

				if (worm)
				{
					worm.PlaceOnSurface();
				}

				// Spawn rotten human meat
				Edible_Base humanMeat = Edible_Base.Cast(GetGame().CreateObject("HumanSteakMeat", stash.GetPosition(), ECE_PLACE_ON_SURFACE));

				if (humanMeat)
				{
					humanMeat.SetQuantity(1);
					humanMeat.ChangeFoodStage(FoodStageType.ROTTEN);
					vector pos = humanMeat.GetPosition();
					pos[0] = pos[0] + Math.RandomFloatInclusive(0.1, -0.1);
					pos[2] = pos[2] + Math.RandomFloatInclusive(0.1, -0.1);
					humanMeat.SetPosition(pos);
					humanMeat.PlaceOnSurface();
					humanMeat.SetHealth(1);
				}
			}
		}

		super.OnFinishProgressServer(action_data);
	};
};