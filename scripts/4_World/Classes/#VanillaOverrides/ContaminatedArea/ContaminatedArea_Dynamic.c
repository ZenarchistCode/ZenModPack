modded class ContaminatedArea_Dynamic
{
	// Spawn like vanilla, but get items from custom config + track them for deletion if required.
	override void SpawnItems()
	{
		if (!ZenModEnabled("ZenDynamicZoneLoot"))
		{
			super.SpawnItems();
			return;
		}

		vector pos = GetPosition();
		ZenContaminatedItemsPlugin zenPlugin = ZenContaminatedItemsPlugin.Cast(GetPlugin(ZenContaminatedItemsPlugin));

		foreach (ZenContaminatedZoneLootSpawn lootCfg : GetZenContaminatedZoneLootConfig().LootSpawns)
		{
			int itemCount = Math.RandomIntInclusive(lootCfg.ItemCountMinimum, lootCfg.ItemCountMaximum);
			int lifetime = GetZenContaminatedZoneLootConfig().ItemLifetime;

			if (lifetime == 0)
				lifetime = GetLifetime();

			for (int i = 0; i < itemCount; i++)
			{
				if (Math.RandomFloat01() > lootCfg.SpawnChance)
					continue;

				vector randomDir2d = vector.RandomDir2D();
				float randomDist = Math.RandomFloatInclusive(lootCfg.SpawnDistanceMinimum, lootCfg.SpawnDistanceMaximum);
				vector spawnPos = pos + (randomDir2d * randomDist);
				InventoryLocation il = new InventoryLocation;
				vector mat[4];
				Math3D.MatrixIdentity4(mat);
				mat[3] = spawnPos;
				il.SetGround(NULL, mat);
				ItemBase item = ItemBase.Cast(GetGame().CreateObjectEx(lootCfg.ItemType, il.GetPos(), ECE_PLACE_ON_SURFACE));

				if (item)
				{
					if (lifetime <= -1 && zenPlugin)
					{
						item.SetLifetime(GetLifetime());
						zenPlugin.AddTrackedItem(pos, item);
					}
					else
					{
						item.SetLifetime(lifetime);
					}

					float qty = Math.RandomFloatInclusive(lootCfg.ItemQuantityMinimum, lootCfg.ItemQuantityMaximum);
						
					if (!ZenFunctions.SetQuantity(item, qty))
						qty = item.GetQuantity();

					if (qty == 0) // Non-stackable
						qty = 1;

					ZMPrint("[ContaminatedZoneLootConfig] Spawned " + qty + "x " + lootCfg.ItemType + " @ " + il.GetPos() + " with lifetime " + item.GetLifetime());
				}
				else
				{
					Error("[ContaminatedZoneLootConfig] Failed to spawn " + lootCfg.ItemType);
				}
			}
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetZenContaminatedZoneLootConfig().ItemLifetime > -1)
			return;

		ZenContaminatedItemsPlugin zenPlugin = ZenContaminatedItemsPlugin.Cast(GetPlugin(ZenContaminatedItemsPlugin));
		if (zenPlugin)
			zenPlugin.HandleZoneRemoval(GetPosition());
	}
}