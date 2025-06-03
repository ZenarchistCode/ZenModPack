class ActionCreateGreenhouseGardenPlotZenKnifeCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_GARDEN * 4.0);
	}
};

class ActionCreateGreenhouseGardenPlotZenKnife extends ActionCreateGreenhouseGardenPlot
{
	void ActionCreateGreenhouseGardenPlotZenKnife()
	{
		m_CallbackClass		= ActionCreateGreenhouseGardenPlotZenKnifeCB;
		m_FullBody			= true;
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONFB_DIGMANIPULATE;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight 	= UASoftSkillsWeight.ROUGH_LOW;
		m_Text = "#make_garden_plot";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenKnifeGardens"))
			return false;

		if ( !target ) 
			return false;
		
		if ( player.IsPlacingLocal() )
			return false;
		
		if (!CfgGameplayHandler.GetDisableColdAreaPlacementCheck() && player.GetInColdArea())
			return false;

		Object target_object = target.GetObject();
		
		Land_Misc_Greenhouse greenHouse = Land_Misc_Greenhouse.Cast( target_object );
		Land_Misc_Polytunnel polytunnel = Land_Misc_Polytunnel.Cast( target_object) ;
		
		if ( target_object && ( greenHouse || polytunnel ) )
		{
			string action_selection = target_object.GetActionComponentName( target.GetComponentIndex() );
			//Update selections in model, name the desired part Soil to improve action condition check
			if ( action_selection != "soil" )
				return false;
			
			//check if there is any gardenplot objects in the current building
			ref array<Object> nearest_objects = new array<Object>;
			ref array<CargoBase> proxy_cargos = new array<CargoBase>;
			vector pos = target_object.GetPosition();
			pos[1] = pos[1] - 1; //Lower by one meter for check if plot already present
			GetGame().GetObjectsAtPosition3D( pos, 2, nearest_objects, proxy_cargos );
	
			for ( int i = 0; i < nearest_objects.Count(); ++i )
			{
				Object object = nearest_objects.Get( i );
				
				if ( object.IsInherited( GardenPlot ) )
				{
					return false;
				}
			}
			
			return true;
		}
		
		return false;
	}

	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if (super.SetupAction(player, target, item, action_data, extra_data))
		{
			if (item)
			{
				SetDiggingAnimation(item);
			}

			return true;
		}

		return false;
	}

	void SetDiggingAnimation(ItemBase item)
	{
		if (item.KindOf("Knife") || item.KindOf("Screwdriver"))
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_1HD;
			m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		}
		else
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGMANIPULATE;
			m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		}
	}
}