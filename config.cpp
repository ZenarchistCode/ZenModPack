/*
	© 2024 | Zenarchist.io | HardcoreDayZ.com | Zen's Enormous Package™️

	This mod consolidates all of my custom mods that are finished 
	and don't require much maintenance. This is for my server, but
	I figure other servers might want to run all of these mods too.

	I will update this mod only when bugs are fixed or new stable content is added.

	I made this combined modpack because "modding" too many vanilla classes
	can cause this bug: https://feedback.bistudio.com/T176938 | (Update: This bug has been fixed as of 1.24!)

	Notes/Credit: 
	- This pack also includes ViewInventory mod made by affenb3rt: https://steamcommunity.com/sharedfiles/filedetails/?id=2833363308
	- And WeatherPresets also by affenb3rt, modified to add weighted chance: https://steamcommunity.com/sharedfiles/filedetails/?id=2844108808
	- EarPlugs & AutoRun is based on code originally written by Daemonforge.
	- My treasure photo mod is inspired by Hunterz mod, but his mod lacked features I needed, so I made my own version of that same idea.

	[CONTAINED MODS]
	InventoryAnimation		- https://steamcommunity.com/sharedfiles/filedetails/?id=2833363308
	ZenConcussionGrenade	- Non-lethal shock grenade that only knocks players uncon
	ZenNotificationAudio	- Play sound when a notification is detected (eg survivor missions) 
	ZenAntiCombatLogout		- Automatic combat detection & prevents logout within 5 minutes		#
	ZenPersistentTrees		- Cut down trees & bushes persist across server restarts			#
	ZenCraftingSounds		- Extra crafting sounds added for certain actions
	ZenImmersiveSodas		- Cans can't be placed in cargo once opened if containing liquid
	ZenImmersiveLogin		- Players lie down with a black screen when logging in				#
	ZenBrokenGlasses		- Broken glasses/scopes have a visual effect on screen				#
	ZenTreesplosions		- Explosions knock down trees (compatible with PersistentTrees)		#
	ZenCauseOfDeath			- Players can inspect a dead player to see their cause of death		#
	ZenChickenCoops			- Players can collect feathers from chicken coops					#
	ZenOpenCansRock			- Players can open canned food on rocks and with Stones				#
	ZenRepairPumps			- Fuel pumps are broken and require repair with a wrench to use		#
	ZenRepairWells			- Wells are broken and require repair with a wrench to use			#
	ZenZombieDoors			- Zombies can knock open doors when aggro'd							#
	ZenCarWorkench			- A craftable mechanic workbench for storing car parts				#
	ZenDiscordAPI			- A raid alarm & Discord API mod									#
	ZenPimpMyRide			- Allows players to attach chemlights underneath cars for coolness	#
	ZenSwissKnife			- Adds a multi-tool knife (saw/screwdriver/knife/bottle opener etc) 
	ZenVikingAxe			- An alternative hatchet with 2x HP and different design			
	ZenArtillery			- Players can find smoke grenades that call artillery strikes
	ZenCatchRain			- Players can catch rain in bottles when it's raining				#
	ZenLeftovers			- Players leave leftover empty consumed food/medical items			#
	ZenEarPlugs				- Players can press a key to lower in-game volume					#
	ZenCamoCamp				- Players can craft a camo net shelter								#
	ZenFireFuel				- Players can soak fires in gasoline to last longer					#
	ZenTimeBomb				- Players can place grenades inside alarm clocks					#
	ZenFireWood				- Players can harvest firewood from static firewood piles			#
	ZenTireRack				- Players can craft a rack to hold several vehicle tires			#
	ZenTreasure				- Photos spawn in the world which, when read, spawn buried treasure	#
	ZenGhillie				- Players can craft a ghillie suit out of camo netting				#
	ZenAutoRun				- Opening map etc cancels auto-run, inventory makes player jog		#
	ZenSplitUI				- Allows players to split stackable items to any quantity			#
	ZenGraves				- When players die, a grave & cross spawns with some of their gear	#
	ZenChess				- Adds a craftable and playable chess board to the Game				#
	ZenNotes				- Players can write persistent notes by using a pen on paper		#
	ZenZippo				- Adds a refillable zippo ligher to the game						#
	Several cosmetic items	- Miscellaneous items for use in Expansion quests or rare loot for markets etc

	# - Means this mod can be enabled/disabled in the ZenModPackConfig json file in %profiles%/Zenarchist/Utilities
		If a mod does not have a master config, it can generally be disabled by excluding types.xml spawns.

	[GENERAL MODDING INFO]
	Repair Kit Types
	1: Weapon Cleaning Kit
	2: Sewing Kit
	3: Leather Sewing Kit
	4: Whetstone
	5: Duct Tape
	6: Tire Repair Kit
	7: Electronic Repair Kit
	8: Epoxy Putty

	Rotation Flags
	ROTATE_Front		=	1
	ROTATE_Top			=	2 
	ROTATE_Left			=	4 
	ROTATE_Right		=	8 
	ROTATE_LeftRight	=	12 
	ROTATE_Back			=	16
	ROTATE_FrontBack	=	17 
	ROTATE_Bottom		=	32 
	ROTATE_BottomTop	=	34 
	ROTATE_All			=	63 
	ROTATE_Ignore		=	64

	CFGVehicles Types with scope=2 as of 3/7/24:
		ZenSleepingBag_PackedBase
		ZenSleepingBagBlack_Packed
		ZenSleepingBagBlue_Packed
		ZenSleepingBagGrey_Packed
		ZenSleepingBagRed_Packed
		ZenSleepingBagGreen_Packed
		ZenSleepingBag_DeployedBase
		ZenSleepingBagBlack_Deployed
		ZenSleepingBagBlue_Deployed
		ZenSleepingBagGrey_Deployed
		ZenSleepingBagRed_Deployed
		ZenSleepingBagGreen_Deployed
		ZenSwissKnife
		ZenSwissKnife_CanOpener
		ZenSwissKnife_Lockpick
		ZenSwissKnife_Screwdriver
		ZenSwissKnife_Knife
		ZenSwissKnife_Scissors
		ZenSwissKnife_SmallSaw
		ZenRune_Air
		ZenRune_Body
		ZenRune_Chaos
		ZenRune_Cosmic
		ZenRune_Earth
		ZenRune_Fire
		ZenRune_Law
		ZenRune_Mind
		ZenRune_Nature
		ZenRune_Water
		ZenJameson
		Empty_ZenJameson
		EyeMask_ZenRabbit_Black
		EyeMask_ZenRabbit_Blue
		EyeMask_ZenRabbit_Pink
		EyeMask_ZenRabbit_Green
		ZenCamps_ShelterStick
		ZenCamps_ShelterFabric
		ZenCamps_ShelterLeather
		ZenRaybans_Black
		ZenRaybans_Red
		ZenRaybans_Green
		ZenRaybans_Blue
		Zen_CassetteCase
		Zen_BoomBox
		Zen_Walkman
		Zen_MusicNoise
		ZenRandomAmmoBox
		ZenTreasure_UndergroundStash
		ZenTreasure_UndergroundStashWinter
		ZenTreasure_WoodenCrate
		ZenTreasure_SeaChest
		ZenTreasure_DebugShovel
		ZenTreasure_RandomPhoto
		ZenPetrolLighter
		ZenZippoLighter
		ZenZippoLighter_Peace
		ZenZippoLighter_Anarchy
		ZenZippoLighter_Zenarchist
		ZenWolfMask
		ZenGraves_DeadPlayerCross
		ZenGraves_DeadPlayerSkeleton
		ZenGraves_UndergroundStash
		Zen_CombatLogFlare
		Zen_CamoShelterKit
		Zen_ImprovisedShelter
		ZenNote
		Pen_Pink
		Pen_Purple
		Pen_Orange
		Zen_VikingAxe
		Zen_ChessBoard
		Zen_ChessBoard_Static
		Zen_ChessWhitePawn
		Zen_ChessBlackPawn
		Zen_ChessWhiteBishop
		Zen_ChessBlackBishop
		Zen_ChessWhiteKing
		Zen_ChessBlackKing
		Zen_ChessWhiteKnight
		Zen_ChessBlackKnight
		Zen_ChessWhiteQueen
		Zen_ChessBlackQueen
		Zen_ChessWhiteRook
		Zen_ChessBlackRook
		Zen_CarWorkbenchKit
		Zen_CarWorkbench
		Zen_TireRack
		Zen_ArtillerySmokeGrenade
		Zen_ArtillerySmokeGrenadeChem
		Zen_ArtilleryBomb
		Zen_CamoNet_GhillieSuit
		Zen_CamoNet_GhillieHood
		Zen_CamoNet_GhillieTop
		Zen_CamoNet_GhillieBushrag
		Zen_CamoNet_GhillieAtt
		Zen_CamoNet_Strip
		Zen_ConcussionGrenade
		Empty_SodaCan_Pipsi
		Empty_SodaCan_Cola
		Empty_SodaCan_Spite
		Empty_SodaCan_Kvass
		Empty_SodaCan_Fronta
		Empty_BoxCerealCrunchin
		Empty_Rice
		Empty_PowderedMilk
		Empty_Honey_NoLiquid
		Empty_Marmalade_NoLiquid
		Empty_Honey
		Empty_Marmalade
		Empty_Zagorky
		Empty_ZagorkyChocolate
		Empty_ZagorkyPeanuts
		Empty_SaltySticks
		Empty_Crackers
		Empty_Chips
		Empty_Can_Opened
		Empty_BakedBeansCan_Opened
		Empty_PeachesCan_Opened
		Empty_TacticalBaconCan_Opened
		Empty_SpaghettiCan_Opened
		Empty_UnknownFoodCan_Opened
		Empty_SardinesCan_Opened
		Empty_TunaCan_Opened
		Empty_FoodCan_250g_Opened_ColorBase
		Empty_DogFoodCan_Opened
		Empty_CatFoodCan_Opened
		Empty_PorkCan_Opened
		Empty_Lunchmeat_Opened
		Empty_Pajka_Opened
		Empty_Pate_Opened
		Empty_BrisketSpread_Opened
		Used_BloodBagIV
		Used_SalineBagIV
		Used_Morphine
		Used_Epinephrine
		Used_AntiChemInjector
*/

class CfgPatches
{
	class ZenModPack
	{
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Gear_Tools",
			"DZ_Gear_Drinks",
			"DZ_Gear_Cooking",
			"DZ_Gear_Camping",
			"DZ_Gear_Consumables",
			"DZ_Vehicles_Wheeled",
			"DZ_Weapons_Supports",
			"DZ_Weapons_Explosives",
			"DZ_Characters",
			"DZ_Characters_Headgear",
			"DZ_Radio"
		};
	};
};

class CfgMods
{
	class ZenModPack
	{
		dir = "ZenModPack";
		name = "Zen's Enormous Package";
		author = "Zenarchist";
		credits = "Daemonforge, affenb3rt, DayZ Modding Community";
		version = "1.0";
		type = "mod";
		inputs = "ZenModPack/data/inputs.xml";
		dependencies[] = 
		{ 
			"Game", 
			"World", 
			"Mission" 
		};
		class defs
		{
			class imageSets
			{
				files[] =
				{
					"ZenModPack/data/gui/tirerack/tirerackslots.imageset",
					"ZenModPack/data/gui/workbench/zencarworkbenchslots.imageset",
					"ZenModPack/data/gui/chess/zenchess_whiteslot.imageset",
					"ZenModPack/data/gui/chess/zenchess_blackslot.imageset",
					"ZenModPack/data/gui/music/zen_music.imageset",
					"ZenModPack/data/gui/sleepingbags/sleepingbag_icon.imageset"
				};
			};
			class engineScriptModule
			{
				files[]=
				{
					"ZenModPack/scripts/1_Core",
					"ZenModPack/scripts/Common"
				};
			};
			class gameLibScriptModule
			{
				files[]=
				{
					"ZenModPack/scripts/2_GameLib",
					"ZenModPack/scripts/Common"
				};
			};
			class gameScriptModule
			{
				files[] = 
				{ 
					"ZenModPack/scripts/3_Game",
					"ZenModPack/scripts/Common"
				};
			};
			class worldScriptModule
			{
				files[] = 
				{ 
					"ZenModPack/scripts/4_World",
					"ZenModPack/scripts/Common"
				};
			};
			class missionScriptModule
			{
				files[] = 
				{ 
					"ZenModPack/scripts/5_mission",
					"ZenModPack/scripts/common"
				};
			};
		};
	};
};

class CfgVehicles
{
	// Some general debugging config
	class ZenModPackConfig 
	{
		// Dumps all CfgVehicle types with scope=2 to script.txt file in profiles folder for types.xml'ing.
		dumpCfgVehicles = 0;
	}

	//! VANILLA CLASS CONFIG REQUIREMENTS
	class M18SmokeGrenade_Green;
	class GhillieAtt_ColorBase;
	class M18SmokeGrenade_Red;
	class SodaCan_ColorBase;
	class UndergroundStash;
	class HouseNoDestruct;
	class Container_Base;
	class ShelterLeather;
	class Inventory_Base;
	class PetrolLighter;
	class ShelterFabric;
	class FireplaceBase;
	class KitchenKnife;
	class Grenade_Base;
	class ShelterStick;
	class Bottle_Base;
	
	class WoodenCrate;
	class Roadflare;
	class CarScript;
	class CanOpener;
	class SeaChest;
	class CarWheel;
	class Box_Base;
	class Clothing;
	class Lockpick;
	class Hatchet;
	class HandSaw;
	class Shovel;
	class Man;

	//! SCREWDRIVER PRE-DEFINE - add workbench slot
	class Screwdriver : Inventory_Base
	{
		inventorySlot[] += { "ZenCWB_Screwdriver" };
	}
	//! MEGAPHONE PRE-DEFINE - add megaphone attachment for raid station
	class Megaphone : Inventory_Base
	{
		inventorySlot[] += { "ZenMegaphone" };
	}

