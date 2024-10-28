class ZenWeatherConfig
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "3";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";

    // My custom winter weather define, if you alternate between summer/winter just add a #define WINTER to your modpack to use this
    private static string zenConfigName = "ZenWeatherConfig.json";

	// Main config data
	string ConfigVersion = "";

	// Config settings
	bool DebugOn = false;
	ref array<ref ZenWeatherPreset> WeatherPresets;

    void ZenWeatherConfig()
    {
#ifdef WINTER // Custom weather #defines I use on my server for alternating weather
        zenConfigName = "ZenWeatherConfig_Winter.json";
#endif
#ifdef SPRING
        zenConfigName = "ZenWeatherConfig_Spring.json";
#endif
    }

	// Load server config file or create default file if config doesn't exsit
	void Load()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		// If config exists, load file
		if (FileExist(zenModFolder + zenConfigName))
		{ 
			JsonFileLoader<ZenWeatherConfig>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenWeatherConfig>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Default config presets
        WeatherPresets = new array<ref ZenWeatherPreset>;
        ZenWeatherPreset preset = new ZenWeatherPreset();

		#ifdef WINTER
        // WINTER
        // Blizzard
        preset.name = "Blizzard";
        preset.chance = 20;
        preset.transition_minutes_min = 10;
        preset.transition_minutes_max = 20;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.8;
        preset.overcast_max = 1.0;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0.7;
        preset.wind_max = 1.0;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0.9;
        preset.snow_max = 1.0;
        preset.fog_min = 0.9;
        preset.fog_max = 1.0;
        preset.storm = true;
        preset.thunder_timeout = 10.0;
        WeatherPresets.Insert(preset);

        // Clear Skies
        preset = new ZenWeatherPreset();
        preset.name = "Clear Skies";
        preset.chance = 200;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 30;
        preset.duration_minutes_max = 90;
        preset.overcast_min = 0.0;
        preset.overcast_max = 0.3;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0;
        preset.fog_max = 0;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Light Snow, Light Overcast
        preset = new ZenWeatherPreset();
        preset.name = "Light Snow, Light Overcast";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.5;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.01;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0.2;
        preset.snow_max = 0.3;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Overcast with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Overcast with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.4;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.05;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.0;
        preset.fog_max = 0.1;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Snow with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Snow with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.05;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0.2;
        preset.snow_max = 0.5;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Snow with No Wind
        preset = new ZenWeatherPreset();
        preset.name = "Snow with No Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.01;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0.2;
        preset.snow_max = 1.5;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);
        #else
        #ifdef SPRING
        // EARLY/LATE WINTER
        
        // Blizzard
        preset.name = "Blizzard";
        preset.chance = 100;
        preset.transition_minutes_min = 10;
        preset.transition_minutes_max = 20;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.8;
        preset.overcast_max = 1.0;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0.9;
        preset.wind_max = 1.0;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0.9;
        preset.snow_max = 1.0;
        preset.fog_min = 0.9;
        preset.fog_max = 1.0;
        preset.storm = true;
        preset.thunder_timeout = 10.0;
        WeatherPresets.Insert(preset);

        // Clear Skies
        preset = new ZenWeatherPreset();
        preset.name = "Clear Skies";
        preset.chance = 200;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 30;
        preset.duration_minutes_max = 90;
        preset.overcast_min = 0.0;
        preset.overcast_max = 0.3;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0;
        preset.fog_max = 0;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Light Snow, Light Overcast
        preset = new ZenWeatherPreset();
        preset.name = "Light Snow, Light Overcast";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.5;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.01;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0.2;
        preset.snow_max = 0.3;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Overcast with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Overcast with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.4;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.05;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.0;
        preset.fog_max = 0.1;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Snow with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Snow with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.05;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0.2;
        preset.snow_max = 0.5;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Snow with No Wind
        preset = new ZenWeatherPreset();
        preset.name = "Snow with No Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.01;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0.2;
        preset.snow_max = 1.5;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Light Rain, Light Overcast
        preset = new ZenWeatherPreset();
        preset.name = "Light Rain, Light Overcast";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.5;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0.2;
        preset.rain_max = 0.3;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);
        #else
        // SUMMER
        // Hurricane
        preset.name = "Hurricane";
        preset.chance = 20;
        preset.transition_minutes_min = 10;
        preset.transition_minutes_max = 20;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.8;
        preset.overcast_max = 1.0;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0.7;
        preset.wind_max = 1.0;
        preset.rain_min = 0.9;
        preset.rain_max = 1.0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.9;
        preset.fog_max = 1.0;
        preset.storm = true;
        preset.thunder_timeout = 10.0;
        WeatherPresets.Insert(preset);

        // Clear Skies
        preset = new ZenWeatherPreset();
        preset.name = "Clear Skies";
        preset.chance = 200;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 30;
        preset.duration_minutes_max = 90;
        preset.overcast_min = 0.0;
        preset.overcast_max = 0.3;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0;
        preset.rain_max = 0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0;
        preset.fog_max = 0;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Light Rain, Light Overcast
        preset = new ZenWeatherPreset();
        preset.name = "Light Rain, Light Overcast";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.5;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0.2;
        preset.rain_max = 0.3;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Overcast with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Overcast with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.4;
        preset.overcast_max = 0.8;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.3;
        preset.rain_min = 0.0;
        preset.rain_max = 0.0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.0;
        preset.fog_max = 0.1;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Rain with Slight Wind
        preset = new ZenWeatherPreset();
        preset.name = "Rain with Slight Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.3;
        preset.rain_min = 0.2;
        preset.rain_max = 0.5;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Rain with No Wind
        preset = new ZenWeatherPreset();
        preset.name = "Rain with No Wind";
        preset.chance = 100;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 30;
        preset.overcast_min = 0.6;
        preset.overcast_max = 0.9;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0;
        preset.wind_max = 0.1;
        preset.rain_min = 0.2;
        preset.rain_max = 0.5;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.1;
        preset.fog_max = 0.2;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Sunshower
        preset = new ZenWeatherPreset();
        preset.name = "Sunshower";
        preset.chance = 50;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 10;
        preset.duration_minutes_max = 15;
        preset.overcast_min = 0.2;
        preset.overcast_max = 0.3;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0.1;
        preset.wind_max = 0.3;
        preset.rain_min = 0;
        preset.rain_max = 0.1;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.0;
        preset.fog_max = 0.0;
        preset.storm = false;
        preset.thunder_timeout = 0;
        WeatherPresets.Insert(preset);

        // Storm
        preset = new ZenWeatherPreset();
        preset.name = "Storm";
        preset.chance = 50;
        preset.transition_minutes_min = 15;
        preset.transition_minutes_max = 30;
        preset.duration_minutes_min = 20;
        preset.duration_minutes_max = 40;
        preset.overcast_min = 0.7;
        preset.overcast_max = 1.0;
        preset.wind_maxspeed = 20.0;
        preset.wind_min = 0.3;
        preset.wind_max = 0.9;
        preset.rain_min = 0.1;
        preset.rain_max = 1.0;
        preset.snow_min = 0;
        preset.snow_max = 0;
        preset.fog_min = 0.0;
        preset.fog_max = 0.1;
        preset.storm = true;
        preset.thunder_timeout = 10;
        WeatherPresets.Insert(preset);
        #endif
        #endif

		// Save config
		Save();
	}

	// Save config
	void Save()
	{
		// If config folder doesn't exist, create it.
		if (!FileExist(zenModFolder))
		{
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenWeatherConfig>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Define a weather preset
class ZenWeatherPreset
{
    string name;

    float chance;

    float transition_minutes_min;
    float transition_minutes_max;

    float duration_minutes_min;
    float duration_minutes_max;

    float overcast_min;
    float overcast_max;

    float wind_maxspeed;

    float wind_min;
    float wind_max;

    float rain_min;
    float rain_max;

    float snow_min;
    float snow_max

    float fog_min;
    float fog_max;
    
    bool storm;
    float thunder_timeout;
}

ref ZenWeatherConfig m_ZenWeatherConfig;

static ZenWeatherConfig GetZenWeatherConfig()
{
	if (!m_ZenWeatherConfig)
	{
		//ZMPrint("[ZenWeatherConfig] Init");
		m_ZenWeatherConfig = new ZenWeatherConfig;
		m_ZenWeatherConfig.Load();
	}

	return m_ZenWeatherConfig;
}