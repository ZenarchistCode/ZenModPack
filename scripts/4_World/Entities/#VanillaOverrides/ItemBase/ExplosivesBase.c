modded class ExplosivesBase
{
	override void OnExplode()
	{
		super.OnExplode();

		#ifdef SERVER
		//! TREESPLOSIONS
		if (ZenModEnabled("ZenTreesplosions"))
			TreeKiller.KillTree(GetPosition(), GetType());
		#endif
	}
}