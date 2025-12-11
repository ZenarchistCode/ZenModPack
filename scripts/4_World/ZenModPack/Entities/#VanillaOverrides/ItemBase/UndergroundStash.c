modded class UndergroundStash
{
	#ifdef SERVER
	override void DeferredInit() 
	{
		super.DeferredInit();
		
		ZenCheckStashType();
	}
	
	// For switching from winter map -> grassy map. Auto-convert any stashes from the previous map type.
	void ZenCheckStashType()
	{
		if (!ZenModEnabled("ZenAutoConvertStash"))
			return;
		
		// Only check vanilla stash types.
		if (GetType() != "UndergroundStashSnow" && GetType() != "UndergroundStash")
			return;
		
		int liquidType;
		string surfaceType;
		GetGame().SurfaceUnderObject(this, surfaceType, liquidType);
		
		string undergroundStashType;
		GetGame().GetSurfaceDigPile(surfaceType, undergroundStashType);
		
		if (GetType() == undergroundStashType)
			return;
	
		Print("[ZenModPack] " + GetType() + " does not match .cpp type: " + undergroundStashType + " @ " + GetPosition() + " - shapeshifting.");
		UndergroundStash stash = UndergroundStash.Cast(GetGame().CreateObjectEx(undergroundStashType, GetPosition(), ECE_PLACE_ON_SURFACE));  
		if (!stash)
		{
			Error("Failed to create stash " + undergroundStashType);
			return;
		}

		stash.SetPosition(GetPosition());
		stash.SetOrientation(GetOrientation());
		stash.SetDirection(GetDirection());
		stash.SetLifetimeMax(GetLifetimeMax());
		stash.SetLifetime(GetLifetime());
		
		ZenFunctions.MoveInventory(this, stash);
	}
	#endif
}