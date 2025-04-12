modded class ActionDismantleStoneCircle: ActionContinuousBase
{
	override string GetSoundCategory(ActionData action_data)
	{
		if (!ZenModEnabled("ZenCraftingSounds"))
			return super.GetSoundCategory(action_data);

		return "Zen_Rocks";
	}

	override void OnFinishProgressClient(ActionData action_data)
	{
		super.OnFinishProgressClient(action_data);

		if (!ZenModEnabled("ZenCraftingSounds"))
			return;

		Object target_object = action_data.m_Target.GetObject();
		if (target_object)
		{
			EffectSound effect = SEffectManager.PlaySound("Zen_RocksAttach_SoundSet", target_object.GetPosition());
			effect.SetAutodestroy(true);
		}
	}
};
