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
        actions.Insert(ActionCheckCauseOfDeath);

        //! CHICKEN COOPS
        actions.Insert(ActionMineCoopHands);

        //! FIREWOOD
        actions.Insert(ActionMineZenWoodStackHand);
		actions.Insert(ActionMineZenWoodStackTool);

        //! FIRE FUEL 
        actions.Insert(ActionPourFuelOnFire);
		actions.Insert(ActionPourFuelOnFirewood);

        //! OPEN CANS ON ROCKS 
        actions.Insert(ActionOpenCanEnv);

        //! REPAIR PUMPS
        actions.Insert(ActionRepairPump);

        //! REPAIR WELLS 
        actions.Insert(ActionRepairWell);

        //! LEFTOVERS 
        actions.Insert(ActionGetJunkPaper);

        //! CATCH RAIN 
        actions.Insert(ActionWashHandsRain);
		actions.Insert(ActionFillBottleRainBase);
		actions.Insert(ActionFillRainGround);

        //! IMMERSIVE SODAS 
        actions.Insert(ActionEmptySodaCan);

        //! ENGRAVE WEAPON
        actions.Insert(ActionZenEngraveWeapon);

        //! FLINT 
        actions.Insert(ActionPrepareZenFlint);

        //! UTILS 
        actions.Insert(ActionZenDumpNearbyObjects);
    }
}