	//! RAID ALARM
	class Zen_RaidAlarmStationKit : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmStation0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmStation1";
		model = "ZenModPack\data\models\RaidAlarm\rack_packed.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\raidalarm\rack_co.paa" };
		weight = 10000;
		itemSize[] = { 5,5 };
		physLayer = "item_small";
		rotationFlags = 17;
		itemBehaviour = 0;
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};
	class Zen_RaidAlarmStation : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmStation0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmStation1";
		model = "ZenModPack\data\models\RaidAlarm\rack.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\raidalarm\rack_co.paa" };
		forceFarBubble = "true";
		physLayer = "item_large";
		weight = 10000;
		itemSize[] = { 10,10 };
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		attachments[] = { "Back", "ZenMegaphone" };
		quantityBar = 1;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 604800; // 1 week of energy from full battery
		varQuantityDestroyOnMin = 0;
		stackedUnit = "time";
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 0;
			convertEnergyToQuantity = 1;
			energyStorageMax = 604800; // 1 week of energy from full battery
			energyUsagePerSecond = 1;
			reduceMaxEnergyByDamageCoef = 0.01;
			energyAtSpawn = 0;
			powerSocketsCount = 1;
			compatiblePlugTypes[] = { 2 };
			updateInterval = 15;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\RaidAlarm\rack.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\RaidAlarm\rack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};
	class Zen_RaidAlarmRadarKit : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmRadar0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmRadar1";
		model = "ZenModPack\data\models\zenkitbox\zenkitbox.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\raiddish\kit_radar_co.paa" };
		weight = 10000;
		itemSize[] = { 9,5 };
		physLayer = "item_small";
		itemBehaviour = 0;
		soundImpactType = "cloth";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\zenkitbox\kit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Zen_RaidAlarmRadar : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenRaidAlarmRadar0";
		descriptionShort = "$STR_CfgVehicles_ZenRaidAlarmRadar1";
		model = "ZenModPack\data\models\RaidDish\radar.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\raiddish\radar_co.paa" };
		weight = 10000;
		itemSize[] = { 10,10 };
		forceFarBubble = "true";
		physLayer = "item_large";
		class EnergyManager
		{
			hasIcon = 1;
			energyUsagePerSecond = 0.01;
			cordLength = 15;
			plugType = 2;
			updateInterval = 60;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\raiddish\radar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\raiddish\radar.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\raiddish\radar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\raiddish\radar_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\raiddish\radar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "wood";
	};

	// Static radar dish for aesthetic purposes
	class Zen_RaidAlarmRadar_Static : HouseNoDestruct
	{
		scope = 1;
		model = "ZenModPack\data\models\RaidDish\radar.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\raiddish\radar_co.paa" };
	};

	//! GENERIC KIT BOX 
	class ZenKitBoxBase : Inventory_Base
	{
		scope = 0;
		displayName = "Zen's Kit Box";
		descriptionShort = "Zen's Kit Box";
		model = "ZenModPack\data\models\zenkitbox\zenkitbox.p3d";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\zenkitbox\kit_blank_co.paa" };
		weight = 10000;
		itemSize[] = { 9,5 };
		physLayer = "item_small";
		itemBehaviour = 0;
		soundImpactType = "cloth";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\zenkitbox\kit.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\zenkitbox\kit_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\zenkitbox\kit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	//! SLEEPING BAGS 
	class ZenSleepingBag_PackedBase: Inventory_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenSleepingBag0";
		descriptionShort="$STR_CfgVehicles_ZenSleepingBag1";
		model="ZenModPack\data\models\sleepingbags\sleepingbag_case.p3d";
		hiddenSelections[]=
		{
			"bag"
		};
		inventorySlot[] =
		{
			"ZenSleepingBag"
		};
		hiddenSelectionsTextures[]={};
		isMeleeWeapon=0;
		weight=1000;
		absorbency=0;
		rotationFlags=16;
		itemSize[]={4,2};
		repairableWithKits[]={5,2};
		repairCosts[]={30,25};
		reversed=0;
		itemBehaviour=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 250;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_case.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_case.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_case_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_case_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_case_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ZenSleepingBagBlack_Packed: ZenSleepingBag_PackedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbagfolded_black_co.paa"
		};
	};
	class ZenSleepingBagBlue_Packed: ZenSleepingBag_PackedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbagfolded_blue_co.paa"
		};
	};
	class ZenSleepingBagGrey_Packed: ZenSleepingBag_PackedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbagfolded_grey_co.paa"
		};
	};
	class ZenSleepingBagRed_Packed: ZenSleepingBag_PackedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbagfolded_red_co.paa"
		};
	};
	class ZenSleepingBagGreen_Packed: ZenSleepingBag_PackedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbagfolded_green_co.paa"
		};
	};
	class ZenSleepingBag_DeployedBase: Container_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenSleepingBag0";
		descriptionShort="$STR_CfgVehicles_ZenSleepingBag1";
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		bounding="BSphere";
		overrideDrawArea="5.0";
		slopeTolerance=0.25;
		hiddenSelections[]=
		{
			"bag"
		};
		hiddenSelectionsTextures[]={};
		openable=0;
		weight=10000;
		carveNavmesh=1;
		itemSize[]={15,15};
		itemBehaviour=2;
		lootCategory="Tents";
		placement="ForceSlopeOnTerrain";
		repairableWithKits[]={5,2};
		repairCosts[]={30,25};
		attachments[]={};
		rotationFlags=2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 250;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\sleepingbags\sleepingbag_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={4,2};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
	};
	class ZenSleepingBagBlack_Deployed : ZenSleepingBag_DeployedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbag_black_co.paa"
		};
	};
	class ZenSleepingBagBlue_Deployed : ZenSleepingBag_DeployedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbag_blue_co.paa"
		};
	};
	class ZenSleepingBagGrey_Deployed : ZenSleepingBag_DeployedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbag_grey_co.paa"
		};
	};
	class ZenSleepingBagRed_Deployed : ZenSleepingBag_DeployedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbag_red_co.paa"
		};
	};
	class ZenSleepingBagGreen_Deployed : ZenSleepingBag_DeployedBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\sleepingbags\sleepingbag_green_co.paa"
		};
	};

	//! SWISS KNIFE 
	// Swiss knife base
	class ZenSwissKnife: Inventory_Base
	{
		scope=2;

		//! TOOL ENABLE/DISABLE CONFIG OVERRIDE - if you don't want one of these available for use on your server, set to 0.
		ZenSwissKnife_CanOpener		= 1;
		ZenSwissKnife_Lockpick		= 1;
		ZenSwissKnife_Screwdriver	= 1;
		ZenSwissKnife_Knife			= 1;
		ZenSwissKnife_Scissors		= 1;
		ZenSwissKnife_SmallSaw		= 1;

		//! SHARED CONFIG
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						// RVMAT damage texture handled in item script
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
	};

	//! CAN OPENER 
	class ZenSwissKnife_CanOpener: CanOpener
	{
		scope=2;
		//! SHARED
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=40; // Vanilla 20
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		//! CAN OPENER
		openItemSpillRange[]={0,0};
	};

	//! LOCKPICK
	class ZenSwissKnife_Lockpick: Lockpick
	{
		scope=2;
		//! SHARED
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=250; // Vanilla 125
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		//! LOCKPICK STUFF
		RestrainUnlockType=0;
		isLockpick=1;
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeFistLight";
				range=1;
			};
			class Heavy
			{
				ammo="MeleeFistHeavy";
				range=1;
			};
			class Sprint
			{
				ammo="MeleeFistHeavy";
				range=2.8;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class lockpicker_move_in
				{
					soundSet="lockpicker_move_in_SoundSet";
					id=110202;
				};
				class lockpicker_move_out
				{
					soundSet="lockpicker_move_out_SoundSet";
					id=110203;
				};
				class lockpicker_loop
				{
					soundSet="lockpicker_loop_SoundSet";
					id=203;
				};
				class lockpicker_end
				{
					soundSet="lockpicker_end_SoundSet";
					id=110204;
				};
			};
		};
	};

	//! SCREWDRIVER 
	class ZenSwissKnife_Screwdriver: Screwdriver
	{
		scope=2;
		//! SHARED
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100; // Vanilla 100
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		//! SCREWDRIVER
		repairableWithKits[]={4};
		repairCosts[]={5};
		suicideAnim="onehanded";
		RestrainUnlockType=1;
		openItemSpillRange[]={20,40};
		action="GestureMeleeBayonetStab";
		lockSoundSet="wrench_loop_SoundSet";
		compatibleLocks[]={1};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeSharpLight_1";
				range=1.1;
			};
			class Heavy
			{
				ammo="MeleeSharpHeavy_1";
				range=1.1;
			};
			class Sprint
			{
				ammo="MeleeSharpHeavy_1";
				range=3.3;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class animalSkinning_in
				{
					soundSet="animalSkinning_in_SoundSet";
					id=516;
				};
				class animalSkinning
				{
					soundSet="animalSkinning_SoundSet";
					id=517;
				};
				class animalSkinning_out
				{
					soundSet="animalSkinning_out_SoundSet";
					id=518;
				};
				class mineScrewdriver_SoundSet
				{
					soundSet="mineScrewdriver_SoundSet";
					id=11161;
				};
			};
		};
	};

	//! KNIFE
	class ZenSwissKnife_Knife: KitchenKnife
	{
		scope=2;
		//! SHARED
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=150; // Vanilla 100
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		//! KNIFE
		repairableWithKits[]={4};
		repairCosts[]={5};
		itemInfo[]={"Knife"};
		RestrainUnlockType=1;
		suicideAnim="onehanded";
		openItemSpillRange[]={10,20};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeSharpLight_2";
				range=1.2;
			};
			class Heavy
			{
				ammo="MeleeSharpHeavy_2";
				range=1.2;
			};
			class Sprint
			{
				ammo="MeleeSharpHeavy_2";
				range=3.3;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class ChoppingTree
				{
					soundSet="ChoppingTree_SoundSet";
					id=415;
				};
				class animalSkinning_in
				{
					soundSet="animalSkinning_in_SoundSet";
					id=516;
				};
				class animalSkinning
				{
					soundSet="animalSkinning_SoundSet";
					id=517;
				};
				class animalSkinning_out
				{
					soundSet="animalSkinning_out_SoundSet";
					id=518;
				};
			};
		};
	};

	//! SCISSORS 
	class ZenSwissKnife_Scissors: ZenSwissKnife_Knife
	{
		scope = 2;
		repairKitType=2;
		sewingDamage=5; // Health=100 / 20 repairs from pristine
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
	}

	//! HAND SAW 
	class ZenSwissKnife_SmallSaw: HandSaw
	{
		scope=2;
		//! SHARED
		displayName="$STR_CfgVehicles_ZenSwissKnife0";
		descriptionShort="$STR_CfgVehicles_ZenSwissKnife1";
		model="ZenModPack\data\models\swissknife\swissknife.p3d";
		inventorySlot[]={ "Knife" };
		animClass="Knife";
		rotationFlags=17;
		isMeleeWeapon=1;
		itemSize[]={1,2};
		weight=300;
		fragility=0.0099999998;
		simpleHiddenSelections[]= 
		{
			"can_opener",
			"packed",
			"large_knife",
			"peg",
			"saw",
			"scissors",
			"small_knife"
	    };
		hiddenSelections[] = 
		{
			"texture"
		};
		hiddenSelectionsTextures[] = 
		{ 
			"ZenModPack\data\textures\swissknife\swissknife_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100; // Vanilla 100
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\swissknife\swissknife.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		//! SAW
		repairableWithKits[]={4};
		repairCosts[]={15};
		itemInfo[]=
		{
			"Knife"
		};
		RestrainUnlockType=1;
		dismantle_action_type=64;
		openItemSpillRange[]={10,40};
		class MeleeModes
		{
			class Default
			{
				ammo="MeleeSharpLight_1";
				range=1.2;
			};
			class Heavy
			{
				ammo="MeleeSharpHeavy_1";
				range=1.2;
			};
			class Sprint
			{
				ammo="MeleeSharpHeavy_1";
				range=3.3;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class CutTiesSaw_end
				{
					soundSet="CutTiesSaw_end_SoundSet";
					id=519;
				};
				class CutTiesSaw_loop
				{
					soundSet="CutTiesSaw_loop_SoundSet";
					id=520;
				};
				class drop
				{
					soundset="hacksaw_drop_SoundSet";
					id=898;
				};
				class HackSaw_loop
				{
					soundSet="HackSaw_loop_SoundSet";
					id=1123;
				};
				class HackSaw_end
				{
					soundSet="HackSaw_end_SoundSet";
					id=1124;
				};
			};
		};
	};

	//! RUNES 
	class ZenRune_Base: Inventory_Base
	{
		scope=0;
		displayName="$STR_CfgVehicles_ZenRune0";
		descriptionShort="$STR_CfgVehicles_ZenRune1";
		model="\ZenModPack\data\models\runes\rune.p3d";
		animClass="NoFireClass";
		weight=500;
		itemSize[]={2,2};
		rotationFlags=16;
		hiddenSelections[]=
		{
			"rune"
		};
		isMeleeWeapon=1;
		soundImpactType="wood";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\rocks_bliss\data\rm_boulder2.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\rocks_bliss\data\rm_boulder2.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\rocks_bliss\data\rm_boulder2.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\rocks_bliss\data\rm_boulder2.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\rocks_bliss\data\rm_boulder2.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ZenRune_Air: ZenRune_Base
	{
		scope=2;
		displayName="Air Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\air.paa"
		};
	};
	class ZenRune_Body: ZenRune_Base
	{
		scope=2;
		displayName="Body Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\body.paa"
		};
	};
	class ZenRune_Chaos: ZenRune_Base
	{
		scope=2;
		displayName="Chaos Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\chaos.paa"
		};
	};
	class ZenRune_Cosmic: ZenRune_Base
	{
		scope=2;
		displayName="Cosmic Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\cosmic.paa"
		};
	};
	class ZenRune_Earth: ZenRune_Base
	{
		scope=2;
		displayName="Earth Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\earth.paa"
		};
	};
	class ZenRune_Fire: ZenRune_Base
	{
		scope=2;
		displayName="Fire Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\fire.paa"
		};
	};
	class ZenRune_Law: ZenRune_Base
	{
		scope=2;
		displayName="Law Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\law.paa"
		};
	};
	class ZenRune_Mind: ZenRune_Base
	{
		scope=2;
		displayName="Mind Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\mind.paa"
		};
	};
	class ZenRune_Nature: ZenRune_Base
	{
		scope=2;
		displayName="Nature Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\nature.paa"
		};
	};
	class ZenRune_Water: ZenRune_Base
	{
		scope=2;
		displayName="Water Rune";
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\runes\water.paa"
		};
	};

	//! JAMESON 
	// Whisky 
	class ZenJameson: Bottle_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenWhisky0";
		descriptionShort="$STR_CfgVehicles_ZenWhisky1";
		model="ZenModPack\data\models\jameson\jameson.p3d";
		weight=450;
		itemSize[]={1,3};
		destroyOnEmpty=0;
		varQuantityDestroyOnMin=0;
		varLiquidTypeInit=2048;
		liquidContainerType="1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varQuantityInit=750;
		varQuantityMin=0;
		varQuantityMax=750;
		varTemperatureMax=1;
		// Syberia 
		medPainkillerLevel=3;
		medPainkillerTimeSec=1;
		medAntidepresantLevel=1;
		medAntidepresantTimer=1;
		overdosedIncrement=0.05;
		// Texture
		hiddenSelections[]= { "zbytek" };
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\jameson\jameson_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=20;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\jameson\jameson.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\jameson\jameson.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\jameson\jameson_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\jameson\jameson_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\jameson\jameson_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="glass";
		class AnimEvents
		{
			class SoundWeapon
			{
				class GlassBottle_in_B
				{
					soundSet="GlassBottle_in_B_SoundSet";
					id=202;
				};
				class GlassBottle_in_C
				{
					soundSet="GlassBottle_in_C_SoundSet";
					id=203;
				};
				class WaterBottle_in_C1
				{
					soundSet="WaterBottle_in_C1_SoundSet";
					id=204;
				};
				class GlassBottle_out_A
				{
					soundSet="GlassBottle_out_A_SoundSet";
					id=205;
				};
				class GlassBottle_out_B
				{
					soundSet="GlassBottle_out_B_SoundSet";
					id=206;
				};
				class WellPond_loop
				{
					soundSet="WellPond_loop_SoundSet";
					id=209;
				};
				class WellBottle_loop
				{
					soundSet="WellBottle_loop_SoundSet";
					id=210;
				};
			};
		};
	};
	class Empty_ZenJameson: Bottle_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenWhisky0";
		descriptionShort="$STR_CfgVehicles_ZenWhisky2";
		model="ZenModPack\data\models\jameson\jameson.p3d";
		weight=450;
		itemSize[]={1,3};
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varQuantityInit = 750;
		varQuantityMin = 0;
		varQuantityMax = 750;
		hiddenSelections[]= { "zbytek" };
		hiddenSelectionsTextures[]=
		{
			"ZenModPack\data\textures\jameson\jameson_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=20;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\jameson\jameson.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\jameson\jameson.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\jameson\jameson_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\jameson\jameson_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\jameson\jameson_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="glass";
		class AnimEvents
		{
			class SoundWeapon
			{
				class GlassBottle_in_B
				{
					soundSet="GlassBottle_in_B_SoundSet";
					id=202;
				};
				class GlassBottle_in_C
				{
					soundSet="GlassBottle_in_C_SoundSet";
					id=203;
				};
				class WaterBottle_in_C1
				{
					soundSet="WaterBottle_in_C1_SoundSet";
					id=204;
				};
				class GlassBottle_out_A
				{
					soundSet="GlassBottle_out_A_SoundSet";
					id=205;
				};
				class GlassBottle_out_B
				{
					soundSet="GlassBottle_out_B_SoundSet";
					id=206;
				};
				class WellPond_loop
				{
					soundSet="WellPond_loop_SoundSet";
					id=209;
				};
				class WellBottle_loop
				{
					soundSet="WellBottle_loop_SoundSet";
					id=210;
				};
			};
		};
	};

	//! BUNNY MASKS 
	class EyeMask_ZenRabbit_ColorBase: Clothing
	{
		displayName="$STR_CfgVehicles_ZenRabbitMask0";
		descriptionShort="$STR_CfgVehicles_ZenRabbitMask1";
		model="\ZenModPack\data\models\bunnymask\rabbitmask_g.p3d";
		vehicleClass="Clothing";
		simulation="clothing";
		inventorySlot[]=
		{
			"Eyewear"
		};
		itemInfo[]=
		{
			"Clothing",
			"Eyewear"
		};
		repairableWithKits[] = { 8 };
		repairCosts[] = { 25 };
		rotationFlags=1;
		weight=25;
		itemSize[]={2,1};
		varWetMax=0.1;
		heatIsolation=0.1;
		noMask=1;
		hiddenSelections[]=
		{
			"rabbitmask"
		};
		class ClothingTypes
		{
			male="\ZenModPack\data\models\bunnymask\rabbitmask_m.p3d";
			female="\ZenModPack\data\models\bunnymask\rabbitmask_f.p3d";
		};
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\bunnymask\rabbitmask_black_co.paa"
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="SportGlasses_pickup_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="SportGlasses_drop_SoundSet";
					id=898;
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=50;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"\ZenModPack\data\textures\bunnymask\rabbitmask.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"\ZenModPack\data\textures\bunnymask\rabbitmask.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"\ZenModPack\data\textures\bunnymask\rabbitmask_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"\ZenModPack\data\textures\bunnymask\rabbitmask_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"\ZenModPack\data\textures\bunnymask\rabbitmask_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class EyeMask_ZenRabbit_Black: EyeMask_ZenRabbit_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\bunnymask\rabbitmask_black_co.paa"
		};
	};
	class EyeMask_ZenRabbit_Blue: EyeMask_ZenRabbit_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\bunnymask\rabbitmask_blue_co.paa"
		};
	};
	class EyeMask_ZenRabbit_Pink: EyeMask_ZenRabbit_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\bunnymask\rabbitmask_pink_co.paa"
		};
	};
	class EyeMask_ZenRabbit_Green: EyeMask_ZenRabbit_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\bunnymask\rabbitmask_green_co.paa"
		};
	};

	//! CAMP SITES 
	class ZenCamps_ShelterStick : ShelterStick
	{
		scope = 2;
		itemsCargoSize[] = { 8,6 };
	};
	class ZenCamps_ShelterFabric : ShelterFabric
	{
		scope = 2;
		itemsCargoSize[] = { 9,8 };
	};
	class ZenCamps_ShelterLeather : ShelterLeather
	{
		scope = 2;
		itemsCargoSize[] = { 10,10 };
	}

	//! RAYBANS 
	class ZenRaybans_ColorBase: Clothing
	{
		scope=0;
		displayName="$STR_CfgVehicles_ZenRayBans0";
		descriptionShort="$STR_CfgVehicles_ZenRayBans1";
		model="\ZenModPack\data\models\raybans\raybans_g.p3d";
		vehicleClass="Clothing";
		simulation="clothing";
		inventorySlot[]=
		{
			"Eyewear"
		};
		itemInfo[]=
		{
			"Clothing",
			"Eyewear"
		};
		// Repairable with epoxy
		repairableWithKits[]={8};
		repairCosts[]={25};
		rotationFlags=17; // 17 = ROTATE_FrontBack
		itemSize[]={3,1};
		weight=20;
		lootCategory="Eyewear";
		lootTag[]=
		{
			"Civilian"
		};
		hiddenSelections[]=
		{
			"frame",
			"glass"
		};
		class ClothingTypes
		{
			male="\ZenModPack\data\models\raybans\raybans_m.p3d";
			female="\ZenModPack\data\models\raybans\raybans_f.p3d";
		};
		soundImpactType="plastic";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="SportGlasses_pickup_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="SportGlasses_drop_SoundSet";
					id=898;
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=50;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\raybans\raybans.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\raybans\raybans.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\raybans\raybans_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\raybans\raybans_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\raybans\raybans_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ZenRaybans_Black: ZenRaybans_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\raybans\frame_co.paa",
			"\ZenModPack\data\textures\raybans\glass_black_ca.paa"

			//"#(argb,8,8,3)color(0,0,0,0.92,CO)" // No texture just plain colour (no Rayban logo)
		};
	};
	class ZenRaybans_Red: ZenRaybans_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\raybans\frame_co.paa",
			"\ZenModPack\data\textures\raybans\glass_red_ca.paa"
		};
	};
	class ZenRaybans_Green: ZenRaybans_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\raybans\frame_co.paa",
			"\ZenModPack\data\textures\raybans\glass_green_ca.paa"
		};
	};
	class ZenRaybans_Blue: ZenRaybans_ColorBase
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\raybans\frame_co.paa",
			"\ZenModPack\data\textures\raybans\glass_blue_ca.paa"
		};
	};

	//! GLOVEBOX 
	class ZenGlovebox : WoodenCrate
	{
		displayName = "$STR_CfgVehicles_ZenGlovebox";
		descriptionShort = "";
		weight = 1;
		itemSize[] = { 1,1 };
		canBeDigged = 0;
		class Cargo
		{
			itemsCargoSize[] = { 10,2 };
			openable = 0;
			allowOwnedCargoManipulation = 1;
		}
		inventorySlot[] +=
		{
			"ZenCarGlovebox"
		};
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"#(argb,8,8,3)color(1,1,1,0,CA)"
		};
	}

	//! MUSIC
	class Zen_CassetteCase : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenCassetteCase0";
		descriptionShort = "$STR_CfgVehicles_ZenCassetteCase1";
		model = "\dz\gear\tools\cleaning_kit_wood.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\music\case\cassette_case.paa" };
		animClass = "Knife";
		rotationFlags = 17;
		weight = 250;
		weightPerQuantityUnit = 0;
		itemSize[] = { 2,3 };
		itemsCargoSize[] = { 10,5 };
		fragility = 0.0099999998;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\tools\data\cleaning_kit_wood.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\tools\data\cleaning_kit_wood.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\tools\data\cleaning_kit_wood_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\tools\data\cleaning_kit_wood_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\tools\data\cleaning_kit_wood_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
		soundImpactType = "wood";
	};
    class Zen_BoomBox : Inventory_Base
    {
        scope = 2;
        displayName = "$STR_CfgVehicles_ZenBoombox0";
        descriptionShort = "$STR_CfgVehicles_ZenBoombox1";
        initVolume = 1.0;
		maxVolume = 2.0;
		noiseType = "Loud";
		attractsZombies = "true";
        model = "\ZenModPack\data\models\music\boombox\zen_boombox.p3d";
		itemBehaviour = 1;
        weight = 500;
        itemSize[] = {4,3};
        attachments[] =
        {
            "BatteryD",
            "ZenCassette"
        };
        repairableWithKits[] = { 7 };
        repairCosts[] = { 25 };
        class EnergyManager
        {
            hasIcon = 1;
            autoSwitchOff = 1;
            energyUsagePerSecond = 0.01;
            plugType = 1;
            attachmentAction = 1;
            wetnessExposure = 0.1;
        };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\music\boombox\boombox.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\music\boombox\boombox.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\music\boombox\boombox_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\music\boombox\boombox_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\music\boombox\boombox_destruct.rvmat"
							}
						}
					};
				};
			};
		};
    };
    class Zen_Walkman : Inventory_Base
    {
        scope = 2;
        displayName = "$STR_CfgVehicles_ZenWalkman0";
        descriptionShort = "$STR_CfgVehicles_ZenWalkman1";
        rotationFlags = 16;
		initVolume = 0.25;
		maxVolume = 0.25;
		noiseType = "Quiet";
		attractsZombies = "true";
        model = "\ZenModPack\data\models\music\walkman\zen_walkman.p3d";
        weight = 500;
        itemSize[] = { 1,2 };
        attachments[] =
        {
            "BatteryD",
            "ZenCassette"
        };
		inventorySlot[] =
		{
			"WalkieTalkie"
		};
        repairableWithKits[] = { 7 };
        repairCosts[] = { 25 };
        class EnergyManager
        {
            hasIcon = 1;
            autoSwitchOff = 1;
            energyUsagePerSecond = 0.01;
            plugType = 1;
            attachmentAction = 1;
            wetnessExposure = 0.1;
        };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\music\walkman\walkman.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\music\walkman\walkman.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\music\walkman\walkman_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\music\walkman\walkman_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\music\walkman\walkman_destruct.rvmat"
							}
						}
					};
				};
			};
		};
    };
    class Zen_Cassette_Base : Inventory_Base
    {
        displayName = "$STR_CfgVehicles_ZenCassette0";
        descriptionShort = "$STR_CfgVehicles_ZenCassette1";
        model = "\dz\gear\tools\cassette.p3d";
        absorbency = 0;
        rotationFlags = 16;
        weight = 100;
        itemSize[] = { 2,1 };
        fragility = 0.0099999998;
        varWetMax = 0.5;
		playSeconds = -1;
		copyrighted = 0;
		isMusic = 1;
        inventorySlot[] =
        {
            "ZenCassette"
        };
		class CfgCassette
		{
			soundSet = "Your_Song_Soundset";
			playSeconds = 69; // play time in secs
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\tools\data\cassette.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\tools\data\cassette.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\tools\data\cassette_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\tools\data\cassette_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\tools\data\cassette_destruct.rvmat"
							}
						}
					};
				};
			};
		};
        class MeleeModes
        {
            class Default
            {
                ammo = "MeleeLightBlunt";
                range = 1;
            };
            class Heavy
            {
                ammo = "MeleeLightBlunt_Heavy";
                range = 1;
            };
            class Sprint
            {
                ammo = "MeleeLightBlunt_Heavy";
                range = 2.8;
            };
        };
    };
	class Zen_MusicNoise : Inventory_Base
	{
		// Zombie noises
		scope = 2;
		class Quiet
		{
			strength = 120;
			type = "sound";
		};
		class Loud
		{
			strength = 500;
			type = "sound";
		};
		class Louder
		{
			strength = 1000;
			type = "sound";
		};
	};
	// Vehicles
	class OffroadHatchback : CarScript
	{
		attachments[] +=
		{
			"ZenCassette",
			"ZenCarGlovebox",
			"Chemlight"
		};
		class GUIInventoryAttachmentsProps
		{
			class ZenMusicCarRadio
			{
				view_index=1;
				name = "$STR_CfgVehicles_ZenCassette0";
				description = "$STR_CfgVehicles_ZenCassette0";
				icon = "set:zen_music image:cassette_icon";
				attachmentSlots[] =
				{
					"ZenCassette"
				};
			}
			class ZenPimpMyRide
			{
				name = "$STR_CfgVehicles_Chemlight_ColorBase0";
				description = "$STR_CfgVehicles_Chemlight_ColorBase0";
				icon = "set:dayz_inventory image:chemlight";
				attachmentSlots[] =
				{
					"Chemlight"
				};
			}
			class ZenCarGlovebox
			{
				name = "$STR_CfgVehicles_ZenGlovebox";
				description = "$STR_CfgVehicles_ZenGlovebox";
				icon = "missing";
				attachmentSlots[] = { "ZenCarGlovebox" };
			}
		}
	}
	class Offroad_02 : CarScript
	{
		attachments[] +=
		{
			"ZenCassette",
			"ZenCarGlovebox",
			"Chemlight"
		};
		class GUIInventoryAttachmentsProps
		{
			class ZenMusicCarRadio
			{
				view_index=1;
				name = "$STR_CfgVehicles_ZenCassette0";
				description = "$STR_CfgVehicles_ZenCassette0";
				icon = "Cassette";
				attachmentSlots[] =
				{
					"ZenCassette"
				};
			}
			class ZenPimpMyRide
			{
				name = "$STR_CfgVehicles_Chemlight_ColorBase0";
				description = "$STR_CfgVehicles_Chemlight_ColorBase0";
				icon = "set:dayz_inventory image:chemlight";
				attachmentSlots[] =
				{
					"Chemlight"
				};
			}
			class ZenCarGlovebox
			{
				name = "$STR_CfgVehicles_ZenGlovebox";
				description = "$STR_CfgVehicles_ZenGlovebox";
				icon = "missing";
				attachmentSlots[] = { "ZenCarGlovebox" };
			}
		}
	}
	class CivilianSedan : CarScript
	{
		attachments[] +=
		{
			"ZenCassette",
			"ZenCarGlovebox",
			"Chemlight"
		};
		class GUIInventoryAttachmentsProps
		{
			class ZenMusicCarRadio
			{
				view_index=1;
				name = "$STR_CfgVehicles_ZenCassette0";
				description = "$STR_CfgVehicles_ZenCassette0";
				icon = "set:zen_music image:cassette_icon";
				attachmentSlots[] =
				{
					"ZenCassette"
				};
			}
			class ZenPimpMyRide
			{
				name = "$STR_CfgVehicles_Chemlight_ColorBase0";
				description = "$STR_CfgVehicles_Chemlight_ColorBase0";
				icon = "set:dayz_inventory image:chemlight";
				attachmentSlots[] =
				{
					"Chemlight"
				};
			}
			class ZenCarGlovebox
			{
				name = "$STR_CfgVehicles_ZenGlovebox";
				description = "$STR_CfgVehicles_ZenGlovebox";
				icon = "missing";
				attachmentSlots[] = { "ZenCarGlovebox" };
			}
		}
	}
	class Hatchback_02 : CarScript
	{
		attachments[] +=
		{
			"ZenCassette",
			"ZenCarGlovebox",
			"Chemlight"
		};
		class GUIInventoryAttachmentsProps
		{
			class ZenMusicCarRadio
			{
				view_index=1;
				name = "$STR_CfgVehicles_ZenCassette0";
				description = "$STR_CfgVehicles_ZenCassette0";
				icon = "Cassette";
				attachmentSlots[] =
				{
					"ZenCassette"
				};
			}
			class ZenPimpMyRide
			{
				name = "$STR_CfgVehicles_Chemlight_ColorBase0";
				description = "$STR_CfgVehicles_Chemlight_ColorBase0";
				icon = "set:dayz_inventory image:chemlight";
				attachmentSlots[] =
				{
					"Chemlight"
				};
			}
			class ZenCarGlovebox
			{
				name = "$STR_CfgVehicles_ZenGlovebox";
				description = "$STR_CfgVehicles_ZenGlovebox";
				icon = "missing";
				attachmentSlots[] = { "ZenCarGlovebox" };
			}
		}
	}
	class Sedan_02 : CarScript
	{
		attachments[] +=
		{
			"ZenCassette",
			"ZenCarGlovebox",
			"Chemlight" 
		};
		class GUIInventoryAttachmentsProps
		{
			class ZenMusicCarRadio
			{
				view_index=1;
				name = "$STR_CfgVehicles_ZenCassette0";
				description = "$STR_CfgVehicles_ZenCassette0";
				icon = "Cassette";
				attachmentSlots[] =
				{
					"ZenCassette"
				};
			}
			class ZenPimpMyRide
			{
				name = "$STR_CfgVehicles_Chemlight_ColorBase0";
				description = "$STR_CfgVehicles_Chemlight_ColorBase0";
				icon = "set:dayz_inventory image:chemlight";
				attachmentSlots[] =
				{
					"Chemlight"
				};
			}
			class ZenCarGlovebox
			{
				name = "$STR_CfgVehicles_ZenGlovebox";
				description = "$STR_CfgVehicles_ZenGlovebox";
				icon = "missing";
				attachmentSlots[] = { "ZenCarGlovebox" };
			}
		}
	}

	//! TREASURE 
	// Turns into a random ammo box
	class ZenRandomAmmoBox: Box_Base
	{
		scope=2;
		displayName="$STR_CfgVehicles_AmmoBox_556x45_20Rnd0";
		descriptionShort="$STR_CfgVehicles_AmmoBox_556x45_20Rnd1";
		model="\dz\weapons\ammunition\556_20RoundBox.p3d";
		debug_ItemCategory=5;
		rotationFlags=17;
		weight=85;
	};

	class ZenTreasure_UndergroundStash			: UndergroundStash {};
	class ZenTreasure_UndergroundStashWinter	: ZenTreasure_UndergroundStash 
	{
		// For WinterChernarus V2 compatibility - ignore if you don't use WinterChernarusV2
		model="WinterStash\Snow_dirtpile\Snow_dirtpile.p3d";
	};

	class ZenTreasure_WoodenCrate : WoodenCrate
	{
		scope = 2;
		descriptionShort = "$STR_CfgVehicles_ZenDamagedBox";
		inventorySlot[] = {};
	};
	class ZenTreasure_SeaChest : SeaChest
	{
		scope = 2;
		descriptionShort = "$STR_CfgVehicles_ZenDamagedBox";
	};

	class ZenTreasure_DebugShovel : Shovel
	{
		scope = 2;
		displayName = "TREASURE DEBUG SHOVEL";
		descriptionShort = "NOT FOR ORDINARY HUMANS - WILL DIG UP STASHES VERY FAST!";
	};

	class ZenTreasure_PhotoBase : Inventory_Base
	{
		scope = 0;
		displayName = "$STR_CfgVehicles_ZenPhoto0";
		descriptionShort = "$STR_CfgVehicles_ZenPhoto1";
		model = "ZenModPack\data\models\treasure\photo.p3d";
		hiddenSelections[] = { "front" };
		weight = 1;
		itemSize[] = { 1,1 };
		isMeleeWeapon=1;
		absorbency=1;
		rotationFlags=1; // 1 = rotate front-up, 16 = rotate front-down
		varWetMax=1;
		treasure_stash_x = -1.0;
		treasure_stash_z = -1.0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=15;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"ZenModPack\data\textures\treasure\photo_material.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"ZenModPack\data\textures\treasure\photo_material.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"ZenModPack\data\textures\treasure\photo_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"ZenModPack\data\textures\treasure\photo_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"ZenModPack\data\textures\treasure\photo_destruct.rvmat"
							}
						}
					};
				};
			}
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpPaper_SoundSet";
					id=797;
				};
			};
		};
		soundImpactType = "cloth";
	};

	// This turns into a random photograph upon spawn. The spawn list is determined by photo *.cpp entries.
	// Name any child photos in treasure addons 'ZenTreasure_PhotoXX', where XX is the photo number, in order, from 01-99+ (eg. ZenTreasure_Photo69)
	class ZenTreasure_RandomPhoto : ZenTreasure_PhotoBase
	{
		scope = 2;
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\treasure\photo_random.paa" };
	}

	//! ZIPPO 
	// Vanilla Petrol lighter EnergyManager config
	class ZenPetrolLighter: PetrolLighter
	{
		varQuantityMin=0;
		varQuantityMax=900; 
		varQuantityInit=900;
		varQuantityDestroyOnMin=0;
		class EnergyManager
		{
			autoSwitchOff=1;
			energyAtSpawn=900; // 15 mins burn time
			energyStorageMax=900;
			energyUsagePerSecond=1;
			updateInterval=1;
			convertEnergyToQuantity=1;
			autoSwitchOffWhenInCargo=1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class LighterZippo
				{
					soundSet=""; // Mute action SFX as lighter is lit
					id=201;
				};
			};
		};
	}

	// My zippos
	class ZenZippoLighter: ZenPetrolLighter
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenZippoLighter0";
		descriptionShort="$STR_CfgVehicles_ZenZippoLighter1";
		model="ZenModPack\data\models\zippo\zippo.p3d";
		rotationFlags=17;
		isMeleeWeapon=1;
		canBeSplit=0;
		absorbency=0;
		weight=50;
		itemSize[]={1,1};
		stackedUnit="ml";
		quantityBar=1;
		varQuantityMin=0;
		varQuantityMax=1800;
		varQuantityInit=1800;
		varQuantityDestroyOnMin=0;
		varTotalLifetime=7200; // How many seconds does this zippo work for before ruining from Pristine?
		simpleHiddenSelections[]= 
		{
			"top_open",
			"top_closed"
	    };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\zippo\zippo_co.paa" };
		class EnergyManager
		{
			autoSwitchOff=1;
			energyAtSpawn=1800; // 30 mins burn time
			energyStorageMax=1800;
			energyUsagePerSecond=1;
			updateInterval=1;
			convertEnergyToQuantity=1;
			autoSwitchOffWhenInCargo=1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\zippo\zippo.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\zippo\zippo.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\zippo\zippo_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\zippo\zippo_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\zippo\zippo_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class LighterZippo
				{
					soundSet=""; // Mute action SFX as lighter is lit
					id=201;
				};
			};
		};
		soundImpactType="metal";
	};
	class ZenZippoLighter_Peace: ZenZippoLighter
	{
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\zippo\zippo_peace_co.paa" };
	};
	class ZenZippoLighter_Anarchy: ZenZippoLighter
	{
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\zippo\zippo_anarchy_co.paa" };
	};
	class ZenZippoLighter_Zenarchist: ZenZippoLighter
	{
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\zippo\zippo_zenarchist_co.paa" };
	};

	//! WOLF MASK 
	class ZenWolfMask: Clothing
	{
		scope=2;
		displayName="$STR_CfgVehicles_ZenWolfMask0";
		descriptionShort="$STR_CfgVehicles_ZenWolfMask1";
		model="\ZenModPack\data\models\wolfmask\wolfmask_g.p3d";
		weight=1000;
		itemSize[]={4,3};
		repairableWithKits[]={8}; // todo
		repairCosts[]={25};
		inventorySlot[]=
		{
			"Headgear"
		};
		simulation="clothing";
		vehicleClass="Clothing";
		itemInfo[]=
		{
			"Clothing",
			"Headgear"
		};
		visibilityModifier=0.89999998;
		varWetMax=0.249;
		heatIsolation=1.0;
		headSelectionsToHide[]=
		{
			"Clipping_HelmetMich"
		};
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"\ZenModPack\data\textures\wolfmask\wolfmask_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=75;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"\ZenModPack\data\textures\wolfmask\wolfmask.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"\ZenModPack\data\textures\wolfmask\wolfmask.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"\ZenModPack\data\textures\wolfmask\wolfmask_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"\ZenModPack\data\textures\wolfmask\wolfmask_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"\ZenModPack\data\textures\wolfmask\wolfmask_destruct.rvmat"
							}
						}
					};
				};
			};
			// Copied from Ballistic Helmet
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0.25;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0.5;
					};
				};
				class Melee
				{
					class Health
					{
						damage=0.55000001;
					};
					class Blood
					{
						damage=0.2;
					};
					class Shock
					{
						damage=0.5;
					};
				};
				class Infected
				{
					class Health
					{
						damage=0.55000001;
					};
					class Blood
					{
						damage=0.2;
					};
					class Shock
					{
						damage=0.5;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=0.5;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0.25999999;
					};
				};
			};
		};
		class ClothingTypes
		{
			male="\ZenModPack\data\models\wolfmask\wolfmask.p3d";
			female="\ZenModPack\data\models\wolfmask\wolfmask.p3d";
		};
		soundImpactType="organic";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpPot_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="BallisticHelmet_drop_SoundSet";
					id=898;
				};
			};
		};
	};

	//! GRAVES 
	class ZenGraves_DeadPlayerCross : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenGrave0";
		descriptionShort = "$STR_CfgVehicles_ZenGrave1";
		model = "\DZ\structures\specific\cemeteries\cemetery_smallcross.p3d";
		forceFarBubble = "true";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 999999;
					healthLevels[] =
					{

						{
							1,
							{}
						},

						{
							0.69999999,
							{}
						},

						{
							0.5,
							{}
						},

						{
							0.30000001,
							{}
						},

						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones {};
		};
	};
	class ZenGraves_DeadPlayerSkeleton : Inventory_Base
	{
		scope = 2;
		canBeDigged = 1;
		displayName = "$STR_CfgVehicles_ZenSkeleton0";
		descriptionShort = "$STR_CfgVehicles_ZenSkeleton1";
		model = "\DZ\structures_bliss\specific\Forest\Forest_HumanSkeleton.p3d";
		attachments[] =		
		{
			"Shoulder",
			"Melee",
			"Headgear",
			"Eyewear",
			"Mask",
			"Body",
			"Gloves",
			"Vest",
			"Hips",
			"Legs",
			"Feet",
			"Back",
			"Armband"
		};
	};

	class ZenGraves_UndergroundStash : UndergroundStash {};

	//! ANTI-COMBAT LOG 
	// Define combat log flare
	class Zen_CombatLogFlare : Roadflare
	{
		scope = 2;
		varQuantityDestroyOnMin = 1;
		class EnergyManager
		{
			energyAtSpawn = 200;
			energyUsagePerSecond = 1;
			updateInterval = 10;
			convertEnergyToQuantity = 1;
		};
		class NoiseRoadFlare
		{
			strength = 0;
			type = "";
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 9999;
				}
			}
		};
	};

	// Define ammo scanner
	class Zen_CombatLogTrigger : Inventory_Base
	{
		scope = 1;
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
	};
	class Zen_CombatLogExplosiveTrigger : Zen_CombatLogTrigger
	{
		scope = 1;
	};

	//! CAMONET SHELTER
	class Zen_CamoShelterKit : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenCamoShelterKit0";
		descriptionShort = "$STR_CfgVehicles_ShelterKit1";
		model = "\DZ\gear\camping\watchtower_kit.p3d";
		rotationFlags = 17;
		itemSize[]={1,5};
		weight = 500;
		itemBehaviour = 1;
		attachments[] =
		{
			"Rope",
			"Material_Shelter_FrameSticks",
			"CamoNet"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] =
					{

						{
							1,
							{}
						},

						{
							0.69999999,
							{}
						},

						{
							0.5,
							{}
						},

						{
							0.30000001,
							{}
						},

						{
							0,
							{}
						}
					};
				};
				class GlobalArmor
				{
					class Projectile
					{
						class Health
						{
							damage = 0;
						};
						class Blood
						{
							damage = 0;
						};
						class Shock
						{
							damage = 0;
						};
					};
					class FragGrenade
					{
						class Health
						{
							damage = 0;
						};
						class Blood
						{
							damage = 0;
						};
						class Shock
						{
							damage = 0;
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.0099999998;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.0099999998;
				initPhase = 1;
			};
			class Inventory : AnimSourceHidden
			{
			};
			class Placing : AnimSourceHidden
			{
			};
		};
		soundImpactType = "wood";
		class AnimEvents
		{
			class SoundWeapon
			{
				class crafting_1
				{
					soundSet = "FenceKit_crafting_1_SoundSet";
					id = 1111;
				};
				class crafting_2
				{
					soundSet = "FenceKit_crafting_2_SoundSet";
					id = 1112;
				};
				class crafting_3
				{
					soundSet = "FenceKit_crafting_3_SoundSet";
					id = 1113;
				};
				class crafting_4
				{
					soundSet = "FenceKit_crafting_4_SoundSet";
					id = 1114;
				};
				class crafting_5
				{
					soundSet = "FenceKit_crafting_5_SoundSet";
					id = 1115;
				};
			};
		};
	};
    class Zen_ImprovisedShelter : Inventory_Base
    {
        scope = 2;
		displayName = "$STR_Zen_ImprovisedShelter0";
		descriptionShort = "$STR_Zen_ImprovisedShelter1";
		model = "\ZenModPack\data\models\camonetshelter\ZenCamoShelter.p3d";
		forceFarBubble = "true";
        class Cargo
        {
            itemsCargoSize[] = { 10,4 };
            openable = 0;
            allowOwnedCargoManipulation = 1;
        }
		attachments[]=
		{
			"Back",
			"Shoulder"
		};
        soundImpactType = "textile";
		physLayer = "item_large";
        weight = 10000;
        itemSize[] = { 20,20 };
		hiddenSelections[] =
		{
			"camonet",
			"sticks",
			"ropes"
		};
		hiddenSelectionsTextures[] = 
		{
			"dz\structures\military\improvised\data\camonet_east_co.paa",
			"ZenModPack\data\textures\camonetshelter\sticks_co.paa",
			"dz\gear\crafting\data\bp_rope_black_co.paa"
		};
        class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=120;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\camping\data\camo_net_p.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\camping\data\camo_net_p.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\camping\data\camo_net_p_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\camping\data\camo_net_p_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\camping\data\camo_net_p_destruct.rvmat"
							}
						}
					};
				};
			};
		};
    }

	//! NOTES 
	// Make paper stackable up to 5
	class Paper : Inventory_Base
	{
		canBeSplit = 1;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 5;
		varStackMax = 5;
		varQuantityDestroyOnMin = 1;
	};

	// Make punched card non-stackable as it inherits from Paper
	class PunchedCard : Paper
	{
		canBeSplit = 0;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 1;
		varStackMax = 1;
		varQuantityDestroyOnMin = 1;
	};

	// Define a written note (non-stackable)
	class ZenNote : Paper
	{
		scope = 2;
		model = "\dz\gear\consumables\Paper.p3d";
		displayName = "$STR_ZenNoteTxt";
		descriptionShort = "$STR_ZenNoteDesc";
		canBeSplit = 0;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 1;
		varStackMax = 1;
	};

	// Define colored pens
	class Pen_ColorBase : Inventory_Base
	{
		quantityBar = 1;
		varQuantityInit = 100;
		varQuantityMin = 0;
		varQuantityMax = 100;
		stackedUnit = "percentage";
	};
	class Pen_Black : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Black";
		penColor[] = { 10,10,10 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_black_co.paa" };
	};
	class Pen_Red : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Red";
		penColor[] = { 200,20,20 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_red_co.paa" };
	};
	class Pen_Green : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Green";
		penColor[] = { 24,150,24 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_green_co.paa" };
	};
	class Pen_Blue : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Blue";
		penColor[] = { 0,65,200 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_blue_co.paa" };
	};
	class Pen_Pink : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Pink";
		penColor[] = { 255,0,162 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_pink_co.paa" };
	};
	class Pen_Purple : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Purple";
		penColor[] = { 144,0,255 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_purple_co.paa" };
	};
	class Pen_Orange : Pen_ColorBase
	{
		scope = 2;
		displayName = "$STR_ZenPen_Orange";
		penColor[] = { 255,150,0 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\notes\loot_pen_orange_co.paa" };
	};

	//! VIKING AXE 
	class Zen_VikingAxe : Hatchet
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZENVIKINGAXE0";
		descriptionShort = "$STR_CfgVehicles_ZENVIKINGAXE1";
		model = "ZenModPack\data\models\vikingaxe\viking_axe.p3d";
		debug_ItemCategory = 2;
		build_action_type = 10;
		dismantle_action_type = 74;
		repairableWithKits[] = { 4 };
		repairCosts[] = { 18 };
		rotationFlags = 17;
		weight = 1133;
		itemSize[] = { 2,5 };
		fragility = 0.0099999998;
		/*simpleHiddenSelections[]= 
		{
			"front",
			"backwards"
	    };*/
		inventorySlot[] +=
		{
			"Hatchet",
			"Hatchet1",
			"Hatchet2"
		};
		lootCategory = "Tools";
		lootTag[] =
		{
			"Work",
			"Forester",
			"Camping"
		};
		itemInfo[] =
		{
			"Axe"
		};
		openItemSpillRange[] = { 20,40 };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 300;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\vikingaxe\viking_axe.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\vikingaxe\viking_axe.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\vikingaxe\viking_axe_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\vikingaxe\viking_axe_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\vikingaxe\viking_axe_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeHatchet";
				range = 1.4;
			};
			class Heavy
			{
				ammo = "MeleeHatchet_Heavy";
				range = 1.4;
			};
			class Sprint
			{
				ammo = "MeleeHatchet_Heavy";
				range = 3.3;
			};
		};
		isMeleeWeapon = 1;
		suicideAnim = "woodaxe";
		soundImpactType = "metal";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickup_light
				{
					soundSet = "hatchet_pickup_light_SoundSet";
					id = 796;
				};
				class pickup
				{
					soundSet = "hatchet_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "woodaxe_drop_SoundSet";
					id = 898;
				};
				class FirefighterAxe_loop_SoundSet
				{
					soundSet = "FirefighterAxe_loop_SoundSet";
					id = 1121;
				};
				class FirefighterAxe_end_SoundSet
				{
					soundSet = "FirefighterAxe_end_SoundSet";
					id = 1122;
				};
				class ShoulderR_Hide
				{
					soundset = "ShoulderR_Hide_SoundSet";
					id = 1210;
				};
				class ShoulderR_Show
				{
					soundset = "ShoulderR_Show_SoundSet";
					id = 1211;
				};
				class WoodHammer_SoundSet
				{
					soundSet = "WoodHammer_SoundSet";
					id = 11161;
				};
				class animalSkinning_in
				{
					soundSet = "animalSkinning_in_SoundSet";
					id = 516;
				};
				class animalSkinning
				{
					soundSet = "animalSkinning_SoundSet";
					id = 517;
				};
				class animalSkinning_out
				{
					soundSet = "animalSkinning_out_SoundSet";
					id = 518;
				};
			};
		};
	};

	//! CHESS 
	class Zen_ChessBoard : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_ZenChessBoardName";
		descriptionShort = "$STR_ZenChessBoardDesc";
		model = "ZenModPack\data\models\chess\chessboard.p3d";
		weight = 500;
		itemSize[] = { 3,3 };
		itemBehaviour = 1;
		repairableWithKits[] = { 8 };
		repairCosts[] = { 30 };
		class Cargo
		{
			itemsCargoSize[]={8,4};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		attachments[] = 
		{ 
			"ZenChess1",
			"ZenChess2",
			"ZenChess3",
			"ZenChess4",
			"ZenChess5",
			"ZenChess6",
			"ZenChess7",
			"ZenChess8",
			"ZenChess9",
			"ZenChess10",
			"ZenChess11",
			"ZenChess12",
			"ZenChess13",
			"ZenChess14",
			"ZenChess15",
			"ZenChess16",
			"ZenChess17",
			"ZenChess18",
			"ZenChess19",
			"ZenChess20",
			"ZenChess21",
			"ZenChess22",
			"ZenChess23",
			"ZenChess24",
			"ZenChess25",
			"ZenChess26",
			"ZenChess27",
			"ZenChess28",
			"ZenChess29",
			"ZenChess30",
			"ZenChess31",
			"ZenChess32",
			"ZenChess33",
			"ZenChess34",
			"ZenChess35",
			"ZenChess36",
			"ZenChess37",
			"ZenChess38",
			"ZenChess39",
			"ZenChess40",
			"ZenChess41",
			"ZenChess42",
			"ZenChess43",
			"ZenChess44",
			"ZenChess45",
			"ZenChess46",
			"ZenChess47",
			"ZenChess48",
			"ZenChess49",
			"ZenChess50",
			"ZenChess51",
			"ZenChess52",
			"ZenChess53",
			"ZenChess54",
			"ZenChess55",
			"ZenChess56",
			"ZenChess57",
			"ZenChess58",
			"ZenChess59",
			"ZenChess60",
			"ZenChess61",
			"ZenChess62",
			"ZenChess63",
			"ZenChess64"
		};
		hiddenSelections[] = { "ChessBoard" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\chess\ChessBoard_co.paa" };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\chess\ChessBoard.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\chess\ChessBoard.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\chess\ChessBoard_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\chess\ChessBoard_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\chess\ChessBoard_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
		soundImpactType = "wood";
	};
	class Zen_ChessBoard_Static : Zen_ChessBoard {};

	class Zen_ChessPieceBase : Inventory_Base
	{
		scope = 0;
		descriptionShort = "$STR_ZenChessPieceDesc";
		weight = 1;
		itemSize[] = { 1,1 };
		itemBehaviour = 1;
		repairableWithKits[] = { 8 };
		repairCosts[] = { 30 };
		inventorySlot[] =
		{
			"ZenChess1",
			"ZenChess2",
			"ZenChess3",
			"ZenChess4",
			"ZenChess5",
			"ZenChess6",
			"ZenChess7",
			"ZenChess8",
			"ZenChess9",
			"ZenChess10",
			"ZenChess11",
			"ZenChess12",
			"ZenChess13",
			"ZenChess14",
			"ZenChess15",
			"ZenChess16",
			"ZenChess17",
			"ZenChess18",
			"ZenChess19",
			"ZenChess20",
			"ZenChess21",
			"ZenChess22",
			"ZenChess23",
			"ZenChess24",
			"ZenChess25",
			"ZenChess26",
			"ZenChess27",
			"ZenChess28",
			"ZenChess29",
			"ZenChess30",
			"ZenChess31",
			"ZenChess32",
			"ZenChess33",
			"ZenChess34",
			"ZenChess35",
			"ZenChess36",
			"ZenChess37",
			"ZenChess38",
			"ZenChess39",
			"ZenChess40",
			"ZenChess41",
			"ZenChess42",
			"ZenChess43",
			"ZenChess44",
			"ZenChess45",
			"ZenChess46",
			"ZenChess47",
			"ZenChess48",
			"ZenChess49",
			"ZenChess50",
			"ZenChess51",
			"ZenChess52",
			"ZenChess53",
			"ZenChess54",
			"ZenChess55",
			"ZenChess56",
			"ZenChess57",
			"ZenChess58",
			"ZenChess59",
			"ZenChess60",
			"ZenChess61",
			"ZenChess62",
			"ZenChess63",
			"ZenChess64"
		};
		soundImpactType = "wood";
	};

	class Zen_ChessPieceBlackBase : Zen_ChessPieceBase
	{
		scope = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 25;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\chess\ChessBlackPiece.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\chess\ChessBlackPiece.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\chess\ChessBlackPiece.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\chess\ChessBlackPiece.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\chess\ChessBlackPiece.rvmat"
							}
						}
					};
				};
			};
		};
	}

	class Zen_ChessPieceWhiteBase : Zen_ChessPieceBase
	{
		scope = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 25;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\chess\ChessWhitePiece.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\chess\ChessWhitePiece.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\chess\ChessWhitePiece.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\chess\ChessWhitePiece.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\chess\ChessWhitePiece.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Zen_ChessWhitePawn : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessPawn";
		model = "ZenModPack\data\models\chess\white_pawn.p3d";
	};
	class Zen_ChessBlackPawn : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessPawn";
		model = "ZenModPack\data\models\chess\black_pawn.p3d";
	};
	class Zen_ChessWhiteBishop : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessBishop";
		model = "ZenModPack\data\models\chess\white_bishop.p3d";
	};
	class Zen_ChessBlackBishop : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessBishop";
		model = "ZenModPack\data\models\chess\black_bishop.p3d";
	};
	class Zen_ChessWhiteKing : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessKing";
		model = "ZenModPack\data\models\chess\white_king.p3d";
	};
	class Zen_ChessBlackKing : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessKing";
		model = "ZenModPack\data\models\chess\black_king.p3d";
	};
	class Zen_ChessWhiteKnight : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessKnight";
		model = "ZenModPack\data\models\chess\white_knight.p3d";
	};
	class Zen_ChessBlackKnight : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessKnight";
		model = "ZenModPack\data\models\chess\black_knight.p3d";
	};
	class Zen_ChessWhiteQueen : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessQueen";
		model = "ZenModPack\data\models\chess\white_queen.p3d";
	};
	class Zen_ChessBlackQueen : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessQueen";
		model = "ZenModPack\data\models\chess\black_queen.p3d";
	};
	class Zen_ChessWhiteRook : Zen_ChessPieceWhiteBase
	{
		scope = 2;
		displayName = "$STR_ZenChessRook";
		model = "ZenModPack\data\models\chess\white_rook.p3d";
	};
	class Zen_ChessBlackRook : Zen_ChessPieceBlackBase
	{
		scope = 2;
		displayName = "$STR_ZenChessRook";
		model = "ZenModPack\data\models\chess\black_rook.p3d";
	};

	//! WORKBENCH 
	class Zen_CarWorkbenchKit : ZenKitBoxBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenCarWorkbenchKit0";
		descriptionShort = "$STR_CfgVehicles_ZenCarWorkbench1";
		hiddenSelections[] = { "texture" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\workbench\workbench_kit_co.paa" };
	};
	class Zen_CarWorkbench : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenCarWorkbench0";
		descriptionShort = "$STR_CfgVehicles_ZenCarWorkbench1";
		model = "ZenModPack\data\models\workbench\workbench.p3d";
		forceFarBubble = "true";
		physLayer = "item_large";
		weight = 10000;
		itemSize[] = { 10,10 };
		itemBehaviour = 0;
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\workbench\workbench_co.paa" };
		class Cargo
		{
			itemsCargoSize[] = { 10,5 };
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		attachments[] = 
		{
			"ZenTireIron",
			"ZenTireRepair",
			"ZenTireGasoline",
			"CarRadiator",
			"CarBattery",
			"TruckBattery",
			"ZenCWB_Sledgehammer",
			"SparkPlug",
			"ZenCWB_Spark2",
			"GlowPlug",
			"Rags",
			"ZenCWB_Pliers",
			"ZenCWB_Screwdriver",
			"ZenCWB_Wrench",
			"ZenCWB_Paint1",
			"ZenCWB_Paint2"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\workbench\workbench.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\workbench\workbench.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\workbench\workbench_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\workbench\workbench_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\workbench\workbench_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "metal";
	};
	class Pliers : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenCWB_Pliers"
		};
	};
	class SledgeHammer : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenCWB_Sledgehammer"
		};
	};
	class Wrench : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenCWB_Wrench"
		};
	};
	class SparkPlug : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenCWB_Spark2"
		};
	};
	class Spraycan_ColorBase : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenCWB_Paint1",
			"ZenCWB_Paint2"
		};
	};

	//! TIRE RACK 
	class Zen_TireRack : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_ZenTireRackName";
		descriptionShort = "$STR_ZenTireRackDesc";
		model = "ZenModPack\data\models\tirerack\TireRack.p3d";
		forceFarBubble = "true";
		physLayer = "item_large"; 
		weight = 5000;
		itemSize[] = { 10,10 };
		itemBehaviour = 0;
		repairableWithKits[] = { 10 };
		repairCosts[] = { 30 };
		attachments[] = 
		{ 
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8",
			"ZenTireIron",
			"ZenTireRepair",
			"ZenTireGasoline"
		};
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "ZenModPack\data\textures\tirerack\TireRack_co.paa" };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\tirerack\tirerack.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\tirerack\tirerack.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\tirerack\tirerack_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\tirerack\tirerack_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\tirerack\tirerack_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType = "metal";
	};
	class LugWrench : Inventory_Base
	{
		inventorySlot[] +=
		{
			"ZenTireIron"
		};
	};
	class TireRepairKit : Inventory_Base
	{
		inventorySlot[] +=
		{	
			"ZenTireRepair"
		};
	};
	class CanisterGasoline : Bottle_Base
	{
		inventorySlot[] +=
		{
			"ZenTireGasoline"
		};
	};
	class HatchbackWheel : CarWheel
	{
		inventorySlot[] += 
		{  
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};
	class CivSedanWheel : CarWheel
	{
		inventorySlot[] +=
		{
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};
	class Hatchback_02_Wheel : CarWheel
	{
		inventorySlot[] +=
		{
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};
	class Sedan_02_Wheel : CarWheel
	{
		inventorySlot[] +=
		{
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};
	class Truck_01_Wheel : CarWheel
	{
		inventorySlot[] +=
		{
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};
	class Offroad_02_Wheel : CarWheel
	{
		inventorySlot[] +=
		{
			"ZenTireRack1",
			"ZenTireRack2",
			"ZenTireRack3",
			"ZenTireRack4",
			"ZenTireRack5",
			"ZenTireRack6",
			"ZenTireRack7",
			"ZenTireRack8"
		};
	};

	//! ZOMBIE DOORS 
	class ZenZombieDoorBangerBang : Inventory_Base
	{
		scope = 1;
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
	};

	//! TREESPLOSIONS
	class Zen40mmTreeKiller : Inventory_Base
	{
		scope = 1;
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
	};

	//! ARTILLERY 
	// Define grenades
	class Zen_ArtillerySmokeGrenade : M18SmokeGrenade_Red
	{
		scope = 2;
		displayName = "$STR_ZENARTILLERYGRENADE0";
		descriptionShort = "$STR_ZENARTILLERYGRENADE1";
		weight = 250;
		hiddenSelectionsTextures[] =
		{
			"ZenModPack\data\textures\artillery\artillerystrikegrenade_red_co.paa"
		};
		class EnergyManager
		{
			energyAtSpawn = 180; // 3 mins lifetime
			energyUsagePerSecond = 1;
			wetnessExposure = 0.1;
			updateInterval = 5;
		};
	};
	class Zen_ArtillerySmokeGrenadeChem : M18SmokeGrenade_Green
	{
		scope = 2;
		displayName = "$STR_ZENARTILLERYCHEMGRENADE0";
		descriptionShort = "$STR_ZENARTILLERYCHEMGRENADE1";
		weight = 250;
		hiddenSelectionsTextures[] =
		{
			"ZenModPack\data\textures\artillery\artillerystrikegrenade_green_co.paa"
		};
		class EnergyManager
		{
			energyAtSpawn = 180; // 3 mins lifetime
			energyUsagePerSecond = 1;
			wetnessExposure = 0.1;
			updateInterval = 5;
		};
	};

	// Define bomb
	class Zen_ArtilleryBomb : Grenade_Base
	{
		scope = 2;
		displayName = "ZenBangBang";
		model = "DZ\weapons\explosives\grenade.p3d";
		itemSize[] = { 1,1 };
		weight = 5000;
		forceFarBubble = "true";
		hiddenSelections[] = { "camo" };
		hiddenSelectionsTextures[] = { "dz\weapons\explosives\data\grenade_co.paa" };
		hiddenSelectionsMaterials[] = { "dz\weapons\explosives\data\grenade.rvmat" };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
					healthLevels[] = { {1,{"DZ\weapons\explosives\data\grenade.rvmat"}},{0.7,{"DZ\weapons\explosives\data\grenade.rvmat"}},{0.5,{"DZ\weapons\explosives\data\grenade_damage.rvmat"}},{0.3,{"DZ\weapons\explosives\data\grenade_damage.rvmat"}},{0,{"DZ\weapons\explosives\data\grenade_destruct.rvmat"}} };
				};
			};
		};
	};

	//! TIME BOMB 
	class AlarmClock_ColorBase : Inventory_Base
	{
		scope = 0;
		attachments[] =
		{
			"VestGrenadeA"
		};
	}

	//! OPEN CANS ON ROCKS 
	class SmallStone : Inventory_Base
	{
		openItemSpillRange[] = { 50,75 };
	};
	class Stone : Inventory_Base
	{
		openItemSpillRange[] = { 70,90 };
	};

	//! CAMO NET GHILLIE 
	class Zen_GhillieSuit_ColorBase : Clothing
	{
		displayName="$STR_cfgVehicles_GhillieSuit_ColorBase0";
		descriptionShort="$STR_cfgVehicles_GhillieSuit_ColorBase1";
		model = "\dz\characters\backpacks\GhillieBushrag_g.p3d";
		inventorySlot[] =
		{
			"Back",
			"Hips",
			"Armband"
		};
		itemInfo[] =
		{
			"Clothing",
			"Back",
			"Hips",
			"Armband"
		};
		lootCategory = "Crafted";
		rotationFlags = 16;
		itemSize[] = { 6,5 };
		weight = 3200;
		absorbency = 1;
		heatIsolation = 0.1;
		soundAttType = "Ghillie";
		hiddenSelections[] =
		{
			"camoGround",
			"camo",
			"camo1"
		};
		class ClothingTypes
		{
			male = "\DZ\characters\backpacks\GhillieSuit_m.p3d";
			female = "\DZ\characters\backpacks\GhillieSuit_m.p3d";
		};
	};
	class Zen_CamoNet_GhillieSuit : Zen_GhillieSuit_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.5;
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{

						{
							1.0,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.69999999,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.5,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.30000001,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.0099999998,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_destruct_co.paa"
							}
						}
					};
				};
			};
		};
	};
	class Zen_GhillieHood_ColorBase : Clothing
	{
		displayName="$STR_CfgVehicles_GhillieHood_ColorBase0";
		descriptionShort="$STR_CfgVehicles_GhillieHood_ColorBase1";
		model = "\DZ\characters\headgear\GhillieHood_g.p3d";
		inventorySlot[] =
		{
			"Headgear"
		};
		simulation = "clothing";
		vehicleClass = "Clothing";
		itemInfo[] =
		{
			"Clothing",
			"Headgear"
		};
		rotationFlags = 2;
		weight = 410;
		itemSize[] = { 4,3 };
		absorbency = 0.89999998;
		heatIsolation = 0.1;
		repairableWithKits[] = { 5,2 };
		repairCosts[] = { 30,25 };
		headSelectionsToHide[] =
		{
			"Clipping_GhillieHood"
		};
		hiddenSelections[] =
		{
			"camoGround",
			"camo",
			"camo1"
		};
		class ClothingTypes
		{
			male = "\DZ\characters\headgear\GhillieHood.p3d";
			female = "\DZ\characters\headgear\GhillieHood.p3d";
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "Shirt_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "Shirt_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class Zen_CamoNet_GhillieHood : Zen_GhillieHood_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.69999999;
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30;
					healthLevels[] =
					{

						{
							1.0,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.69999999,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.5,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.30000001,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.0099999998,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_destruct_co.paa"
							}
						}
					};
				};
			};
		};
	};
	class Zen_GhillieTop_ColorBase : Clothing
	{
		displayName="$STR_cfgVehicles_GhillieTop_ColorBase0";
		descriptionShort="$STR_cfgVehicles_GhillieTop_ColorBase1";
		model = "\dz\characters\backpacks\GhillieBushrag_g.p3d";
		inventorySlot[] =
		{
			"Back",
			"Hips",
			"Armband"
		};
		itemInfo[] =
		{
			"Clothing",
			"Back",
			"Hips",
			"Armband"
		};
		lootCategory = "Crafted";
		rotationFlags = 16;
		itemSize[] = { 4,4 };
		weight = 800;
		absorbency = 1;
		heatIsolation = 0.1;
		repairableWithKits[] = { 5,2 };
		repairCosts[] = { 30,25 };
		soundAttType = "Ghillie";
		hiddenSelections[] =
		{
			"camoGround",
			"camo",
			"camo1"
		};
		class ClothingTypes
		{
			male = "\DZ\characters\backpacks\GhillieTop_m.p3d";
			female = "\DZ\characters\backpacks\GhillieTop_m.p3d";
		};
	};
	class Zen_CamoNet_GhillieTop : Zen_GhillieTop_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.60000002;
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30;
					healthLevels[] =
					{

						{
							1.0,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.69999999,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.5,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.30000001,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.0099999998,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_destruct_co.paa"
							}
						}
					};
				};
			};
		};
	};
	class Zen_GhillieBushrag_ColorBase : Clothing
	{
		displayName="$STR_cfgvehicles_GhillieBushrag_colorbase0";
		descriptionShort="$STR_cfgVehicles_GhillieBushrag_ColorBase1";
		model = "\dz\characters\backpacks\GhillieBushrag_g.p3d";
		inventorySlot[] =
		{
			"Back",
			"Hips",
			"Armband"
		};
		itemInfo[] =
		{
			"Clothing",
			"Back",
			"Hips",
			"Armband"
		};
		rotationFlags = 16;
		itemSize[] = { 4,3 };
		weight = 700;
		absorbency = 1;
		heatIsolation = 0.1;
		repairableWithKits[] = { 5,2 };
		repairCosts[] = { 30,25 };
		soundAttType = "Ghillie";
		hiddenSelections[] =
		{
			"camoGround",
			"camo",
			"camo1"
		};
		class ClothingTypes
		{
			male = "\DZ\characters\backpacks\GhillieBushrag_m.p3d";
			female = "\DZ\characters\backpacks\GhillieBushrag_m.p3d";
		};
	};
	class Zen_CamoNet_GhillieBushrag : Zen_GhillieBushrag_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.69999999;
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30;
					healthLevels[] =
					{

						{
							1.0,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.69999999,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.5,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.30000001,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.0099999998,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_destruct_co.paa"
							}
						}
					};
				};
			};
		};
	};
	class Zen_CamoNet_GhillieAtt : GhillieAtt_ColorBase
	{
		scope = 2;
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa",
			"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{

						{
							1.0,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.69999999,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_co.paa"
							}
						},

						{
							0.5,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.30000001,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_damage_co.paa"
							}
						},

						{
							0.0099999998,

							{
								"\ZenModPack\data\textures\ghillie\ghillie_net_destruct_co.paa"
							}
						}
					};
				};
			};
		};
	};
	class Zen_CamoNet_Strip : Inventory_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenCamoNetStrip0";
		descriptionShort = "$STR_CfgVehicles_ZenCamoNetStrip1";
		model = "\DZ\gear\crafting\sackcloth_rag.p3d";
		hiddenSelections[] =
		{
			"zbytek"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\ghillie\strip_co.paa"
		};
		weight = 50;
		itemSize[] = { 1,3 };
		canBeSplit = 1;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 10;
		rotationFlags = 17;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\consumables\data\weaponcamo.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\consumables\data\weaponcamo.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\consumables\data\weaponcamo_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\consumables\data\weaponcamo_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\consumables\data\weaponcamo_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	//! FIRE FUEL 
	// Define fire fuel object. This object is locked to fire when poured so players should never be able to interact with it unless it's spawned in by an admin
	class Zen_FireFuel : Inventory_Base
	{
		scope = 2;
		itemSize[] = { 1,1 };
		model = "\dz\vehicles\parts\JerryCan.p3d";
		displayName = "$STR_ZenFireFuel0";
		descriptionShort = "$STR_ZenFireFuel1";
		inventorySlot[] = { "ZenFuel" };
		canBeSplit = 0;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 1;
		varQuantityDestroyOnMin = 1;
	};

	// Add fuel attachment to fireplaces
	class Fireplace : FireplaceBase
	{
		attachments[] +=
		{
			"ZenFuel"
		};
		class GUIInventoryAttachmentsProps
		{
			class Gasoline
			{
				name = "$STR_ZenFireFuel0";
				description = "";
				attachmentSlots[] =
				{
					"ZenFuel"
				};
				icon = "gascanister";
				view_index = 7;
			}
		}
	};
	class FireplaceIndoor : FireplaceBase
	{
		attachments[] +=
		{
			"ZenFuel"
		};
		class GUIInventoryAttachmentsProps
		{
			class Gasoline
			{
				name = "$STR_ZenFireFuel0";
				description = "";
				attachmentSlots[] =
				{
					"ZenFuel"
				};
				icon = "gascanister";
				view_index = 7;
			}
		}
	};
	class OvenIndoor : FireplaceBase
	{
		attachments[] +=
		{
			"ZenFuel"
		};
		class GUIInventoryAttachmentsProps
		{
			class Gasoline
			{
				name = "$STR_ZenFireFuel0";
				description = "";
				attachmentSlots[] =
				{
					"ZenFuel"
				};
				icon = "gascanister";
				view_index = 7;
			}
		}
	};
	class BarrelHoles_ColorBase : FireplaceBase
	{
		attachments[] +=
		{
			"ZenFuel"
		};
		class GUIInventoryAttachmentsProps
		{
			class Gasoline
			{
				name = "$STR_ZenFireFuel0";
				description = "";
				attachmentSlots[] =
				{
					"ZenFuel"
				};
				icon = "gascanister";
				view_index = 7;
			}
		}
	}

	//! CONCUSSION GRENADE 
	class Zen_ConcussionGrenade : Grenade_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenConcussionGrenade0";
		descriptionShort = "$STR_CfgVehicles_ZenConcussionGrenade1";
		model = "\dz\weapons\explosives\flashbang.p3d";
		rotationFlags = 17;
		weight = 350;
		hiddenSelections[] =
		{
			"camo"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\concussion\zen_concussion_co.paa"
		};
		hiddenSelectionsMaterials[] =
		{
			"dz\weapons\explosives\data\flashbang.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\weapons\explosives\data\flashbang.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\weapons\explosives\data\flashbang.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\weapons\explosives\data\flashbang_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\weapons\explosives\data\flashbang_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\weapons\explosives\data\flashbang_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class Grenade_unpin
				{
					soundSet = "Grenade_unpin_SoundSet";
					id = 201;
				};
				class turnOnRadio_Pin
				{
					soundSet = "Grenade_pin_SoundSet";
					id = 1006;
				};
			};
		};
	};

	//! STATIC BARBED WIRE 
	class ZenStaticBarbedWire : Inventory_Base
	{
		scope = 1;
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
	};

	//! FIREWOOD
	class Zen_WoodReserve : HouseNoDestruct
	{
		scope = 1;
		model = "ZenModPack\data\models\woodreserves\zen_woodreserve.p3d";
	};
	class Zen_WoodReserve_Debug : HouseNoDestruct
	{
		scope = 1;
		model = "ZenModPack\data\models\woodreserves\zen_woodreserve_debug.p3d";
	};

	//! CHICKEN COOPS 
	// Create invisible model that we can interact with and will envelope the chicken coop
	class Zen_ChickenCoop : HouseNoDestruct
	{
		scope = 1;
		model = "ZenModPack\data\models\chickencoops\zen_coop.p3d";
	};
	class Zen_ChickenCoop_Debug : Zen_ChickenCoop
	{
		scope = 1;
		model = "ZenModPack\data\models\chickencoops\zen_coop_debug.p3d";
	};
	class Zen_ChickenCoop2 : Zen_ChickenCoop
	{
		scope = 1;
		model = "ZenModPack\data\models\chickencoops\zen_coop2.p3d";
	};
	class Zen_ChickenCoop2_Debug : Zen_ChickenCoop
	{
		scope = 1;
		model = "ZenModPack\data\models\chickencoops\zen_coop2_debug.p3d";
	};

	//! LEFTOVERS 
	class Zen_EmptyFood : Inventory_Base 
	{
		displayNameEmpty = "$STR_ZenLeftoversEmpty";
	};
	class Empty_SodaCan_ColorBase : Bottle_Base 
	{
		scope = 0;
		descriptionShort = "$STR_ZenLeftoversDescription";
		model="\dz\gear\drinks\SodaCan.p3d";
		hiddenSelections[]= { "camoGround" };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 100;
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varTemperatureMax=200;
		inventorySlot[] =
		{
			"DirectCookingA",
			"DirectCookingB",
			"DirectCookingC"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\drinks\data\sodacan.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\drinks\data\sodacan.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\drinks\data\sodacan_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\drinks\data\sodacan_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\drinks\data\sodacan_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	//class SodaCan_Pipsi;
	class Empty_SodaCan_Pipsi : Empty_SodaCan_ColorBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SodaCan_Pipsi0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\sodacan_pipsi_empty_co.paa" };
		varTemperatureMax=200;
		varWetInit = 0;
		varWetMax = 3;
	};
	class Empty_SodaCan_Cola : Empty_SodaCan_ColorBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SodaCan_Cola0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\sodacan_cola_empty_co.paa" };
		varTemperatureMax=200;
		varWetInit = 0;
		varWetMax = 3;
	};
	class Empty_SodaCan_Spite : Empty_SodaCan_ColorBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SodaCan_Spite0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\sodacan_spite_empty_co.paa" };
		varTemperatureMax=200;
		varWetInit = 0;
		varWetMax = 3;
	};
	class Empty_SodaCan_Kvass : Empty_SodaCan_ColorBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SodaCan_Kvass0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\sodacan_rasputin_kvass_empty_co.paa" };
		varTemperatureMax=200;
		varWetInit = 0;
		varWetMax = 3;
	};
	class Empty_SodaCan_Fronta : Empty_SodaCan_ColorBase
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SodaCan_Fronta0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\sodacan_fanda_empty_co.paa" };
		varTemperatureMax=200;
		varWetInit = 0;
		varWetMax = 3;
	};

	// Edibles
	class Empty_BoxCerealCrunchin : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_BoxCerealCrunchin0";
		model = "\dz\gear\food\BoxCereal.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 17;
		weight = 1;
		itemSize[] = { 3,3 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		inventorySlot[] =
		{
			"Paper"
		};
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\CerealBox_01_CO.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\Cereal_box.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\Cereal_box.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\Cereal_box_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\Cereal_box_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\Cereal_box_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickup
				{
					soundSet = "BoxCerealCrunchin_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "BoxCerealCrunchin_drop_SoundSet";
					id = 898;
				};
				class interact
				{
					soundset="ammoboxUnpack_SoundSet";
					id=70;
				};
			};
		};
	};
	class Empty_Rice : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Rice0";
		model = "\dz\gear\food\Rice.p3d";
		debug_ItemCategory = 6;
		weight = 1;
		itemSize[] = { 2,3 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		absorbency = 0.89999998;
		rotationFlags = 63;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 40;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\rice.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\rice.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\rice_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\rice_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\rice_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_PowderedMilk : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_PowderedMilk0";
		model = "\dz\gear\food\PowderedMilk.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 17;
		weight = 1;
		itemSize[] = { 1,2 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		absorbency = 0.89999998;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\powdered_milk.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\powdered_milk.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\powdered_milk_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\powdered_milk_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\powdered_milk_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class Nutrition
		{
			fullnessIndex = 2;
			energy = 400;
			water = 0;
			nutritionalIndex = 1;
			toxicity = 0;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpBloodBag_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "bloodbag_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class Empty_Honey_NoLiquid : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_Honey0";
		model = "\dz\gear\food\Honey.p3d";
		debug_ItemCategory = 6;
		weight = 15;
		itemSize[] = { 2,2 };
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\honey_jar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\honey_jar.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\honey_jar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\honey_jar_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\honey_jar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "Marmalade_pickup_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Empty_Marmalade_NoLiquid : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Marmalade0";
		model = "\dz\gear\food\Marmalade.p3d";
		debug_ItemCategory = 6;
		weight = 1;
		itemSize[] = { 2,2 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\marmalade_jar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\marmalade_jar.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\marmalade_jar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\marmalade_jar_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\marmalade_jar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "Marmalade_pickup_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Empty_Honey : Bottle_Base
	{
		scope = 2;
		displayName = "$STR_Honey0";
		model = "\dz\gear\food\Honey.p3d";
		debug_ItemCategory = 6;
		weight = 15;
		itemSize[] = { 2,2 };
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 250;
		isMeleeWeapon = 1;
		varWetInit = 0;
		varWetMax = 3;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\honey_jar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\honey_jar.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\honey_jar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\honey_jar_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\honey_jar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class WaterBottle_in_B
				{
					soundSet = "WaterBottle_in_B_SoundSet";
					id = 202;
				};
				class WaterBottle_in_C
				{
					soundSet = "WaterBottle_in_C_SoundSet";
					id = 203;
				};
				class WaterBottle_in_C1
				{
					soundSet = "WaterBottle_in_C1_SoundSet";
					id = 204;
				};
				class WaterBottle_out_A
				{
					soundSet = "WaterBottle_out_A_SoundSet";
					id = 205;
				};
				class WaterBottle_out_B
				{
					soundSet = "WaterBottle_out_B_SoundSet";
					id = 206;
				};
				class WellPond_loop
				{
					soundSet = "WellPond_loop_SoundSet";
					id = 209;
				};
				class WellBottle_loop
				{
					soundSet = "WellBottle_loop_SoundSet";
					id = 210;
				};
				class pickup
				{
					soundSet = "Marmalade_pickup_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Empty_Marmalade : Bottle_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Marmalade0";
		model = "\dz\gear\food\Marmalade.p3d";
		debug_ItemCategory = 6;
		weight = 15;
		itemSize[] = { 2,2 };
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 250;
		isMeleeWeapon = 1;
		varWetInit = 0;
		varWetMax = 3;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\marmalade_jar.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\marmalade_jar.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\marmalade_jar_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\marmalade_jar_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\marmalade_jar_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class WaterBottle_in_B
				{
					soundSet = "WaterBottle_in_B_SoundSet";
					id = 202;
				};
				class WaterBottle_in_C
				{
					soundSet = "WaterBottle_in_C_SoundSet";
					id = 203;
				};
				class WaterBottle_in_C1
				{
					soundSet = "WaterBottle_in_C1_SoundSet";
					id = 204;
				};
				class WaterBottle_out_A
				{
					soundSet = "WaterBottle_out_A_SoundSet";
					id = 205;
				};
				class WaterBottle_out_B
				{
					soundSet = "WaterBottle_out_B_SoundSet";
					id = 206;
				};
				class WellPond_loop
				{
					soundSet = "WellPond_loop_SoundSet";
					id = 209;
				};
				class WellBottle_loop
				{
					soundSet = "WellBottle_loop_SoundSet";
					id = 210;
				};
				class pickup
				{
					soundSet = "Marmalade_pickup_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Zagorky_Empty : Zen_EmptyFood
	{
		model = "\dz\gear\food\Zagorky.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 1;
		weight = 1;
		itemSize[] = { 1,2 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\zagorky.rvmat",
								"DZ\gear\food\data\zagorky_wrapping.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\zagorky.rvmat",
								"DZ\gear\food\data\zagorky_wrapping.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\zagorky_damage.rvmat",
								"DZ\gear\food\data\zagorky_wrapping_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\zagorky_damage.rvmat",
								"DZ\gear\food\data\zagorky_wrapping_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\zagorky_destruct.rvmat",
								"DZ\gear\food\data\zagorky_wrapping_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpBloodBag_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "bloodbag_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class Empty_Zagorky : Zagorky_Empty
	{
		scope = 2;
		displayName = "$STR_Zagorky0";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		hiddenSelections[] =
		{
			"camoground"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\zagorky_milk_ca.paa"
		};
	};
	class Empty_ZagorkyChocolate : Zagorky_Empty
	{
		scope = 2;
		displayName = "$STR_ZagorkyChocolate0";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		hiddenSelections[] =
		{
			"camoground"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\zagorky_choco_ca.paa"
		};
	};
	class Empty_ZagorkyPeanuts : Zagorky_Empty
	{
		scope = 2;
		displayName = "$STR_ZagorkyPeanuts0";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		hiddenSelections[] =
		{
			"camoground"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\zagorky_nuts_ca.paa"
		};
	};
	class Empty_Snack_ColorBase : Zen_EmptyFood
	{
		model = "\DZ\gear\food\salty_crackers.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 1;
		weight = 1;
		itemSize[] = { 2,2 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\tycinky.rvmat",
								"DZ\gear\food\data\tycinky_wrapping.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\tycinky.rvmat",
								"DZ\gear\food\data\tycinky_wrapping.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\tycinky_damage.rvmat",
								"DZ\gear\food\data\tycinky_wrapping_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\tycinky_damage.rvmat",
								"DZ\gear\food\data\tycinky_wrapping_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\tycinky_destruct.rvmat",
								"DZ\gear\food\data\tycinky_wrapping_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpBloodBag_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "bloodbag_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class Empty_SaltySticks : Empty_Snack_ColorBase
	{
		scope = 2;
		displayName = "$STR_SaltySticks0";
	};
	class Empty_Crackers : Empty_Snack_ColorBase
	{
		scope = 2;
		displayName = "$STR_Crackers0";
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\salted_crackers_co.paa"
		};
	};
	class Empty_Chips : Empty_Snack_ColorBase
	{
		scope = 2;
		displayName = "$STR_Chips0";
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\food\Data\salted_chips_co.paa"
		};
	};

	// Canned food
	class Empty_Can_Opened : Bottle_Base
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_ZenEmptyCan0";
		descriptionShort = "$STR_CfgVehicles_ZenEmptyCan1";
		model = "\dz\gear\food\food_can_open.p3d";
		debug_ItemCategory = 6;
		itemSize[] = { 2,2 };
		weight = 20;
		destroyOnEmpty = 0;
		varQuantityDestroyOnMin = 0;
		varLiquidTypeInit = 512;
		liquidContainerType = "1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768 + 65536  - (1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256) -32768";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 100;
		isMeleeWeapon = 1;
		varTemperatureMax=200;
		inventorySlot[] =
		{
			"DirectCookingA",
			"DirectCookingB",
			"DirectCookingC"
		};
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\leftovers\deez_beans_empty_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] =
					{
						{
							1,
							{
								"DZ\gear\food\data\food_can.rvmat"
							}
						},
						{
							0.69999999,
							{
								"DZ\gear\food\data\food_can.rvmat"
							}
						},
						{
							0.5,
							{
								"DZ\gear\food\data\food_can_damage.rvmat"
							}
						},
						{
							0.30000001,
							{
								"DZ\gear\food\data\food_can_damage.rvmat"
							}
						},
						{
							0,
							{
								"DZ\gear\food\data\food_can_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_BakedBeansCan_Opened : Empty_Can_Opened
	{
		scope = 2;
		displayName = "$STR_BakedBeansCan0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\deez_beans_empty_co.paa" };
	};
	class Empty_PeachesCan_Opened : Empty_Can_Opened
	{
		scope = 2;
		displayName = "$STR_PeachesCan0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\canned_peaches_empty_co.paa" };
	};
	class Empty_TacticalBaconCan_Opened : Empty_Can_Opened
	{
		scope = 2;
		displayName = "$STR_TacticalBaconCan0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\tactical_bacon_empty_co.paa" };
	};
	class Empty_SpaghettiCan_Opened : Empty_Can_Opened
	{
		scope = 2;
		displayName = "$STR_SpaghettiCan0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\canned_spaghetti_empty_co.paa" };
	};
	class Empty_UnknownFoodCan_Opened : Empty_Can_Opened
	{
		scope = 2;
		displayName = "$STR_UnknownFoodCan0";
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\unknownfoodcan_empty_co.paa" };
	};
	class Empty_SardinesCan_Opened : Zen_EmptyFood
	{
		scope = 2;
		displayName = "$STR_Sardines0";
		model = "\dz\gear\food\can_sardines_open.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 34;
		itemSize[] = { 2,1 };
		weight = 50;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\leftovers\sardines_empty_co.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 70;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\Sardines.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\Sardines.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\Sardines_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\Sardines_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\Sardines_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_TunaCan_Opened : Zen_EmptyFood // Tuna can doesn't allow retexture for some reason? Just spawn closed can instead for now.
	{
		scope = 2;
		displayName = "$STR_TunaCan0";
		model = "\dz\gear\food\Tuna.p3d";
		debug_ItemCategory = 6;
		rotationFlags = 32;
		itemSize[] = { 2,1 };
		weight = 265;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		inventorySlot[] =
		{
			"DirectCookingA",
			"DirectCookingB",
			"DirectCookingC"
		};
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"dz\gear\food\data\tuna_CO.paa"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 70;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\tuna.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\tuna.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\tuna_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\tuna_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\tuna_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_FoodCan_250g_Opened_ColorBase : Zen_EmptyFood
	{
		scope = 2;
		debug_ItemCategory = 6;
		model = "\dz\gear\food\FoodCan_250g_open.p3d";
		itemSize[] = { 2,1 };
		weight = 5;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 70;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\FoodCan_250g.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\FoodCan_250g.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\FoodCan_250g_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\FoodCan_250g_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\FoodCan_250g_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_DogFoodCan_Opened : Empty_FoodCan_250g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_DogFoodCan0";
		hiddenSelections[] = { "camoGround" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\foodcan_250g_dog_empty_co.paa" };
	};
	class Empty_CatFoodCan_Opened : Empty_FoodCan_250g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_CatFoodCan0";
		hiddenSelections[] = { "camoGround" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\foodcan_250g_cat_empty_co.paa" };
	};
	class Empty_PorkCan_Opened : Empty_FoodCan_250g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_PorkCan0";
		hiddenSelections[] = { "camoGround" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\foodcan_250g_pork_empty_co.paa" };
	};
	class Empty_Lunchmeat_Opened : Empty_FoodCan_250g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_Lunchmeat0";
		hiddenSelections[] = { "camoGround" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\foodcan_lunchmeat_open_empty_co.paa" };
	};

	// 100g cans
	class Empty_FoodCan_100g_Opened_ColorBase : Zen_EmptyFood
	{
		debug_ItemCategory = 6;
		model = "\dz\gear\food\FoodCan_100g_open.p3d";
		itemSize[] = { 1,1 };
		weight = 5;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		isMeleeWeapon = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 70;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\food\data\FoodCan_100g.rvmat",
								"DZ\gear\food\data\FoodCan_100g_white.rvmat",
								"DZ\gear\food\data\FoodCan_100g_content.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\food\data\FoodCan_100g.rvmat",
								"DZ\gear\food\data\FoodCan_100g_white.rvmat",
								"DZ\gear\food\data\FoodCan_100g_content.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\food\data\FoodCan_100g_damage.rvmat",
								"DZ\gear\food\data\FoodCan_100g_white_damage.rvmat",
								"DZ\gear\food\data\FoodCan_100g_content_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\food\data\FoodCan_100g_damage.rvmat",
								"DZ\gear\food\data\FoodCan_100g_white_damage.rvmat",
								"DZ\gear\food\data\FoodCan_100g_content_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\food\data\FoodCan_100g_destruct.rvmat",
								"DZ\gear\food\data\FoodCan_100g_white_destruct.rvmat",
								"DZ\gear\food\data\FoodCan_100g_content_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Empty_Pajka_Opened : Empty_FoodCan_100g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_Pajka0";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		hiddenSelections[] = { "camoGround" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\foodcan_100g_pajka_empty_co.paa" };
	};
	class Empty_Pate_Opened : Empty_FoodCan_100g_Opened_ColorBase
	{
		scope = 2;
		displayName = "$STR_Pate0";
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		hiddenSelections[] =
		{
			"camoGround"
		};
		hiddenSelectionsTextures[] =
		{
			"\ZenModPack\data\textures\leftovers\foodcan_100g_liver_empty_co.paa"
		};
	};
	class Empty_BrisketSpread_Opened : Empty_Pate_Opened
	{
		scope = 2;
		displayName = "$STR_BrisketSpread0";
	};
	// Define empty medical items
	class Used_MedicalItem : Zen_EmptyFood 
	{
		displayNameUsed = "$STR_ZenLeftoversUsed";
	};
	class Used_BloodBagIV : Used_MedicalItem
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_BloodBagIV0";
		model = "\dz\gear\medical\BloodBag_Full_IV.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "\ZenModPack\data\textures\leftovers\bloodbag_used_ca.paa" };
		itemSize[] = { 2,2 };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\BloodBag_Full.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\BloodBag_Full.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\BloodBag_Full_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\BloodBag_Full_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\BloodBag_Full_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Used_SalineBagIV : Used_MedicalItem
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_SalineBagIV0";
		model = "\dz\gear\medical\TransfusionKit.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "dz\gear\medical\data\saline_bag_ca.paa" };
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		itemSize[] = { 2,2 };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20;
					healthLevels[] =
					{
						{
							1,

							{
								"DZ\gear\medical\data\transfusion_kit.rvmat"
							}
						},
						{
							0.69999999,

							{
								"DZ\gear\medical\data\transfusion_kit.rvmat"
							}
						},
						{
							0.5,

							{
								"DZ\gear\medical\data\transfusion_kit_damage.rvmat"
							}
						},
						{
							0.30000001,

							{
								"DZ\gear\medical\data\transfusion_kit_damage.rvmat"
							}
						},
						{
							0,

							{
								"DZ\gear\medical\data\transfusion_kit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Used_Morphine : Used_MedicalItem
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Morphine0";
		model = "\dz\gear\medical\Morphine.p3d";
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 30;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\Morphine.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\Morphine.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\Morphine_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\Morphine_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\Morphine_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Used_Epinephrine : Used_MedicalItem
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_Epinephrine0";
		model = "\dz\gear\medical\Epinephrine.p3d";
		debug_ItemCategory = 7;
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 30;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\epipen.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\epipen.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\epipen_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\epipen_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\epipen_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Used_AntiChemInjector : Used_MedicalItem
	{
		scope = 2;
		displayName = "$STR_AntiChemInjector0";
		model = "\dz\gear\medical\Epinephrine.p3d";
		debug_ItemCategory = 7;
		hiddenSelections[] =
		{
			"zbytek"
		};
		hiddenSelectionsTextures[] =
		{
			"\dz\gear\medical\data\antidote_co.paa"
		};
		rotationFlags = 17;
		itemSize[] = { 1,2 };
		weight = 30;
		varQuantityInit = 0;
		varQuantityMin = 0;
		varQuantityMax = 0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] =
					{

						{
							1,

							{
								"DZ\gear\medical\data\epipen.rvmat"
							}
						},

						{
							0.69999999,

							{
								"DZ\gear\medical\data\epipen.rvmat"
							}
						},

						{
							0.5,

							{
								"DZ\gear\medical\data\epipen_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"DZ\gear\medical\data\epipen_damage.rvmat"
							}
						},

						{
							0,

							{
								"DZ\gear\medical\data\epipen_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	//! MISCELLANEOUS STATIC OBJECTS

	//! Mural
	class Zen_Decal_Mural_Base: HouseNoDestruct
	{
		scope=1;
		model="\ZenModPack\data\models\mural\mural.p3d";
		physLayer="item_small";
		carveNavmesh=0;
	};
	class Zen_Decal_Mural_Repent: Zen_Decal_Mural_Base
	{
		scope=1;
		hiddenSelections[]={"texture"};
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\mural\mural_decal_28dayslater.paa"};
	};

	//! Ice plane (useful for winter/snow maps to block certain lakes etc)
	class Land_IceSheet : HouseNoDestruct
	{
		scope=1;
		model="\ZenModPack\data\models\iceplane\ice_plane_simple.p3d";
		forceFarBubble = "true";
	}

	//! Tree houses
	class Land_ZenTreeHouse_Ladder: HouseNoDestruct
	{
		scope=1;
		model="\ZenModPack\data\models\Treehouse\treehouse_ladder.p3d";
		forceFarBubble = "true";
	};
	class Land_ZenTreeHouse_Bare: HouseNoDestruct
	{
		scope=1;
		model="\ZenModPack\data\models\Treehouse\treehouse_ladder_bare.p3d";
		forceFarBubble = "true";
	};

	//! Climbable Ladder
	class Land_ZenLadder_SmallWood: HouseNoDestruct
	{
		scope=1;
		model="\ZenModPack\data\models\Ladders\ladder_small.p3d";
		forceFarBubble = "true";
	};

	// Static Sleeping Bags 
	class ZenSleepingBagStatic_Black: HouseNoDestruct
	{
		scope=1;
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		hiddenSelections[]={"bag"};
		weight=10000;
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\sleepingbags\sleepingbag_black_co.paa"};
	}
	class ZenSleepingBagStatic_Blue: HouseNoDestruct 
	{ 
		scope=1;
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		hiddenSelections[]={"bag"};
		weight=10000;
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\sleepingbags\sleepingbag_blue_co.paa"}; 
	};
	class ZenSleepingBagStatic_Grey: HouseNoDestruct 
	{ 
		scope=1;
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		hiddenSelections[]={"bag"};
		weight=10000;
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\sleepingbags\sleepingbag_grey_co.paa"}; 
	};
	class ZenSleepingBagStatic_Red: HouseNoDestruct 
	{
		scope=1;
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		hiddenSelections[]={"bag"};
		weight=10000;
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\sleepingbags\sleepingbag_red_co.paa"}; 
	};
	class ZenSleepingBagStatic_Green: HouseNoDestruct 
	{ 
		scope=1;
		model="ZenModPack\data\models\sleepingbags\sleepingbag.p3d";
		hiddenSelections[]={"bag"};
		weight=10000;
		hiddenSelectionsTextures[]={"ZenModPack\data\textures\sleepingbags\sleepingbag_green_co.paa"}; 
	};

	// Some various lighting objects
	class Zen_SpotlightObject: Inventory_Base
	{
		scope=1;
		storageCategory = 10;
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
		weight=9999;
		rotationFlags=2;
		class PointLights
		{
			class SpotlightLight
			{
				brightness = 20.0;
				radius = 20;
				dayLight = 0;
				color[] = { 2.5, 2.5, 1.5, 0.0 };
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
				};
			};
		};
	};
	class Zen_StreetLight : Inventory_Base
	{
		scope=1;
		storageCategory = 10;
		model = "\dz\gear\consumables\Stone.p3d";
		forceFarBubble = "true";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
				};
			};
		};
		class PointLights
		{
			class PointLight
			{
				brightness = 1.0;
				radius = 25;
				dayLight = 0;
				color[] = { 2.5, 2.5, 1.5, 0.0 };
			};
		};
	};
	class Zen_InteriorLight : Inventory_Base
	{
		scope=1;
		storageCategory = 10;
		model = "\dz\gear\consumables\Stone.p3d";
		forceFarBubble = "true";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
				};
			};
		};
		class PointLights
		{
			class PointLight
			{
				brightness = 0.5;
				radius = 5;
				dayLight = 0;
				color[] = { 2.5, 2.5, 1.5, 0.0 };
			};
		};
	};
	class Zen_InteriorLightDim : Inventory_Base
	{
		scope=1;
		storageCategory = 10;
		model = "\dz\gear\consumables\Stone.p3d";
		forceFarBubble = "true";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };

		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
				};
			};
		};
		class PointLights
		{
			class PointLight
			{
				brightness = 0.3;
				radius = 3;
				dayLight = 0;
				color[] = { 2.5, 2.5, 1.5, 0.0 };
			};
		};
	};

	// Static music boombox
	class Zen_BoomBox_Static : Inventory_Base
    {
		scope=1;
		storageCategory = 10;
        displayName = "$STR_CfgVehicles_ZenBoombox0";
        descriptionShort = "$STR_CfgVehicles_ZenBoombox1";
		initVolume = 0.5;
        maxVolume = 1.0;
		attractsZombies = "false";
        model = "\ZenModPack\data\models\music\boombox\zen_boombox.p3d";
		itemBehaviour = 1;
        weight = 99999;
        itemSize[] = {4,3};
        attachments[] =
        {
            "BatteryD",
            "ZenCassette"
        };
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 99999;
					healthLevels[] =
					{

						{
							1,

							{
								"ZenModPack\data\textures\music\boombox\boombox.rvmat"
							}
						},

						{
							0.69999999,

							{
								"ZenModPack\data\textures\music\boombox\boombox.rvmat"
							}
						},

						{
							0.5,

							{
								"ZenModPack\data\textures\music\boombox\boombox_damage.rvmat"
							}
						},

						{
							0.30000001,

							{
								"ZenModPack\data\textures\music\boombox\boombox_damage.rvmat"
							}
						},

						{
							0,

							{
								"ZenModPack\data\textures\music\boombox\boombox_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};

	// Static invisible music boombox
	class Zen_BoomBox_Invisible_Static : Zen_BoomBox_Static
    {
		scope=1;
		storageCategory = 10;
		initVolume = 0.4;
		maxVolume = 0.4;
		displayName = "";
		descriptionShort = "";
		model = "\dz\gear\consumables\Stone.p3d";
		hiddenSelections[] = { "zbytek" };
		hiddenSelectionsTextures[] = { "#(argb,8,8,3)color(1,1,1,0,CA)" };
	};

	// Fireplace which burns for eternity
	class Zen_EternalFireplace: Fireplace {};

	// Vanilla tree has sound effects, this doesn't
	class Zen_ChristmasTree_Green: HouseNoDestruct
	{
		model="DZ\plants\tree\t_PiceaAbies_2s_green_xmas.p3d";
	};

	// THIS MUST BE AT THE END FOR DUMP CFGVEHICLES TO WORK! 
	class ZenModPack_EndDump {};
};

class CfgSlots
{
	//! RAID ALARM
	class Slot_ZenMegaphone
	{
		name = "ZenMegaphone";
		displayName = "$STR_CfgVehicles_Megaphone0";
		ghostIcon = "missing";
	}

	//! SLEEPING BAGS 
	class Slot_ZenSleepingBag
	{
		name = "ZenSleepingBag";
		displayName = "$STR_CfgVehicles_ZenSleepingBag0";
		ghostIcon = "set:sleepingbag_icon image:sleepingbag";
	};

	//! GLOVEBOX 
	class Slot_ZenCarGlovebox
	{
		name = "ZenCarGlovebox";
		displayName = "$STR_CfgVehicles_ZenGlovebox";
		ghostIcon = "missing";
		description = "$STR_CfgVehicles_ZenGlovebox";
	};

	//! FIRE FUEL
	class Slot_ZenFuel
	{
		name = "ZenFuel";
		displayName = "$STR_ZenFireFuel0";
		ghostIcon = "gascanister";
	};

	//! MUSIC 
	class Slot_ZenCassette
    {
        name = "ZenCassette";
        displayName = "$STR_CfgVehicles_ZenCassette0";
		ghostIcon = "set:zen_music image:cassette_icon";
    };

	//! CAR WORKBENCH 
	class Slot_ZenCWB_Spark2
	{
		name = "ZenCWB_Spark2";
		displayName = "$STR_CfgVehicles_SparkPlug0";
		ghostIcon = "set:dayz_inventory image:sparkplug";
	};
	class Slot_ZenCWB_Pliers
	{
		name = "ZenCWB_Pliers";
		displayName = "$STR_CfgVehicles_Pliers0";
		ghostIcon = "set:zencarworkbenchslots image:pliers";
	};
	class Slot_ZenCWB_Screwdriver
	{
		name = "ZenCWB_Screwdriver";
		displayName = "$STR_CfgVehicles_Screwdriver0";
		ghostIcon = "set:zencarworkbenchslots image:screwdriver";
	};
	class Slot_ZenCWB_Sledgehammer
	{
		name = "ZenCWB_Sledgehammer";
		displayName = "$STR_CfgVehicles_SledgeHammer0";
		ghostIcon = "set:zencarworkbenchslots image:sledgehammer";
	};
	class Slot_ZenCWB_Wrench
	{
		name = "ZenCWB_Wrench";
		displayName = "$STR_CfgVehicles_Wrench0";
		ghostIcon = "set:zencarworkbenchslots image:wrench";
	};
	class Slot_ZenCWB_Paint1
	{
		name = "ZenCWB_Paint1";
		displayName = "$STR_CfgVehicles_ZenSprayCan";
		ghostIcon = "set:zencarworkbenchslots image:spraycan";
	};
	class Slot_ZenCWB_Paint2
	{
		name = "ZenCWB_Paint2";
		displayName = "$STR_CfgVehicles_ZenSprayCan";
		ghostIcon = "set:zencarworkbenchslots image:spraycan";
	};

	//! TIRE RACK 
	class Slot_ZenTireRack1
	{
		name = "ZenTireRack1";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack2
	{
		name = "ZenTireRack2";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack3
	{
		name = "ZenTireRack3";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack4
	{
		name = "ZenTireRack4";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack5
	{
		name = "ZenTireRack5";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack6
	{
		name = "ZenTireRack6";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack7
	{
		name = "ZenTireRack7";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireRack8
	{
		name = "ZenTireRack8";
		displayName = "$STR_ZenTireRackWheel";
		ghostIcon = "set:dayz_inventory image:wheel";
	};
	class Slot_ZenTireIron
	{
		name = "ZenTireIron";
		displayName = "$STR_CfgVehicles_LugWrench0";
		ghostIcon = "set:tirerackslots image:tireiron";
	};
	class Slot_ZenTireRepair
	{
		name = "ZenTireRepair";
		displayName = "$STR_CfgVehicles_TireRepairKit0";
		ghostIcon = "set:tirerackslots image:tirerepairkit";
	};
	class Slot_ZenTireGasoline
	{
		name = "ZenTireGasoline";
		displayName = "$STR_CfgVehicles_CanisterGasoline0";
		ghostIcon = "set:tirerackslots image:gasoline";
	};

	//! CHESS 
	class Slot_ZenChess1
	{
		name = "ZenChess1";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess2
	{
		name = "ZenChess2";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess3
	{
		name = "ZenChess3";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess4
	{
		name = "ZenChess4";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess5
	{
		name = "ZenChess5";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess6
	{
		name = "ZenChess6";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess7
	{
		name = "ZenChess7";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess8
	{
		name = "ZenChess8";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess9
	{
		name = "ZenChess9";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess10
	{
		name = "ZenChess10";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess11
	{
		name = "ZenChess11";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess12
	{
		name = "ZenChess12";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess13
	{
		name = "ZenChess13";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess14
	{
		name = "ZenChess14";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess15
	{
		name = "ZenChess15";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess16
	{
		name = "ZenChess16";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess17
	{
		name = "ZenChess17";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess18
	{
		name = "ZenChess18";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess19
	{
		name = "ZenChess19";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess20
	{
		name = "ZenChess20";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess21
	{
		name = "ZenChess21";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess22
	{
		name = "ZenChess22";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess23
	{
		name = "ZenChess23";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess24
	{
		name = "ZenChess24";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess25
	{
		name = "ZenChess25";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess26
	{
		name = "ZenChess26";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess27
	{
		name = "ZenChess27";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess28
	{
		name = "ZenChess28";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess29
	{
		name = "ZenChess29";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess30
	{
		name = "ZenChess30";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess31
	{
		name = "ZenChess31";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess32
	{
		name = "ZenChess32";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess33
	{
		name = "ZenChess33";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess34
	{
		name = "ZenChess34";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess35
	{
		name = "ZenChess35";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess36
	{
		name = "ZenChess36";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess37
	{
		name = "ZenChess37";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess38
	{
		name = "ZenChess38";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess39
	{
		name = "ZenChess39";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess40
	{
		name = "ZenChess40";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess41
	{
		name = "ZenChess41";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess42
	{
		name = "ZenChess42";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess43
	{
		name = "ZenChess43";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess44
	{
		name = "ZenChess44";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess45
	{
		name = "ZenChess45";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess46
	{
		name = "ZenChess46";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess47
	{
		name = "ZenChess47";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess48
	{
		name = "ZenChess48";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess49
	{
		name = "ZenChess49";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess50
	{
		name = "ZenChess50";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess51
	{
		name = "ZenChess51";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess52
	{
		name = "ZenChess52";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess53
	{
		name = "ZenChess53";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess54
	{
		name = "ZenChess54";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess55
	{
		name = "ZenChess55";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess56
	{
		name = "ZenChess56";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess57
	{
		name = "ZenChess57";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess58
	{
		name = "ZenChess58";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess59
	{
		name = "ZenChess59";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess60
	{
		name = "ZenChess60";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess61
	{
		name = "ZenChess61";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess62
	{
		name = "ZenChess62";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
	class Slot_ZenChess63
	{
		name = "ZenChess63";
		displayName = "";
		ghostIcon = "set:zenchess_blackslot image:blackslot";
	};
	class Slot_ZenChess64
	{
		name = "ZenChess64";
		displayName = "";
		ghostIcon = "set:zenchess_whiteslot image:whiteslot";
	};
};

class CfgNonAIVehicles
{
	class ProxyAttachment;

	//! RAID ALARM
	class Proxyzenbaseradio_prox : ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Back";
		model = "ZenModPack/data/models/RaidAlarm/proxy/zenbaseradio_prox.p3d";
	}
	class Proxyzenmegaphone_prox : ProxyAttachment
	{
		scope = 2;
		inventorySlot = "ZenMegaphone";
		model = "ZenModPack/data/models/RaidAlarm/proxy/zenmegaphone_prox.p3d";
	}

	//! CAMONET SHELTER
	class Proxyalicebackpack_g : ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"Back"};
		model = "\DZ\characters\backpacks\alicebackpack_g.p3d";
	}
	class Proxymosin9130 : ProxyAttachment
	{
		scope = 2;
		inventorySlot[] = {"Shoulder"};
		model = "\DZ\weapons\firearms\mosin9130\mosin9130.p3d";
	}

	//! WORKBENCH 
	class ProxyZenCWBTireRepairKit : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRepair";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBTireRepairKit.p3d";
	};
	class ProxyZenCWBBattery : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "CarBattery";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBBattery.p3d";
	};
	class ProxyZenCWBRadiator : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "CarRadiator";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBRadiator.p3d";
	};
	class ProxyZenCWBTruckBattery : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "TruckBattery";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBTruckBattery.p3d";
	};
	class ProxyZenCWBGlowplug : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "Glowplug";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBGlowplug.p3d";
	};
	class ProxyZenCWBSparkplug : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "SparkPlug";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBSparkplug.p3d";
	};
	class ProxyZenCWBSparkplug2 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Spark2";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBSparkplug2.p3d";
	};
	class ProxyZenCWBJerrycan : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireGasoline";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBJerrycan.p3d";
	};
	class ProxyZenCWBPliers : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Pliers";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBPliers.p3d";
	};
	class ProxyZenCWBRags : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "Rags";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBRags.p3d";
	};
	class ProxyZenCWBScrewdriver : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Screwdriver";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBScrewdriver.p3d";
	};
	class ProxyZenCWBSledgehammer : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Sledgehammer";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBSledgehammer.p3d";
	};
	class ProxyZenCWBTireIron : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireIron";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBTireIron.p3d";
	};
	class ProxyZenCWBWrench : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Wrench";
		model = "\ZenModPack\data\models\workbench\proxy\ProxyZenCWBWrench.p3d";
	};
	class ProxyZenCWBPaint1 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Paint1";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBPaint1.p3d";
	};
	class ProxyZenCWBPaint2 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenCWB_Paint2";
		model = "\ZenModPack\data\models\workbench\proxy\ZenCWBPaint2.p3d";
	};

	//! TIRE RACK
	class ProxyZenTireProxy1 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack1";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy1.p3d";
	};
	class ProxyZenTireProxy2 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack2";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy2.p3d";
	};
	class ProxyZenTireProxy3 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack3";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy3.p3d";
	};
	class ProxyZenTireProxy4 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack4";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy4.p3d";
	};
	class ProxyZenTireProxy5 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack5";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy5.p3d";
	};
	class ProxyZenTireProxy6 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack6";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy6.p3d";
	};
	class ProxyZenTireProxy7 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack7";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy7.p3d";
	};
	class ProxyZenTireProxy8 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRack8";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy8.p3d";
	};
	class ProxyZenTireProxy9 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireIron";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy9.p3d";
	};
	class ProxyZenTireProxy10 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireRepair";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy10.p3d";
	};
	class ProxyZenTireProxy11 : ProxyAttachment
	{
		scope = 1;
		inventorySlot = "ZenTireGasoline";
		model = "\ZenModPack\data\models\tirerack\proxy\ZenTireProxy11.p3d";
	};

	//! CHESS 
	class ZenChessProxyBase : ProxyAttachment
	{
		scope = 1;
		model = "\ZenModPack\data\models\chess\proxy\ZenChessProxy.p3d";
	};
	class ProxyZenChessProxy1  : ZenChessProxyBase { inventorySlot = "ZenChess64"; };
	class ProxyZenChessProxy2  : ZenChessProxyBase { inventorySlot = "ZenChess63"; };
	class ProxyZenChessProxy3  : ZenChessProxyBase { inventorySlot = "ZenChess62"; };
	class ProxyZenChessProxy4  : ZenChessProxyBase { inventorySlot = "ZenChess61"; };
	class ProxyZenChessProxy5  : ZenChessProxyBase { inventorySlot = "ZenChess60"; };
	class ProxyZenChessProxy6  : ZenChessProxyBase { inventorySlot = "ZenChess59"; };
	class ProxyZenChessProxy7  : ZenChessProxyBase { inventorySlot = "ZenChess58"; };
	class ProxyZenChessProxy8  : ZenChessProxyBase { inventorySlot = "ZenChess57"; };
	class ProxyZenChessProxy9  : ZenChessProxyBase { inventorySlot = "ZenChess56"; };
	class ProxyZenChessProxy10 : ZenChessProxyBase { inventorySlot = "ZenChess55"; };
	class ProxyZenChessProxy11 : ZenChessProxyBase { inventorySlot = "ZenChess54"; };
	class ProxyZenChessProxy12 : ZenChessProxyBase { inventorySlot = "ZenChess53"; };
	class ProxyZenChessProxy13 : ZenChessProxyBase { inventorySlot = "ZenChess52"; };
	class ProxyZenChessProxy14 : ZenChessProxyBase { inventorySlot = "ZenChess51"; };
	class ProxyZenChessProxy15 : ZenChessProxyBase { inventorySlot = "ZenChess50"; };
	class ProxyZenChessProxy16 : ZenChessProxyBase { inventorySlot = "ZenChess49"; };
	class ProxyZenChessProxy17 : ZenChessProxyBase { inventorySlot = "ZenChess48"; };
	class ProxyZenChessProxy18 : ZenChessProxyBase { inventorySlot = "ZenChess47"; };
	class ProxyZenChessProxy19 : ZenChessProxyBase { inventorySlot = "ZenChess46"; };
	class ProxyZenChessProxy20 : ZenChessProxyBase { inventorySlot = "ZenChess45"; };
	class ProxyZenChessProxy21 : ZenChessProxyBase { inventorySlot = "ZenChess44"; };
	class ProxyZenChessProxy22 : ZenChessProxyBase { inventorySlot = "ZenChess43"; };
	class ProxyZenChessProxy23 : ZenChessProxyBase { inventorySlot = "ZenChess42"; };
	class ProxyZenChessProxy24 : ZenChessProxyBase { inventorySlot = "ZenChess41"; };
	class ProxyZenChessProxy25 : ZenChessProxyBase { inventorySlot = "ZenChess40"; };
	class ProxyZenChessProxy26 : ZenChessProxyBase { inventorySlot = "ZenChess39"; };
	class ProxyZenChessProxy27 : ZenChessProxyBase { inventorySlot = "ZenChess38"; };
	class ProxyZenChessProxy28 : ZenChessProxyBase { inventorySlot = "ZenChess37"; };
	class ProxyZenChessProxy29 : ZenChessProxyBase { inventorySlot = "ZenChess36"; };
	class ProxyZenChessProxy30 : ZenChessProxyBase { inventorySlot = "ZenChess35"; };
	class ProxyZenChessProxy31 : ZenChessProxyBase { inventorySlot = "ZenChess34"; };
	class ProxyZenChessProxy32 : ZenChessProxyBase { inventorySlot = "ZenChess33"; };
	class ProxyZenChessProxy33 : ZenChessProxyBase { inventorySlot = "ZenChess32"; };
	class ProxyZenChessProxy34 : ZenChessProxyBase { inventorySlot = "ZenChess31"; };
	class ProxyZenChessProxy35 : ZenChessProxyBase { inventorySlot = "ZenChess30"; };
	class ProxyZenChessProxy36 : ZenChessProxyBase { inventorySlot = "ZenChess29"; };
	class ProxyZenChessProxy37 : ZenChessProxyBase { inventorySlot = "ZenChess28"; };
	class ProxyZenChessProxy38 : ZenChessProxyBase { inventorySlot = "ZenChess27"; };
	class ProxyZenChessProxy39 : ZenChessProxyBase { inventorySlot = "ZenChess26"; };
	class ProxyZenChessProxy40 : ZenChessProxyBase { inventorySlot = "ZenChess25"; };
	class ProxyZenChessProxy41 : ZenChessProxyBase { inventorySlot = "ZenChess24"; };
	class ProxyZenChessProxy42 : ZenChessProxyBase { inventorySlot = "ZenChess23"; };
	class ProxyZenChessProxy43 : ZenChessProxyBase { inventorySlot = "ZenChess22"; };
	class ProxyZenChessProxy44 : ZenChessProxyBase { inventorySlot = "ZenChess21"; };
	class ProxyZenChessProxy45 : ZenChessProxyBase { inventorySlot = "ZenChess20"; };
	class ProxyZenChessProxy46 : ZenChessProxyBase { inventorySlot = "ZenChess19"; };
	class ProxyZenChessProxy47 : ZenChessProxyBase { inventorySlot = "ZenChess18"; };
	class ProxyZenChessProxy48 : ZenChessProxyBase { inventorySlot = "ZenChess17"; };
	class ProxyZenChessProxy49 : ZenChessProxyBase { inventorySlot = "ZenChess16"; };
	class ProxyZenChessProxy50 : ZenChessProxyBase { inventorySlot = "ZenChess15"; };
	class ProxyZenChessProxy51 : ZenChessProxyBase { inventorySlot = "ZenChess14"; };
	class ProxyZenChessProxy52 : ZenChessProxyBase { inventorySlot = "ZenChess13"; };
	class ProxyZenChessProxy53 : ZenChessProxyBase { inventorySlot = "ZenChess12"; };
	class ProxyZenChessProxy54 : ZenChessProxyBase { inventorySlot = "ZenChess11"; };
	class ProxyZenChessProxy55 : ZenChessProxyBase { inventorySlot = "ZenChess10"; };
	class ProxyZenChessProxy56 : ZenChessProxyBase { inventorySlot = "ZenChess9";  };
	class ProxyZenChessProxy57 : ZenChessProxyBase { inventorySlot = "ZenChess8";  };
	class ProxyZenChessProxy58 : ZenChessProxyBase { inventorySlot = "ZenChess7";  };
	class ProxyZenChessProxy59 : ZenChessProxyBase { inventorySlot = "ZenChess6";  };
	class ProxyZenChessProxy60 : ZenChessProxyBase { inventorySlot = "ZenChess5";  };
	class ProxyZenChessProxy61 : ZenChessProxyBase { inventorySlot = "ZenChess4";  };
	class ProxyZenChessProxy62 : ZenChessProxyBase { inventorySlot = "ZenChess3";  };
	class ProxyZenChessProxy63 : ZenChessProxyBase { inventorySlot = "ZenChess2";  };
	class ProxyZenChessProxy64 : ZenChessProxyBase { inventorySlot = "ZenChess1";  };
};

class CfgAmmo
{
	class DefaultAmmo;

	//! CONCUSSION GRENADE
	class Zen_ConcussionGrenade_Ammo : DefaultAmmo
	{
		indirectHit = 1;
		indirectHitRange = 5;
		indirectHitRangeMultiplier = 4;
		explosive = 1;
		typicalSpeed = 3;
		initSpeed = 3;
		slowdownThreshold = 100;
		simulation = "shotShell";
		simulationStep = 0.050000001;
		soundSetExplosion[] =
		{
			"Flashgrenade_Explosion_SoundSet",
			"Flashgrenade_Explosion_Tail_SoundSet"
		};
		class DamageApplied
		{
			type = "FragGrenade";
			class Health
			{
				damage = 1;
			};
			class Blood
			{
				damage = 0;
			};
			class Shock
			{
				damage = 1;
			};
		};
		class NoiseExplosion
		{
			strength = 100;
			type = "shot";
		};
	};

	//! ARTILLERY
	class Zen_ArtilleryBomb_Ammo : DefaultAmmo
	{
		indirectHit = 1;
		indirectHitRange = 15;
		explosive = 1;
		typicalSpeed = 3;
		initSpeed = 3;
		simulation = "shotShell";
		simulationStep = 0.05;

		soundSetExplosion[] =
		{
			"Landmine_Explosion_SoundSet",
			"Plastic_Explosion_SoundSet",
			"Landmine_Explosion_Tail_SoundSet",
			"Plastic_Explosion_Tail_SoundSet"
		};

		class DamageApplied
		{
			type = "FragGrenade";
			class Health
			{
				damage = 1;
			};
			class Blood
			{
				damage = 1;
			};
			class Shock
			{
				damage = 33;
			};
		};
		class NoiseExplosion
		{
			strength = 100;
			type = "shot";
		};
	};
};

class CfgSoundShaders
{
	//! RAID ALARM
	class Zen_RaidAlarm_SoundShader
	{
		frequency = 1;
		range = 500;
		volume = 1;
		samples[] =
		{
			{ "ZenModPack\data\sounds\raidalarm\alarm", 1 }
		};
	}

	//! NOTIFICATION AUDIO 
	class Zen_NotificationAudio_SoundShader_Base
	{
		frequency = 1;
		range = 10;
		volume = 1;
	};

	// Radio static
	class Zen_MissionRadioStatic_SoundShader : Zen_NotificationAudio_SoundShader_Base { samples[] = { { "ZenModPack\data\sounds\notifications\radiostatic", 1 } }; };

	//! MUSIC
	class Zen_Cassette_SoundShader_Base
    {
        samples[] = {};
        frequency = 1;
        range = 100;
        volume = 1;
    };

	//! CHESS 
	class ZenChess_SoundShader_Base
	{
		samples[] = {};
		frequency = 1;
		range = 5;
		volume = 5;
	};

	class ZenChess_SoundShader : ZenChess_SoundShader_Base
	{ 
		samples[] = 
		{ 
			{ "ZenModPack\data\sounds\chess\chess1", 1 },
			{ "ZenModPack\data\sounds\chess\chess2", 1 },
			{ "ZenModPack\data\sounds\chess\chess3", 1 },
			{ "ZenModPack\data\sounds\chess\chess4", 1 },
			{ "ZenModPack\data\sounds\chess\chess5", 1 },
			{ "ZenModPack\data\sounds\chess\chess6", 1 }
		}; 
	};

	//! ZOMBIE DOORS 
	class Zombie_Hit_Door_SoundShader
	{
		samples[] =
		{
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_1",
				1
			},
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_2",
				1
			},
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_3",
				1
			},
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_4",
				1
			},
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_5",
				1
			},
			{
				"\DZ\sounds\vehicles\shared\collisions\offroad_hit_character_6",
				1
			}
		};
		volume = 1;
		range = 40;
	};

	//! CRAFTING SOUNDS
	class baseCharacter_SoundShader;

	// Whetstone
	class Zen_SharpenMelee_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop1", 1 },
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop2", 1 },
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop3", 1 },
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop4", 1 },
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop5", 1 },
			{ "ZenModPack\data\sounds\SharpenMelee\SharpenMelee_loop6", 1 }
		};
		volume=0.5;
	};
	// Weapon Cleaning Kit
	class Zen_CleanWeapon_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop1", 1 },
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop2", 1 },
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop3", 1 },
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop4", 1 },
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop5", 1 },
			{ "ZenModPack\data\sounds\CleanWeapon\CleanWeapon_loop6", 1 }
		};
		volume=0.3;
	};
	// Disinfectant bottle
	class Zen_DisinfectBottle_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\DisinfectBottle\DisinfectBottle_loop1", 1 }
		};
		volume=1.0;
	};
	// Disinfectant spray
	class Zen_DisinfectSpray_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\DisinfectSpray\DisinfectSpray_loop1", 1 },
			{ "ZenModPack\data\sounds\DisinfectSpray\DisinfectSpray_loop2", 1 },
			{ "ZenModPack\data\sounds\DisinfectSpray\DisinfectSpray_loop3", 1 }
		};
		volume=0.2;
	};
	// Hands in the bush
	class Zen_HandsBush_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_1", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_2", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_3", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_4", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_5", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_6", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_7", 1 },
			{ "\DZ\sounds\Characters\movement\vegetation\bush_walk_8", 1 }
		};
		volume=0.3;
	};
	// Duct tape
	class Zen_DuctTape_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\DuctTape\DuctTape_loop1", 1 },
			{ "ZenModPack\data\sounds\DuctTape\DuctTape_loop2", 1 },
			{ "ZenModPack\data\sounds\DuctTape\DuctTape_loop3", 1 },
			{ "ZenModPack\data\sounds\DuctTape\DuctTape_loop4", 1 },
			{ "ZenModPack\data\sounds\DuctTape\DuctTape_loop5", 1 }
		};
		volume=0.2;
	};
	// Break sticks
	class Zen_BreakStick_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop1", 1 },
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop2", 1 },
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop3", 1 },
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop4", 1 },
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop5", 1 },
			{ "ZenModPack\data\sounds\BreakStick\BreakStick_loop6", 1 }
		};
		volume=0.5;
	};
	// Break water bottle
	class Zen_BreakWaterBottle_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\BreakWaterBottle\BreakWaterBottle_loop1", 1 },
			{ "ZenModPack\data\sounds\BreakWaterBottle\BreakWaterBottle_loop2", 1 },
			{ "ZenModPack\data\sounds\BreakWaterBottle\BreakWaterBottle_loop3", 1 }
		};
		volume=0.2;
	};
	// Craft bolt / sharpen stick
	class Zen_CraftBolts_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\CraftBolts\CraftBolts_loop1", 1 },
			{ "ZenModPack\data\sounds\CraftBolts\CraftBolts_loop2", 1 },
			{ "ZenModPack\data\sounds\CraftBolts\CraftBolts_loop3", 1 }
		};
		volume=0.5;
	};
	// Spray paint
	class Zen_SprayPaint_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\SprayPaint\SprayPaint_loop1", 1 },
			{ "ZenModPack\data\sounds\SprayPaint\SprayPaint_loop2", 1 },
			{ "ZenModPack\data\sounds\SprayPaint\SprayPaint_loop3", 1 }
		};
		volume=0.3;
	};
	// Sewing kit
	class Zen_SewingKit_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\SewingKit\SewingKit_loop1", 1 },
			{ "ZenModPack\data\sounds\SewingKit\SewingKit_loop2", 1 },
			{ "ZenModPack\data\sounds\SewingKit\SewingKit_loop3", 1 }
		};
		volume=1.0;
	};
	// Leather sewing kit
	class Zen_LeatherSewingKit_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop1", 1 },
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop2", 1 },
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop3", 1 },
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop4", 1 },
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop5", 1 },
			{ "ZenModPack\data\sounds\LeatherSewingKit\LeatherSewingKit_loop6", 1 }
		};
		volume=0.2;
	};
	// Rubber repair
	class Zen_Rubber_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Rubber\Rubber_loop1", 1 },
			{ "ZenModPack\data\sounds\Rubber\Rubber_loop2", 1 },
			{ "ZenModPack\data\sounds\Rubber\Rubber_loop3", 1 },
			{ "ZenModPack\data\sounds\Rubber\Rubber_loop4", 1 },
			{ "ZenModPack\data\sounds\Rubber\Rubber_loop5", 1 }
		};
		volume=0.4;
	};
	// Vanilla saw wood sounds
	class Zen_SawWood_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_1", 1 },
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_2", 1 },
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_3", 1 },
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_4", 1 },
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_5", 1 },
			{ "DZ\sounds\Characters\actions\construction\HackSaw_loop_6", 1 }
		};
		volume=0.1;
	};
	// Vanilla screwdriver mining sounds
	class Zen_BreakStone_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_1", 1 },
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_2", 1 },
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_3", 1 },
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_4", 1 },
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_5", 1 },
			{ "DZ\sounds\Characters\actions\horticulture\mineScrewdriver_6", 1 }
		};
		volume=1;
	};
	// Electronic repair soldering
	class Zen_ElectronicRepair_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop1", 1 },
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop2", 1 },
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop3", 1 },
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop4", 1 },
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop5", 1 },
			{ "ZenModPack\data\sounds\ElectronicRepair\ElectronicRepair_loop6", 1 }
		};
		volume=0.3;
	};
	// Stick sharpening
	class Zen_Bones_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Bones\Bones_loop1", 1 },
			{ "ZenModPack\data\sounds\Bones\Bones_loop2", 1 }
		};
		volume=0.3;
	};
	// Metal wire vanilla plier sounds
	class Zen_MetalWire_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\items\pliers\pliers_loop_1", 1 },
			{ "DZ\sounds\Characters\actions\items\pliers\pliers_loop_2", 1 },
			{ "DZ\sounds\Characters\actions\items\pliers\pliers_loop_3", 1 },
			{ "DZ\sounds\Characters\actions\items\pliers\pliers_loop_4", 1 }
		};
		volume=1.0;
	};
	// Syringe vanilla sounds
	class Zen_BloodTest_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\healing\syringe_out_0",		1 },
			{ "DZ\sounds\Characters\actions\healing\syringe_spear_0",	1 },
			{ "DZ\sounds\Characters\actions\healing\syringe_spear_1",	1 },
			{ "DZ\sounds\Characters\actions\healing\syringe_splash_0",	1 }
		};
		volume=1.0;
	};
	// Rocks
	class Zen_Rocks_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop1", 1 },
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop2", 1 },
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop3", 1 },
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop4", 1 },
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop5", 1 },
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop6", 1 }
		};
		volume=0.3;
	};
	// Nails
	class Zen_Nails_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Nails\nails1", 1 },
			{ "ZenModPack\data\sounds\Nails\nails2", 1 },
			{ "ZenModPack\data\sounds\Nails\nails3", 1 },
			{ "ZenModPack\data\sounds\Nails\nails4", 1 },
			{ "ZenModPack\data\sounds\Nails\nails5", 1 },
			{ "ZenModPack\data\sounds\Nails\nails6", 1 }
		};
		volume=1.0;
	};
	// Wood & firewood
	class Zen_Wood_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Wood\Wood1", 1 },
			{ "ZenModPack\data\sounds\Wood\Wood2", 1 },
			{ "ZenModPack\data\sounds\Wood\Wood3", 1 }
		};
		volume=0.3;
	};
	// Paper
	class Zen_Paper_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\items\note\noteIn_1", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteIn_2", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteIn_3", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteIn_4", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteIn_5", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteIn_6", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_1", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_2", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_3", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_4", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_5", 1 },
			{ "DZ\sounds\Characters\actions\items\note\noteOut_6", 1 }
		};
		volume=0.17782794;
	};
	// Pain male
	class Zen_PainMale_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\voice\male_1\pain_1", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_2", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_3", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_4", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_5", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_6", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_7", 1 },
			{ "DZ\sounds\Characters\voice\male_1\pain_8", 1 }
		};
		volume="0.5 * (male factor[(1-0.2),(1-0.1)]) * (male factor[(0.2+1),(0.1+1)])";
	};
	// Pain female
	class Zen_PainFemale_loop_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\voice\female_1\pain_1", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_2", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_3", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_4", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_5", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_6", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_7", 1 },
			{ "DZ\sounds\Characters\voice\female_1\pain_8", 1 }
		};
		volume="0.5 * (female factor[(1-0.2),(1-0.1)]) * (female factor[(0.2+1),(0.1+1)])";
	};


	// ATTACH/DETACH
	// Rocks move
	class Zen_RocksAttach_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop5", 1 }
		};
		volume=0.6;
	};
	class Zen_RocksDetach_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Rocks\Rocks_loop6", 1 }
		};
		volume=0.6;
	};
	class Zen_PlankAttach_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\Wood\Wood3", 1 }
		};
		volume=1.0;
	};
	class Zen_SticksAttach_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "DZ\sounds\Characters\actions\items\shelters\Shelter_Attach_Wooden_Stick", 1 }
		};
		volume=1.0;
	}
	class Zen_MetalPlateAttach_Soundshader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{ "ZenModPack\data\sounds\MetalPlate\metalplate1", 1 },
			{ "ZenModPack\data\sounds\MetalPlate\metalplate2", 1 }
		};
		volume=1.0;
	};

	//! UTILITIES
	class Zen_Notify_SoundShader_Base
	{
		samples[] = {};
		frequency = 1;
		range = 10;
		volume = 1;
	};

	class Zen_NotifySoundset : Zen_Notify_SoundShader_Base
	{
		samples[] =
		{
			{ "ZenModPack\data\sounds\Notifications\notify", 1 }
		};
	};
};

