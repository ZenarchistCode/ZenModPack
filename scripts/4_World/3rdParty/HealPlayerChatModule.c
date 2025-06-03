#ifdef VPPADMINTOOLS

modded class HealPlayerChatModule
{
    override void ExecuteCommand(PlayerBase caller, array<Man> targets, array<string> args)
    {
        super.ExecuteCommand(caller, targets, args);

        if (caller == null) 
            return;

        foreach (Man target : targets)
        {
            PlayerBase playerTarget = PlayerBase.Cast(target);
            if (playerTarget != null)
            {
                // vpp admin tools doesn't heal player of sickness for some reason. Annoying as fuck when you're debugging a server and your char is always sick.
				if (playerTarget.m_AgentPool)
					playerTarget.m_AgentPool.RemoveAllAgents();
            }
        }
    }
}

#endif