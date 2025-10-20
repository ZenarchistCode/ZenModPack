modded class ActionTurnOffAlarmClock : ActionTurnOffWhileInHands
{
	//! TIME BOMB
	override void OnExecuteServer(ActionData action_data)
	{
		AlarmClock_ColorBase alarm = AlarmClock_ColorBase.Cast(action_data.m_MainItem);
		if (alarm)
		{
			alarm.m_ShouldExplode = false;
		}

		super.OnExecuteServer(action_data);
	}
}