// This "object" is invisible and only exists to spawn a sound effect inside player network bubble. It is instantly deleted after spawning.
class ZenZombieDoorBangerBang extends Inventory_Base
{
	// Delete object immediately
	override void EEInit()
	{
		super.EEInit();

		// Delete immediately to trigger sound (DeleteSafe sync's deletion between server/client and this is called on both)
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteSafe, 1, false);
	}

	// Play sound upon deletion
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetGame().IsClient())
		{
			vector pos = GetPosition();
			pos[0] = pos[0] + Math.RandomFloatInclusive(-0.1, 0.1);
			pos[1] = pos[1] + Math.RandomFloatInclusive(-0.1, 0.1);
			pos[2] = pos[2] + Math.RandomFloatInclusive(-0.1, 0.1);
			SEffectManager.PlaySound("Zombie_Hit_Door_SoundSet", pos);
		}
	}
};