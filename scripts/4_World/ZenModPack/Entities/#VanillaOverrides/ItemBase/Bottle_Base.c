modded class Bottle_Base
{
	//! SHARED 
	void Bottle_Base()
	{
		//! CATCH RAIN
		RegisterNetSyncVariableBool("m_IsZenCatchingRain");
		m_ZenRainProcComponent = new RainProcurementComponentZenBottle(this);
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
	protected ref RainProcurementComponentZenBottle m_ZenRainProcComponent;
	bool m_IsZenCatchingRain = false;

	void SetZenCatchingRain(bool b)
	{
		m_IsZenCatchingRain = b;
		SetSynchDirty();

		if (m_IsZenCatchingRain)
		{
			m_ZenRainProcComponent.StartRainProcurement();
		}
		else
		{
			m_ZenRainProcComponent.StopRainProcurement();
		}
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);

		SetZenCatchingRain(false);
	}

	bool IsZenCatchingRain()
	{
		return m_IsZenCatchingRain;
	}
}

//! CATCH RAIN 
class RainProcurementComponentZenBottle : RainProcurementComponentBase 
{
	override float GetBaseLiquidAmount()
	{
		return ZenRainWaterConstants.FILL_GROUND_SPEED; // Fill twice as fast as barrels
	}
}