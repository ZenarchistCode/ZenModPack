modded class ActionTestBloodTarget: ActionContinuousBase
{
	override string GetSoundCategory(ActionData action_data)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
			return super.GetSoundCategory(action_data);

		return "Zen_BloodTest";
	}
};
