ref ZenTreasure_Triggers m_ZenTreasure_TriggersInstance;

static ZenTreasure_Triggers GetZenTreasure_Triggers()
{
	if (!m_ZenTreasure_TriggersInstance)
	{
		ZMPrint("[ZenTreasure_Triggers] Init");
		m_ZenTreasure_TriggersInstance = new ZenTreasure_Triggers;
	}

	return m_ZenTreasure_TriggersInstance;
}

class ZenTreasure_Triggers extends Managed
{
	static const float TRIGGER_RADIUS = 50;

	ref array<ZenTreasure_StashTrigger> m_Triggers = new array<ZenTreasure_StashTrigger>;

	void SpawnStartupTriggers()
	{
		// Loop through config to spawn triggers on server startup
		foreach (ZenTreasureTriggerConfig cfg : GetZenTreasureConfig_SpawnTriggers().TreasureTriggers)
		{
			SpawnTrigger(cfg);
		}
	}

	void SpawnTrigger(ZenTreasureTriggerConfig cfg, PlayerBase player = NULL)
	{
		// Check if one already exists for this cfg first
		for (int i = m_Triggers.Count() - 1; i >= 0; i--)
		{
			ZenTreasure_StashTrigger existingTrigger = m_Triggers.Get(i);
			if (vector.Distance(existingTrigger.GetConfig().Position, cfg.Position) < 1)
			{
				existingTrigger.SetConfig(cfg);
				ZMPrint("ZenTreasure_Triggers::SpawnTrigger() - Updated trigger @ " + cfg.Position);
				return;
			}
		}

		ZenTreasure_StashTrigger trigger = ZenTreasure_StashTrigger.Cast(GetGame().CreateObject("ZenTreasure_StashTrigger", cfg.Position));
		trigger.SetPosition(cfg.Position); // Ensure no place on surface etc happens

		if (!trigger)
		{
			ZMPrint("ZenTreasure_Triggers::SpawnTrigger() - Failed to spawn trigger!");
			return;
		}

		trigger.SetConfig(cfg);
		trigger.SetCollisionSphere(TRIGGER_RADIUS);
		m_Triggers.Insert(trigger);
		ZMPrint("ZenTreasure_Triggers::SpawnTrigger() - Spawned trigger @ " + cfg.Position);

		if (player && vector.Distance(trigger.GetPosition(), player.GetPosition()) <= TRIGGER_RADIUS)
			trigger.OnEnter(player);
	}

	void RemoveTrigger(ZenTreasure_StashTrigger trigger)
	{
		// Don't delete the trigger if other players are still attached to it
		if (trigger.GetConfig().TreasureHunters.Count() > 0)
			return;

		for (int i = m_Triggers.Count() - 1; i >= 0; i--)
		{
			ZenTreasure_StashTrigger existingTrigger = m_Triggers.Get(i);
			if (existingTrigger == trigger)
			{
				existingTrigger.DeleteSafe();
				m_Triggers.Remove(i);
			}
		}
	}
}

// This trigger spawns a buried stash when the player who activated the treasure hunt enters it
class ZenTreasure_StashTrigger extends ManTrigger
{
	private ref ZenTreasureTriggerConfig m_TreasureConfig;

	ZenTreasureTriggerConfig GetConfig()
	{
		return m_TreasureConfig;
	}

	void SetConfig(ZenTreasureTriggerConfig p_config)
	{
		m_TreasureConfig = p_config;
	}

	override void OnEnter(Object obj)
	{
		if (!GetGame().IsDedicatedServer())
			return;

		PlayerBase player = PlayerBase.Cast(obj);
		if (!player)
			return;

		int stashType = -1;
		for (int i = m_TreasureConfig.TreasureHunters.Count() - 1; i >= 0; i--)
		{
			ZenTreasureHunter hunter = m_TreasureConfig.TreasureHunters.Get(i);
			if (hunter.SteamID == player.GetCachedID())
			{
				stashType = hunter.StashType;
				m_TreasureConfig.TreasureHunters.Remove(i);
				GetZenTreasureConfig_SpawnTriggers().Save();
				break;
			}
		}

		if (stashType == -1)
			return;

		// Spawn treasure, delete self + delete config.
		SpawnTreasureLoot(player, stashType);
	}

