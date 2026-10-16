// This class prints the item count of the specified items list, helpful for keeping track of raid materials economy etc
class ZenItemStatsLogger
{
	static bool ALREADY_PRINTED = false;
	static ref map<string, int> ITEM_STATS;

	static void PrintStats() // Called by MissionServer start after all items are loaded (~30 secs delay)
	{
		ALREADY_PRINTED = true;

		if (!GetZenUtilitiesConfig().ShouldLogItemCount || !ITEM_STATS)
			return;

		int statsCount = ITEM_STATS.Count();
		if (statsCount == 0)
		{
			delete ITEM_STATS;
			return;
		}
		
		ZenLogger.Log("ItemStats", "item_stats", "---");

		for (int i = 0; i < statsCount; i++)
		{
			int itemCount;
			string key = ITEM_STATS.GetKey(i);
			if (ITEM_STATS.Find(key, itemCount))
			{
				ZenLogger.Log("ItemStats", "item_stats", key + " count=" + itemCount);
			}
		}

		ZenLogger.Log("ItemStats", "item_stats", "\n");
		delete ITEM_STATS;
	}

	static void IncreaseItemCount(string type, int quantity)
	{
		if (ALREADY_PRINTED || !GetZenUtilitiesConfig().ShouldLogItemCount)
			return;

		if (!ITEM_STATS)
			ITEM_STATS = new map<string, int>;

		if (quantity <= 0)
			quantity = 1;

		int itemCount;
		if (!ITEM_STATS.Find(type, itemCount))
		{
			ITEM_STATS.Insert(type, quantity);
			return;
		}

		ITEM_STATS.Set(type, itemCount + quantity);
	}
}