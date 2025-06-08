modded class CarHud
{
	override void RefreshVehicleHud(float timeslice)
	{
		super.RefreshVehicleHud(timeslice);

		if (!ZenModEnabled("ZenCarBatteryIcon"))
			return;

		if (m_CurrentVehicle && !GetGame().GetUIManager().GetMenu() && m_VehiclePanel.IsVisible())
		{
			int batColor = Colors.COLOR_PRISTINE;
			int energy = m_CurrentVehicle.GetZenBatteryEnergy0To100();

			if (!m_CurrentVehicle.GetBattery())
				batColor = Colors.GRAY;
			else 
			if(energy <= 10)
				batColor = Colors.COLOR_RUINED;
			else 
			if(energy <= 25)
				batColor = Colors.COLOR_BADLY_DAMAGED;
			else 
			if(energy <= 50)
				batColor = Colors.YELLOW;
			else 
			if(energy < 75)
				batColor = Colors.COLOR_WORN;

			m_VehicleBatteryLight.Show(true);
			m_VehicleBatteryLight.SetColor(batColor);
			m_VehicleBatteryLight.SetAlpha(1);
		}
	}
}