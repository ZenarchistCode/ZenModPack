class ActionBoomboxVolDown : ActionInteractBase
{
	void ActionBoomboxVolDown()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Turn volume down";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!target || !target.GetObject())
			return false;

		Object target_object = target.GetObject();

		Zen_MusicDeviceBase ntarget = Zen_MusicDeviceBase.Cast(target_object);
		if (ntarget && ntarget.IsPlayingSong())
			return ntarget.GetVolume() != 0;

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Object target_object = action_data.m_Target.GetObject();

		Zen_MusicDeviceBase ntarget = Zen_MusicDeviceBase.Cast(target_object);
		if (ntarget)
			ntarget.TurnVolumeDownServer();
	}
}