class Zen_WoodReserve extends BuildingSuper
{
	protected int m_WoodLeft = 0;

	void Zen_WoodReserve()
	{
		if (GetGame().IsDedicatedServer())
		{
			int minWood = GetZenFirewoodConfig().GetMinWood(this.GetType());
			int maxWood = GetZenFirewoodConfig().GetMaxWood(this.GetType());
			m_WoodLeft = Math.RandomIntInclusive(minWood, maxWood);
		}
	}

	void ConsumeWood(int amount = 1)
	{
		m_WoodLeft = m_WoodLeft - amount;
	}

	int GetWoodLeft()
	{
		return m_WoodLeft;
	}

	void SetWoodLeft(int wood)
	{
		m_WoodLeft = wood;
	}

	override bool IsBuilding()
	{
		return true;
	}
};