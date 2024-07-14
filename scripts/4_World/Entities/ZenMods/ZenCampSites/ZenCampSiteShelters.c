class ZenCamps_ShelterStick extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);
		}
	};

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}
};

class ZenCamps_ShelterFabric extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);
		}
	};

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}
};

class ZenCamps_ShelterLeather extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);
		}
	};

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}
};
