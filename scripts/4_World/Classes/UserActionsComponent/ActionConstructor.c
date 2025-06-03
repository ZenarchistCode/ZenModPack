modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        //! KNIFE GARDEN PLOTS 
        actions.Insert(ActionCreateGreenhouseGardenPlotZenKnife);

        //! ZIPPO 
        actions.Insert(ActionRefuelZenZippo);

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

        //! NOTES 
        actions.Insert(ActionZenWritePaper);
        actions.Insert(ActionZenReadNoteHands);
        actions.Insert(ActionZenReadNoteTarget);

        //! MUSIC 
        actions.Insert(ActionToggleMusic);
        actions.Insert(ActionMusicVolUp);
        actions.Insert(ActionMusicVolDn);
        actions.Insert(ActionBoomboxVolUp);
        actions.Insert(ActionBoomboxVolDown);
        actions.Insert(ActionStaticBoomboxTurnOn);
        actions.Insert(ActionStaticBoomboxTurnOff);
        actions.Insert(ActionStaticBoomboxNextSong);

        //! SWISS KNIFE
        actions.Insert(ActionZenSwissKnifeConvert_BaseTool);
        actions.Insert(ActionZenSwissKnifeConvert_Knife);
        actions.Insert(ActionZenSwissKnifeConvert_CanOpener);
        actions.Insert(ActionZenSwissKnifeConvert_Screwdriver);
        actions.Insert(ActionZenSwissKnifeConvert_SmallSaw);
        actions.Insert(ActionZenSwissKnifeConvert_Lockpick);
        actions.Insert(ActionZenSwissKnifeConvert_Scissors);

        //! RAID ALARM
        actions.Insert(ActionTurnOffZenRaidAlarm);
        actions.Insert(ActionTurnOnZenRaidAlarm);
        actions.Insert(ActionViewRaidAlarmGUI);
        actions.Insert(ActionBuildZenRaidAlarmStation);
        actions.Insert(ActionDismantleZenRaidAlarmStation);
        actions.Insert(ActionDismantleZenRaidAlarmRadar);
        actions.Insert(ActionFoldZenRaidAlarmKit);

        //! ENGRAVE WEAPON
        actions.Insert(ActionZenEngraveWeapon);

        //! FLINT 
        actions.Insert(ActionPrepareZenFlint);

        //! SHOVE RESTRAINED PLAYER 
        actions.Insert(ActionZenShovePlayer);
    }
}