modded class ZenAdminCommandHandler
{
    override bool HandleAdminCommand(PlayerBase player, string uid, string cmd, array<string> params)
    {
		if (super.HandleAdminCommand(player, uid, cmd, params))
			return true;
		
        if (cmd == "zenmod")
        {
            if (params.Count() != 2)
            {
                SendMsg(player, "Incorrect syntax - use:    !mod MODNAME on    or    !mod MODNAME off");
                return true;
            }

            string modName = params.Get(0);
            string paramEnable = params.Get(1);
            paramEnable.ToLower();
            bool enable = paramEnable == "on";

            if (SetZenModEnabled(modName, enable))
            {
                SendMsg(player, modName + " set to " + enable);

                // Re-sync config
                GetZenModPackConfig().SyncToClient();
            }
            else
            {
                SendMsg(player, modName + " mod not found!");
            }

            return true;
        }

        return false;
    }
}