class ActionZenSwissKnife_ConvertBase : ActionSingleUseBase
{
	void ActionZenSwissKnife_ConvertBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#STR_ZenAction_Extend ";
	}

	string GetConvertToolType()
	{
		return "ZenSwissKnife"; // Replace with actual tool type in child actions
	}

	override bool HasTarget() 
	{
		return false; 
	}

	// Check condition of selected tool type
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// Base tool config - check if this tool is enabled by server owner
		if (!item.ConfigGetBool(GetConvertToolType()))
			return false;

		//! ZenSwissKnife base
		if (item.IsInherited(ZenSwissKnife))
		{
			ZenSwissKnife swissBase = ZenSwissKnife.Cast(item);
			return swissBase.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_CanOpener
		if (item.IsInherited(ZenSwissKnife_CanOpener))
		{
			ZenSwissKnife_CanOpener canOpener = ZenSwissKnife_CanOpener.Cast(item);
			return canOpener.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_Knife
		if (item.IsInherited(ZenSwissKnife_Knife))
		{
			ZenSwissKnife_Knife knife = ZenSwissKnife_Knife.Cast(item);
			return knife.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_Lockpick
		if (item.IsInherited(ZenSwissKnife_Lockpick))
		{
			ZenSwissKnife_Lockpick lockpick = ZenSwissKnife_Lockpick.Cast(item);
			return lockpick.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_Scissors
		if (item.IsInherited(ZenSwissKnife_Scissors))
		{
			ZenSwissKnife_Scissors scissors = ZenSwissKnife_Scissors.Cast(item);
			return scissors.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_Screwdriver
		if (item.IsInherited(ZenSwissKnife_Screwdriver))
		{
			ZenSwissKnife_Screwdriver screwdriver = ZenSwissKnife_Screwdriver.Cast(item);
			return screwdriver.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		//! ZenSwissKnife_SmallSaw
		if (item.IsInherited(ZenSwissKnife_SmallSaw))
		{
			ZenSwissKnife_SmallSaw saw = ZenSwissKnife_SmallSaw.Cast(item);
			return saw.GetSwissToolHealth(GetConvertToolType()) > 0;
		}

		return false;
	}

	override void OnExecuteClient(ActionData action_data)
    {
		super.OnExecuteClient(action_data);

		ClearInventoryReservationEx(action_data);
    }

	override void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);

		ZenSwissKnife.ConvertKnife(action_data.m_MainItem, GetConvertToolType());
	}
}
