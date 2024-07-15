// Actions with swiss tool take longer
modded class CAContinuousTime : CAContinuousBase
{
	override void Setup(ActionData action_data)
	{
		super.Setup(action_data);

		if (ZenSwissKnife.IsSwissKnifeUsed(action_data))
		{
			m_AdjustedTimeToComplete = m_AdjustedTimeToComplete * ZenSwissKnife.DURATION_NERF;
		}
	}
}