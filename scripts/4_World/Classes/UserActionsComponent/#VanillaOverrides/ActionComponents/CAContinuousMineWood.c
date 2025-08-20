modded class CAContinuousMineWood : CAContinuousBase
{
	override void CreatePrimaryItems(ActionData action_data)
	{
		if (!ZenModEnabled("ZenWoodHealth"))
		{
			super.CreatePrimaryItems(action_data);
		}
		else 
		{
			// Set wood health based on tool used to harvest it
			string material;
			int increment;
			for (int i = 0; i < m_MaterialAndQuantityMap.Count(); i++)
			{
				material = m_MaterialAndQuantityMap.GetKey(i);
			
				if (material != "")
				{
					increment = m_MaterialAndQuantityMap.GetElement(i);
				
					if ( !m_MinedItem[i] )
					{
						m_MinedItem[i] = ItemBase.Cast(GetGame().CreateObjectEx(material,action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
						m_MinedItem[i].SetQuantity(increment);

						if (action_data.m_MainItem)
						{
							m_MinedItem[i].SetHealth01("", "", action_data.m_MainItem.GetHealth01());
						}
					}
					else 
					if (m_MinedItem[i].HasQuantity())
					{
						if ( m_MinedItem[i].IsFullQuantity() )
						{
							int stack_max = m_MinedItem[i].GetQuantityMax();
						
							increment -= stack_max - m_MinedItem[i].GetQuantity();
							if (increment >= 1.0)
							{
								m_MinedItem[i] = ItemBase.Cast(GetGame().CreateObjectEx(material,action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
								m_MinedItem[i].SetQuantity(increment,false);

								if (action_data.m_MainItem)
								{
									m_MinedItem[i].SetHealth01("", "", action_data.m_MainItem.GetHealth01());
								}
							}
						}
						else
						{
							m_MinedItem[i].AddQuantity(increment,false);
						}
					}
					else
					{
						m_MinedItem[i] = ItemBase.Cast(GetGame().CreateObjectEx(material,action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
						if (action_data.m_MainItem)
						{
							m_MinedItem[i].SetHealth01("", "", action_data.m_MainItem.GetHealth01());
						}
					}
				}
			}
		}
		
		SaveDeadTreeActionData(action_data);
	}

	// Separate this from CreatePrimaryItems to allow other mods to override CreatePrimaryItems if needed and call this method
	void SaveDeadTreeActionData(ActionData action_data)
	{
		if (!ZenModEnabled("ZenPersistentTrees"))
			return;

		Object targetObject;
		if (!Class.CastTo(targetObject, action_data.m_Target.GetObject()))
			return;

		SaveDeadTreeObject(targetObject);
	}

	// Separate this from above method so ZenTreesplosions can more easily save dead trees from explosions
	static void SaveDeadTreeObject(Object targetObject)
	{
		// Detect hard tree being cut down
		if (GetZenTreesConfig().TreeHard && targetObject.IsInherited(TreeHard) && targetObject.IsCuttable())
			GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

		// Detect soft tree being cut down
		if (GetZenTreesConfig().TreeSoft && targetObject.IsInherited(TreeSoft) && targetObject.IsCuttable())
			GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

		// Detect hard bush being cut down
		if (GetZenTreesConfig().BushHard && targetObject.IsInherited(BushHard))
			GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());

		// Detect soft bush being cut down
		if (GetZenTreesConfig().BushSoft && targetObject.IsInherited(BushSoft))
			GetZenTrees_Save().SaveTreeState(targetObject.GetPosition(), targetObject.GetHealth());
	}
}