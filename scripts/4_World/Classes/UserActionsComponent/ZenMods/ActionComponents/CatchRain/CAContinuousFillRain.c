// Copied from CAContinuousFill.c to reduce chances of future vanilla updates breaking this mod (or this mod breaking vanilla).
class CAContinuousFillRain : CAContinuousBase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_ItemQuantity;
	protected float 				m_AdjustedQuantityFilledPerSecond;
	protected float 				m_QuantityFilledPerSecond;
	protected ref Param1<float>		m_SpentUnits;
	protected float 				m_TimeElpased;
	protected float 				m_DefaultTimeStep = 0.25;
	protected int 					m_liquid_type;

	void CAContinuousFillRain(float quantity_filled_per_second)
	{
		m_QuantityFilledPerSecond = quantity_filled_per_second;
		m_liquid_type = LIQUID_WATER;
	}

	override void Setup(ActionData action_data)
	{
		m_TimeElpased = 0;

		if (!m_SpentUnits)
		{
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{
			m_SpentUnits.param1 = 0;
		}

		m_ItemQuantity = action_data.m_MainItem.GetQuantity();
		m_TargetUnits = action_data.m_MainItem.GetQuantityMax() - action_data.m_MainItem.GetQuantity();
		m_AdjustedQuantityFilledPerSecond = action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus(m_QuantityFilledPerSecond, m_Action.GetSpecialtyWeight(), true);
	}

	override int Execute(ActionData action_data)
	{
		if (!action_data.m_Player || !action_data.m_MainItem)
		{
			return UA_ERROR;
		}

		if (action_data.m_MainItem.GetQuantity() >= action_data.m_MainItem.GetQuantityMax())
		{
			return UA_FINISHED;
		}
		else
		{
			m_AdjustedQuantityFilledPerSecond = action_data.m_Player.GetSoftSkillsManager().AddSpecialtyBonus(GetUpdatedFillSpeed(action_data), m_Action.GetSpecialtyWeight(), true);

			if (m_SpentQuantity_total < m_TargetUnits)
			{
				m_SpentQuantity += m_AdjustedQuantityFilledPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();

				if (m_TimeElpased >= m_DefaultTimeStep)
				{
					CalcAndSetQuantity(action_data);
					m_TimeElpased = 0;
				}

				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(action_data);
				OnCompletePogress(action_data);
				return UA_FINISHED;
			}
		}
	}

	override int Cancel(ActionData action_data)
	{
		if (!action_data.m_Player || !action_data.m_MainItem)
		{
			return UA_ERROR;
		}

		CalcAndSetQuantity(action_data);
		return UA_CANCEL;
	}

	override int Interrupt(ActionData action_data)
	{
		if (GetGame().IsDedicatedServer())
		{
			action_data.m_Player.GetSoftSkillsManager().AddSpecialty(UASoftSkillsWeight.PRECISE_LOW);
		}

		return super.Interrupt(action_data);
	}

	override float GetProgress()
	{
		return m_SpentQuantity_total / m_TargetUnits;
	}

	// Calculate and set quantity for bottle_base
	void CalcAndSetQuantity(ActionData action_data)
	{
		m_SpentQuantity_total += m_SpentQuantity;
		if (GetGame().IsDedicatedServer())
		{
			if (m_SpentUnits)
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}

			Liquid.FillContainerEnviro(action_data.m_MainItem, m_liquid_type, m_SpentQuantity, false); // False = no inject agents
		}

		m_SpentQuantity = 0;
	}

	// Update rain water fill speed based on player movement
	float GetUpdatedFillSpeed(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;

		float fill_speed = ZenRainWaterConstants.FILL_RAIN_SPEED; // Fill normal speed while jogging
		switch (player.m_MovementState.m_iMovement)
		{
			case DayZPlayerConstants.MOVEMENTIDX_WALK: // Fill faster when walking slowly
				fill_speed = fill_speed * ZenRainWaterConstants.FILL_MULTI_WALK;
				break;
			case DayZPlayerConstants.MOVEMENTIDX_IDLE: // Fill even faster when idle / still
				fill_speed = fill_speed * ZenRainWaterConstants.FILL_MULTI_IDLE;
				break;
		}

		// Fill faster if using a pot
		if (action_data.m_MainItem.IsKindOf("Pot"))
			fill_speed = fill_speed * ZenRainWaterConstants.FILL_MULTI_POT;

		return fill_speed;
	}
};