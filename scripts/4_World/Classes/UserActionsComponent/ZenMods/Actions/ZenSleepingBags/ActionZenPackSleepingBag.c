class ActionZenPackSleepingBagCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(5);
    }
}

class ActionZenPackSleepingBag : ActionContinuousBase
{
    void ActionZenPackSleepingBag()
    {
        m_CallbackClass = ActionZenPackSleepingBagCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
        m_ConditionItem = new CCINotPresent;
    }

    override string GetText()
    {
        return "#pack_tent";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object targetObject = target.GetObject();
        if (targetObject && targetObject.IsInherited(ZenSleepingBag_DeployedBase))
        {
            ZenSleepingBag_DeployedBase bag;
            if (Class.CastTo(bag, targetObject))
            {
                return true; //return bag.GetNumberOfItems() == 0;
            }
        }

        return false;
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        Object targetObject = action_data.m_Target.GetObject();
        ZenSleepingBag_DeployedBase bag;
        if (Class.CastTo(bag, targetObject))
        {
            string newType = targetObject.GetType();
            newType.Replace("_Deployed", "_Packed");

            Object m_Object = GetGame().CreateObject(newType, targetObject.GetPosition(), false);
            if (m_Object)
            {
                // Drop items in bag - sometimes the bag inventory can't be seen in weird spots, so allow packing it with gear inside but just drop the gear.
                MiscGameplayFunctions.DropAllItemsInInventoryInBounds(bag, vector.Zero);
                m_Object.SetPosition(action_data.m_Player.GetPosition());
                m_Object.SetHealth(action_data.m_Target.GetObject().GetHealth() - 5); // 250hp = 50 placements if pristine
                GetGame().ObjectDelete(action_data.m_Target.GetObject());
            }
        }
    }
}
