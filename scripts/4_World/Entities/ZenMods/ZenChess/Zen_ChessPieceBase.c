class Zen_ChessPieceBase extends ItemBase
{
	int m_LastKnownSlotID = -1;

	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

		#ifndef SERVER
		// Play sound on client
		EffectSound effect = SEffectManager.PlaySound("ZenChess_SoundSet", GetPosition());
		effect.SetAutodestroy(true);
		return;
		#endif

		m_LastKnownSlotID = slot_id;
	}
	
	override void OnWasDetached(EntityAI parent, int slot_id)
	{
		super.OnWasDetached(parent, slot_id);
		m_LastKnownSlotID = slot_id;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	// If chess piece is removed from static chessboard, delete it and re-spawn it back on the board
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);

		if (old_owner == new_owner || !GetGame().IsDedicatedServer())
			return;

		if (!old_owner || !old_owner.IsInherited(Zen_ChessBoard_Static))
			return;

		if (!new_owner || !new_owner.IsInherited(Zen_ChessBoard_Static))
		{
			// Delete moved piece
			DeleteSafe();

			// Respawn moved piece (first attempt to spawn it back at its previous slot, if that fails just spawn in cargo)
			EntityAI replacePiece;
			if (m_LastKnownSlotID != -1)
				replacePiece = old_owner.GetInventory().CreateAttachmentEx(this.GetType(), m_LastKnownSlotID);

			if (!replacePiece)
				replacePiece = old_owner.GetInventory().CreateEntityInCargo(this.GetType());

			SetSynchDirty();
		}
	}
};
