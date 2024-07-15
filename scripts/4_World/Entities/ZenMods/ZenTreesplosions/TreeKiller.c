class TreeKiller
{
	static void KillTree(vector pos, string ammo)
	{
		// Get explosion type config
		TreesplosionType tt = GetTreesplosionsConfig().GetTreesplosionType(ammo);

		// If tree explosion type doesn't exist, stop here
		if (!pos || !tt || !tt.Radius)
			return;

		// Get objects at position
		array<Object> objectsNearBomb = new array<Object>;
		GetGame().GetObjectsAtPosition(pos, tt.Radius, objectsNearBomb, null);

		for (int x = 0; x < objectsNearBomb.Count(); x++)
		{
			Object obj = objectsNearBomb[x];
			if ((obj.IsInherited(TreeSoft) && tt.CutTreeSoft) || (obj.IsInherited(BushHard) && tt.CutBushHard) || (obj.IsInherited(BushSoft) && tt.CutBushSoft) || (obj.IsInherited(TreeHard) && tt.CutTreeHard))
			{
				// Kill tree
				obj.SetHealth(0);
				dBodyDestroy(obj);

				// For ZenPersistentTrees compatibility
				CAContinuousMineWood.SaveDeadTreeObject(obj);
			}
		};
	}
};
