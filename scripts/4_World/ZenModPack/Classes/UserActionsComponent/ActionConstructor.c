modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        //! KNIFE GARDEN PLOTS 
        actions.Insert(ActionCreateGreenhouseGardenPlotZenKnife);

        //! CRAFT RAG HANDS
        actions.Insert(ActionZenCraftRagsWithHands);

        //! CAMO SHELTER
        actions.Insert(ActionPackZenCamoShelter);

        //! OPEN CANS ON ROCKS 
        actions.Insert(ActionZenOpenCanEnv);

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