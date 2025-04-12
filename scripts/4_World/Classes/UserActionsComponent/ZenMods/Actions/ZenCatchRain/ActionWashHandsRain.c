class ActionWashHandsRainCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.WASH_HANDS * 10);
	}
};

class ActionWashHandsRain : ActionContinuousBase
{
	void ActionWashHandsRain()
	{
		m_CallbackClass = ActionWashHandsRainCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_WASHHANDSPOND;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#wash_hands";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenCatchRain"))
			return false;

		return WashCheck(player);
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		return WashCheck(action_data.m_Player, true);
	}

	// Clean hands
	override void OnFinishProgressServer(ActionData action_data)
	{
		PluginLifespan module_lifespan = PluginLifespan.Cast(GetPlugin(PluginLifespan));
		module_lifespan.UpdateBloodyHandsVisibility(action_data.m_Player, false);
	}

	bool WashCheck(notnull PlayerBase player, bool noLookAtSky = false)
	{
#ifdef SERVER
		return true;
#endif

		if (!player.HasBloodyHands())
			return false;

		if (player.GetItemInHands() != NULL)
			return false;

		if (!ZenFunctions.IsRaining())
			return false;

		if (MiscGameplayFunctions.IsUnderRoof(player))
			return false;

		if (noLookAtSky)
			return true;

		return ZenFunctions.GetCameraPitch() >= ZenRainWaterConstants.CAMERA_PITCH_TO_CATCH_RAIN;
	}
};