	//! LOOT SPAWN CODE
	void SpawnTreasureLoot(notnull PlayerBase player, int stashType)
	{
		int i;
		int j;

		ZenTreasureStashType treasureConfig = GetZenTreasureConfig().TreasureTypes.Get(stashType);
		if (!treasureConfig)
		{
			Error("[ZenTreasure] Could not spawn treasure loot - config not found for m_StashType=" + stashType);
			return;
		}

		// Check for existing treasure and delete
		array<Object> objectsNearTreasure = new array<Object>;
		GetGame().GetObjectsAtPosition3D(m_TreasureConfig.Position, 1, objectsNearTreasure, NULL);
		for (i = 0; i < objectsNearTreasure.Count(); i++)
		{
			Object obj = objectsNearTreasure.Get(i);
			if (obj && obj.IsKindOf("ZenTreasure_UndergroundStash"))
				GetGame().ObjectDelete(obj);
		}

		// Spawn container
		EntityAI container = EntityAI.Cast(GetGame().CreateObject(treasureConfig.ContainerType, m_TreasureConfig.Position, ECE_PLACE_ON_SURFACE));
		if (!container)
		{
			Error("[ZenTreasure] Failed to spawn treasure loot container - " + treasureConfig.ContainerType + " @ " + m_TreasureConfig.Position);
			return;
		}

		// Clone loot array which auto-deletes after spawning everything (makes it easier to load generic loot from all possible loot configs)
		array<ref ZenTreasureLootSpawn> tempLootArray = new array<ref ZenTreasureLootSpawn>;

		// If loot type is 0 or 1 (generic) then fill Loot array with all possible loot 
		if (stashType == 0 || stashType == 1)
		{
			foreach (ZenTreasureStashType tempConfig : GetZenTreasureConfig().TreasureTypes)
			{
				for (i = 0; i < tempConfig.Loot.Count(); i++)
				{
					tempLootArray.Insert(tempConfig.Loot.Get(i));
				}
			}
		}
		else
		{
			foreach (ZenTreasureLootSpawn zloot : treasureConfig.Loot)
			{
				tempLootArray.Insert(zloot);
			}
		}

		// Spawn loot into container 
		int lootCount = 0;
		for (i = 0; i < tempLootArray.Count(); i++)
		{
			ZenTreasureLootSpawn lootSpawn = tempLootArray.Get(i);

			if (lootSpawn.ClassName.Contains("Predef_"))
			{
				foreach (ZenTreasurePredefinedType predefLoot : GetZenTreasureConfig().PredefinedTypes)
				{
					if (predefLoot.ConfigName == lootSpawn.ClassName)
					{
						lootSpawn = predefLoot.Item;
						break;
					}
				}
			}

			EntityAI itemSpawned = SpawnLootItem(container, lootSpawn);
			if (itemSpawned != NULL)
			{
				lootCount++;

				// Spawn attachments
				for (j = 0; j < lootSpawn.Attachments.Count(); j++)
				{
					lootCount++;
					if (SpawnLootItemAttachment(itemSpawned, lootSpawn.Attachments.Get(j)) == NULL)
						SpawnLootItemAttachment(container, lootSpawn.Attachments.Get(j)); // If couldn't spawn on gun, try spawning in container
				}
			}
		}

		// Spawn stash
		float heightOffset = 0.24;
		UndergroundStash stash = UndergroundStash.Cast(GetGame().CreateObject("ZenTreasure_UndergroundStash", m_TreasureConfig.Position, ECE_PLACE_ON_SURFACE));

		if (!stash)
		{
			Error("[ZenTreasure] Failed to spawn buried UndergroundStash @ " + m_TreasureConfig.Position);
			return;
		}

		stash.PlaceOnGround();

		// Correct for surface placement (if buried below treehouse for example, PlaceOnGround places the stash on the treehouse floor)
		vector stashPos = stash.GetPosition();
		stashPos[1] = GetGame().SurfaceY(stashPos[0], stashPos[2]) + heightOffset;
		stash.SetPosition(stashPos);

		// Align to ground 
		vector transform[4];
		stash.GetTransform(transform);
		vector ground_position, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -100, ground_position, ground_dir, component, null, null, null, false, true);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		vector local_ori = stash.GetDirection();
		transform[0] = surface_normal * local_ori;
		transform[1] = surface_normal;
		transform[2] = surface_normal * (local_ori * vector.Up);
		stash.SetTransform(transform);
		stash.Update();

