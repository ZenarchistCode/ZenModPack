class ZenPerformanceMonitorPlugin extends PluginBase
{
    // General vars
    protected float m_DeltaTime;
    protected int m_LastDrivingWarning;

    // Action tracker
    protected int m_ActionStartFPS;
    protected int m_ActionLowestFPS;
    protected string m_ActionClassName;

    // Lowest session FPS tracker
    protected int m_LowestFPS;
    protected int m_LowestFPS_PlayerCount;
    protected int m_LowestFPS_ItemCount;
    protected int m_LowestFPS_AnimalCount;
    protected int m_LowestFPS_ZombieCount;
    protected int m_LowestFPS_DrivingVehicles;

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
        m_LastDrivingWarning = 0;
        m_LastDrivingWarning = GetGame().GetTickTime();
    }

    override void OnDestroy()
    {
        Print(GetPerformanceDump());
    }

    string GetPerformanceDump()
    {
        string msg = "\n\n[ZenModPack::DayZGame] Session Performance Breakdown:\n\n";
        msg = msg + "\nRuntime: " + (GetGame().GetTickTime() / 60) + " minutes";

        msg = msg + "\n\nLowest FPS: " + m_LowestFPS;
        msg = msg + "\nPlayer Count: " + m_LowestFPS_PlayerCount;
        msg = msg + "\Item Count: " + m_LowestFPS_ItemCount;
        msg = msg + "\Animal Count: " + m_LowestFPS_AnimalCount;
        msg = msg + "\Zombie Count: " + m_LowestFPS_ZombieCount;
        msg = msg + "\Driving Count: " + m_LowestFPS_DrivingVehicles;
#ifdef EXPANSIONMODAI
        msg = msg + "\AI Count: " + m_LowestFPS_AICount;
#endif

        msg = msg + "\n\nHighest FPS: " + m_HighestFPS;
        msg = msg + "\nPlayer Count: " + m_HighestFPS_PlayerCount;
        msg = msg + "\Item Count: " + m_HighestFPS_ItemCount;
        msg = msg + "\Animal Count: " + m_HighestFPS_AnimalCount;
        msg = msg + "\Zombie Count: " + m_HighestFPS_ZombieCount;
        msg = msg + "\Driving Count: " + m_HighestFPS_DrivingVehicles;
#ifdef EXPANSIONMODAI
        msg = msg + "\AI Count: " + m_HighestFPS_AICount;
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
        int fps = GetZenServerFPS();

        //! CHECK FPS WARNING
        if (fps <= GetZenUtilitiesConfig().ServerFPSWarning)
        {
            Error("[ZenModPack::DayZGame] Server FPS low: " + fps);
            Error(GetPerformanceDump());

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
        m_ActionStartFPS = GetZenServerFPS();
        m_ActionLowestFPS = m_ActionStartFPS;
        m_ActionClassName = action_data.m_Action.ClassName();
    }

    void ActionBase_OnUpdate(ActionData action_data)
    {
        int fps = GetZenServerFPS();
        if (fps < m_ActionLowestFPS)
            m_ActionLowestFPS = fps;
    }

    void ActionBase_End(ActionData action_data) 
    {
        float pctChange = (1 - (m_ActionLowestFPS / m_ActionStartFPS)) * 100;
        if (pctChange >= GetZenUtilitiesConfig().ServerActionFPSDropWarningPct)
        {
            string msg = "[ZenPerformanceMonitorPlugin] :: WARNING! ServerFPS dropped -" + pctChange + "% (low: " + m_ActionLowestFPS + " / start: " + m_ActionStartFPS + ") during action " + m_ActionClassName;
            Error(msg);
            ZenModLogger.Log(msg, "performance");
        }
    }
}