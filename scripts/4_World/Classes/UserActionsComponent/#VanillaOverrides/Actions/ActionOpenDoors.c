modded class ActionOpenDoors
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);

		//! ZOMBIE DOORS
		if (!ZenModEnabled("ZenZombieDoors"))
			return;

		ZenZombieDoorsPlugin doorsPlugin = ZenZombieDoorsPlugin.Cast(GetPlugin(ZenZombieDoorsPlugin));
		if (!doorsPlugin)
			return;

		// Door is no longer shut, no need to keep it stored as a reference
		Building building;
		if (Class.CastTo(building, action_data.m_Target.GetObject()))
		{
			int doorIndex = building.GetDoorIndex(action_data.m_Target.GetComponentIndex());
			if (doorIndex != -1 && !building.GetType().Contains("Wreck") && !building.GetType().Contains("wreck"))
			{
				doorsPlugin.RemoveDoorAt(building.GetDoorSoundPos(doorIndex));
			}
		}
	}
};