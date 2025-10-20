// You cannot set a damage zone on a static object or it crashes the server when a player interacts with its trigger (no crash logs, no idea why) 
// So this is a work-around by creating an invisible Inventory_Base object on top of the barbed wire that the barbed wire damage zone is linked to.
class ZenStaticBarbedWire extends Inventory_Base
{
	protected ref AreaDamageLoopedDeferred_NoVehicle m_AreaDamage;

	void ZenStaticBarbedWire()
	{
		SetPilotLight(true);
		SetIsHologram(true);
	}

	void SetupDamageZone(Object obj, ZenStaticBarbedWireType wireType)
	{
		m_AreaDamage = new AreaDamageLoopedDeferred_NoVehicle(this);
		m_AreaDamage.SetDamageComponentType(AreaDamageComponentTypes.HITZONE);
		m_AreaDamage.SetLoopInterval(wireType.LoopInterval);
		m_AreaDamage.SetDeferDuration(wireType.DeferDuration);
		m_AreaDamage.SetExtents(wireType.MinExtents.ToVector(), wireType.MaxExtents.ToVector());
		m_AreaDamage.SetHitZones(wireType.DamageZones);
		m_AreaDamage.SetAmmoName(wireType.AmmoName);
		m_AreaDamage.Spawn();
	}
};