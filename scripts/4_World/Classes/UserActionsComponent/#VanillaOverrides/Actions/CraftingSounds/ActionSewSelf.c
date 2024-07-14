modded class ActionBurnSewSelf
{
	override string GetSoundCategory(ActionData action_data)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
			return super.GetSoundCategory(action_data);

		if (action_data.m_Player && action_data.m_Player.IsMale())
			return "Zen_PainMale";
		else
			return "Zen_PainFemale";
	}
};

modded class ActionBurnSewTarget
{
	override string GetSoundCategory(ActionData action_data)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
			return super.GetSoundCategory(action_data);

		PlayerBase target = PlayerBase.Cast(action_data.m_Target.GetObject());

		if (target && target.IsMale())
			return "Zen_PainMale";
		else
			return "Zen_PainFemale";
	}
};
