class MusicActionInput : DefaultActionInput
{
	ref ActionTarget target_new;

	void MusicActionInput(PlayerBase player)
	{
		SetInput("UAZenMusic");
		m_InputType = ActionInputType.AIT_SINGLE;
		m_Priority = 100;
	}

	override void UpdatePossibleActions(PlayerBase player, ActionTarget target, ItemBase item, int action_condition_mask)
	{
		if (ForceActionCheck(player))
		{
			m_SelectAction = m_ForcedActionData.m_Action;
			return;
		}

		m_SelectAction = NULL;
		array<ActionBase_Basic> possible_actions;
		ActionBase action;
		int i;

		m_MainItem = NULL;
		if (player && !player.IsInVehicle())
		{
			ClearForcedTarget();
		}
		else 
		if (player && player.IsInVehicle())
		{
			HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
			if (vehCommand)
			{
				Transport trans = vehCommand.GetTransport();
				if (trans)
				{
					target_new = new ActionTarget(trans, null, -1, vector.Zero, -1);
					ForceActionTarget(target_new);
				}
			}

			if (!target_new)
				ClearForcedTarget();
		}

		target = m_ForcedTarget;
		m_Target = m_ForcedTarget;

		if (target && target.GetObject())
		{
			target.GetObject().GetActions(this.Type(), possible_actions);
			if (possible_actions)
			{
				for (i = 0; i < possible_actions.Count(); i++)
				{
					action = ActionBase.Cast(possible_actions.Get(i));
					if (action.Can(player, target, m_MainItem, action_condition_mask))
					{
						m_SelectAction = action;
						return;
					}
				}
			}
		}
	}

	override ActionBase GetAction()
	{
		return m_SelectAction;
	}
};