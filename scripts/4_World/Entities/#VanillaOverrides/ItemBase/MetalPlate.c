modded class MetalPlate
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		//! CRAFTING SOUNDS
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("Zen_MetalPlateAttach_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
};
