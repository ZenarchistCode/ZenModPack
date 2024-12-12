class ZenCamps_ShelterStick extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);

#ifdef GAMELABS 
			_registeredInstanceZenGL = new _Event("ZenCamps_ShelterStick", "tent", this);
			GetGameLabs().RegisterEvent(_registeredInstanceZenGL);
#endif
		}
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}

#ifdef GAMELABS 
	ref _Event _registeredInstanceZenGL;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGameLabs())
			return;

		if (_registeredInstanceZenGL)
		{
			GetGameLabs().RemoveEvent(_registeredInstanceZenGL);
		}
	}
#endif
}

class ZenCamps_ShelterFabric extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);

#ifdef GAMELABS 
			_registeredInstanceZenGL = new _Event("ZenCamps_ShelterFabric", "tent", this);
			GetGameLabs().RegisterEvent(_registeredInstanceZenGL);
#endif
		}
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}

#ifdef GAMELABS 
	ref _Event _registeredInstanceZenGL;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGameLabs())
			return;

		if (_registeredInstanceZenGL)
		{
			GetGameLabs().RemoveEvent(_registeredInstanceZenGL);
		}
	}
#endif
}

class ZenCamps_ShelterLeather extends ShelterStick
{
	override void EEInit() 
    {
        super.EEInit();

        if (GetGame().IsDedicatedServer()) 
        {
			SetAllowDamage(false);

#ifdef GAMELABS 
			_registeredInstanceZenGL = new _Event("ZenCamps_ShelterLeather", "tent", this);
			GetGameLabs().RegisterEvent(_registeredInstanceZenGL);
#endif
		}
	}

	override void SetActions()
	{
		super.SetActions();
		RemoveAction(ActionDeconstructShelter);
	}

#ifdef GAMELABS 
	ref _Event _registeredInstanceZenGL;

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGameLabs())
			return;

		if (_registeredInstanceZenGL)
		{
			GetGameLabs().RemoveEvent(_registeredInstanceZenGL);
		}
	}
#endif
}