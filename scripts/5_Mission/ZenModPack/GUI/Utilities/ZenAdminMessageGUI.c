class ZenAdminMessageGUI extends UIScriptedMenu
{
	// Title widgets
	private ref RichTextWidget m_ReplyText;
	private ref ButtonWidget m_BtnOK;
	private bool m_PopupUpdateMsg;

	// Init widgets
	override Widget Init()
	{
		// Load layout
		int screenWidth;
		int screenHeight;
		GetScreenSize( screenWidth, screenHeight );
		layoutRoot = g_Game.GetWorkspace().CreateWidgets("ZenModPack/data/gui/layouts/utilities/ZenAdminMessageGUI.layout");

		// Load required widgets
		m_ReplyText = RichTextWidget.Cast(layoutRoot.FindAnyWidget("reply_text"));
		m_BtnOK = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ok_button"));

		if (!m_ReplyText || !m_BtnOK)
		{
			ErrorMsg("Failed to load widgets for ZenAdminMessageGUI Contact Form!");
			return NULL;
		}

		return layoutRoot;
	}

	void SetAdminMessage(string msg, bool updateMsg, bool showOK)
	{
		m_ReplyText.SetText(msg);
		m_ReplyText.Update();
		m_PopupUpdateMsg = updateMsg;
		
		if (!showOK)
			m_BtnOK.Show(false);
	}
	
	override void Update(float timeslice)
	{
		super.Update(timeslice);

		ZenMissionFunctions.FreezePlayerControls();
	}
	
	override void OnShow()
	{
		super.OnShow();

		ZenMissionFunctions.FreezePlayerControls();
	}

	override void OnHide()
	{
		super.OnHide();

		ZenMissionFunctions.UnfreezePlayerControls();
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
			
		Close();

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
		g_Game.GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", msg, ""));
	}
}