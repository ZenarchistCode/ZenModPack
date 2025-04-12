class ActionStaticBoomboxNextSong : ActionInteractBase
{
	void ActionStaticBoomboxNextSong()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Next song";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!target || !target.GetObject())
			return false;

		Object target_object = target.GetObject();

		Zen_BoomBox_Static ntarget = Zen_BoomBox_Static.Cast(target_object);
		return ntarget && ntarget.IsPlayingSong();
	}

	override void OnExecuteServer(ActionData action_data)
	{
		Object target_object = action_data.m_Target.GetObject();

		// Prep static boombox to play a random cassette
		Zen_BoomBox_Static ntarget = Zen_BoomBox_Static.Cast(target_object);
		if (!ntarget)
			return;

		ntarget.StopSongServer();
		ntarget.PrepareForNewMusic(true);
	}
}