class CfgSoundSets
{
	class baseVehicles_SoundSet;
	class baseCharacter_SoundSet;

	//! RAID ALARM
	class Zen_RaidAlarm_SoundSet : baseVehicles_SoundSet
	{
		soundShaders[] = { "Zen_RaidAlarm_SoundShader" };
		volumefactor = 1;
		class Noise
		{
			strength = 500;
			type = "shot";
		}
	}

	//! NOTIFICATION 
	// Tip: Apply a 1000hz High Pass Filter in Audacity to get radio sounding voice audio
	class Zen_BaseMissionRadio_SoundSet
	{
		spatial=0;
		doppler=0;
		loop=0;
		range=50;
		volume=2.0; // Due to high-pass filter audio file volume drops quite a bit.
	};

	//! UTILITIES
	class Zen_NotifySoundset { soundShaders[] = { "Zen_NotifySoundset" }; volumeFactor = 0.15; };

	//! CHESS 
	class ZenChess_SoundSet { soundShaders[] = { "ZenChess_SoundShader" }; volumeFactor = 1.0; };

	//! ZOMBIE DOORS 
	class Zombie_Hit_Door_SoundSet : baseVehicles_SoundSet
	{
		soundShaders[] =
		{
			"Zombie_Hit_Door_SoundShader"
		};
		volumefactor = 1;
		frequencyrandomizer = 1;
		volumerandomizer = 1;
		class Noise
		{
			strength = 20;
			type = "sound";
		};
	};

