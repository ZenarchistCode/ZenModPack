modded class WoodenLog
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
		//! CRAFTING SOUNDS
		#ifndef SERVER
		EffectSound effect = SEffectManager.PlaySound("putDown_WoodLog_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		#endif
	}
};
