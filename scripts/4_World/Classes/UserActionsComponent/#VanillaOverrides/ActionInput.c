// Allow action to set Zen_GetDisplayPriority() to force certain actions to display before other actions when needed
// eg. On my server I can skin and search zombies for loot, skinning always showed up first - setting the search 
// action to Zen_GetDisplayPriority() = 1 it forces the search action to display before the skinning action without
// needing to screw around with adding/removing actions to ActionConstructor/Entity.c and get all janky with my shit.
modded class StandardActionInput
{
	protected bool m_ZenResetOverride = false;

	override void UpdatePossibleActions(PlayerBase player, ActionTarget target, ItemBase item, int action_condition_mask)
	{
		super.UpdatePossibleActions(player, target, item, action_condition_mask);

		if (!m_SelectActions || m_SelectActions.Count() == 0 || m_ZenResetOverride)
			return;

		int overrideIndex = m_selectedActionIndex;
		int highestPriority = 0;

		// Sort by highest priority and assign selected index to highest priority action.
		for (int i = 0; i < m_SelectActions.Count(); i++)
		{
			if (m_SelectActions.Get(i) && m_SelectActions.Get(i).Zen_GetDisplayPriority() > highestPriority)
			{
				highestPriority = m_SelectActions.Get(i).Zen_GetDisplayPriority();
				overrideIndex = i;
			}
		}

		m_selectedActionIndex = overrideIndex;
	}

	override void ActionsSelectReset()
	{
		super.ActionsSelectReset();
		m_ZenResetOverride = false;
	}
	
	override void SelectNextAction()
	{
		super.SelectNextAction();
		m_ZenResetOverride = true;
	}
	
	override void SelectPrevAction()
	{
		super.SelectPrevAction();
		m_ZenResetOverride = true;
	}
}