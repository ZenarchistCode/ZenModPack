// Just a road flare that cannot be picked up or interacted with (used for giving away a combat logger's position)
class Zen_CombatLogFlare extends Roadflare
{
	// Called when object is initialized
	override void EEInit()
	{
		super.EEInit();

		// Set lifetime to combat logout timer seconds.
		SetLifetime(GetZenAntiCombatLogoutConfig().CombatLogoutSecs);

		// 1 second after creation, ignite flare
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(IgniteCombatFlare, 1000, false);
	}

	// Ignite flare and stand it up
	private void IgniteCombatFlare()
	{
		if (GetCompEM())
		{
			GetCompEM().SwitchOn();
		}

		SetModelState(RoadflareModelStates.UNCAPPED_UNIGNITED);

		HideSelection(STANDS_FOLDED);
		ShowSelection(STANDS_UNFOLDED);

		vector ori_rotate = GetOrientation();
		ori_rotate = ori_rotate + Vector(180, 32, 0);
		SetOrientation(ori_rotate);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
}