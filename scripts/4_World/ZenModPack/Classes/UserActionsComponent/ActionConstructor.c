modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        //! KNIFE GARDEN PLOTS 
        actions.Insert(ActionCreateGreenhouseGardenPlotZenKnife);

        //! CAMO SHELTER
        actions.Insert(ActionPackZenCamoShelter);

        //! CAUSE OF DEATH 
        actions.Insert(ActionZenCheckCauseOfDeath);

        //! CHICKEN COOPS
        actions.Insert(ActionZenMineCoopHands);

        //! FIREWOOD
        actions.Insert(ActionMineZenWoodStackHand);
		actions.Insert(ActionMineZenWoodStackTool);

        //! FIRE FUEL 
        actions.Insert(ActionZenPourFuelOnFire);
		actions.Insert(ActionZenPourFuelOnFirewood);

        //! OPEN CANS ON ROCKS 
        actions.Insert(ActionZenOpenCanEnv);

        //! REPAIR PUMPS
        actions.Insert(ActionZenRepairPump);

        //! REPAIR WELLS 
        actions.Insert(ActionZenRepairWell);

        //! LEFTOVERS 
        actions.Insert(ActionZenGetJunkPaper);

        //! CATCH RAIN 
        actions.Insert(ActionZenWashHandsRain);
		actions.Insert(ActionZenFillBottleRainBase);
		actions.Insert(ActionZenFillRainGround);

        //! IMMERSIVE SODAS 
        actions.Insert(ActionZenEmptySodaCan);

        //! ENGRAVE WEAPON
        actions.Insert(ActionZenEngraveWeapon);

        //! FLINT 
        actions.Insert(ActionPrepareZenFlint);

        //! UTILS 
        actions.Insert(ActionZenDumpNearbyObjects);
    }
}