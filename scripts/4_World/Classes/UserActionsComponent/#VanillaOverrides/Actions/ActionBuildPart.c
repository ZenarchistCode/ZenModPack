/*modded class ActionBuildPart
{
	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);

		if (action_data.m_MainItem && action_data.m_MainItem.Type() == Zen_VikingAxe)
		{
			Zen_VikingAxe axe = Zen_VikingAxe.Cast(action_data.m_MainItem);
			if (axe)
				axe.SetAxeBackwards(true);
		}
	}

	override void OnEndClient(ActionData action_data)
	{
		super.OnEndClient(action_data);

		if (action_data.m_MainItem && action_data.m_MainItem.Type() == Zen_VikingAxe)
		{
			Zen_VikingAxe axe = Zen_VikingAxe.Cast(action_data.m_MainItem);
			if (axe)
				axe.SetAxeBackwards(false);
		}
	}
}
*/