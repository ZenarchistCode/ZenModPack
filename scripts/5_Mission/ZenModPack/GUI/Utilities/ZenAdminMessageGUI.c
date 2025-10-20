class ZenAdminMessageGUI extends UIScriptedMenu
{
	// Title widgets
	private ref MultilineTextWidget m_ReplyText;
	private ref ButtonWidget m_BtnOK;
	private bool m_PopupUpdateMsg;

	// Init widgets
	override Widget Init()
	{
		// Load layout
		int screenWidth;
		int screenHeight;
		GetScreenSize( screenWidth, screenHeight );
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("ZenModPack/data/gui/layouts/utilities/ZenAdminMessageGUI.layout");

		// Load required widgets
		m_ReplyText = MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("reply_text"));
		m_BtnOK = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_button"));

		if (!m_ReplyText || !m_BtnOK)
		{
			ErrorMsg("Failed to load widgets for ZenAdminMessageGUI Contact Form!");
			return NULL;
		}

		// Disable controls & hud
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DisableControls, 100, false);

		return layoutRoot;
	}

	void SetAdminMessage(string msg, bool updateMsg)
	{
		m_ReplyText.SetText(msg);
		m_ReplyText.Update();
		m_PopupUpdateMsg = updateMsg;
	}

	// Diable controls & hud
	void DisableControls()
	{
		ZenFunctions.SetPlayerControl(false, true);
	}

	// Handles clicks on button widgets
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w == m_BtnOK)
			return OnOkBtnClick();

		return true;
	}

	// Send button
	private bool OnOkBtnClick()
	{
		// Inform server player has read the reply
		if (m_PopupUpdateMsg)
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_SendZenPlayerUpdateMessageConfirmRead", new Param1< bool >(true), true, NULL);
		else
			GetRPCManager().SendRPC("ZenMod_RPC", "RPC_SendZenPlayerMessageConfirmRead", new Param1< bool >(true), true, NULL);
			
		// Close window
		UIManager uiManager = GetGame().GetUIManager();

		if (!uiManager)
			return true;

		UIScriptedMenu menu = uiManager.GetMenu();
		if (menu)
		{
			ZenAdminMessageGUI replyMenu = ZenAdminMessageGUI.Cast(menu);
			if (replyMenu)
			{
				uiManager.HideScriptedMenu(replyMenu);
				ZenFunctions.SetPlayerControl(true, true);
			}
		}

		return true;
	}

	// Display an error message
	private void ErrorMsg(string s)
	{
		ZenFunctions.DebugMessage("[ZenAdminMessageGUI] Error - " + s);
		Error("[ZenAdminMessageGUI] ERROR: " + s);
	}

	// Used only for debugging
	private void ZenInfo_DebugMsg(string msg)
	{
		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", msg, ""));
	}
}