class ZenNightLightingPlugin extends PluginBase
{
    protected float m_DeltaTime;
    protected bool m_IsNight;
    protected bool m_IsNightLast;
    protected int m_NightTimeLightingID;

	override void OnInit()
    {
        ZMPrint("[ZenNightLightingPlugin] :: OnInit");

        m_NightTimeLightingID = GetZenNightConfig().BrightNightLightingConfigID;
        m_DeltaTime = 0;
        m_IsNightLast = false;
    }

    int GetLightingConfigID()
    {
        return m_NightTimeLightingID;
    }

    void SetLightingConfigID(int id)
    {
        m_NightTimeLightingID = id;

        // Update sync to all clients
        array<Man> players();
        PlayerBase pb;
        GetGame().GetPlayers(players);
        Param1<int> lightID = new Param1<int>(GetLightingConfigID());

        for (int i = 0; i < players.Count(); i++)
        {
            pb = PlayerBase.Cast(players.Get(i));
            if (pb && pb.GetZenNightConfigID() != GetLightingConfigID())
            {
                GetGame().RPCSingleParam(pb, ERPCs.RPC_SEND_LIGHTING_SETUP, lightID, true, pb.GetIdentity());
                pb.SetZenNightConfigID(GetLightingConfigID());
            }
        }
    }

    override void OnDestroy()
    {
    }

    override void OnUpdate(float delta_time)
    {
        m_DeltaTime += delta_time;

        if (m_DeltaTime < 30)
            return;

        m_DeltaTime = 0;

        m_IsNight = GetGame().GetWorld().IsNight();

        if (m_IsNight && (GetZenNightConfig().UpdateOvercastLightingAnyTime || m_IsNight != m_IsNightLast))
        {
            ProcessNightTimeShift();
        }

        m_IsNightLast = m_IsNight;
    }

    void ProcessNightTimeShift()
    {
        // Only go dark if overcast exceeds threshold
        if (GetGame().GetWeather().GetOvercast().GetActual() > GetZenNightConfig().OvercastToTriggerDarkNights)
        {
            if (GetLightingConfigID() != GetZenNightConfig().DarkNightLightingConfigID)
            {
                SetLightingConfigID(GetZenNightConfig().DarkNightLightingConfigID); // dark
                ZMPrint("[ZenNightLightingPlugin] Overcast " + GetGame().GetWeather().GetOvercast().GetActual() + " > " + GetZenNightConfig().OvercastToTriggerDarkNights + ", setting light config id=" + GetLightingConfigID());
            }
        }
        else 
        {
            if (GetLightingConfigID() != GetZenNightConfig().BrightNightLightingConfigID)
            {
                SetLightingConfigID(GetZenNightConfig().BrightNightLightingConfigID); // light
                ZMPrint("[ZenNightLightingPlugin] Overcast " + GetGame().GetWeather().GetOvercast().GetActual() + " < " + GetZenNightConfig().OvercastToTriggerDarkNights + ", setting light config id=" + GetLightingConfigID());
            }
        }
    }
}