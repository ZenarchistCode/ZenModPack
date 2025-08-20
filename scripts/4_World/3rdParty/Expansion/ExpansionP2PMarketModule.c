#ifdef EXPANSIONMODP2PMARKET
modded class ExpansionP2PMarketModule
{
	override protected void ServerModuleInit()
	{
		super.ServerModuleInit();

		if (!ZenModEnabled("ZenBasebuildingConfig"))
			return;

		if (!GetZenBasebuildingConfig().NoBuildZonesExpansionAutoCreate)
			return;

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_MarketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));

			foreach (ExpansionP2PMarketTraderConfig config: m_P2PTraderConfig)
			{
				ZMPrint("[ZenExpansionOverrides] Adding ZenBasebuildingConfig 50-meter no-build zone for p2p trader area @ " + config.m_Position);
				GetZenBasebuildingConfig().NoBuildZones.Insert(new ZenNoBuildZone("Trader Zone", config.m_Position, 50, true));
			}
		}
	}
}
#endif