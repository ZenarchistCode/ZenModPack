class ActionZenPackSleepingBagCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(3);
    }
}

class ActionZenPackSleepingBag : ActionContinuousBase
{
    void ActionZenPackSleepingBag()
    {
        m_CallbackClass = ActionZenPackSleepingBagCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
        m_FullBody = true;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
        m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
    }

    override void CreateConditionComponents()
    {
        m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
        m_ConditionItem = new CCINotPresent;
    }

    override string GetText()
    {
        return "#pack_tent";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        Object targetObject = target.GetObject();
        if (targetObject && targetObject.IsKindOf("ZenSleepingBag_DeployedBase"))
        {
            ZenSleepingBag_DeployedBase bag;
            Class.CastTo(bag, targetObject);

            if (bag.GetNumberOfItems() == 0)
            {
                return true;
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
                m_Object.SetPosition(action_data.m_Player.GetPosition());
                m_Object.SetHealth(action_data.m_Target.GetObject().GetHealth() - 5); // 250hp = 50 placements if pristine
                GetGame().ObjectDelete(action_data.m_Target.GetObject());
            }
        }
    }
}