		// Place container inside stash and set persistence
		stash.SetLifetimeMax(GetZenTreasureConfig().TreasurePersistenceSecs);
		stash.SetLifetime(GetZenTreasureConfig().TreasurePersistenceSecs);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(MoveToStash, 1000, false, player, container, stash);

		ZenModLogger.Log(player.GetCachedID() + " spawned treasure stash @ " + m_TreasureConfig.Position + " with config " + treasureConfig.ConfigName + " lootCount=" + lootCount, "ZenTreasure");
	}

	void MoveToStash(notnull PlayerBase player, notnull EntityAI container, notnull EntityAI stash)
	{
		player.ServerTakeEntityToTargetCargo(stash, container);

		// Finally, remove this trigger.
		GetZenTreasure_Triggers().RemoveTrigger(this);
	}

	EntityAI SpawnLootItem(EntityAI lootEAI, ZenTreasureLootSpawn lootSpawn)
	{
		// Check random chance
		if (!lootEAI || !lootSpawn || !lootEAI.GetInventory() || Math.RandomFloat01() > lootSpawn.SpawnChance)
			return NULL;

		// Create item
		string itemType = lootSpawn.ClassName;

		// Check if it's a list
		if (itemType.Contains("|"))
		{
			TStringArray itemList = new TStringArray;
			itemType.Split("|", itemList);
			itemType = itemList.GetRandomElement();
		}

		EntityAI item = lootEAI.GetInventory().CreateInInventory(itemType);
		if (!item)
			item = lootEAI.GetInventory().CreateAttachment(itemType);

		if (!item)
			return NULL;

		// If item was successfully created, set its health & quantity
		int quantity = Math.RandomIntInclusive(lootSpawn.MinQuantity, lootSpawn.MaxQuantity);
		float health = Math.RandomFloatInclusive(lootSpawn.MinHealth / 100, 1.0);

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
								float health2 = Math.RandomFloatInclusive(lootSpawn.MinHealth / 100, 1.0);
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
		}

		return item;
	}

	EntityAI SpawnLootItemAttachment(EntityAI lootEAI, string itemType)
	{
		if (!lootEAI || !lootEAI.GetInventory())
			return NULL;

		float chance = 1.0;
		float minHealth = 0.1;
		int quantity = 1;

		// Check if it's a list
		if (itemType.Contains("|"))
		{
			TStringArray parameters = new TStringArray;
			itemType.Split("|", parameters);
			foreach (string s : parameters)
			{
				s.ToLower();
				if (s.Contains("c="))
				{
					s.Replace("c=", "");
					chance = s.ToFloat();
				}
				else 
				if (s.Contains("h="))
				{
					s.Replace("h=", "");
					minHealth = s.ToFloat();
				}
			}
			itemType = parameters.Get(0);
		}

		if (Math.RandomFloat01() > chance)
			return NULL;

		EntityAI item = lootEAI.GetInventory().CreateInInventory(itemType);
		if (!item)
			item = lootEAI.GetInventory().CreateAttachment(itemType);

		if (!item)
			return NULL;

		// If item was successfully created, set its health & quantity
		float health = item.GetMaxHealth() * Math.RandomFloatInclusive(minHealth, 1.0);
		item.SetHealth(health);
		return item;
	}
};