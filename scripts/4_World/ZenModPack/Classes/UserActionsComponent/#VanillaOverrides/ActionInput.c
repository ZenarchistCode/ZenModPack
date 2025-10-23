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

		if (!m_SelectActions || m_SelectActions.Count() <= 1 || m_ZenResetOverride)
			return;

		for (int i = 1; i < m_SelectActions.Count(); i++)
		{
			ActionBase ab = m_SelectActions[i];
			if (!ab)
				continue;

			int displayPriority = ab.Zen_GetDisplayPriority();

			int j = i - 1;
			while (j >= 0 && m_SelectActions[j].Zen_GetDisplayPriority() > displayPriority)
			{
				m_SelectActions[j + 1] = m_SelectActions[j];
				j--;
			}

			m_SelectActions[j + 1] = ab;
		}
	}

	override void ActionsSelectReset()
	{
		super.ActionsSelectReset();
		m_ZenResetOverride = false;
	}

	override void SelectNextAction()
	{
		super.SelectNextAction();
		m_ZenResetOverride = true; // let player scroll without our resort fighting it
	}

	override void SelectPrevAction()
	{
		super.SelectPrevAction();
		m_ZenResetOverride = true;
	}
}