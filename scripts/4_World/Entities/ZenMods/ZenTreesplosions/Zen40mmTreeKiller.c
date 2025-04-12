// This "object" is invisible and only exists to explode trees around a 40mm shell
class Zen40mmTreeKiller extends Inventory_Base
{
	void Zen40mmTreeKiller()
	{
		// Delete immediately to trigger tree killing
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DeleteSafe, 1, false);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (!GetGame().IsDedicatedServer())
			return;

		TreeKiller.KillTree(GetPosition(), GetType());
	}
};