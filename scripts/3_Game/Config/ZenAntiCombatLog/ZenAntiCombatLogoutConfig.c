// TODO: Config logout messages here instead of stringtable?
class ZenAntiCombatLogout
{
	// Static constant config version (is NOT saved to json)
	static const string CONFIG_VERSION = "1";

	// Config location
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenAntiCombatLogout.json";

	// Config data
	string ConfigVersion = "";
	int CombatLogoutSecs = 300; // How many seconds to keep player on server after engaging in combat (shooting at player, meleeing them or grenading them if TriggerOnExplosiveRadius > 0)
	int DisableExitButtonSecs = 5; // How long to disable the Exit button when logging out after combat
	bool NotifyPlayerOfPenalty = true; // Whether or not to notify the player of the penalty for combat logging
	bool TriggerOnGunshot = false; // Whether or not to trigger the combat log timer for simply firing your gun (hunting, raiding etc)
	int TriggerOnBulletImpactRadius = 0; // // Experimental (set to 0 to disable) - Radius around bullet impact to detect and flag players for engaging in combat
	int TriggerOnExplosiveRadius = 30; // Radius around explosions to detect and flag players for engaging in combat (set to 0 to disable)
	int DropFlareOnPlayer = 0; // Whether or not to drop a flare on a player (0 = disabled, 1 = when they logout before timer, 2 = when they are killed for combat logging)
	int KillPlayer = 0; // Whether or not to kill a player for leaving the server before the combat log timer runs out

	// Load config file or create default file if config doesn't exsit
	void Load()
	{
		if (FileExist(zenModFolder + zenConfigName))
		{	
			// If config exists, load file
			JsonFileLoader<ZenAntiCombatLogout>.JsonLoadFile(zenModFolder + zenConfigName, this);

			// If version mismatch, backup old version of json before replacing it
			if (ConfigVersion != CONFIG_VERSION)
			{
				JsonFileLoader<ZenAntiCombatLogout>.JsonSaveFile(zenModFolder + zenConfigName + "_old", this);
			}
			else
			{
				// Config exists and version matches, stop here.
				return;
			}
		}

		// Set new config version
		ConfigVersion = CONFIG_VERSION;

		// Save config
		Save();
	};

	// Save config
	void Save()
	{
		if (!FileExist(zenModFolder))
		{	
			// If config folder doesn't exist, create it.
			MakeDirectory(zenModFolder);
		}

		// Save JSON config
		JsonFileLoader<ZenAntiCombatLogout>.JsonSaveFile(zenModFolder + zenConfigName, this);
	}
}

// Save config data
ref ZenAntiCombatLogout m_ZenAntiCombatLogout;

// Helper function to return Config data storage object
static ZenAntiCombatLogout GetZenAntiCombatLogoutConfig()
{
	if (!m_ZenAntiCombatLogout)
	{
		m_ZenAntiCombatLogout = new ZenAntiCombatLogout;

		// Only load JSON config on the server
		if (GetGame().IsDedicatedServer())
		{
			m_ZenAntiCombatLogout.Load();
		}
	}

	return m_ZenAntiCombatLogout;
};