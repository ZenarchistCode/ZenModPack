class ZenNoteGUI extends UIScriptedMenu
{
	// Layout constants
	static const string LAYOUT_FILE = "ZenModPack/data/gui/layouts/notes/NoteGUI.layout";

	// Font variables
	int m_FontIndex = 0;

	// Text widgets
	ref array<ref MultilineEditBoxWidget> m_TextBoxes = {};
	ref array<ref TextWidget> m_Dates = {};

	// Read-only widgets
	ref array<ref MultilineTextWidget> m_TextReadBoxes = {};

	// Button widgets
	ButtonWidget m_ExitBtn;
	ButtonWidget m_SelectFontBtn;

	// Damage layer image widget
	ImageWidget m_DamageLayer;

	// Pen & paper variables
	ItemBase m_Paper;
	bool m_ReadOnly;
	int m_PenColour;
	static string DATE_TEXT = "";
	static bool CAN_CHANGE_FONTS = false;

	// Not sure if this is necessary, but I always clean up after myself if you know what I mean
	void ~ZenNoteGUI()
	{
		// Cleanup - unlink widgets
		m_ExitBtn.Unlink();
		m_SelectFontBtn.Unlink();

		// Unlink arrays
		foreach (MultilineEditBoxWidget box1 : m_TextBoxes)
		{
			box1.Unlink();
		}

		foreach(MultilineTextWidget box2 : m_TextReadBoxes)
		{
			box2.Unlink();
		}

		foreach (TextWidget date : m_Dates)
		{
			date.Unlink();
		}
	}

	// Start blur
	override void OnShow()
	{
		super.OnShow();

		GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
		GetGame().GetMission().GetHud().Show(false);
		GetUApi().GetInputByName("UAPersonView").Lock();
		PPEffects.SetBlurMenu(0.5);
	}

	// Remove blur 
	override void OnHide()
	{
		super.OnHide();

		GetGame().GetMission().PlayerControlEnable(true);
		GetGame().GetMission().GetHud().Show(true);
		GetUApi().GetInputByName("UAPersonView").Unlock();
		PPEffects.SetBlurMenu(0);
	}

	// For other mods to easily override this
	string GetLayoutFile()
	{
		return LAYOUT_FILE;
	}

	// Init widgets
	override Widget Init()
	{
		// Load layout
		layoutRoot = GetGame().GetWorkspace().CreateWidgets(GetLayoutFile());

		// Load text widget arrays
		for (int i = 0; i < 100; i++)
		{
			MultilineEditBoxWidget box = MultilineEditBoxWidget.Cast(layoutRoot.FindAnyWidget("NoteText" + i));
			MultilineTextWidget readBox = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("NoteReadText" + i));
			TextWidget date = TextWidget.Cast(layoutRoot.FindAnyWidget("DateText" + i));

			if (box && date && readBox)
			{
				m_TextReadBoxes.Insert(readBox);
				m_TextBoxes.Insert(box);
				m_Dates.Insert(date);
				date.SetText(DATE_TEXT);
			}
			else
			{
				break; // We've loaded all textboxes
			}
		}

		// Load required widgets
		m_ExitBtn		= ButtonWidget.Cast(layoutRoot.FindAnyWidget("CloseBtn"));
		m_SelectFontBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("FontBtn"));
		m_DamageLayer	= ImageWidget.Cast(layoutRoot.FindAnyWidget("DamageLayer"));
		
		// Set select style language
		ButtonWidget fontBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("FontBtn"));
		if (fontBtn)
		{
			fontBtn.SetText("#STR_ZenNoteStyle");
		}

		if (!m_ExitBtn || !m_SelectFontBtn)
		{
			ErrorMsg("Failed to load widgets for ZenNotes!");
			return NULL;
		}

		// Hide font button
		m_SelectFontBtn.Enable(false);
		m_SelectFontBtn.Show(false);

		return layoutRoot;
	}

	// Flag as a keyboard input menu
	override bool UseKeyboard()
	{
		return true;
	}

	// Set the paper reference object
	void SetPaper(ItemBase item)
	{
		m_Paper = item;

		int dmgState = m_Paper.GetHealthLevel("");

		if (dmgState != GameConstants.STATE_PRISTINE)
			m_DamageLayer.Show(true);

		if (dmgState == GameConstants.STATE_WORN)
			m_DamageLayer.SetAlpha(0.3);
		else if (dmgState == GameConstants.STATE_DAMAGED)
			m_DamageLayer.SetAlpha(0.6);
		else if (dmgState <= GameConstants.STATE_BADLY_DAMAGED)
			m_DamageLayer.SetAlpha(0.9);
	}

	// READ-ONLY NOTE DATA SENT - we're reading a note that's already written and saved.
	void SetNoteData(ZenNoteData data)
	{
		// Set font index first
		SetFontIndex(data.m_FontIndex);

		// Get text boxes
		MultilineTextWidget read_box = m_TextReadBoxes.Get(m_FontIndex);
		TextWidget date = m_Dates.Get(m_FontIndex);

		if (!read_box || !date)
		{
			OnExitBtnClick();
			return;
		}

		MultilineTextWidget temp_readBox;
		MultilineEditBoxWidget temp_box;
		TextWidget temp_date;

		// Hide all editboxes
		for (int i = 0; i < m_TextBoxes.Count(); i++)
		{
			temp_readBox = m_TextReadBoxes.Get(i);
			temp_box = m_TextBoxes.Get(i);
			temp_date = m_Dates.Get(i);
			temp_readBox.Enable(false);
			temp_readBox.Show(false);
			temp_box.Enable(false);
			temp_box.Show(false);
			temp_date.Enable(false);
			temp_date.Show(false);
		}

		read_box.Show(true);
		read_box.SetText(data.m_NoteText);
		date.Show(true);
		date.SetText(data.m_DateText);

		// Hide font selection
		m_SelectFontBtn.Enable(false);
		m_SelectFontBtn.Show(false);

		// Set text colour
		SetPenColour(data.m_ARGBColour);
	}

	// Set note date (formatted by server)
	void SetDate()
	{
		TextWidget dateWidget = m_Dates.Get(m_FontIndex);
		if (dateWidget)
		{
			dateWidget.SetText(DATE_TEXT);
		}
	}

	// Set whether or not font changing is enabled by server
	void SetFontEnabled()
	{
		m_SelectFontBtn.Enable(CAN_CHANGE_FONTS);
		m_SelectFontBtn.Show(CAN_CHANGE_FONTS);
	}

	// Set font index
	void SetFontIndex(int index)
	{
		m_FontIndex = index;
	}

	// Set pen text colour
	void SetPenColour(int colour)
	{
		// Set all edit box colours
		foreach (MultilineEditBoxWidget box : m_TextBoxes)
		{
			box.SetColor(colour);
		}

		// Set all read box colours
		foreach(MultilineTextWidget readBox : m_TextReadBoxes)
		{
			readBox.SetColor(colour);
		}

		// Set all date colours
		foreach (TextWidget date : m_Dates)
		{
			date.SetColor(colour);
		}

		m_ExitBtn.SetTextColor(colour);
		m_SelectFontBtn.SetTextColor(colour);
		m_PenColour = colour;
	}

	// Set read only
	void SetReadOnly(bool readOnly)
	{
		m_ReadOnly = readOnly;

		if (readOnly)
		{
			// Hide all edit boxes
			foreach(MultilineEditBoxWidget box : m_TextBoxes)
			{
				box.Enable(false);
				box.Show(false);
			}
		}

		// If note is NOT read-only, set last-used font
		if (!readOnly)
			HandleFontClick(-1, GetZenNotesClientConfig().LastUsedFontStyle);
	}

	// Handles clicks on button widgets
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w == m_ExitBtn)
		{
			return OnExitBtnClick();
		}

		if (w == m_SelectFontBtn && CAN_CHANGE_FONTS)
		{
			return HandleFontClick(button);
		}

		return true;
	}

	// Suppress view key
	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (GetUApi())
			GetUApi().GetInputByName("UAPersonView").Supress();
	};

	// Handle font click
	bool HandleFontClick(int btn, int savedIndex = -1)
	{
		// Hide all read-only boxes
		MultilineTextWidget read_box;
		for (int i = 0; i < m_TextBoxes.Count(); i++)
		{
			read_box = m_TextReadBoxes.Get(i);
			read_box.Show(false);
			read_box.Enable(false);
		}

		// Prepare txt string for swapping editbox contents
		string msgTxt;

		// Get old edit box
		MultilineEditBoxWidget edit_box_old = m_TextBoxes.Get(m_FontIndex);
		TextWidget date_old = m_Dates.Get(m_FontIndex);

		// Check button - left click = next font, right click = last font
		if (btn == 0) // Left click
			btn = 1;
		else // Right click
			btn = -1;

		if (edit_box_old)
		{
			// Check for saved font index override
			if (savedIndex != -1)
				m_FontIndex = savedIndex;
			else
				m_FontIndex = m_FontIndex + btn;

			// Validate lower font index range
			if (m_FontIndex <= -1)
				m_FontIndex = m_TextBoxes.Count() - 1;

			// Validate upper font index range
			if (m_FontIndex >= m_TextBoxes.Count())
				m_FontIndex = 0;

			MultilineEditBoxWidget edit_box_new = m_TextBoxes.Get(m_FontIndex);
			TextWidget date_new = m_Dates.Get(m_FontIndex);

			// Update edit boxes
			if (edit_box_new && date_new)
			{
				edit_box_old.GetText(msgTxt);
				edit_box_new.SetText(msgTxt);
				date_new.SetText(DATE_TEXT);

				edit_box_old.Show(false);
				edit_box_old.Enable(false);
				date_old.Show(false);

				edit_box_new.Show(true);
				edit_box_new.Enable(true);
				date_new.Show(true);
			}
			else
			{
				ErrorMsg("NEW EDIT BOX NOT FOUND!");
			}
		}
		else
		{
			ErrorMsg("OLD EDIT BOX NOT FOUND!");
		}

		return true;
	}

	// Close GUI
	bool OnExitBtnClick()
	{
		string msgTxt;

		MultilineEditBoxWidget box = m_TextBoxes.Get(m_FontIndex);

		if (box)
		{
			box.GetText(msgTxt);
		}
		else
		{
			ErrorMsg("EDIT BOX NOT FOUND FOR FONT INDEX " + m_FontIndex);
			return false;
		}

		// Send written note data to server to save + sync to other clients
		if (m_Paper && !m_ReadOnly)
		{
			ZenNoteData note_data = new ZenNoteData;
			note_data.m_FontIndex = m_FontIndex;
			note_data.m_NoteText = msgTxt;
			note_data.m_DateText = DATE_TEXT;
			note_data.m_ARGBColour = m_PenColour;

			Param1<ref ZenNoteData> params = new Param1<ref ZenNoteData>(note_data);

			if (msgTxt != "" && GetGame().GetPlayer())
			{
				m_Paper.RPCSingleParam(ZenRPCs.SEND_WRITTEN_NOTE, params, true, NULL);
			}

			// Save last used font client-side
			GetZenNotesClientConfig().LastUsedFontStyle = m_FontIndex;
			GetZenNotesClientConfig().Save();
		}

		UIManager uiManager = GetGame().GetUIManager();

		if (!uiManager)
			return true;

		// Exit menu and restore player controls
		UIScriptedMenu menu = uiManager.GetMenu();
		if (menu)
		{
			ZenNoteGUI noteMenu = ZenNoteGUI.Cast(menu);
			if (noteMenu)
			{
				uiManager.HideScriptedMenu(noteMenu);

				Mission mission = GetGame().GetMission();
				if (mission)
				{
					mission.PlayerControlEnable(true);
					mission.GetHud().Show(true);
					GetUApi().GetInputByName("UAPersonView").Unlock();
				}
			}
		}

		return true;
	}

	// Display an error message
	private void ErrorMsg(string s)
	{
		ZMPrint("[ZenNotesGUI] Error - " + s);
		ZenNote_DebugMsg("[ZenNotesGUI] Error - " + s + ". Please tell the server admin to contact Zenarchist");
	}

	// Used only for debugging
	private void ZenNote_DebugMsg(string msg)
	{
		ZMPrint(msg);
		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", msg, ""));
	}
}