	//! CRAFTING SOUNDS 
	class Zen_SharpenMelee_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_SharpenMelee_loop_Soundshader" };
	};
	class Zen_CleanWeapon_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_CleanWeapon_loop_Soundshader" };
	};
	class Zen_DisinfectBottle_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_DisinfectBottle_loop_Soundshader" };
	};
	class Zen_DisinfectSpray_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_DisinfectSpray_loop_Soundshader" };
	};
	class Zen_HandsBush_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_HandsBush_loop_Soundshader" };
	};
	class Zen_DuctTape_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_DuctTape_loop_Soundshader" };
	};
	class Zen_BreakStick_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_BreakStick_loop_Soundshader" };
	};
	class Zen_BreakWaterBottle_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_BreakWaterBottle_loop_Soundshader" };
	};
	class Zen_CraftBolts_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_CraftBolts_loop_Soundshader" };
	};
	class Zen_SprayPaint_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_SprayPaint_loop_Soundshader" };
	};
	class Zen_SewingKit_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_SewingKit_loop_Soundshader" };
	};
	class Zen_LeatherSewingKit_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_LeatherSewingKit_loop_Soundshader" };
	};
	class Zen_Rubber_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Rubber_loop_Soundshader" };
	};
	class Zen_SawWood_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_SawWood_loop_Soundshader" };
	};
	class Zen_BreakStone_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_BreakStone_loop_Soundshader" };
	};
	class Zen_ElectronicRepair_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_ElectronicRepair_loop_Soundshader" };
	};
	class Zen_Bones_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Bones_loop_Soundshader" };
	};
	class Zen_MetalWire_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_MetalWire_loop_Soundshader" };
	};
	class Zen_BloodTest_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_BloodTest_loop_Soundshader" };
	};
	class Zen_Rocks_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Rocks_loop_Soundshader" };
	};
	class Zen_Nails_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Nails_loop_Soundshader" };
	};
	class Zen_Wood_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Wood_loop_Soundshader" };
	};
	class Zen_Paper_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_Paper_loop_Soundshader" };
	};
	class Zen_PainMale_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_PainMale_loop_Soundshader" };
	};
	class Zen_PainFemale_loop_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_PainFemale_loop_Soundshader" };
	};


	// ATTACH/DETACH
	class Zen_RocksAttach_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_RocksAttach_Soundshader" };
	};
	class Zen_RocksDetach_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_RocksDetach_Soundshader" };
	};
	class Zen_PlankAttach_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_PlankAttach_Soundshader" };
	};
	class Zen_SticksAttach_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_SticksAttach_Soundshader" };
	};
	class Zen_MetalPlateAttach_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]= { "Zen_MetalPlateAttach_Soundshader" };
	};
};

