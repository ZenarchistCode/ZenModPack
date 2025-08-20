Check the Wiki guide for details & instructions: https://github.com/ZenarchistCode/ZenModPack/wiki



Change Notes:

20th August 2025:
ZenDiscordAPI:
- Moved into its own .pbo file
- Fixed JSON config file not auto-generating if Utilities folder doesn't exist in Zenarchist folder
- To generate a new config with the latest default parameters, simply delete your old file + run server
- Updated GitHub JSON config template file and descriptions
- Changed default config to have all the optional immersion settings turned off - with new default settings:
  - All you need is a raid station, a Field Transceiver, and a car battery attached
  - You no longer need any tools or a territory to build/dismantle a raid alarm
  - You no longer need to connect a raid dish before you can connect to Discord
  - You no longer need to have a clear view of sky above raid dish for Discord alerts to work
  - Turn these settings on for added immersion, but this will improve ease of setting up the mod for newbies
- Added #define ZENDISCORDAPI to modpack (this is inconsistent and usually only works if you repack the mod)
- Added compatibility with my ZenTerritories mod contained in my ZenModPack Enormous Package mod
- Added how long the battery of the raid station will last to the in-game item tooltip/description (days/hours/etc)
- Added ability to set energy usage per second (604800 max energy means 1.0 = ~1 week of power from full battery)
- Added ability to configure the tools used for building/dismantling raid station, including bare hands
- Added ability to type anything into webhook slots, but text will be validated when OK is clicked
  - Should be able to CTRL + V paste text now without using the button, or type the link manually
  - But any link which does not start with "https://discord.com/api/webhooks/" will be ignored as it's invalid API

ZenAntiCombatLogout:
- Fixed unpinning smoke grenades triggering combat log timer

ZenModPack:
- Fixed several defines not working properly when individual .pbos were removed from modpack and some residual repeat code from mods I've moved into their own pbos.

9th June 2025:
- ZenSleep changes:
- Fixed some more nullpointer errors under certain circumstances
- Set MaximumSecsBuzzedByCombat to zero by default as it can be confusing for players not understanding why they can't sleep after getting attacked by zeds

8th June 2025:
- Fixed Winchester rifles despawning (my bad, sorry)
- Fixed nullpointer in ZenSleep mod when sleeping in tents/modded objects
- Added GameLabs compatibility with ZenTerritories mod
- Added ZenCarBatteryIcon based on https://steamcommunity.com/sharedfiles/filedetails/?id=2812312144
- Added ZenDropRuinedClothing to ZenModPackConfig.json - drops clothing items to ground when ruined
- Added ZenBetterChatFont - adds a shadow underneath the vanilla chat font to make it easier to read
- Added persistent static objects types to XML file (eg. Zen_EternalFireplace, Zen_CommunityBarrel etc)

5th June 2025:
- Fixed debug message spam when cooking on a stick
- Fixed potential nullpointer on ZenMap when used with Expansion Quests
- Added #define ZENANTICOMBATLOGOUT
- Moved ZenShove to its own .pbo
- Moved ZenCraftingSounds to its own .pbo
- Fixed ZenServerGUI spawning rule book even when disabled in ZenModPackConfig.json

4th June 2025:
- Fixed ZenBasebuildingConfig missing from ZenModPackConfig.JSON
- Added Whitelist string array to ZenBasebuildingConfig's no build zones
- Fixed incorrect typename for ZenJournal in types folder
- Added ZenSleep's extra PBOs to mod folder

3rd June 2025 - 1.28 Update:

- Fixed several minor bugs across many mods, and any 1.28 issues along with any server & client error logs I could find
- Fixed all missing types.xml entries in the mod's extras folder - everything is up to date now with current mod list
- Removed ZenWeatherConfig altogether (can't get it to work how I'd like it to)
- Removed Ice Planes mod from the modpack (too niche to keep it in)
  - Use my standalone mod if you wish to keep it on your server: https://steamcommunity.com/sharedfiles/filedetails/?id=3334870001
- Removed deprecated sleeping bag objects (and replaced them with my new ZenSleepV2 sleeping bags)
- Removed ZenWolfMask since vanilla added a model which is way superior to my crappy one
- Removed broken glass effect from scopes & NVGs since vanilla finally added their own visual fx
- Added ZenShelterCargo setting to server-side mod config
  - If enabled, shelters have slightly different cargo space to make the harder-to-craft ones better and more worthwhile
  - Stick = 80 slots, Fabric = 100, Leather = 120
  - Don't forget to add the ZenShelter types.xml entries as my custom classnames will override vanilla's when built
  - Turning this mod on/off mid-wipe won't break anything, it will only affect new shelters built AFTER it has been enabled
  - Basically when a player "builds" a shelter it will spawn my custom shelter types instead of the vanilla ones
