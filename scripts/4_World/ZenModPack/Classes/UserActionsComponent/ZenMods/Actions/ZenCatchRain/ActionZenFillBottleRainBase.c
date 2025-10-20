class ActionZenFillBottleRainBaseCB : ActionContinuousBaseCB
{
	// Create action component with appropriate fill speed
	override void CreateActionComponent()
	{
		float time = ZenRainWaterConstants.FILL_RAIN_SPEED;

		// Fill cooking pots faster than bottles.
		if (m_ActionData && m_ActionData.m_MainItem && m_ActionData.m_MainItem.IsKindOf("Pot"))
			time = ZenRainWaterConstants.FILL_RAIN_SPEED * ZenRainWaterConstants.FILL_MULTI_POT;

		// Keep in mind CAContinuousFillRain adjusts fill speed based on player movement (run, walk, idle etc)
		m_ActionData.m_ActionComponent = new CAContinuousFillRain(time);
	}
};

class ActionZenFillBottleRainBase : ActionContinuousBase
{
	// Set up action parameters.
	void ActionZenFillBottleRainBase()
	{
		m_CallbackClass		= ActionZenFillBottleRainBaseCB;
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_CATCHRAIN;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONMOD_CATCHRAIN;
		m_FullBody			= false; // Allow movement by default. Set to false to disable, also set HasProneException() to false and change m_CommandUID to CMD_ACTIONFB_FILLBOTTLEPOND
		m_SpecialtyWeight	= UASoftSkillsWeight.PRECISE_LOW;
		m_Text				= "#STR_ZenRain_Txt";
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	// Only condition is a non-ruined item (bottle_base)
	override void CreateConditionComponents()
	{
		m_ConditionItem	  = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	// For some reason CMD_ACTIONMOD_CATCHRAIN only works if prone exception is turned on...
	// But if player is actually prone when catching rain, his character turns into a flying superman pose. So I disable ActionCondition while prone.
	override bool HasProneException()
	{
		return true;
	}

	// Can player catch rain?
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenCatchRain"))
			return false;

		if (item.IsFullQuantity())
			return false;

		if (!player.IsAlive() || player.IsPlacingLocal())
			return false;

		if (!Liquid.CanFillContainer(item, LIQUID_WATER))
			return false;

		if (player.m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
			return false;

		return RainCheck(player);
	}

	// Can player continue to catch rain? Same conditions as start conditions except player no longer needs to look up to sky.
	override bool ActionConditionContinue(ActionData action_data)
	{
		return RainCheck(action_data.m_Player, true);
	}

	bool RainCheck(notnull PlayerBase player, bool noLookAtSky = false)
	{
#ifdef SERVER
		return true;
#endif

		if (!ZenFunctions.IsRaining())
			return false;

		if (MiscGameplayFunctions.IsUnderRoof(player))
			return false;

		if (noLookAtSky)
			return true;

		return ZenFunctions.GetCameraPitch() >= ZenRainWaterConstants.CAMERA_PITCH_TO_CATCH_RAIN;
	}
};