class CfgSoundTables
{
	class CfgActionsSoundTables
	{
		class Craft_LookupTable
		{
			class Zen_SharpenMelee_loop
			{
				category="Zen_SharpenMelee"; // This is the ID to link this sound effect to an action/recipe
				soundSets[]= { "Zen_SharpenMelee_loop_SoundSet" };
			};
			class Zen_CleanWeapon_loop
			{
				category="Zen_CleanWeapon";
				soundSets[]= { "Zen_CleanWeapon_loop_SoundSet" };
			};
			class Zen_DisinfectBottle_loop
			{
				category="Zen_DisinfectBottle";
				soundSets[]= { "Zen_DisinfectBottle_loop_SoundSet" };
			};
			class Zen_DisinfectSpray_loop
			{
				category="Zen_DisinfectSpray";
				soundSets[]= { "Zen_DisinfectSpray_loop_SoundSet" };
			};
			class Zen_HandsBush_loop
			{
				category="Zen_HandsBush";
				soundSets[]= { "Zen_HandsBush_loop_SoundSet" };
			};
			class Zen_DuctTape_loop
			{
				category="Zen_DuctTape";
				soundSets[]= { "Zen_DuctTape_loop_SoundSet" };
			};
			class Zen_BreakStick_loop
			{
				category="Zen_BreakStick";
				soundSets[]= { "Zen_BreakStick_loop_SoundSet" };
			};
			class Zen_BreakWaterBottle_loop
			{
				category="Zen_BreakWaterBottle";
				soundSets[]= { "Zen_BreakWaterBottle_loop_SoundSet" };
			};
			class Zen_CraftBolts_loop
			{
				category="Zen_CraftBolts";
				soundSets[]= { "Zen_CraftBolts_loop_SoundSet" };
			};
			class Zen_SprayPaint_loop
			{
				category="Zen_SprayPaint";
				soundSets[]= { "Zen_SprayPaint_loop_SoundSet" };
			};
			class Zen_SewingKit_loop
			{
				category="Zen_SewingKit";
				soundSets[]= { "Zen_SewingKit_loop_SoundSet" };
			};
			class Zen_LeatherSewingKit_loop
			{
				category="Zen_LeatherSewingKit";
				soundSets[]= { "Zen_LeatherSewingKit_loop_SoundSet" };
			};
			class Zen_Rubber_loop
			{
				category="Zen_Rubber";
				soundSets[]= { "Zen_Rubber_loop_SoundSet" };
			};
			class Zen_SawWood_loop
			{
				category="Zen_SawWood";
				soundSets[]= { "Zen_SawWood_loop_SoundSet" };
			};
			class Zen_BreakStone_loop
			{
				category="Zen_BreakStone";
				soundSets[]= { "Zen_BreakStone_loop_SoundSet" };
			};
			class Zen_ElectronicRepair_loop
			{
				category="Zen_ElectronicRepair";
				soundSets[]= { "Zen_ElectronicRepair_loop_SoundSet" };
			};
			class Zen_Bones_loop
			{
				category="Zen_Bones";
				soundSets[]= { "Zen_Bones_loop_SoundSet" };
			};
			class Zen_MetalWire_loop
			{
				category="Zen_MetalWire";
				soundSets[]= { "Zen_MetalWire_loop_SoundSet" };
			};
			class Zen_BloodTest_loop
			{
				category="Zen_BloodTest";
				soundSets[]= { "Zen_BloodTest_loop_SoundSet" };
			};
			class Zen_Rocks_loop
			{
				category="Zen_Rocks";
				soundSets[]= { "Zen_Rocks_loop_SoundSet" };
			};
			class Zen_Nails_loop
			{
				category="Zen_Nails";
				soundSets[]= { "Zen_Nails_loop_SoundSet" };
			};
			class Zen_Wood_loop
			{
				category="Zen_Wood";
				soundSets[]= { "Zen_Wood_loop_SoundSet" };
			};
			class Zen_Paper_loop
			{
				category="Zen_Paper";
				soundSets[]= { "Zen_Paper_loop_SoundSet" };
			};
			class Zen_PainMale_loop
			{
				category="Zen_PainMale";
				soundSets[]= { "Zen_PainMale_loop_SoundSet" };
			};
			class Zen_PainFemale_loop
			{
				category="Zen_PainFemale";
				soundSets[]= { "Zen_PainFemale_loop_SoundSet" };
			};
		};
	};
};
