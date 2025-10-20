modded class CAContinuousMineWood
{
    bool ZenIsWoodHealthEnabled()
    {
        return ZenModEnabled("ZenWoodHealth");
    }

    // After-creation hook other perks/mods can override
    protected void ZenAfterCreatePrimaryItems(ActionData action_data)
    {
    }

    override void CreatePrimaryItems(ActionData action_data)
    {
        // Snapshot pointers BEFORE vanilla runs
		bool woodHealthEnabled = ZenIsWoodHealthEnabled();
        ItemBase prevMined[MINEDITEM_MAX];
        int maxIdx;

		if (woodHealthEnabled)
		{
			if (m_MaterialAndQuantityMap)
			{
				maxIdx = m_MaterialAndQuantityMap.Count();
				if (maxIdx > MINEDITEM_MAX)
				{
					maxIdx = MINEDITEM_MAX;
				}
			}
			else
			{
				maxIdx = 0;
			}

			int i;
			for (i = 0; i < maxIdx; i++)
			{
				prevMined[i] = m_MinedItem[i];
			}
		}

        // Let vanilla spawn/stack everything
        super.CreatePrimaryItems(action_data);

        // Apply tool health ONLY to items that are new this tick
        if (!GetGame().IsDedicatedServer())
			return;

		if (woodHealthEnabled && action_data && action_data.m_MainItem)
		{
			float toolHealth01 = action_data.m_MainItem.GetHealth01();

			for (i = 0; i < maxIdx; i++)
			{
				ItemBase nowItem = m_MinedItem[i];
				ItemBase wasItem = prevMined[i];

				// New reference appeared or pointer changed -> set health
				if (nowItem && nowItem != wasItem)
				{
					nowItem.SetHealth01("", "", toolHealth01);
				}
			}
		}

		ZenAfterCreatePrimaryItems(action_data);
		SaveDeadTreeActionData(action_data);
    }

    void SaveDeadTreeActionData(ActionData action_data)
    {
        if (!ZenModEnabled("ZenPersistentTrees"))
            return;

        Object targetObject;
        bool ok = Class.CastTo(targetObject, action_data.m_Target.GetObject());
        if (!ok)
            return;

        SaveDeadTreeObject(targetObject);
    }

    static void SaveDeadTreeObject(Object targetObject)
    {
        if (GetZenTreesConfig().TreeHard && targetObject.IsInherited(TreeHard) && targetObject.IsCuttable())
            GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

        if (GetZenTreesConfig().TreeSoft && targetObject.IsInherited(TreeSoft) && targetObject.IsCuttable())
            GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

        if (GetZenTreesConfig().BushHard && targetObject.IsInherited(BushHard))
            GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

        if (GetZenTreesConfig().BushSoft && targetObject.IsInherited(BushSoft))
            GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());
    }
}