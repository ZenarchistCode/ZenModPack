class ZenCampSitesPlugin extends PluginBase
{
	protected float m_DeltaTime;
    protected vector m_LastCamp;

	override void OnInit()
	{
		ZMPrint("[ZenCampSitesPlugin] :: OnInit");
		GetZenCampSitesConfig();
		m_DeltaTime = 0;
	}

	override void OnUpdate(float delta_time)
	{
		m_DeltaTime += delta_time;
		if (m_DeltaTime < GetZenCampSitesConfig().SpawnSecs)
			return;

		m_DeltaTime = 0;
		ManageCamps();
	}

	// Called every X minutes to spawn in a new camp
	void ManageCamps()
	{
		// If game is not ready for whatever reason, stop
		if (!GetGame() || !GetGame().GetWorld() || !GetZenCampSitesConfig())
			return;

		// Get new camp location
		CampSite camp = GetZenCampSitesConfig().CampSites.GetRandomElement();

		if (!camp)
			return;

		// If new camp is same as last camp, try for another 
		if (camp.Location == m_LastCamp)
		{
			camp = GetZenCampSitesConfig().CampSites.GetRandomElement();
		}

		// If it's still the same camp then skip this spawn.
		if (camp.Location == m_LastCamp)
			return;

		// Spawn new camp
		if (!camp)
		{
			Error("ZenCampSites :: IMPORTANT ERROR: Failed to create camp from JSON!");
			ZenCampSitesDebug("IMPORTANT ERROR: Failed to create camp from JSON!");
			return;
		}

		// Save last camp location
		m_LastCamp = camp.Location;

		// If a player is too close (within PlayerDetectZone) then don't spawn new camp right in front of their eyes.
		int x;
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		PlayerBase selectedPlayer;
		for (x = 0; x < players.Count(); x++)
		{
			selectedPlayer = PlayerBase.Cast(players.Get(x));
			if (selectedPlayer)
			{
				if (vector.Distance(selectedPlayer.GetPosition(), camp.Location) <= GetZenCampSitesConfig().PlayerDetectZone)
				{
					// This player is too close to spawn a camp near, stop.
					return;
				}
			}
		}

		// Define loop array index & Game object for this scope
		int i;
		Object obj;

		// Spawn fire
		FireplaceBase campFire = FireplaceBase.Cast(GetGame().CreateObject("Fireplace", camp.Location));
		if (!campFire)
		{
			Error("ZenCampSites :: IMPORTANT ERROR: Failed to create fire at " + camp.Location);
			ZenCampSitesDebug("IMPORTANT ERROR: Failed to create fire at " + camp.Location);
			return;
		}

		campFire.PlaceOnSurface();

		// Create firewood
		ItemBase fireWood = ItemBase.Cast(campFire.GetInventory().CreateAttachment("Firewood"));
		if (fireWood)
			fireWood.SetQuantity(6);

		// Create sticks
		ItemBase fireSticks = ItemBase.Cast(campFire.GetInventory().CreateAttachment("WoodenStick"));
		if (fireSticks)
			fireSticks.SetQuantity(10);

		// Create kindling
		ItemBase fireRags = ItemBase.Cast(campFire.GetInventory().CreateAttachment("Rag"));
		if (fireRags)
			fireRags.SetQuantity(6);

		// Create fuel 
		if (ZenModEnabled("ZenFireFuel"))
			ItemBase fuel = ItemBase.Cast(campFire.GetInventory().CreateAttachment("Zen_FireFuel"));

		// Create stone circle 
		ItemBase stones = ItemBase.Cast(campFire.GetInventory().CreateAttachment("Stone"));
		if (stones)
		{
			stones.SetQuantity(8);
			stones.LockToParent();
			campFire.SetStoneCircleState(true);
		}

		// Create clutter cutter 
		GetGame().CreateObjectEx("ClutterCutterFireplace", campFire.GetPosition(), ECE_PLACE_ON_SURFACE);

		// Twisted fire starter
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartFire, 250, false, campFire);
		campFire.SetLifetimeMax(GetZenCampSitesConfig().SpawnSecs);
		campFire.SetLifetime(GetZenCampSitesConfig().SpawnSecs);
		ZenFunctions.AlignToTerrain(campFire);

		// Generate random tent location near fire
		vector tentPos = ZenFunctions.GetRandomPointInCircle(camp.Location, GetZenCampSitesConfig().MinFireDistance, GetZenCampSitesConfig().MaxFireDistance);

		// Spawn camp tent
		EntityAI lootEAI = EntityAI.Cast(SpawnCampObject(camp.TentType, tentPos, "0 0 0"));

		if (!lootEAI)
		{
			Error("ZenCampSites :: IMPORTANT ERROR: Failed to create object: " + camp.TentType + " @ " + camp.Location);
			ZenCampSitesDebug("IMPORTANT ERROR: Failed to create object: " + camp.TentType + " @ " + camp.Location);
			return;
		}

		// Set orientation + persistence
		ZenCampSitesDebug("Spawned " + camp.TentType + " @ " + tentPos);
		ZenFunctions.OrientObjectToPosition(lootEAI, campFire.GetPosition(), "180 0 0");
		ZenFunctions.AlignToTerrain(lootEAI);
		lootEAI.SetLifetimeMax(GetZenCampSitesConfig().SpawnSecs);
		lootEAI.SetLifetime(GetZenCampSitesConfig().SpawnSecs);

		// Shuffle loot array order so we get better random loot dispersion
		GetZenCampSitesConfig().ShuffleLoot();

		// Spawn specific loot in object if appropriate
		if (camp.Loot.Count() > 0)
		{
			for (i = 0; i < camp.Loot.Count(); i++)
			{
				SpawnLoot(lootEAI, camp.Loot.Get(i));
			}
		}

		// Get zombie count - check general config first
		int zedCount = Math.RandomIntInclusive(GetZenCampSitesConfig().MinZombies, GetZenCampSitesConfig().MaxZombies);

		// Spawn predefined loot in object if appropriate
		if (camp.PredefinedLoot != "")
		{
			PredefinedLootSpawn predefLoot = GetZenCampSitesConfig().GetPredefinedLoot(camp.PredefinedLoot);

			if (predefLoot != NULL)
			{
				if (predefLoot.MaxZombies != 0)
					zedCount = Math.RandomIntInclusive(predefLoot.MinZombies, predefLoot.MaxZombies);

				for (i = 0; i < predefLoot.Loot.Count(); i++)
				{
					SpawnLoot(lootEAI, predefLoot.Loot.Get(i));
				}
			}
		}

		// Spawn zombie trigger
		if (zedCount > 0 && GetZenCampSitesConfig().ZombieTypes.Count() > 0)
		{
			ZenCampZombieTrigger zed_trigger = ZenCampZombieTrigger.Cast(GetGame().CreateObjectEx("ZenCampZombieTrigger", camp.Location, ECE_CREATEPHYSICS));
			zed_trigger.m_zombieCount = zedCount;
			zed_trigger.SetCollisionSphere(150);
			zed_trigger.SetLifetimeMax(GetZenCampSitesConfig().SpawnSecs);
			zed_trigger.SetLifetime(GetZenCampSitesConfig().SpawnSecs);
		}

		// Log it
		ZenCampSitesDebug("Spawned camp @ " + camp.Location + " with " + zedCount + " zombies");

		// Debug it 
		if (GetZenCampSitesConfig().DebugOn)
		{
			// Teleport player nearby if debug on
			for (x = 0; x < players.Count(); x++)
			{
				selectedPlayer = PlayerBase.Cast(players.Get(x));
				if (selectedPlayer)
				{
					DeveloperTeleport.SetPlayerPosition(selectedPlayer, camp.Location + "10 0 0");
				}
			}
		}
	}

	// Spawn loot in the given object
	void SpawnLoot(notnull EntityAI lootEAI, notnull LootSpawn lootSpawn)
	{
		// Check random chance
		if (!lootEAI.GetInventory() || Math.RandomFloat01() > lootSpawn.SpawnChance)
			return;

		// Create item
		string itemType = lootSpawn.ClassName;
		EntityAI item = lootEAI.GetInventory().CreateInInventory(itemType);
		if (!item)
			return;

		// If item was successfully created, set its health & quantity
		int quantity = Math.RandomIntInclusive(lootSpawn.MinQuantity, lootSpawn.MaxQuantity);
		float health = Math.RandomFloatInclusive(GetZenCampSitesConfig().MinHealthPercent / 100, GetZenCampSitesConfig().MaxHealthPercent / 100);

		Ammunition_Base ammo = Ammunition_Base.Cast(item);
		if (ammo) // Item is ammunition, treat it differently to itembase
		{
			ammo.ServerSetAmmoCount(quantity);
			ammo.SetHealth(item.GetMaxHealth() * health);
		}
		else // Item is ItemBase
		{
			ItemBase ib = ItemBase.Cast(item);
			if (ib)
			{
				ib.SetHealth(ib.GetMaxHealth() * health);

				if (ib.ConfigIsExisting("quantityBar") && ib.ConfigGetFloat("quantityBar") != 1)
				{
					ib.SetQuantity(quantity);
				}
				else
				if (ib.GetQuantity() != quantity)
				{
					EntityAI item2;
					ItemBase itemBase2;

					for (int i = 0; i < lootSpawn.MaxQuantity - 1; i++)
					{
						if (lootEAI && lootEAI.GetInventory() && Math.RandomFloat01() <= lootSpawn.SpawnChance)
						{
							itemBase2 = ItemBase.Cast(lootEAI.GetInventory().CreateInInventory(itemType));

							if (item2)
							{
								float health2 = Math.RandomFloatInclusive(GetZenCampSitesConfig().MinHealthPercent / 100, GetZenCampSitesConfig().MaxHealthPercent / 100);
								itemBase2 = ItemBase.Cast(item2);

								if (itemBase2)
								{
									itemBase2.SetHealth(itemBase2.GetMaxHealth() * health2);
								}
							}
						}
					}
				}
			}

			// Log spawns
			ZenModLogger.Log("Spawned: " + itemType + " x " + quantity, "ZenCampSites");
		}
	}

	// Start the fire
	void StartFire(FireplaceBase campFire)
	{
		campFire.StartFire(true);
	}

	// Spawn an object and apply appropriate flags/physics updates
	static Object SpawnCampObject(string type, vector position, vector orientation, float scale = 1.0)
	{
		EntityAI obj = EntityAI.Cast(GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS));
		if (!obj)
		{
			Error("Failed to create object " + type);
			return NULL;
		}

		obj.SetPosition(position);
		obj.SetOrientation(orientation);
		obj.SetScale(scale);
		obj.Update();
		obj.SetAffectPathgraph(true, false);
		obj.SetLifetimeMax(GetZenCampSitesConfig().SpawnSecs);
		obj.SetLifetime(GetZenCampSitesConfig().SpawnSecs);

		if (obj.CanAffectPathgraph())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
		}

		return obj;
	}

	void ZenCampSitesDebug(string msg)
	{
		if (GetZenCampSitesConfig().LogsOn)
			ZenModLogger.Log(msg, "ZenCampSites");

		if (GetZenCampSitesConfig().DebugOn)
			ZenFunctions.DebugMessage(msg);
	}
}
