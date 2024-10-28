// Original author: affenb3rt (https://steamcommunity.com/sharedfiles/filedetails/?id=2844108808)
// Slightly modified to add weighted chance when selecting next m_Weather preset from config
class ZenWeatherPlugin extends PluginBase
{
    static const string PERSIST_FILE_NAME = "$mission:storage_%1/data/weather.bin";

    protected float m_CurrentWeatherTimeDelta = 0;
    protected float m_NextChangeTime = 0;

    protected bool m_Started = false;
    protected bool m_InitialChange = true;
    protected int m_InstanceID = -1;

    protected ref ZenWeatherPreset m_CurrentWeather;
    protected Weather m_Weather;

    protected void Start()
    {
        WeatherDebug("Started");
        m_Started = true;
        m_Weather = GetGame().GetWeather();
        UpdateWeather();
    }

    protected void Stop()
    {
        WeatherDebug("Stopped");
        m_Started = false;
    }

    ZenWeatherPreset GetCurrentWeatherPreset()
    {
        return m_CurrentWeather;
    }

    protected void SetRain(float forecast, float time, float minDuration)
    {
        WeatherDebug("Rain forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(m_Weather.GetRain(), "Set", time * 0.9 * 1000, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetSnowfall(float forecast, float time, float minDuration)
    {
        WeatherDebug("Snow forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(m_Weather.GetSnowfall(), "Set", time * 0.9 * 1000, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetOvercast(float forecast, float time, float minDuration)
    {
        WeatherDebug("Overcast forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);
        m_Weather.GetOvercast().Set(forecast, time, minDuration);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName(m_Weather.GetOvercast(), "Set", 1, false, new Param3<float, float, float>(forecast, (minDuration - time) * 0.4, minDuration - time));
    }

    protected void SetFog(float forecast, float time, float minDuration)
    {
        WeatherDebug("Fog forecast:" + forecast + " timeToChange:" + time + " duration:" + minDuration);

        if (!m_Weather.IsDynVolFogEnabled())
        {
            m_Weather.GetFog().Set(forecast, time, minDuration);
        }
        else
        {
            m_Weather.SetDynVolFogDistanceDensity(forecast, time);
        }
    }

    protected void SetWind(float wind, float windMax, float transition, float duration)
    {
        WeatherDebug("Wind:" + wind);

        m_Weather.GetWindMagnitude().SetForecastChangeLimits(0, windMax);
        m_Weather.GetWindMagnitude().Set(windMax * wind, transition, duration);
    }

    float ApplyPreset(ZenWeatherPreset preset, bool initialChange, string forcePreset = "")
    {
        if (forcePreset != "")
        {
            foreach (ZenWeatherPreset checkPreset : GetZenWeatherConfig().WeatherPresets)
            {
                string presetName = checkPreset.name;
                presetName.ToLower();
                forcePreset.ToLower();

                if (presetName == forcePreset)
                {
                    preset = checkPreset;
                    break;
                }
            }
        }

        if (!preset)
        {
            WeatherDebug("ERROR: FAILED TO FIND PRESET " + forcePreset);
            return -1;
        }

        WeatherDebug("Apply Preset: " + preset.name);

        float duration = Math.RandomFloatInclusive(preset.duration_minutes_min, preset.duration_minutes_max) * 60;
        float speedOfChange = duration * 0.4;
        if (preset.transition_minutes_min >= 0 && preset.transition_minutes_max > 0.0)
        {
            speedOfChange = Math.RandomFloatInclusive(preset.transition_minutes_min, preset.transition_minutes_max) * 60;
            duration += speedOfChange;
        }

        if (initialChange)
        {
            speedOfChange = 0.01;
        }

        m_Weather.GetOvercast().SetLimits(0.0, 1.0);
        m_Weather.GetOvercast().SetForecastChangeLimits(0.0, 1.0);
        m_Weather.GetOvercast().SetForecastTimeLimits(0.0, duration + speedOfChange);

        if (!m_Weather.IsDynVolFogEnabled())
        {
            m_Weather.GetFog().SetLimits(0.0, 1.0);
            m_Weather.GetFog().SetForecastChangeLimits(0.0, 1.0);
            m_Weather.GetFog().SetForecastTimeLimits(0.0, duration + speedOfChange);
        }
        else
        {
            // Don't use regular fog, dynamic volumetric fog is enabled
            m_Weather.GetFog().SetLimits(0, 0);
            m_Weather.GetFog().SetForecastChangeLimits(0, 0);
            m_Weather.GetFog().SetForecastTimeLimits(0.0, duration + speedOfChange);
        }

        m_Weather.GetRain().SetLimits(0.0, 1.0);
        m_Weather.GetRain().SetForecastChangeLimits(0.0, 1.0);
        m_Weather.GetRain().SetForecastTimeLimits(0.0, duration + speedOfChange);

        m_Weather.GetSnowfall().SetLimits(0.0, 1.0);
        m_Weather.GetSnowfall().SetForecastChangeLimits(0.0, 1.0);
        m_Weather.GetSnowfall().SetForecastTimeLimits(0.0, duration + speedOfChange);

        float overcast = Math.Clamp(Math.RandomFloatInclusive(preset.overcast_min, preset.overcast_max), 0, 1);
        float fog = Math.Clamp(Math.RandomFloatInclusive(preset.fog_min, preset.fog_max), 0, 1);
        float rain = Math.Clamp(Math.RandomFloatInclusive(preset.rain_min, preset.rain_max), 0, 1);
        float snow = Math.Clamp(Math.RandomFloatInclusive(preset.snow_min, preset.snow_max), 0, 1);
        float wind = Math.Clamp(Math.RandomFloatInclusive( preset.wind_min, preset.wind_max), 0, 1);
        
        SetOvercast(overcast, speedOfChange, duration);

        if (rain > 0)
        {
            m_Weather.SetRainThresholds(overcast * 0.5, Math.Min(overcast * 1.5, 1.0), speedOfChange * 0.15);
        }
        else
        {
            if (overcast > m_Weather.GetOvercast().GetActual())
            {
                m_Weather.SetRainThresholds(1.0, 1.0, speedOfChange * 0.15);
            }
            else
            {
                m_Weather.SetRainThresholds(0.0, 0.0, speedOfChange * 0.15);
            }
        }

        if (snow > 0)
        {
            m_Weather.SetSnowfallThresholds(overcast * 0.5, Math.Min(overcast * 1.5, 1.0), speedOfChange * 0.15);
        }
        else
        {
            if (overcast > m_Weather.GetSnowfall().GetActual())
            {
                m_Weather.SetSnowfallThresholds(1.0, 1.0, speedOfChange * 0.15);
            }
            else
            {
                m_Weather.SetSnowfallThresholds(0.0, 0.0, speedOfChange * 0.15);
            }
        }

        SetRain(rain, speedOfChange, duration);
        SetSnowfall(snow, speedOfChange, duration);
        SetFog(fog, speedOfChange, duration);
        SetWind(wind, preset.wind_maxspeed, speedOfChange, duration);

        if (preset.storm)
        {
            m_Weather.SetStorm(1.0, overcast * 0.9, preset.thunder_timeout);
        }
        else
        {
            m_Weather.SetStorm(0.0, 1.0, 99999.0);
        }

        m_CurrentWeather = preset;
        return duration;
    }

    // Save file
    override void OnDestroy()
    {
        string name = string.Format(PERSIST_FILE_NAME, m_InstanceID);
        FileSerializer serializer = new FileSerializer();

        if (m_CurrentWeather && serializer.Open(name, FileMode.WRITE))
        {
            m_CurrentWeather.duration_minutes_min = m_CurrentWeather.duration_minutes_min * 0.5;
            m_CurrentWeather.duration_minutes_max = m_CurrentWeather.duration_minutes_min * 0.5;
            serializer.Write(m_CurrentWeather);
            serializer.Close();
            WeatherDebug("Successfully saved weather config to persistent data file.");
        }
    }

    // Read file / or start new weather event
    void UpdateWeather()
    {
        if (!m_Started) 
            return;

        if (!g_Game) 
            return;

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
                        m_NextChangeTime = ApplyPreset(preset, m_InitialChange);
                        m_InitialChange = false;
                        WeatherDebug("Successfuly restored persistent weather preset: " + preset.name);
                        
                        serializer.Close();
                        return;
                    }
                    else
                    {
                        serializer.Close();
                        WeatherDebug("Failed to read persistent weather data! Starting new data file...");
                    }
                }
            }
        }

        ZenWeatherPreset newPreset = SelectNextWeather();
        m_NextChangeTime = ApplyPreset(newPreset, m_InitialChange);
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
        WeatherDebug("TotalChanceWeight: " + totalChance + " / selectedPoint=" + randomPoint);
    
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

        GetZenWeatherConfig(); // Loads m_Weather config

        if (!GetZenWeatherConfig().WeatherPresets || GetZenWeatherConfig().WeatherPresets.Count() == 0)
        {
            Error("ZenWeatherPlugin: Broken Config!");
            return;
        }

        Start();

        m_Weather.MissionWeather(true);
    }
 
    override void OnUpdate(float delta_time)
    {
        if (!m_Started)
            return;

        m_CurrentWeatherTimeDelta += delta_time;
        if (m_CurrentWeatherTimeDelta >= m_NextChangeTime)
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