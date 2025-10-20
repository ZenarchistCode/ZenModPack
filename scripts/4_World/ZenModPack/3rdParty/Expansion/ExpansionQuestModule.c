#ifdef EXPANSIONMODQUESTS
modded class ExpansionQuestModule
{
	override protected void LoadQuestNPCData(array<string> questNPCFiles)
	{
		super.LoadQuestNPCData(questNPCFiles);

		for (int i = 0; i < m_QuestsNPCs.Count(); i++)
		{
			ExpansionQuestNPCData questNpc = m_QuestsNPCs.GetElement(i);
			if (questNpc)
				AddZenQuestNoBuildZone(questNpc.Position);
		}
	}

	override protected void LoadObjectivesData(array<string> objectiveFiles, string path)
	{
		super.LoadObjectivesData(objectiveFiles, path);

		int i;

		for (i = 0; i < m_TravelObjectivesConfigs.Count(); i++)
		{
			ExpansionQuestObjectiveTravelConfig travelObj = m_TravelObjectivesConfigs.GetElement(i);
			if (travelObj)
				AddZenQuestNoBuildZone(travelObj.Position);
		}

		for (i = 0; i < m_TargetObjectivesConfigs.Count(); i++)
		{
			ExpansionQuestObjectiveTargetConfig targetObj = m_TargetObjectivesConfigs.GetElement(i);
			if (targetObj)
				AddZenQuestNoBuildZone(targetObj.Position);
		}

		for (i = 0; i < m_AIEscortObjectivesConfigs.Count(); i++)
		{
			ExpansionQuestObjectiveAIEscortConfig escortObj = m_AIEscortObjectivesConfigs.GetElement(i);
			if (escortObj)
				AddZenQuestNoBuildZone(escortObj.Position);
		}
	}

	void AddZenQuestNoBuildZone(vector pos)
	{
		if (pos == vector.Zero)
			return;

		if (ZenModEnabled("ZenBasebuildingConfig"))
		{
			if (GetZenBasebuildingConfig().NoBuildZonesExpansionAutoCreate)
			{
				for (int i = 0; i < GetZenBasebuildingConfig().NoBuildZones.Count(); i++)
				{
					if (vector.Distance(GetZenBasebuildingConfig().NoBuildZones.Get(i).Position, pos) < 1)
					{
						return;
					}
				}

				ZMPrint("[ZenExpansionOverrides] Adding ZenBasebuildingConfig 50-meter no-build zone for quest area @ " + pos);
				GetZenBasebuildingConfig().NoBuildZones.Insert(new ZenNoBuildZone("Quest Area", pos, 50, true));
			}
		}

		//! TODO: Expansion territories no-build zones
	}
}
#endif