- Added Zen_Canteen_Black (requires types.xml to add it to loot table - just a black skinned vanilla canteen for some variety)
- Added ZenCanteenTablets to ZenModPackConfig.json (adds a Purification tablet attachment slot on canteens - disabled by default)
- Added ZenJournal which is a Bible-skinned book item the player can attach notes, paper, map, compass and pen to (check types.xml)
- Added Zen_NBCCase which can hold all NBC gear (cannot attach gear with items inside, or see inventory while attached)
- Added Zen_CommunityBarrel which is a barrel which cannot despawn or be moved. Can only be placed with admin tools/DayZ Editor
- Added "ZenBasebuildingConfig" mod (disabled by default in ZenModConfig.json)
  - Mainly intended for use on Vanilla+ servers like mine where I want to reduce build anywhere abuse
  - Only applies to BaseBuilding stuff if "disableIsBaseViableCheck" is true in vanilla cfggameplay.json for build anywhere
  - "disableIsBaseViableCheck = true" makes the game ignore bounding boxes, allowing placing objects literally inside themselves
  - Enabling this is the only way to allow double-stacked high fences for anti-boost etc, but side-effect is players can abuse it
  - "CantPlaceInsideOfSelf" - lists objects which cannot be placed within X meters of themselves (eg. tents inside tents)
  - "BasebuildingLimits" - restricts how closely some basebuilding objects can be placed (eg. no fences inside fences within X distance)
  - This BasebuildingLimits setting restricts horizontal and vertical placement limits separately, allowing more control over limits
  - "PackTentAnywhere" - allows packing tents from outside if the tent is empty of gear (prevents tents getting bugged inside walls etc)
- Added ZenTerritories (a basic territory management mod - use a combination lock on a fully built territory flag to create a territory)
  - Check mod wiki for more info: https://github.com/ZenarchistCode/ZenModPack/wiki/Zen's-Territories
- Added ZenShove on/off config (allows "shoving" a restrained player in the direction you're facing without dealing health damage)
- Added ZenImmersiveChatHUD mod to modpack (disabled by default - enable/disable it in ZenModPackConfig.json)
- Added ZenZombiesConfig mod to server-side mods (allows adding simple loot spawns via JSON, and adding extra shock/bleed damage etc)
- Added ZenMapEnhancement mod (enhances vanilla map with map markers etc - see wiki guide for more info)
- Added ZenWatchGPS (wearable GPS device which acts as a compass and GPS when worn with a battery attached and turned on)
- Added ZenNightConfig mod (allows you to randomize the chance of bright and dark night lighting config each restart)
  - There are two options: one is random on each restart, the other is dependent on the weather's overcast value when night begins
- Added "ZenPainting" mod config to ZenModPackConfig.json - enables/disables the ability to paint some vanilla items black/green/camo
  - Requires adding Zen_SprayCans to types.xml loot table - check mod wiki for more info (https://github.com/ZenarchistCode/ZenModPack/wiki/Zen's-Spray-Painting)
- Added effects for drinking alcohol (Jameson & Flask) - drinking > 100mL gives painkiller effect, but drinking > 500 gives food poisoning
  - The player "drains" 25mL of alcohol from their system every 60 seconds, so drinking booze will accumulate in their system over time
  - Drinking a full bottle of Jameson (750mL) will take approximately ~30 minutes to drain to zero
- Added "PersistentModEnabled" to ZenModPackConfig.json file for enabling/disabling any mods which require server database persistence
  - Added ZenEngrave on/off config (allows engraving your Steam profile name onto guns using a knife)
  - Added my new ZenSleep mod (LINK TO WORKSHOP)
  - Added combination lock overhaul mod (LINK TO WORKSHOP)
  - Alcohol is a minor optional persistence mod (saves player's booze level when they logout)


*** IMPORTANT NOTES: ***
- I recommend only turning any of the mods in the Persistent mod list such as ZenSleep, ZenEngrave & ZenComboLock mods on/off AFTER you have freshly wiped your server to avoid potential persistence corruption issues as these 3 mods save server-side data directly onto items/players in the game's database files. If you turn any of these 3 mods on/off in-between server wipes on an active server then you may encounter potential server instability requiring a server wipe to fix it. These 3 mods are disabled by default to avoid any issues when someone who hasn't read this readme file adds my modpack to a new server. If you wish to run these mods on your server then I recommend only turning them on BEFORE any players load in to your freshly wiped server.

PBO Compiler Files To Include: *.emat;*.edds;*.ptc;*.c;*.imageset;*.layout;*.ogg;*.paa;*.fnt;*.tga;*.xml;*.csv;*.rvmat;*.map;*.html