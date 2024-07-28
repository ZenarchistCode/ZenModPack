class ZenPerformanceMonitorPlugin extends PluginBase
{
    // General vars
    static const int MINIMUM_SECONDS_ONLINE = 120;
    protected float m_DeltaTime;
    protected bool m_MonitoringFPS;
    protected int m_LastDrivingWarning;

    // Action tracker
    protected int m_CurrentActionStartFPS;
    protected int m_CurrentActionLowestFPS;
    protected string m_CurrentActionClassName;

    // Lowest session FPS tracker
    protected int m_LowestFPS;
    protected int m_LowestFPS_PlayerCount;
    protected int m_LowestFPS_ItemCount;
    protected int m_LowestFPS_AnimalCount;
    protected int m_LowestFPS_ZombieCount;
    protected int m_LowestFPS_DrivingVehicles;
    protected int m_ActionLowestFPS;
    protected string m_ActionLowestFPSClassName;

    // Highest session FPS tracker
    protected int m_HighestFPS;
    protected int m_HighestFPS_PlayerCount;
    protected int m_HighestFPS_ItemCount;
    protected int m_HighestFPS_AnimalCount;
    protected int m_HighestFPS_ZombieCount;
    protected int m_HighestFPS_DrivingVehicles;

#ifdef EXPANSIONMODAI
    protected int m_LowestFPS_AICount;
    protected int m_HighestFPS_AICount;
#endif

    override void OnInit()
    {
        Print("[ZenPerformanceMonitorPlugin] :: OnInit");
        m_DeltaTime = 0;
        m_HighestFPS = 0;
        m_LowestFPS = 99999999;
        m_ActionLowestFPS = 99999999;
        m_LastDrivingWarning = 0;
        m_LastDrivingWarning = GetGame().GetTickTime();
        m_MonitoringFPS = false;
        m_ActionLowestFPSClassName = "";
    }

    override void OnDestroy()
    {
        if (GetGame().GetTickTime() >= MINIMUM_SECONDS_ONLINE)
        {
            string msg = GetPerformanceDump();
            Print(msg);
            ZenModLogger.LogEx(msg, "performance");

        }
    }

    string GetPerformanceDump()
    {
        string msg = "\n\n[ZenModPack::DayZGame] Session Performance Breakdown:\n\n";
        msg = msg + "\nRuntime: " + (GetGame().GetTickTime() / 60) + " minutes";
        msg = msg + "\nAverage FPS: " + GetZenAvgServerFPS();

        msg = msg + "\n\nLowest FPS: " + m_LowestFPS;
        msg = msg + "\nPlayer Count: " + m_LowestFPS_PlayerCount;
        msg = msg + "\nItem Count: " + m_LowestFPS_ItemCount;
        msg = msg + "\nAnimal Count: " + m_LowestFPS_AnimalCount;
        msg = msg + "\nZombie Count: " + m_LowestFPS_ZombieCount;
        msg = msg + "\nDriving Count: " + m_LowestFPS_DrivingVehicles;
#ifdef EXPANSIONMODAI
        msg = msg + "\nAI Count: " + m_LowestFPS_AICount;
#endif
        if (m_ActionLowestFPSClassName != "")
            msg = msg + "\nWorst Action: " + m_ActionLowestFPSClassName + "  (" + m_ActionLowestFPS + " FPS)";

        msg = msg + "\n\nHighest FPS: " + m_HighestFPS;
        msg = msg + "\nPlayer Count: " + m_HighestFPS_PlayerCount;
        msg = msg + "\nItem Count: " + m_HighestFPS_ItemCount;
        msg = msg + "\nAnimal Count: " + m_HighestFPS_AnimalCount;
        msg = msg + "\nZombie Count: " + m_HighestFPS_ZombieCount;
        msg = msg + "\nDriving Count: " + m_HighestFPS_DrivingVehicles;
#ifdef EXPANSIONMODAI
        msg = msg + "\nAI Count: " + m_HighestFPS_AICount;
#endif
        return msg + "\n\n";
    }

    override void OnUpdate(float delta_time)
    {
        m_DeltaTime += delta_time;
        if (m_DeltaTime < 1)
            return;

        m_DeltaTime = 0;
        ProcessUpdate();
    }

    void ProcessUpdate()
    {
        if (!m_MonitoringFPS) // Don't monitor FPS til server has been up for 2 minutes minimum
            m_MonitoringFPS = GetGame().GetTickTime() > MINIMUM_SECONDS_ONLINE;

        if (!m_MonitoringFPS)
            return;

        int fps = GetZenServerFPS();

        //! CHECK FPS WARNING
        if (m_MonitoringFPS && fps <= GetZenUtilitiesConfig().ServerFPSWarning)
        {
            Error("[ZenModPack::DayZGame] Server FPS low: " + fps + GetPerformanceDump());

            // Notify anyone driving of low FPS warning
            if (GetGame().GetTickTime() - m_LastDrivingWarning >= 120)
            {
                array<Man> players();
                PlayerBase pb;

                GetGame().GetPlayers(players);
                for (int i = 0; i < players.Count(); i++)
                {
                    pb = PlayerBase.Cast(players.Get(i));
                    if (pb != NULL && pb.IsInVehicle())
                    {
                        pb.Zen_SendMessage(string.Format(GetZenUtilitiesConfig().ServerFPSWarningMsg, fps));
                    }
                }

                m_LastDrivingWarning = GetGame().GetTickTime();
            }
        }

        //! TRACK LOWEST FPS
        if (fps < m_LowestFPS)
        {
            m_LowestFPS = fps;
            m_LowestFPS_PlayerCount = PlayerBase.ZENMOD_PLAYER_COUNT;
            m_LowestFPS_ItemCount = ItemBase.ZENMOD_ITEM_COUNT;
            m_LowestFPS_AnimalCount = AnimalBase.ZENMOD_ANIMAL_COUNT;
            m_LowestFPS_ZombieCount = ZombieBase.ZENMOD_ZOMBIE_COUNT;
            m_LowestFPS_DrivingVehicles = PlayerBase.ZENMOD_DRIVER_COUNT;
#ifdef EXPANSIONMODAI
            m_LowestFPS_AICount = eAIBase.ZENMOD_AI_COUNT;
#endif
        }

        //! TRACK HIGHEST FPS
        if (fps > m_HighestFPS)
        {
            m_HighestFPS = fps;
            m_HighestFPS_PlayerCount = PlayerBase.ZENMOD_PLAYER_COUNT;
            m_HighestFPS_ItemCount = ItemBase.ZENMOD_ITEM_COUNT;
            m_HighestFPS_AnimalCount = AnimalBase.ZENMOD_ANIMAL_COUNT;
            m_HighestFPS_ZombieCount = ZombieBase.ZENMOD_ZOMBIE_COUNT;
            m_HighestFPS_DrivingVehicles = PlayerBase.ZENMOD_DRIVER_COUNT;
#ifdef EXPANSIONMODAI
            m_HighestFPS_AICount = eAIBase.ZENMOD_AI_COUNT;
#endif
        }
    }

    void ActionBase_Start(ActionData action_data)
    {
        if (!m_MonitoringFPS)
            return;

        m_CurrentActionStartFPS = GetZenServerFPS();
        m_CurrentActionLowestFPS = m_CurrentActionStartFPS;
        m_CurrentActionClassName = action_data.m_Action.ClassName();
    }

    void ActionBase_OnUpdate(ActionData action_data)
    {
        if (!m_MonitoringFPS)
            return;

        int fps = GetZenServerFPS();
        if (fps < m_CurrentActionLowestFPS)
            m_CurrentActionLowestFPS = fps;
    }

    void ActionBase_End(ActionData action_data) 
    {
        if (!m_MonitoringFPS)
            return;

        float pctChange = (1 - (m_CurrentActionLowestFPS / m_CurrentActionStartFPS)) * 100;
        if (GetZenUtilitiesConfig().ServerActionFPSDropWarningPct > 0 && pctChange >= GetZenUtilitiesConfig().ServerActionFPSDropWarningPct)
        {
            string msg = "[ZenPerformanceMonitorPlugin] :: WARNING! ServerFPS dropped -" + pctChange + "% (low: " + m_CurrentActionLowestFPS + " / start: " + m_CurrentActionStartFPS + ") during action " + m_CurrentActionClassName;
            Error(msg);
            ZenModLogger.Log(msg, "performance");
        }

        if (m_CurrentActionLowestFPS < m_ActionLowestFPS)
        {
            m_CurrentActionLowestFPS = m_CurrentActionLowestFPS;
            m_ActionLowestFPSClassName = m_CurrentActionClassName;
        }
    }
}