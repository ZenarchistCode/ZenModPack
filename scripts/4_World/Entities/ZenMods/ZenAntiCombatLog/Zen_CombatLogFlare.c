// Just a road flare that cannot be picked up or interacted with (used for giving away a combat logger's position)
class Zen_CombatLogFlare extends Roadflare
{
	// Called when object is initialized
	override void EEInit()
	{
		super.EEInit();

		// If the server just restarted, delete any spawned flares as they're now useless since player is gone
		#ifdef SERVER
		if (SERVER_RESTART)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteFlare, 2000, false);
			return;
		}
		#endif

		// 1 second after creation, ignite flare
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(IgniteCombatFlare, 1000, false);
	}

	// Ignite flare and stand it up
	private void IgniteCombatFlare()
	{
		if (GetCompEM())
			GetCompEM().SwitchOn();

		SetModelState(RoadflareModelStates.UNCAPPED_UNIGNITED);

		HideSelection(STANDS_FOLDED);
		ShowSelection(STANDS_UNFOLDED);

		vector ori_rotate = GetOrientation();
		ori_rotate = ori_rotate + Vector(180, 32, 0);
		SetOrientation(ori_rotate);
	}

	// Don't allow placing in cargo
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	// Don't allow taking it
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	// Delete the flare on server restarts
	private void DeleteFlare()
	{
		DeleteSafe();
	}

	// Used to delete any combat log flares on a server restart. MissionServer sets this flag to false shortly after server starts up.
	static bool SERVER_RESTART = true;
	static void SetServerRestarted()
	{
		SERVER_RESTART = false;
	}
};