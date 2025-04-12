modded class Firewood
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		//! CRAFTING SOUNDS
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("Zen_Wood_loop_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
};
