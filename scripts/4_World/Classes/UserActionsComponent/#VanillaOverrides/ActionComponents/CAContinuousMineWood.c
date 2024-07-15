modded class CAContinuousMineWood : CAContinuousBase
{
	override void CreatePrimaryItems(ActionData action_data)
	{
		super.CreatePrimaryItems(action_data);
		SaveDeadTreeActionData(action_data);
	};

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