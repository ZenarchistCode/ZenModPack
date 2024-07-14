// Copy vanilla underground stash so that we can have separate persistence on this stash type
class ZenGraves_UndergroundStash extends UndergroundStash 
{
	override void EEInit()
	{
		super.EEInit();
		GetGame().CreateObjectEx("ClutterCutterFireplace", GetPosition(), ECE_PLACE_ON_SURFACE);
	}

	override bool IsRefresherSignalingViable()
	{
		return false;
	}

	override void PlaceOnGround()
	{
		// Correct for surface placement (if buried below treehouse for example, PlaceOnGround places the stash on the treehouse floor)
		vector stashPos = GetPosition();
		stashPos[1] = GetGame().SurfaceY(stashPos[0], stashPos[2]) + 0.17;
		SetPosition(stashPos);

		// Align to ground 
		vector transform[4];
		GetTransform(transform);
		vector ground_position, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -100, ground_position, ground_dir, component, null, null, null, false, true);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		vector local_ori = GetDirection();
		transform[0] = surface_normal * local_ori;
		transform[1] = surface_normal;
		transform[2] = surface_normal * (local_ori * vector.Up);
		SetTransform(transform);
		Update();
	}
};
