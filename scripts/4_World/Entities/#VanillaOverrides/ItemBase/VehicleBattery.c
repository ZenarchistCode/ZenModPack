/*
TEMPORARY VANILLA BUGFIX: https://feedback.bistudio.com/T183055

All I did was add check for BaseRadio. 

!TODO: Remove if Bohemia fix this.
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
}