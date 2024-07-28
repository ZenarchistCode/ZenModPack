class ActionPourFuelOnFirewood : ActionContinuousBase
{
	void ActionPourFuelOnFirewood()
	{
		m_CallbackClass = ActionPourFuelOnFireCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTY_VESSEL;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_Text = "#pour_liquid";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override bool HasProneException()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenFireFuel"))
			return false;

		if (item && target.GetObject())
		{
			// Check if this target is firewood and it's not ruined
			Firewood fb = Firewood.Cast(target.GetObject());
			if (!fb || fb.IsRuined())
				return false;

			// If the container we're holding contains more than 1L of fuel, allow action
			if (item.IsInherited(Bottle_Base) && item.GetQuantity() >= 1000)
			{
				return item.GetLiquidType() == LIQUID_GASOLINE || item.GetLiquidType() == LIQUID_DIESEL;
			}
		}

		return false;
	}

	// Create fireplace reduce quantity of container
	override void OnFinishProgressServer(ActionData action_data)
	{
		Firewood wood = Firewood.Cast(action_data.m_Target.GetObject());
		if (wood && action_data.m_MainItem)
		{
			FireplaceBase fire = FireplaceBase.Cast(GetGame().CreateObject("Fireplace", wood.GetPosition()));
			if (fire)
			{
				// Create fire fuel
				EntityAI fuel = fire.GetInventory().CreateAttachment("Zen_FireFuel");

				if (fuel)
				{
					// Consume gasoline
					action_data.m_MainItem.SetQuantity(action_data.m_MainItem.GetQuantity() - 1000);

					// Spawn firewood in fire (my server has firewood stackable, so this loop is to ensure it works with both non-stackable and stackable wood)
					for (int i = 0; i < wood.GetQuantity(); i++)
					{
						EntityAI woodAtt = fire.GetInventory().CreateAttachment("Firewood");
						MiscGameplayFunctions.TransferItemProperties(wood, woodAtt);
					}

					// Delete original firewood
					GetGame().ObjectDelete(wood);
				}
			}
		}
	}

	// Start pour sound
	override void OnStartAnimationLoop(ActionData action_data)
	{
#ifdef SERVER
		Bottle_Base vessel_in_hands = Bottle_Base.Cast(action_data.m_MainItem);
		Param1<bool> play = new Param1<bool>(true);
		GetGame().RPCSingleParam(vessel_in_hands, SoundTypeBottle.POURING, play, true);
#endif
	}

	// Stop pour sound
	override void OnEndAnimationLoop(ActionData action_data)
	{
#ifdef SERVER
		Bottle_Base target_vessel = Bottle_Base.Cast(action_data.m_MainItem);
		Param1<bool> play = new Param1<bool>(false);
		GetGame().RPCSingleParam(target_vessel, SoundTypeBottle.POURING, play, true);
#endif
	}
}