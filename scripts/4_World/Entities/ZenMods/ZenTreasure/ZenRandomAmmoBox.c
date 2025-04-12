class ZenRandomAmmoBox : Box_Base
{
	static const ref array<string> AMMO_BOXES = 
	{
		"AmmoBox_556x45_20Rnd",
		"AmmoBox_556x45Tracer_20Rnd",
		"AmmoBox_308Win_20Rnd",
		"AmmoBox_762x54_20Rnd",
		"AmmoBox_762x54Tracer_20Rnd",
		"AmmoBox_762x39_20Rnd",
		"AmmoBox_762x39Tracer_20Rnd",
		"AmmoBox_22_50Rnd",
		"AmmoBox_357_20Rnd",
		"AmmoBox_45ACP_25rnd",
		"AmmoBox_9x19_25rnd",
		"AmmoBox_380_35rnd",
		"AmmoBox_00buck_10rnd",
		"AmmoBox_12gaSlug_10Rnd",
		"AmmoBox_545x39_20Rnd",
		"AmmoBox_9x39_20Rnd",
		"AmmoBox_Expansion_46x30_25rnd",
		"AmmoBox_Expansion_338_15rnd",
		"Expansion_AmmoBox_8mm_15rnd"
	};

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		if (GetGame().IsDedicatedServer())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ShapeshifterMorph, 1000, false);
	}

	private void ShapeshifterMorph()
	{
		GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ReplaceItemWithNewLambda(this, AMMO_BOXES.GetRandomElement(), NULL));
	}
}

class ReplaceRandomAmmoLambda : ReplaceItemWithNewLambdaBase
{
	void ReplaceRandomAmmoLambda(EntityAI old_item, string new_item_type)
	{
		m_OldItem = old_item;
		m_NewItemType = new_item_type;
	}
};