class ActionZenSleepOnBagCB : ActionInteractBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CASingleUse;
	}

	override void InitActionComponent()
    {
        // ZenSleep compatibility
        #ifdef ZenSleep
        if (m_ActionData.m_Player)
        {
            m_ActionData.m_Player.ResetSleep();
        }
        #endif

        if (m_ActionData.m_Target && m_ActionData.m_Target.GetObject())
        {
            // Set player orientation to match bag - don't teleport their position to center of bag as it can be exploited for raids etc.
            vector sleepOrientation = m_ActionData.m_Target.GetObject().GetOrientation() + "-90 0 0";
            m_ActionData.m_Player.SetOrientation(sleepOrientation);
        }

        super.InitActionComponent();
    }
}

class ActionZenSleepOnBag : ActionInteractBase
{
    void ActionZenSleepOnBag()
    {
		m_CallbackClass = ActionZenSleepOnBagCB;
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
    }

    override string GetText()
    {
        return "#STR_USRACT_ID_EMOTE_LYINGDOWN";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (player.GetItemInHands() || !target || !target.GetObject() || !target.GetObject().IsKindOf("ZenSleepingBag_DeployedBase"))
            return false;

        float distance = vector.Distance(target.GetObject().GetPosition(), player.GetPosition());
        return distance <= 0.5;
    }

    override void OnEndClient(ActionData action_data)
    {
        if (action_data.m_Player.GetEmoteManager())
        {
            action_data.m_Player.GetEmoteManager().CreateEmoteCBFromMenu(EmoteConstants.ID_EMOTE_LYINGDOWN);
        }
    }
}