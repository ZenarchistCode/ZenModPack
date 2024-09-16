modded class ActionLightItemOnFire
{
	//! FLINT
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if (super.SetupAction(player, target, item, action_data, extra_data))
		{
			if (item && item.Type() == ZenFlint)
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;

			return true;
		}

		return false;
	}

	override string GetSoundCategory(ActionData action_data)
	{
		if (action_data.m_MainItem && action_data.m_MainItem.Type() == ZenFlint)
			return "Zen_FlintStrike";

		return super.GetSoundCategory(action_data);
	}
}