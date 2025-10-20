modded class ActionManagerBase
{
    override bool ActionPossibilityCheck(int pCurrentCommandID)
    {
        if (ViewInventoyCommandActionCallback.Cast(m_Player.GetCommandModifier_Action()))
        {
            return ActionPossibilityCheck2(pCurrentCommandID);
        }
        else
        {
            return super.ActionPossibilityCheck(pCurrentCommandID);
        }
    }

    // same as ActionPossibilityCheck but doesnt check for running command modifier
    // NOTE: This must be checked with game updates. The logic below is taken from DayZ 1.28
    bool ActionPossibilityCheck2(int pCurrentCommandID)
    {
        if (!m_ActionsEnabled || m_Player.IsSprinting() || m_Player.IsUnconscious() /*||m_Player.GetCommandModifier_Action()*/ || m_Player.GetCommand_Action() || m_Player.IsEmotePlaying() || m_Player.GetThrowing().IsThrowingAnimationPlaying() || m_Player.GetDayZPlayerInventory().IsProcessing() || m_Player.IsItemsToDelete())
            return false;

        if (m_Player.GetWeaponManager().IsRunning() || m_Player.GetThrowing().IsThrowingAnimationPlaying() || m_Player.GetDayZPlayerInventory().IsProcessing() || m_Player.IsItemsToDelete() || m_Player.IsRolling())
			return false;

        return (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE || pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM || pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER || pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE);
    }


    // From 1.18 version in case ^ updated code breaks something
    /*
    bool ActionPossibilityCheck2(int pCurrentCommandID)
    {
        if (!m_ActionsEnabled || m_Player.IsSprinting() || m_Player.IsUnconscious() || m_Player.GetCommand_Action() || m_Player.IsEmotePlaying() || m_Player.GetThrowing().IsThrowingAnimationPlaying() || m_Player.GetDayZPlayerInventory().IsProcessing() || m_Player.IsItemsToDelete())
            return false;

        if (m_Player.GetWeaponManager().IsRunning())
            return false;

        return (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE || pCurrentCommandID == DayZPlayerConstants.COMMANDID_SWIM || pCurrentCommandID == DayZPlayerConstants.COMMANDID_LADDER || pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE);
    }
    */

    bool HasPendingAction()
    {
        return false;
    }
}