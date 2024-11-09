class ZenSleepingBagStatic_Base extends BuildingSuper
{
    void ZenSleepingBagStatic_Base()
    {
        SetAllowDamage(false);
    }
    
    override void SetActions()
    {
        super.SetActions();

        AddAction(ActionZenSleepOnBag);
    }

    override bool IsBuilding()
    {
        return false;
    }
}

class ZenSleepingBagStatic_Black extends ZenSleepingBagStatic_Base {};
class ZenSleepingBagStatic_Blue extends ZenSleepingBagStatic_Base {};
class ZenSleepingBagStatic_Grey extends ZenSleepingBagStatic_Base {};
class ZenSleepingBagStatic_Red extends ZenSleepingBagStatic_Base {};
class ZenSleepingBagStatic_Green extends ZenSleepingBagStatic_Base {};