class Zen_IceaxeDebug extends Iceaxe {};

class Land_ZenIceSheet extends BuildingSuper
{
	void Land_ZenIceSheet()
	{
		SetAllowDamage(false);
	}

	override bool IsBuilding()
	{
		return false;
	}
}

class Land_ZenIceSheet_4x4 extends BuildingSuper
{
	void Land_ZenIceSheet_4x4()
	{
		SetAllowDamage(false);
	}

	override bool IsBuilding()
	{
		return false;
	}
}

class Land_ZenIceSheet_4x4_Hole extends ItemBase
{
	void Land_ZenIceSheet_4x4_Hole()
	{
		SetAllowDamage(false);
		SetTakeable(false);
	}

	override void DeferredInit()
	{
		super.DeferredInit();
		//SetScale(GetScale() * 1.02); //! TODO: Temporary fix for P3D model problem. Blender texture is fucko'd due to hole in middle?

		if (GetGame().IsClient())
			return;

		// Delete any ice sheets that exist over this hole (allows for persistence on these objects)
		// when the server restarts it spawns in the ice sheets, once this object spawns in it deletes any sheet ontop of it.
		array<Object> objectsAtPosition = new array<Object>;
		GetGame().GetObjectsAtPosition3D(GetPosition(), 0.1, objectsAtPosition, null);
		Object obj;

		for (int o = 0; o < objectsAtPosition.Count(); o++)
		{
			obj = objectsAtPosition.Get(o);

			if (!obj)
				continue;

			if (obj.GetType().Contains("Land_ZenIceSheet") && obj != this)
				GetGame().ObjectDelete(obj);
		}

#ifdef WINTER // Custom define - delete any leftover ice holes between season changes on my server (HardcoreDayZ.com)
		DeleteSafe();
#endif
	}

	override bool DisableVicinityIcon()
	{
		return true;
	}

	override int GetHideIconMask()
	{
		return EInventoryIconVisibility.HIDE_VICINITY;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override EWaterSourceObjectType GetWaterSourceObjectType()
	{
		return EWaterSourceObjectType.THROUGH;
	}

	override void SetActions()
	{
		super.SetActions();

		RemoveAction(ActionDrink);

		AddAction(ActionWashHandsWell);
		AddAction(ActionDrinkThroughContinuous);
	}
}


/*
These ice sheets can be used to make certain lakes/ponds unfishable/uninteractible with water items like bottle etc.

But, you can use a pickaxe to "cut" a hole in them (swap the model for one with a fishing hole in it)

I use this function to spawn these on my server in Init.c - it takes 4 vector map points and fills in that area with ice sheets.
The / from izurvive is replaced with the height coordinate at the end.

eg:

SpawnIcePlanes("5543.75 / 1822.27", "5641.02 / 1822.27", "5546.09 / 1775.39", "5643.36 / 1775.00", 388.226);

static void SpawnIcePlanes(string pointAS, string pointBS, string pointCS, string pointDS, float height = 0.0, float gridSizeMeters = 8.0)
{
	if (height == 0.0)
		return;

	pointAS.Replace("/", "0");
	pointBS.Replace("/", "0");
	pointCS.Replace("/", "0");
	pointDS.Replace("/", "0");

	vector pointA = pointAS.ToVector();
	vector pointB = pointBS.ToVector();
	vector pointC = pointCS.ToVector();
	vector pointD = pointDS.ToVector();

	// Find the min/max bounds for X and Z from the 4 points
	float minX = Math.Min(pointA[0], Math.Min(pointB[0], Math.Min(pointC[0], pointD[0])));
	float maxX = Math.Max(pointA[0], Math.Max(pointB[0], Math.Max(pointC[0], pointD[0])));
	float minZ = Math.Min(pointA[2], Math.Min(pointB[2], Math.Min(pointC[2], pointD[2])));
	float maxZ = Math.Max(pointA[2], Math.Max(pointB[2], Math.Max(pointC[2], pointD[2])));

	// Calculate grid counts based on the bounding box
	int gridXCount = Math.Ceil((maxX - minX) / gridSizeMeters);
	int gridZCount = Math.Ceil((maxZ - minZ) / gridSizeMeters);

	int objectSpawnCount = 0;

	// Spawn the ice planes within the bounds
	for (int x = 0; x < gridXCount; x++)
	{
		for (int z = 0; z < gridZCount; z++)
		{
			// Calculate the position for each ice plane
			float spawnX = minX + (x * gridSizeMeters);
			float spawnZ = minZ + (z * gridSizeMeters);
			vector spawnPos = Vector(spawnX, height, spawnZ);

			// Spawn the ice plane
			SpawnObject("Land_ZenIceSheet_4x4", spawnPos, "0 0 0", false, false);
			objectSpawnCount++;
		}
	}

	Print("Spawned " + objectSpawnCount + " ice planes @ " + pointA);
}
*/