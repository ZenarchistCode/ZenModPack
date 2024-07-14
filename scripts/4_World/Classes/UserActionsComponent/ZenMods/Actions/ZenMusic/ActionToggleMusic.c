class ActionToggleMusic : ActionInteractBase
{
	void ActionToggleMusic()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_HEADLIGHT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	};

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	};

	override typename GetInputType()
	{
		return MusicActionInput;
	};

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				CarScript car;
				if (Class.CastTo(car, trans))
				{
					if (car.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_DRIVER || car.CrewMemberIndex(player) == DayZPlayerConstants.VEHICLESEAT_CODRIVER)
					{
						EntityAI battery = null;

						if (car.IsVitalCarBattery())
						{
							CarBattery carBattery = CarBattery.Cast(car.FindAttachmentBySlotName("CarBattery"));
							if (!carBattery || !carBattery.GetCompEM() || !carBattery.GetCompEM().GetEnergy() > 0 || carBattery.IsRuined())
								return false;
						}

						if (car.IsVitalTruckBattery())
						{
							TruckBattery truckBattery = TruckBattery.Cast(car.FindAttachmentBySlotName("TruckBattery"));
							if (!truckBattery || !truckBattery.GetCompEM() || !truckBattery.GetCompEM().GetEnergy() > 0 || truckBattery.IsRuined())
								return false;
						}

						return true;
					}
					else
					{
						return true;
					}
				}
			}
		}
		return false;
	};

	override void OnExecuteServer(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			Transport trans = vehCommand.GetTransport();
			if (trans)
			{
				CarScript car;
				if (Class.CastTo(car, trans))
				{
					car.ToggleMusic();
				}
			}
		}
	};

	override bool CanBeUsedInVehicle()
	{
		return true;
	};
};