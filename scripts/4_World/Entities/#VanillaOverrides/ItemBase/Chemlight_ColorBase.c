modded class Chemlight_ColorBase
{
	override void OnWorkStop()
	{
		super.OnWorkStop();

		#ifndef SERVER
		CarScript carParent = CarScript.Cast(GetHierarchyRoot());
		if (carParent)
			carParent.DestroyZenPimpLight();
		#endif
	}

	override void OnWork(float consumed_energy)
	{
		super.OnWork(consumed_energy);

		#ifndef SERVER
		CarScript carParent = CarScript.Cast(GetHierarchyRoot());
		if (carParent)
			carParent.UpdateZenPimpLight();
		#endif
	}
}