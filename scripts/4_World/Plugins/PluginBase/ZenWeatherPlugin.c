// Original author: affenb3rt (https://steamcommunity.com/sharedfiles/filedetails/?id=2844108808)
// Slightly modified to add weighted chance when selecting next weather preset from config
class ZenWeatherPlugin extends PluginBase
{
    static const string PERSIST_FILE_NAME = "$mission:storage_%1/data/weather.bin";

    protected float m_PresetWeatherTime = 0.0;
    protected float m_NextChangeTime = 0.0;

    protected bool m_Started = false;
    protected bool m_InitialChange = true;
    protected int m_InstanceID = -1;

    protected int m_WeatherType;

    protected ref ZenWeatherPreset m_CurrentWeather;

    protected void Start()
    {
        WeatherDebug("Started");
        m_Started = true;
        UpdateWeather();
    }

    protected void Stop()
    {
        WeatherDebug("Stopped");
        m_Started = false;
    }

    protected void SetRain(Weather weather, float forecast, float time, float minDuration)
    {
        WeatherDebug("Rain forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(weather.GetRain(), "Set", time * 0.9 * 1000, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetSnowfall(Weather weather, float forecast, float time, float minDuration)
    {
        WeatherDebug("Snow forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(weather.GetSnowfall(), "Set", time * 0.9 * 1000, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetOvercast(Weather weather, float forecast, float time, float minDuration)
    {
        WeatherDebug("Overcast forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        weather.GetOvercast().Set(forecast, time, minDuration);


        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(weather.GetOvercast(), "Set", 1, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetFog(Weather weather, float forecast, float time, float minDuration)
    {
        WeatherDebug("Fog forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        weather.GetFog().Set(forecast, time, minDuration);
    }

    protected void SetWind(Weather weather, float wind, float time = 60)
    {
        WeatherDebug("Wind:" + wind);
        weather.SetWindFunctionParams(0.0, 1.0, 20.0 * wind);
        weather.SetWindMaximumSpeed(20 * wind);
        weather.GetWindMagnitude().SetForecastChangeLimits(0, 20 * wind);
    }

    protected float ApplyPreset(Weather weather, ZenWeatherPreset preset, bool initialChange)
    {
        weather.GetOvercast().SetLimits(0.0, 1.0);
        weather.GetOvercast().SetForecastChangeLimits(0.0, 1.0);
        weather.GetOvercast().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetFog().SetLimits(0.0, 1.0);
        weather.GetFog().SetForecastChangeLimits(0.0, 1.0);
        weather.GetFog().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetRain().SetLimits(0.0, 1.0);
        weather.GetRain().SetForecastChangeLimits(0.0, 1.0);
        weather.GetRain().SetForecastTimeLimits(0.0, 99999.0);
        weather.GetSnowfall().SetLimits(0.0, 1.0);
        weather.GetSnowfall().SetForecastChangeLimits(0.0, 1.0);
        weather.GetSnowfall().SetForecastTimeLimits(0.0, 99999.0);

        WeatherDebug("Apply:" + preset.name);

        float duration = Math.RandomFloatInclusive(preset.duration_minutes_min, preset.duration_minutes_max) * 60;

        float speedOfChange = duration * 0.4;
        if (preset.transition_minutes_min >= 0 && preset.transition_minutes_max > 0.0)
        {
            speedOfChange = Math.RandomFloatInclusive(preset.transition_minutes_min, preset.transition_minutes_max) * 60;
            duration += speedOfChange;
        }
        if (initialChange)
        {
            speedOfChange = 0.0;
        }

        float overcast = Math.Clamp(Math.RandomFloatInclusive(preset.overcast_min, preset.overcast_max), 0, 1);
        float fog = Math.Clamp(Math.RandomFloatInclusive(preset.fog_min, preset.fog_max), 0, 1);
        float rain = Math.Clamp(Math.RandomFloatInclusive(preset.rain_min, preset.rain_max), 0, 1);
        float snow = Math.Clamp(Math.RandomFloatInclusive(preset.snow_min, preset.snow_max), 0, 1);
        float wind = Math.Clamp(Math.RandomFloatInclusive( preset.wind_min, preset.wind_max), 0, 1);
        
        SetOvercast(weather, overcast, speedOfChange, duration);
        if (rain > 0.0)
        {
            weather.SetRainThresholds(overcast * 0.8, Math.Min(overcast * 1.2, 1.0), speedOfChange * 0.15);
        }
        else
        {
            if (overcast > weather.GetOvercast().GetActual())
            {
                weather.SetRainThresholds(1.0, 1.0, speedOfChange * 0.15);
            }
            else
            {
                weather.SetRainThresholds(0.0, 0.0, speedOfChange * 0.15);
            }
        }

        if (snow > 0.0)
        {
            weather.SetSnowfallThresholds(overcast * 0.8, Math.Min(overcast * 1.2, 1.0), speedOfChange * 0.15);
        }
        else
        {
            if (overcast > weather.GetSnowfall().GetActual())
            {
                weather.SetSnowfallThresholds(1.0, 1.0, speedOfChange * 0.15);
            }
            else
            {
                weather.SetSnowfallThresholds(0.0, 0.0, speedOfChange * 0.15);
            }
        }

        SetRain(weather, rain, speedOfChange, duration);
        SetSnowfall(weather, snow, speedOfChange, duration);
        SetFog(weather, fog, speedOfChange, duration);
        SetWind(weather, wind);

        if (preset.storm)
        {
            weather.SetStorm(1.0, overcast * 0.9, preset.thunder_timeout);
        }
        else
        {
            weather.SetStorm(0.0, 1.0, 99999.0);
        }

        m_CurrentWeather = preset;
        
        m_WeatherType = WorldDataWeatherConstants.CLEAR_WEATHER;
        if ((rain >= 0.75 || snow >= 0.75) && overcast > 0.8)
                m_WeatherType = WorldDataWeatherConstants.BAD_WEATHER;
        else if ((rain >= 0.5 || snow >= 0.5) && overcast > 0.5)
            m_WeatherType = WorldDataWeatherConstants.CLOUDY_WEATHER;

        return duration;
    }

    protected void UpdateWeather()
    {
        if (!m_Started) 
            return;

        if (!g_Game) 
            return;

        Weather weather = g_Game.GetWeather();
        if (!weather) 
            return;

        WeatherDebug("Update");

        m_PresetWeatherTime = 0.0;
        m_InstanceID = GetGame().ServerConfigGetInt("instanceId");

        // Load last used WeatherPreset from persistent storage
        if (m_InitialChange)
        {
            string name = string.Format(PERSIST_FILE_NAME, m_InstanceID);
            if (FileExist(name))
            {
                FileSerializer serializer = new FileSerializer();
 
                if (serializer.Open(name, FileMode.READ))
                {
                    ZenWeatherPreset preset = new ZenWeatherPreset();
                    if (serializer.Read(preset))
                    {
                        WeatherDebug("Load persistent weather");
                        m_NextChangeTime = ApplyPreset(weather, preset, m_InitialChange);
                        m_InitialChange = false;
                        serializer.Close();
                        return;
                    }

                    serializer.Close();
                }
            }
        }

        m_NextChangeTime = ApplyPreset(weather, SelectNextWeather(), m_InitialChange);
        m_InitialChange = false;
    }

    protected ZenWeatherPreset SelectNextWeather()
    {
        // Calculate total chance weight
        float totalChance = 0;
        foreach (ZenWeatherPreset preset1 : GetZenWeatherConfig().WeatherPresets) 
        {
            totalChance += preset1.chance;
        }

        float randomPoint = Math.RandomFloatInclusive(0, totalChance);
        WeatherDebug("TotalWeight: " + totalChance + " / selectedPoint=" + randomPoint);
    
        foreach (ZenWeatherPreset preset2 : GetZenWeatherConfig().WeatherPresets) 
        {
            randomPoint -= preset2.chance;
            if (randomPoint <= 0)
                return preset2;
        }

        return GetZenWeatherConfig().WeatherPresets[0];
    }

    override void OnInit()
    {
        Print("[ZenWeatherPlugin] :: OnInit");

        GetZenWeatherConfig(); // Loads weather config

        if (!GetZenWeatherConfig().WeatherPresets || GetZenWeatherConfig().WeatherPresets.Count() == 0)
        {
            Error("ZenWeatherPlugin: Broken Config!");
            return;
        }

        Weather weather = g_Game.GetWeather();
        if (weather)
        {
            weather.MissionWeather(true);
            Start();
        }
    }

    override void OnDestroy()
    {
        string name = string.Format(PERSIST_FILE_NAME, m_InstanceID);
        FileSerializer serializer = new FileSerializer();

         if (m_CurrentWeather && serializer.Open(name, FileMode.WRITE))
         {
            m_CurrentWeather.duration_minutes_min = m_CurrentWeather.duration_minutes_min * 0.5;
            m_CurrentWeather.duration_minutes_max = m_CurrentWeather.duration_minutes_max * 0.5;
            serializer.Write(m_CurrentWeather);
            serializer.Close();
        }
    }

    override void OnUpdate(float delta_time)
    {
        if (!m_Started)
            return;

        m_PresetWeatherTime += delta_time;
        if (m_PresetWeatherTime >= m_NextChangeTime)
        {
            UpdateWeather();
        }
    }

    static void WeatherDebug(string s)
    {
        if (GetZenWeatherConfig().DebugOn)
        {
            ZenFunctions.DebugMessage("[ZenWeatherPlugin] " + s);
        }

        ZMPrint("[ZenWeatherPlugin] " + s);
    }
}