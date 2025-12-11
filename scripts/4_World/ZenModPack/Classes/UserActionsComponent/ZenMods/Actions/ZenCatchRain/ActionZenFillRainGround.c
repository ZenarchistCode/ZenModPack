class ActionZenFillRainGround : ActionInteractBase
{
	// Create & set up action
	void ActionZenFillRainGround()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text		 = "#STR_ZenRain_Txt";
	}

	// Check action conditions for starting the rain catching process
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenCatchRain"))
			return false;

		if (!ZenFunctions.IsRaining())
			return false;

		// If there is no bottle or it's ruined, stop.
		EntityAI target_entity = EntityAI.Cast(target.GetObject());
		if (!target_entity || target_entity.IsRuined())
			return false;

		InventoryLocation loc = new InventoryLocation;
		target_entity.GetInventory().GetCurrentInventoryLocation(loc);

		// If the bottle is not on the ground, stop.
		if (loc.GetType() != InventoryLocationType.GROUND)
			return false;

		Bottle_Base bottle = Bottle_Base.Cast(target_entity);
		if (!bottle)
			return false;

		// If the bottle cannot receive water (eg. it's filled with petrol), stop.
		if (!Liquid.CanFillContainer(ItemBase.Cast(target_entity), LIQUID_WATER))
			return false;

		// Check the final conditions.
		return player.IsAlive() && !bottle.IsZenCatchingRain() && !MiscGameplayFunctions.IsUnderRoof(target_entity);
	}

	// Server-side - start filling water bottle if it's raining
	override void OnExecuteServer(ActionData action_data)
	{
		EntityAI target_entity = EntityAI.Cast(action_data.m_Target.GetObject());
		if (!target_entity)
			return;

		Bottle_Base bottle = Bottle_Base.Cast(target_entity);
		if (!bottle)
			return;

		bottle.SetZenCatchingRain(true);
	}
}