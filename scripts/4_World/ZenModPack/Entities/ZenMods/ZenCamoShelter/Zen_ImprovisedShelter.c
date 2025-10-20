// Actual shelter object
class Zen_ImprovisedShelter : DeployableContainer_Base
{
	// Save the state of the kit items (rope and camonet health)
	float m_RopeHealth = 0;
	float m_CamoNetHealth = 0;

	// Hide selections
	//override void EOnInit(IEntity other, int extra)
	//{
	//	super.EOnInit(other, extra);
	//}

	void SetIngredientHealth(float hp1, float hp2)
	{
		m_RopeHealth = hp1;
		m_CamoNetHealth = hp2;
	}

	float GetRopeHealth()
	{
		return m_RopeHealth;
	}

	float GetCamoNetHealth()
	{
		return m_CamoNetHealth;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionPackZenCamoShelter);
	}

	// Save shelter data
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		// Save ingredient health
		ctx.Write(m_RopeHealth);
		ctx.Write(m_CamoNetHealth);
	}

	// Load shelter data
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_RopeHealth))
		{
			m_RopeHealth = 0;
		}

		if (!ctx.Read(m_CamoNetHealth))
		{
			m_CamoNetHealth = 0;
		}

		return true;
	}
};
