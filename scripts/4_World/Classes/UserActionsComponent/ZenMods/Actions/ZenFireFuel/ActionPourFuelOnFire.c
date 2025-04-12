class ActionPourFuelOnFireCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(3);
	}
};

class ActionPourFuelOnFire : ActionContinuousBase
{
	void ActionPourFuelOnFire()
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
			// Check if this target is a fireplace and if it has sticks or firewood attached
			FireplaceBase fb = FireplaceBase.Cast(target.GetObject());
			if (!fb || fb.GetFuelCount_ZenFireFuel() == 0)
				return false;

			// Check if it has already been doused in gasoline
			ItemBase fuel = ItemBase.Cast(fb.FindAttachmentBySlotName(FireplaceBase.ATT_FIREFUEL));
			if (fuel && fuel.IsInherited(Zen_FireFuel))
			{
				return false;
			}

			// If the container we're holding contains more than 1L of fuel, allow action
			if (item.IsInherited(Bottle_Base) && item.GetQuantity() >= 1000)
			{
				return item.GetLiquidType() == LIQUID_GASOLINE || item.GetLiquidType() == LIQUID_DIESEL;
			}
		}

		return false;
	}

	// Create fuel and reduce quantity of container
	override void OnFinishProgressServer(ActionData action_data)
	{
		FireplaceBase fb = FireplaceBase.Cast(action_data.m_Target.GetObject());
		if (fb && action_data.m_MainItem)
		{
			EntityAI fuel = fb.GetInventory().CreateAttachment("Zen_FireFuel");

			if (fuel)
			{
				action_data.m_MainItem.SetQuantity(action_data.m_MainItem.GetQuantity() - 1000);
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