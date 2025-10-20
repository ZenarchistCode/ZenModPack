#ifdef EXPANSIONMODMARKET 
modded class ExpansionTraderObjectBase
{
	override void UpdateTraderZone()
	{
		super.UpdateTraderZone();

		if (!ZenModEnabled("ZenBasebuildingConfig"))
			return;

		if (!GetZenBasebuildingConfig().NoBuildZonesExpansionAutoCreate)
			return;

		if (!m_TraderZone || !GetTraderEntity())
			return;

		ZMPrint("[ZenExpansionOverrides] Adding ZenBasebuildingConfig 50-meter no-build zone for trader area @ " + GetTraderEntity().GetPosition());
		GetZenBasebuildingConfig().NoBuildZones.Insert(new ZenNoBuildZone("Trader Zone", GetTraderEntity().GetPosition(), 50, true));
	}
}
#endif