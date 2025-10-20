modded class ActionManagerServer
{
    //! VIEW INVENTORY ANIMATION
    override bool HasPendingAction()
    {
        return m_PendingAction != null;
    }

    override void StartDeliveredAction()
    {
        ViewInventoyCommandActionCallback callback;
        if ( Class.CastTo( callback, m_Player.GetCommandModifier_Action() ) )
        {
            StartDeliveredAction2();
        }
        else
        {
            super.StartDeliveredAction();
        }
    }

    // same as StartDeliveredAction but doesnt check for running command modifier
    // NOTE: This must be checked with game updates. The logic below is taken from DayZ 1.18
    void StartDeliveredAction2()
    {
        if ( !m_CurrentActionData )
        {
            //! error - expected action data
            //Interrupt();
            return;
        }
        
        m_Interrupted = false;
        
        ActionBase picked_action;
        bool accepted = false;
        bool is_target_free = true;
        ActionTarget target;
        ItemBase item;
        
        picked_action = m_CurrentActionData.m_Action;
        target = m_CurrentActionData.m_Target;
        item = m_CurrentActionData.m_MainItem;

        if ( LogManager.IsActionLogEnable() )
        {
            Debug.ActionLog("Item = " + item + ", " + target.DumpToString(), picked_action.ToString() , "n/a", "DeliveredAction", m_Player.ToString() );
        }

        if ( is_target_free /*&& !m_Player.GetCommandModifier_Action()*/ && !m_Player.GetCommand_Action() && !m_Player.IsSprinting() && picked_action && picked_action.Can(m_Player,target,item)) 
        {
            accepted = true;
            if ( picked_action.HasTarget())
            {
                EntityAI targetEntity;
                if ( EntityAI.CastTo(targetEntity,target.GetObject()) )
                {
                    //Man target_owner;
                    //target_owner = targetEntity.GetHierarchyRootPlayer();
                    //if( target_owner && target_owner != m_Player && target_owner.IsAlive() )
                    //{
                    //    accepted = false;
                    //}
                    if ( !AdvancedCommunication.Cast(targetEntity) && !Building.Cast(targetEntity) && !Fireplace.Cast(targetEntity) )
                    {
                        //Lock target
                        if ( !GetGame().AddActionJuncture(m_Player,targetEntity,10000) )
                        {
                            accepted = false;
                        }
                    }
                }
            }
        }
        
        if ( accepted )
        {
            if ( LogManager.IsActionLogEnable() )
            {
                Debug.ActionLog("Action accepted", picked_action.ToString() , "n/a", "CheckDeliveredAction", m_Player.ToString() );
            }
            //Debug.Log("[AM] Action acccepted");
            if (picked_action.UseAcknowledgment())
            {
                        //Unlock target
                        //GetGame().ClearJuncture(m_Player, oldTrgetItem);
                m_CurrentActionData.m_State = UA_AM_PENDING;
                DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, true);
            }
            else
            {
                m_CurrentActionData.m_State = UA_AM_ACCEPTED;
            }
        }
        else
        {
            if ( LogManager.IsActionLogEnable() )
            {
                Debug.ActionLog("Action rejected", picked_action.ToString() , "n/a", "CheckDeliveredAction", m_Player.ToString() );
            }
            if (picked_action.UseAcknowledgment())
            {
                DayZPlayerSyncJunctures.SendActionAcknowledgment(m_Player, m_PendingActionAcknowledgmentID, false);
            }
            else
            {
                DayZPlayerSyncJunctures.SendActionInterrupt(m_Player);
            }
        }
    }
}