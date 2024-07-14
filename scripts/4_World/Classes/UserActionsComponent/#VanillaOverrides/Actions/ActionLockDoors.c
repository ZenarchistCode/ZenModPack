modded class ActionLockDoors
{
	override void LockDoor(ActionTarget target)
	{
		super.LockDoor(target);

		//! ZOMBIE DOORS
		if (!ZenModEnabled("ZenZombieDoors"))
			return;

		// Door has been locked, store reference so zombies can hit it
		Building building;
		if (Class.CastTo(building, target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(target.GetComponentIndex());
			if (doorIndex != -1)
			{
				GetZenZombieDoorManager().AddDoor(building.GetDoorSoundPos(doorIndex));
			}
		}
	}
}