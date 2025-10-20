class Zen_ChickenCoop extends BuildingSuper
{
	protected int m_FeathersLeft = 0;

	void Zen_ChickenCoop()
	{
		if (GetGame().IsDedicatedServer())
		{
			int minFeathers = GetZenChickenCoopsConfig().GetMinFeathers(this.GetType());
			int maxFeathers = GetZenChickenCoopsConfig().GetMaxFeathers(this.GetType());
			m_FeathersLeft = Math.RandomIntInclusive(minFeathers, maxFeathers);
		}
	}

	void ConsumeFeathers(int amount = 1)
	{
		m_FeathersLeft = m_FeathersLeft - amount;
	}

	int GetFeathersLeft()
	{
		return m_FeathersLeft;
	}

	void SetFeathersLeft(int featherCount)
	{
		m_FeathersLeft = featherCount;
	}

	override bool IsBuilding()
	{
		return true;
	}
};

class Zen_ChickenCoop2 extends Zen_ChickenCoop {};
class Zen_ChickenCoop_Debug extends Zen_ChickenCoop {};
class Zen_ChickenCoop2_Debug extends Zen_ChickenCoop {};