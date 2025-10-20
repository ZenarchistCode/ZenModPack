modded class ActionManagerClient
{
    override bool HasPendingAction()
    {
        return m_PendingActionData != null || GetActionState() != UA_NONE;
    }
}