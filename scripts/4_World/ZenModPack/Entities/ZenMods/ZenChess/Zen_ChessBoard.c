class Zen_ChessBoard extends ItemBase
{
	bool m_KillNextPiece = false;

	override bool IsRefresherSignalingViable()
	{
		return !IsRuined();
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (!GetGame().IsDedicatedServer())
			return;

		if (newLevel == GameConstants.STATE_RUINED)
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, vector.Zero);
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!item || !item.IsInherited(Zen_ChessPieceBase))
			return false;

		// Allow moving chess piece into cargo
		EntityAI parent = item.GetHierarchyParent();
		if (parent && parent.IsInherited(Zen_ChessBoard_Static))
			return true;

		// Don't allow adding more pieces than required to play
		return CheckPieceAllowed(item);
	}

	bool CheckPieceAllowed(EntityAI item)
	{
		return true;
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (!item || !GetGame().IsDedicatedServer())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckKilledPiece, 1, false, item.GetType(), slot_name);
	}

	void CheckKilledPiece(string killerPiece, string slot_name)
	{
		EntityAI killedPiece = FindAttachmentBySlotName(slot_name);

		if (killedPiece)
		{
			InventoryLocation loc = new InventoryLocation;
			killedPiece.GetInventory().GetCurrentInventoryLocation(loc);
			if (loc.GetType() == InventoryLocationType.CARGO)
			{
				m_KillNextPiece = false;
				return;
			}

			if (m_KillNextPiece)
			{
				EntityAI movedPiece = GetInventory().CreateEntityInCargo(killedPiece.GetType());
				if (movedPiece)
					movedPiece.SetHealth(killedPiece.GetHealth());

				killedPiece.DeleteSafe();
				SetSynchDirty();
				return;
			}

			if (killerPiece.Contains("Black") && killedPiece.GetType().Contains("Black"))
				return;

			if (killerPiece.Contains("White") && killedPiece.GetType().Contains("White"))
				return;

			m_KillNextPiece = true;
			return;
		}

		m_KillNextPiece = false;
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}
};
