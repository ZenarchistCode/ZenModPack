modded class ZenAdminCommandHandler
{
	override bool HandleCivilianCommand(PlayerBase player, string uid, string cmd, array<string> params)
	{
		if (super.HandleCivilianCommand(player, uid, cmd, params))
			return true;
			
		if (cmd == "restart" || cmd == "nextrestart")
		{
			if (GetZenUpdateMessage().RestartHours <= 0)
				return false;
				
			int RESTART_EVERY_MS = GetZenUpdateMessage().RestartHours * 60 * 60 * 1000; // 4h

			int uptimeMs 		= g_Game.GetTime(); // server uptime in ms
			int remMs    		= uptimeMs % RESTART_EVERY_MS;
			int msLeft 			= (RESTART_EVERY_MS - remMs);
			int totalMinLeft 	= (msLeft + 59999) / 60000; // round up to next minute

			string timeStr;
			if (totalMinLeft < 60)
			{
				timeStr = string.Format("%1 #STR_ZenGui_Minutes", totalMinLeft);
			}
			else
			{
				// round to nearest 0.5 hour for readability
				float hours = totalMinLeft / 60.0;
				float rounded = Math.Round(hours * 2.0) / 2.0;

				// show .0 only if needed
				if (Math.AbsFloat(rounded - Math.Floor(rounded)) < 0.01)
					timeStr = string.Format("%1 #STR_ZenGui_Hours", Math.Floor(rounded));
				else
					timeStr = string.Format("%1 #STR_ZenGui_Hours", rounded);
			}
			
			player.Zen_SendMessage("Next server restart is in " + timeStr);
			return true;
		}
		
		return false;
	}
	
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