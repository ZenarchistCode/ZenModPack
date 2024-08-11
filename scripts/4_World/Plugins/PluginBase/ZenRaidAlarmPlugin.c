class ZenRaidAlarmPlugin extends PluginBase
{
    protected ref array<Zen_RaidAlarmStation> m_RaidStations;
    protected ref map<string, int> m_RaidTriggers; // Can't use vector map? Fuck it, use a string map then

    override void OnInit()
    {
        m_RaidStations = new array<Zen_RaidAlarmStation>;
        m_RaidTriggers = new map<string, int>;
    }

    void RegisterRaidStation(Zen_RaidAlarmStation station)
    {
        if (m_RaidStations.Find(station) == -1)
        {
            m_RaidStations.Insert(station);
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DelayedRegisterMessage, 1000, false, station);
        }
    }

    void UnregisterRaidStation(Zen_RaidAlarmStation station)
    {
        int idx = m_RaidStations.Find(station);
        if (idx != -1)
        {
            m_RaidStations.Remove(idx);
            Print("[ZenRaidAlarmPlugin] Unregistered raid station @ " + station.GetPosition());
        }
    }

    // CompEM Switches on before webhooks are loaded from storage .bin files which "registers" the raid alarm
    // before we have the webhooks ready, so I delay the registration message by 1 sec to allow time to load the webhooks
    void DelayedRegisterMessage(Zen_RaidAlarmStation station)
    {
        if (station != NULL)
        {
            Print("[ZenRaidAlarmPlugin] Registered raid station @ " + station.GetPosition() + " - Has Valid Webhook: " + station.HasValidWebhook());
        }
    }

    Zen_RaidAlarmStation GetNearestRaidStation(vector pos)
    {
        for (int i = 0; i < m_RaidStations.Count(); i++)
        {
            Zen_RaidAlarmStation station = m_RaidStations.Get(i);
            if (vector.Distance(station.GetPosition(), pos) <= GetZenDiscordConfig().RaidDetectionDistance)
                return station;
        }

        return NULL;
    }

    void AlertNearestRaidStation(vector objectPos)
    {
        // Trigger player webhooks (and admin webhooks if station is detected)
        Zen_RaidAlarmStation station = GetNearestRaidStation(objectPos);
        if (station)
            station.TriggerRaidAlarm();

        for (int i = 0; i < m_RaidTriggers.Count(); i++)
        {
            if (vector.Distance(m_RaidTriggers.GetKey(i).ToVector(), objectPos) <= GetZenDiscordConfig().RaidDetectionDistance)
            {
                // We've already been notified of this raid this session
                if (GetGame().GetTickTime() - m_RaidTriggers.GetElement(i) < 3600)
                    return;
            }
        }

        // Trigger admin webhooks - only if no station is detected. Otherwise we send the same player alert to the admins in TriggerRaidAlarm() ^
        if (GetZenDiscordConfig().TriggerAdminRaidAlert)
        {
            // Collect player IDs involved in raid (victims included)
            string playersInvolved = "";
            array<Man> onlinePlayers = new array<Man>;
		    GetGame().GetPlayers(onlinePlayers);
            foreach (Man man : onlinePlayers)
            {
                PlayerBase pb = PlayerBase.Cast(man);
                if (pb != NULL && vector.Distance(objectPos, pb.GetPosition()) <= GetZenDiscordConfig().RaidDetectionDistance)
                {
                    playersInvolved = playersInvolved + "[" + pb.GetIdentity().GetName() + "](http://steamcommunity.com/profiles/" + pb.GetIdentity().GetPlainId() + ")" + " (" + pb.GetIdentity().GetId() + ")\n";
                }
            }

            string mapLink = "";
		    if (GetZenDiscordConfig().MapURL == "")
			    mapLink = " @ " + objectPos[0] + " / " + objectPos[2];
            else
		        mapLink = "@ [" + objectPos[0] + " / " + objectPos[2] + "](" + GetZenDiscordConfig().MapURL + "#location=" + objectPos[0] + ";" + objectPos[2] + ")";

            // Craft alert 
            ZenDiscordMessage msg = new ZenDiscordMessage(GetZenDiscordConfig().RaidAlarmMessageTitle, true);
		    msg.SetTitle(GetZenDiscordConfig().RaidAlarmMessageTitle);
		    msg.SetMessage("A raid is occuring:\n\n" + mapLink + "\n\nPlayers Involved:\n" + playersInvolved);
		    msg.SetColor(255, 0, 0);
		    msg.AddWebhooks(GetZenDiscordConfig().AdminWebhooks);
		    GetZenDiscordAPI().SendMessage(msg);
            m_RaidTriggers.Set(objectPos.ToString(false), GetGame().GetTickTime());
        }
    }
}