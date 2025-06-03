// This plugin tracks items spawned by dynamic contaminated zones.
// If enabled in the config (ItemLifetime = -1), any leftover items
// will be set to ruined when the zone is deleted or the server shuts down.
class ZenContaminatedItemsPlugin extends PluginBase
{
	protected ref map<string, ref map<string, ItemBase>> m_ZenSpawnedItems;

	override void OnInit()
	{
		ZMPrint("[ZenContaminatedItemsPlugin] :: OnInit");

		m_ZenSpawnedItems = new map<string, ref map<string, ItemBase>>;
	}

	void AddTrackedItem(vector zonePosition, ItemBase item)
	{
		map<string, ItemBase> trackedItems;
		if (!m_ZenSpawnedItems.Find(zonePosition.ToString(false), trackedItems))
			trackedItems = new map<string, ItemBase>;

		trackedItems.Set(item.GetPosition().ToString(false), item);
		m_ZenSpawnedItems.Set(zonePosition.ToString(false), trackedItems);
	}

	override void OnDestroy()
	{
		ZMPrint("[ZenContaminatedItemsPlugin] OnDestroy :: Deleting items from " + m_ZenSpawnedItems.Count() + " zones");

		int deleted = 0;
		for (int i = 0; i < m_ZenSpawnedItems.Count(); i++)
		{
			map<string, ItemBase> trackedItems = m_ZenSpawnedItems.GetElement(i);
			for (int j = 0; j < trackedItems.Count(); j++)
			{
				if (trackedItems.GetElement(j))
				{
					// If item has moved, don't delete it (player has touched it)
					if (trackedItems.GetElement(j).GetPosition() != trackedItems.GetKey(j).ToVector())
						continue;

					trackedItems.GetElement(j).SetHealth(0);
					deleted++;
				}
			}
		}

		m_ZenSpawnedItems.Clear();
		ZMPrint("[ZenContaminatedItemsPlugin] OnDestroy :: Ruined " + deleted + " leftover items on server shutdown.");
	}

	void HandleZoneRemoval(vector zonePosition)
	{
		map<string, ItemBase> trackedItems;
		if (!m_ZenSpawnedItems.Find(zonePosition.ToString(false), trackedItems))
			return;

		int deleted = 0;
		for (int j = 0; j < trackedItems.Count(); j++)
		{
			if (trackedItems.GetElement(j))
			{
				// If item has moved, don't delete it (player has touched it)
				if (trackedItems.GetElement(j).GetPosition() != trackedItems.GetKey(j).ToVector())
					continue;

				trackedItems.GetElement(j).SetHealth(0);
				deleted++;
			}
		}

		ZMPrint("[ZenContaminatedItemsPlugin] HandleZoneRemoval :: Ruined " + deleted + " leftover items from expired zone.");
	}
}