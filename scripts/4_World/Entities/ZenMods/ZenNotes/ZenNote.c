class ZenNote extends Paper
{
	// Store a reference to this note's data
	protected ref ZenNoteData m_ZenNoteData;

	// Get item description - if player has read the note, display some note text
	override string GetTooltip()
	{
		if (m_ZenNoteData && m_ZenNoteData.m_NoteText)
		{
			// Split all the lines in the note
			TStringArray lineArray = new TStringArray;
			m_ZenNoteData.m_NoteText.Split("\n", lineArray);

			// Trim the whitespace before/after the line
			string description = "";
			foreach (string s : lineArray)
			{
				description = description + s.Trim() + ". ";
			}

			// Delete any whitespace within the line
			description.Replace("  ", "");

			// If note is very long, shorten it
			if (description.Length() > 280)
				description = description.Substring(0, 280);

			// Display the note as a description tooltip
			return description;
		}

		return ConfigGetString("descriptionShort");
	}

	// Set note actions
	override void SetActions()
	{
		super.SetActions();

		// Remove actions that aren't related to notes (ie. attach to fireplace, take item by tapping F)
		RemoveAction(ActionTakeItem);
		RemoveAction(ActionAttach);

		// Add actions related to written notes
		AddAction(ActionZenReadNoteHands);
		AddAction(ActionZenReadNoteTarget);
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	// Receive note data from server
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		Param1<ref ZenNoteData> params = new Param1<ref ZenNoteData>(m_ZenNoteData);

		// Client-side receiver
		if (rpc_type == ZenRPCs.RECEIVE_WRITTEN_NOTE)
		{
			if (!ctx.Read(params) || !params.param1)
			{
				Error("ERROR! Failed to read ZenNote data ZENNOTERPCs.RECEIVE_WRITTEN_NOTE - tell Zenarchist he fucked up!");
				return;
			}

			m_ZenNoteData = params.param1;
			if (!GetGame().GetUIManager())
				return;

			// Show note GUI as read-only
			ZenNoteGUI gui = ZenNoteGUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.NOTE_GUI, GetGame().GetUIManager().GetMenu()));

			if (gui)
			{
				gui.SetPaper(this);
				gui.SetNoteData(m_ZenNoteData);
				gui.SetReadOnly(true);
			}

			// For compatibility with @ZenCraftingSounds
			if (GetGame().ConfigIsExisting("CfgSoundSets Zen_Paper_loop_SoundSet"))
			{
				EffectSound effect = SEffectManager.PlaySound("Zen_Paper_loop_SoundSet", GetPosition());
				effect.SetAutodestroy(true);
			}
		}
	}

	// Return this note's data
	ZenNoteData GetNoteData()
	{
		return m_ZenNoteData;
	}

	// Set this note's data
	void SetNoteData(ZenNoteData data)
	{
		m_ZenNoteData = data;
	}

	// Load note data
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		// Load the note's persistent data
		if (!ctx.Read(m_ZenNoteData))
		{
			return false;
		}

		return true;
	}

	// Save note data
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		// Save the note's persistent data
		ctx.Write(m_ZenNoteData);
	}
};