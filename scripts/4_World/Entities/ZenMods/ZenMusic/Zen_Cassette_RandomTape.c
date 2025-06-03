class Zen_Cassette_RandomTape extends ItemBase
{
	static ref array<string> RANDOM_TAPES = new array<string>;

	override void EEInit()
	{
		super.EEInit();

		if (GetGame().IsDedicatedServer())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShapeshifterMorph, 1000, false);
	}

	private void ShapeshifterMorph()
	{
		GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ReplaceItemWithNewLambdaBase(this, RANDOM_TAPES.GetRandomElement()));
	}
}