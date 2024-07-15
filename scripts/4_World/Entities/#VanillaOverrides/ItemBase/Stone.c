modded class Stone
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		//! CRAFTING SOUNDS
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("Zen_RocksAttach_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
	
	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);
		
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("Zen_RocksDetach_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
};
