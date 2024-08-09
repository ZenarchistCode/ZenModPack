/*
TEMPORARY VANILLA BUGFIX: https://feedback.bistudio.com/T183055

All I did was add check for BaseRadio.
*/
modded class VehicleBattery
{
	override bool CanPutIntoHands(EntityAI parent)
	{
		if (HasEnergyManager())
		{
			ItemBase poweredDevice = ItemBase.Cast(GetCompEM().GetPluggedDevice());
			if (poweredDevice && (poweredDevice.IsInherited(MetalWire) || poweredDevice.IsInherited(BaseRadio)))
				return true;
		}

		return super.CanPutIntoHands(parent);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		ItemBase poweredDevice = ItemBase.Cast(GetCompEM().GetPluggedDevice());
		if (poweredDevice != NULL && poweredDevice.IsInherited(BaseRadio))
			return true;

		return super.CanPutInCargo(parent);
	}

	//! UPDATE RAID STATION ENERGY WHEN BATTERY IS ATTACHED TO BASE RADIO
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

#ifdef SERVER
		UpdateRaidStationEnergy(parent);
#endif
	}

	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);

#ifdef SERVER
		UpdateRaidStationEnergy(parent);
#endif
	}

	void UpdateRaidStationEnergy(EntityAI parent)
	{
		BaseRadio radio = BaseRadio.Cast(parent);
		if (!radio)
			return;

		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(radio.GetHierarchyParent());
		if (!station)
			return;

		station.UpdateBatteryEnergy();
	}
}