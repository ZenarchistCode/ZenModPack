//! Handles all "!chatmsg" text input from client.
class ZenAdminMessagePlugin extends PluginBase
{
    protected float m_DeltaTime = 0;

    bool HandleAdminCommand(notnull PlayerIdentity identity, string text)
    {
        Print("[ZenAdminMessagePlugin] Received admin command: " + text + " from: " + identity.GetId());

        // Get player object
        int highBits, lowBits;
		GetGame().GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), lowBits, highBits);
		PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));
        if (!player)
        {
            Error("[ZenDiscordAPI] Failed to retrieve player network object in ZenAdminMessagePlugin.c from uid -> " + identity.GetId());
            return false;
        }

        // Strip '!'
        text = text.Substring(1, text.Length() - 1);
        string message = "";

        // Get message/parameters if they exist
        int spaceIndex = text.IndexOf(" ");
        if (spaceIndex != -1)
        {
            // Calculate the length of the message substring
            int messageLength = text.Length() - (spaceIndex + 1);
            message = text.Substring(spaceIndex + 1, messageLength);
        }

        // Get command
        string command;
        if (spaceIndex != -1)
        {
            command = text.Substring(0, spaceIndex);
        }
        else
        {
            command = text;
        }

        command.ToLower();
        array<string> params = new array<string>;
        string parameters = message;
        parameters.ToLower();
        parameters.Split(" ", params);

        // Act on command if recognized
        if (ProcessCommand(player, command, message, params))
            return true;

        Print("[ZenAdminMessagePlugin] Message was not handled.");
        return false;
    }

    // Process regular civilian commands
    bool ProcessCommand(notnull PlayerBase player, string command, string text, array<string> params)
    {
        if (command == "")
            return false;

        // Check admin privileges first
        if (IsAdmin(player.GetCachedID()))
        {
            if (ProcessAdminCommand(player, command, text, params))
                return true;
        }

        // !admin - sends admin request to server
        if (command == GetZenDiscordConfig().PingAdminCommand)
        {
            string profileName = player.GetIdentity().GetName();
            string uid = player.GetCachedID();
            string steamid = player.GetIdentity().GetPlainId();
            string from = "[" + profileName + "](http://steamcommunity.com/profiles/" + steamid + ")\n" + uid + "\n" + steamid;
            string status = GetZenDiscordConfig().GetMapLinkPosition(player.GetPosition()) + "\nHP=" + (player.GetHealth01() * 100) + "%";

            string title = command;
            title.ToUpper();

            // Build discord notification string
            ZenDiscordMessage msg = new ZenDiscordMessage(title);
		    msg.SetTitle(title);
		    msg.SetMessage(text + "\n\n" + from + "\n\n" + status);
		    msg.SetColor(255, 160, 0);
		    msg.AddWebhooks(GetZenDiscordConfig().AdminWebhooks);
		    GetZenDiscordAPI().SendMessage(msg);

            // Notify player
			MsgPlayer(player, GetZenDiscordConfig().MessageReceived);
            return true;
        }

        // Server FPS check (!TODO: Add setting to restrict this to admins only)
        if (command == GetZenUtilitiesConfig().FpsChatCommand)
        {
            string uptime = "Uptime: " + Math.Round(GetGame().GetTickTime() / 60);
            MsgPlayer(player, "Current Server FPS: " + GetZenServerFPS() + " | Session Average FPS: " + GetZenAvgServerFPS() + " | " + uptime + " minutes");
            return true;
        }

        return false;
    }

    // Process admin commands
    bool ProcessAdminCommand(notnull PlayerBase player, string command, string text, array<string> params)
    {
        // Server FPS check (ADMINS)
        if (command == GetZenUtilitiesConfig().FpsChatCommand)
        {
            string uptime = "Uptime: " + Math.Round(GetGame().GetTickTime() / 60);
            MsgPlayer(player, "Current Server FPS: " + GetZenServerFPS() + " | Session Average FPS: " + GetZenAvgServerFPS() + " | " + uptime + " minutes");
            string msg = "Players: " + PlayerBase.ZENMOD_PLAYER_COUNT + " Items: " + ItemBase.ZENMOD_ITEM_COUNT;
            msg = msg + " Animals: " + AnimalBase.ZENMOD_ANIMAL_COUNT + " Zombies: " + ZombieBase.ZENMOD_ZOMBIE_COUNT;
            msg = msg + " Drivers: " + PlayerBase.ZENMOD_DRIVER_COUNT;
#ifdef EXPANSIONMODAI
            msg = msg + " eAI: " + eAIBase.ZENMOD_AI_COUNT;
#endif
            NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 15.0, "SERVER PERFORMANCE", msg, "");
            return true;
        }

        // Safespot (teleports admin to testing bunker on Livonia - replace with your own safespot coords if you have one)
        if (command == "safespot")
        {
            // Rewrite command to teleport to safespot
            MsgPlayer(player, "Teleporting to safespot");
            player.SetPosition("580.643005 592.011536 1153.810913");
            return true;
        }

        // Tele
        if (command == "tele")
        {
            vector teleSpot = player.GetPosition();

            if (params.Count() == 2)
            {
                float x = params.Get(0).ToFloat();
                float z = params.Get(1).ToFloat();
                teleSpot[0] = x;
                teleSpot[2] = z;
                teleSpot[1] = GetGame().SurfaceY(x, z);
            }
            else
            if (params.Count() == 3)
            {
                teleSpot = text.ToVector();
            }
            else
            {
                teleSpot = "0 0 0";
            }

            if (teleSpot[0] == 0 && teleSpot[2] == 0)
            {
                MsgPlayer(player, "Incorrect syntax - use !tele X Z or !tele X Y Z");
                return true;
            }

            MsgPlayer(player, "Teleporting to " + teleSpot);
            player.SetPosition(teleSpot);
            return true;
        }

        // Mod enable/disable 
        if (command == "zenmod")
        {
            if (params.Count() != 2)
            {
                MsgPlayer(player, "Incorrect syntax - use:    !mod MODNAME on    or    !mod MODNAME off");
                return true;
            }

            string modName = params.Get(0);
            string paramEnable = params.Get(1);
            paramEnable.ToLower();
            bool enable = paramEnable == "on";

            if (SetZenModEnabled(modName, enable))
            {
                MsgPlayer(player, modName + " set to " + enable);

                // Re-sync config
                ResyncConfig();
            }
            else
            {
                MsgPlayer(player, modName + " mod not found!");
            }

            return true;
        }

        // Reload config 
        if (command == "reload")
        {
            bool reload = false;

            if (params.Get(0) == "discord")
            {
                GetZenDiscordConfig().Load();
                reload = true;
            }
            else if (params.Get(0) == "utilities")
            {
                GetZenUtilitiesConfig().Load();
                reload = true;
            }

            if (reload)
            {
                MsgPlayer(player, "Reloaded " + params.Get(0) + " config");
                ResyncConfig();
            }
            else
            {
                MsgPlayer(player, "Couldn't find " + params.Get(0) + " config - valid options: discord, utilities, weather");
            }

            return true;
        }

        return false;
    }

    // Trigger ProcessUpdate() once per second
    override void OnUpdate(float delta_time)
    {
        m_DeltaTime += delta_time;
        if (m_DeltaTime < 1)
            return;

        m_DeltaTime = 0;
        ProcessUpdate();
    }
    
    // Override this to do anything with this plugin, unless you need milisecond precision then override above
    void ProcessUpdate() {};

    // Sends a text message to the given player
    protected void MsgPlayer(notnull PlayerBase player, string msg)
    {
        GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, new Param1<string>(msg), true, player.GetIdentity()); 
    }

    // Check if given player id is an admin
    bool IsAdmin(string uid)
    {
        for (int i = 0; i < GetZenDiscordConfig().AdminUIDs.Count(); i++)
        {
            if (GetZenDiscordConfig().AdminUIDs.Get(i) == uid)
                return true;
        }

        return true;
    }

    // Re-sync ZenModPack config to all online players
    void ResyncConfig()
    {
        array<Man> players();
        PlayerBase pb;

        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            pb = PlayerBase.Cast(players.Get(i));
            if (pb)
            {
                GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenModPackConfig", new Param1<ref map<string, bool>>(GetZenModPackConfig().ModEnabled), true, pb.GetIdentity());
            }
        }
    }
}