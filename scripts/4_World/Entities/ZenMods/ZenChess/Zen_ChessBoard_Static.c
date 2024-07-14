class Zen_ChessBoard_Static extends Zen_ChessBoard
{
	override void EEInit()
	{
		super.EEInit();

		SetAllowDamage(false);

		// Spawn pieces if required
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckAllPieces, 1000);
		GetGame().CreateObjectEx("ClutterCutter2x2", GetPosition(), ECE_PLACE_ON_SURFACE);
	}

	void CheckAllPieces()
	{
		int i;

		int whitePawns = GetChessPieceCount("Zen_ChessWhitePawn");
		for (i = 0; i < 8 - whitePawns; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessWhitePawn");
		}

		int blackPawns = GetChessPieceCount("Zen_ChessBlackPawn");
		for (i = 0; i < 8 - blackPawns; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessBlackPawn");
		}

		int whiteBishops = GetChessPieceCount("Zen_ChessWhiteBishop");
		for (i = 0; i < 2 - whiteBishops; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessWhiteBishop");
		}

		int blackBishops = GetChessPieceCount("Zen_ChessBlackBishop");
		for (i = 0; i < 2 - blackBishops; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessBlackBishop");
		}

		int whiteRooks = GetChessPieceCount("Zen_ChessWhiteRook");
		for (i = 0; i < 2 - whiteRooks; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessWhiteRook");
		}

		int blackRooks = GetChessPieceCount("Zen_ChessBlackRook");
		for (i = 0; i < 2 - blackRooks; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessBlackRook");
		}

		int whiteKnights = GetChessPieceCount("Zen_ChessWhiteKnight");
		for (i = 0; i < 2 - whiteKnights; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessWhiteKnight");
		}

		int blackKnights = GetChessPieceCount("Zen_ChessBlackKnight");
		for (i = 0; i < 2 - blackKnights; i++)
		{
			GetInventory().CreateInInventory("Zen_ChessBlackKnight");
		}

		int whiteKing = GetChessPieceCount("Zen_ChessWhiteKing");
		if (whiteKing == 0)
			GetInventory().CreateInInventory("Zen_ChessWhiteKing");

		int blackKing = GetChessPieceCount("Zen_ChessBlackKing");
		if (blackKing == 0)
			GetInventory().CreateInInventory("Zen_ChessBlackKing");

		int whiteQueen = GetChessPieceCount("Zen_ChessWhiteQueen");
		if (whiteQueen == 0)
			GetInventory().CreateInInventory("Zen_ChessWhiteQueen");

		int blackQueen = GetChessPieceCount("Zen_ChessBlackQueen");
		if (blackQueen == 0)
			GetInventory().CreateInInventory("Zen_ChessBlackQueen");
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CheckPieceAllowed(EntityAI item)
	{
		bool allow = false;

		switch (item.GetType())
		{
			case "Zen_ChessWhitePawn":
				allow = GetChessPieceCount("Zen_ChessWhitePawn") < 8;
				break;
			case "Zen_ChessBlackPawn":
				allow = GetChessPieceCount("Zen_ChessBlackPawn") < 8;
				break;
			case "Zen_ChessWhiteBishop":
				allow = GetChessPieceCount("Zen_ChessWhiteBishop") < 2;
				break;
			case "Zen_ChessBlackBishop":
				allow = GetChessPieceCount("Zen_ChessBlackBishop") < 2;
				break;
			case "Zen_ChessWhiteRook":
				allow = GetChessPieceCount("Zen_ChessWhiteRook") < 2;
				break;
			case "Zen_ChessBlackRook":
				allow = GetChessPieceCount("Zen_ChessBlackRook") < 2;
				break;
			case "Zen_ChessWhiteKnight":
				allow = GetChessPieceCount("Zen_ChessWhiteKnight") < 2;
				break;
			case "Zen_ChessBlackKnight":
				allow = GetChessPieceCount("Zen_ChessBlackKnight") < 2;
				break;
			case "Zen_ChessWhiteKing":
				allow = GetChessPieceCount("Zen_ChessBlackBishop") < 1;
				break;
			case "Zen_ChessBlackKing":
				allow = GetChessPieceCount("Zen_ChessBlackKing") < 1;
				break;
			case "Zen_ChessWhiteQueen":
				allow = GetChessPieceCount("Zen_ChessWhiteQueen") < 1;
				break;
			case "Zen_ChessBlackQueen":
				allow = GetChessPieceCount("Zen_ChessBlackQueen") < 1;
				break;
		}

		return allow;
	}

	int GetChessPieceCount(string item_type)
	{
		int count = 0;
		array<EntityAI> itemsArray = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
            	    
        for (int i = 0; i < itemsArray.Count(); i++)
		{
			if (itemsArray.Get(i).GetType() == item_type)
			{
				count++;
			}
		}

		return count;
	};
};
