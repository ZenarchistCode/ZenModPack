class ViewInventoryHandlerBase
{
    protected PlayerBase m_Player;
    protected HumanCommandActionCallback m_Callback
    protected bool m_IsViewingInventorySynced = false;

    void ViewInventoryHandlerBase( PlayerBase player )
    {
        m_Player = player;
    }

    bool PlayAnimationCondition( bool cancel_condition = false )
    {
        if ( !m_IsViewingInventorySynced || m_Player.IsRestrained() || m_Player.IsRestrainStarted() )
        {
            return false;
        }
        if ( !cancel_condition && m_Player.GetCommandModifier_Action() )
        {
            return false;
        }
        if ( cancel_condition && !ViewInventoyCommandActionCallback.Cast( m_Player.GetCommandModifier_Action() ) )
        {
            return false;
        }
        if ( m_Player.GetActionManager() && ( m_Player.GetActionManager().GetRunningAction() || m_Player.GetActionManager().HasPendingAction() ) )
        {
            return false;
        }
        if ( m_Player.GetDayZPlayerInventory() && m_Player.GetDayZPlayerInventory().IsProcessing() )
        {
            return false;
        }
        if ( m_Player.GetWeaponManager() && m_Player.GetWeaponManager().IsRunning() )
        {
            return false;
        }
        ItemBase hands = m_Player.GetItemInHands();
        if ( hands && hands.IsHeavyBehaviour() )
        {
            return false;
        }
        return true;
    }

    void OnUpdate( int pCurrentCommandID, float pDt )
    {
        if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE )
        {
            if ( m_Callback )
            {
                // for some reason the cancel condition of the command callback is not realiable enough..
                if ( !PlayAnimationCondition( true ) )
                {
                    m_Callback.Cancel();
                    m_Callback = null;
                }
            }
            else
            {
                if ( !PlayAnimationCondition() )
                {
                    return;
                }

                m_Callback = m_Player.AddCommandModifier_Action( DayZPlayerConstants.CMD_ACTIONMOD_SEARCHINVENTORY, ViewInventoyCommandActionCallback );
            }
        }
    }

    bool OnInputUserDataProcess( ParamsReadContext ctx )
    {
        return false;
    }
}