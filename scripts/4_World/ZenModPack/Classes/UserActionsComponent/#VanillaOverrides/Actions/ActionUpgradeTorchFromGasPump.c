modded class ActionUpgradeTorchFromGasPump
{
	override bool CanContinue(ActionData action_data)
	{
		return super.CanContinue(action_data) && action_data.m_Player.IsAllowedZenWellOrPumpUsage(action_data);
	}
}