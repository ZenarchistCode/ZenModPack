modded class Bottle_Base
{
	//! SHARED 
	void Bottle_Base()
	{
		//! CATCH RAIN
		RegisterNetSyncVariableBool("m_IsCatchingRain");
		m_RainProcComponent = new RainProcurementComponentBottle(this);
	}

	override void SetActions()
	{
		super.SetActions();

		//! FIRE FUEL
		AddAction(ActionZenPourFuelOnFire);
		AddAction(ActionZenPourFuelOnFirewood);

		//! CATCH RAIN 
		AddAction(ActionZenFillBottleRainBase);
		AddAction(ActionZenFillRainGround);
	}

	//! CATCH RAIN
	protected ref RainProcurementComponentBottle m_RainProcComponent;
	bool m_IsCatchingRain = false;

	void SetCatchingRain(bool b)
	{
		m_IsCatchingRain = b;
		SetSynchDirty();

		if (m_IsCatchingRain)
		{
			m_RainProcComponent.StartRainProcurement();
		}
		else
		{
			m_RainProcComponent.StopRainProcurement();
		}
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);

		SetCatchingRain(false);
	}

	bool IsCatchingRain()
	{
		return m_IsCatchingRain;
	}
}

//! CATCH RAIN 
class RainProcurementComponentBottle : RainProcurementComponentBase 
{
	override float GetBaseLiquidAmount()
	{
		return ZenRainWaterConstants.FILL_GROUND_SPEED; // Fill twice as fast as barrels
	}
}