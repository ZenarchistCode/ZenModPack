class Zen_EmptyFood : ItemBase
{
	// This makes it easier to add "Empty" to all the food items without re-translating all of their names
	override string GetDisplayName()
	{
		return ConfigGetString("displayNameEmpty") + " " + ConfigGetString("displayName");
	}

	// Override description
	override string GetTooltip()
	{
		return "#STR_ZenLeftoversDescription";
	}
};

class Empty_Can_Opened : Bottle_Base
{
	// Don't allow empty cans to be placed in inventory if they have liquid in them
	override bool CanPutInCargo(EntityAI parent)
	{
		return super.CanPutInCargo(parent) && GetQuantity() <= 0.01;
	}

	override bool IsContainer()
	{
		return true;
	}

	override string GetPouringSoundset()
	{
		return "emptyVessle_Pot_SoundSet";
	}
	
		override string GetEmptyingLoopSoundsetHard()
	{
		return "pour_HardGround_Pot_SoundSet";
	}
	
	override string GetEmptyingLoopSoundsetSoft()
	{
		return "pour_SoftGround_Pot_SoundSet";
	}
	
	override string GetEmptyingLoopSoundsetWater()
	{
		return "pour_Water_Pot_SoundSet";
	}
	
	override string GetEmptyingEndSoundsetHard()
	{
		return "pour_End_HardGround_Pot_SoundSet";
	}
	
	override string GetEmptyingEndSoundsetSoft()
	{
		return "pour_End_SoftGround_Pot_SoundSet";
	}
	
	override string GetEmptyingEndSoundsetWater()
	{
		return "pour_End_Water_Pot_SoundSet";
	}
}

class Empty_SodaCan_ColorBase : Empty_Can_Opened {};

class Used_MedicalItem : ItemBase
{
	// This makes it easier to add "Used" to all the medical items without re-translating all of their names
	override string GetDisplayName()
	{
		return ConfigGetString("displayNameUsed") + " " + ConfigGetString("displayName");
	}

	// Override description
	override string GetTooltip()
	{
		return "#STR_ZenLeftoversMedDescription";
	}
}

class Empty_BoxCerealCrunchin : Zen_EmptyFood
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionGetJunkPaper);
	}
};

class Empty_Rice : Zen_EmptyFood
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionGetJunkPaper);
	}
};

class Empty_PowderedMilk : Zen_EmptyFood
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionGetJunkPaper);
	}
};

class Empty_Honey_NoLiquid : Zen_EmptyFood {};
class Empty_Marmalade_NoLiquid : Zen_EmptyFood {};
class Empty_Honey : Zen_EmptyFood {};
class Empty_Marmalade : Zen_EmptyFood {};