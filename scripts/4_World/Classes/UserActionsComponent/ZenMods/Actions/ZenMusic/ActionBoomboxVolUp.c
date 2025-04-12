class ActionBoomboxVolUp : ActionInteractBase
{
	void ActionBoomboxVolUp()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#STR_ZenMusic_VolumeUp";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!target || !target.GetObject())
			return false;

		Object target_object = target.GetObject();

		Zen_MusicDeviceBase ntarget = Zen_MusicDeviceBase.Cast(target_object);
		if (ntarget && ntarget.IsPlayingSong())
			return ntarget.GetVolume() != ntarget.GetMaxVolume();

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Object target_object = action_data.m_Target.GetObject();

		Zen_MusicDeviceBase ntarget = Zen_MusicDeviceBase.Cast(target_object);
		if (ntarget)
			ntarget.TurnVolumeUpServer();
	}
}