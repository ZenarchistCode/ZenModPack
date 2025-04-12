modded class CAContinuousTimeCooking
{
	override int Execute(ActionData action_data)
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		int cooking_state_update = 0;
		float cooking_time = m_ItemToCook.GetFoodStage().GetCookingTime();
		if (m_AdjustedTimeToCook > cooking_time)
		{
			m_TimeElapsed += action_data.m_Player.GetDeltaT();
			m_TimeElapsedRepeat += action_data.m_Player.GetDeltaT();
			
			if (m_ItemToCook.GetTemperature() < m_MinTempToCook)
				m_AdjustedTimeToCook += action_data.m_Player.GetDeltaT();
			
			if (m_TimeElapsedRepeat >= m_AdjustedCookingUpdateTime)
			{
				cooking_state_update = m_CookingProcess.CookOnStick(m_ItemToCook, m_CookingUpdateTime);

				if (GetGame().IsServer() && m_ItemToCook.GetTemperature() >= m_MinTempToCook)
				{
					m_TimeElapsedDamage += m_TimeElapsedRepeat;

					if (m_TimeElapsedDamage >= TIME_DAMAGE_THRESHOLD)
					{
						m_TimeElapsedDamage = 0;
						action_data.m_MainItem.DecreaseHealth(GameConstants.FIRE_ATTACHMENT_DAMAGE_PER_SECOND * TIME_DAMAGE_THRESHOLD, false);
						ZenFunctions.DebugMessage("DAMAGE " + action_data.m_MainItem.GetType() + " by " + (GameConstants.FIRE_ATTACHMENT_DAMAGE_PER_SECOND * TIME_DAMAGE_THRESHOLD));
					}
				}
				
				if (cooking_state_update == 1)
				{
					Setup(action_data);
					
					if (!GetGame().IsServer())
					{
						if (m_ItemToCook.IsMeat() && m_ItemToCook.GetFoodStage().GetFoodStageType() == FoodStageType.BAKED)
						{
							GetGame().GetAnalyticsClient().OnActionCookedSteak();
						}
					}
				}
			
				m_TimeElapsedRepeat = 0;
			}				
					
			return UA_PROCESSING;
		}
		else
		{
			if (m_SpentUnits)
			{
				m_SpentUnits.param1 = m_TimeElapsed;
				SetACData(m_SpentUnits);
			}
			OnCompletePogress(action_data);
			return UA_FINISHED;
		}
	}
}