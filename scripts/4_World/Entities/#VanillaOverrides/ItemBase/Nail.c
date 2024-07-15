modded class Nail
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		//! CRAFTING SOUNDS
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("Zen_Nails_loop_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
};
