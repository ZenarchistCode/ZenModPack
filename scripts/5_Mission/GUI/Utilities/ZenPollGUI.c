class ZenPollGUI extends UIScriptedMenu
{
	//! Widgets
	protected ref array<ref CheckBoxWidget> m_Checkboxes;
	protected ref array<ref MultilineTextWidget> m_PollOptions;

	protected ref TextWidget m_TitleWidget;
	protected ref TextWidget m_SubtitleWidget;

	protected ref ButtonWidget m_CancelBtn;
	protected ref ButtonWidget m_SubmitBtn;

	//! Settings 
	protected bool m_OnlyOneOptionAllowed;

	// Init widgets
	override Widget Init()
	{
		// Load layout
		int screenWidth;
		int screenHeight;
		GetScreenSize( screenWidth, screenHeight );
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("ZenModPack/data/gui/layouts/utilities/ZenPollGUI.layout");

		// Load required widgets
		m_TitleWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("title_text"));
		m_SubtitleWidget = TextWidget.Cast(layoutRoot.FindAnyWidget("title_sub_text"));
		m_CancelBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("cancel_button"));
		m_SubmitBtn = ButtonWidget.Cast(layoutRoot.FindAnyWidget("submit_button"));

		m_Checkboxes = new ref array<ref CheckBoxWidget>;
		m_PollOptions = new ref array<ref MultilineTextWidget>;

		for (int i = 0; i < 6; i++)
		{
			m_Checkboxes.Insert(CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("CheckBoxWidget" + i)));
			m_PollOptions.Insert(MultilineTextWidget.Cast(layoutRoot.FindAnyWidget("MultilineTextWidget" + i)));
			m_PollOptions.Get(i).SetText("");
		}

		m_OnlyOneOptionAllowed = true;

		return layoutRoot;
	}

	void SetPollSettings(string title, string subtitle, bool onlyOneOption, array<string> options)
	{
		m_TitleWidget.SetText(title);
		m_SubtitleWidget.SetText(subtitle);
		m_OnlyOneOptionAllowed = onlyOneOption;

		for (int i = 0; i < ZenPollConfig.MAX_OPTIONS; i++)
		{
			if (options.Get(i) == "")
			{
				m_Checkboxes.Get(i).Show(false);
				m_PollOptions.Get(i).Show(false);
				m_PollOptions.Get(i).SetText("");
			}
			else 
			{
				m_Checkboxes.Get(i).Show(true);
				m_PollOptions.Get(i).Show(true);
				m_PollOptions.Get(i).SetText(options.Get(i));
				m_PollOptions.Get(i).Update();
			}
		}
	}

	// Handles clicks on button widgets
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w == m_SubmitBtn)
		{
			OnSubmitBtnClick();
			return true;
		}

		if (w == m_CancelBtn)
		{
			OnCancelBtnClick();
			return true;
		}

		for (int i = 0; i < m_Checkboxes.Count(); i++)
		{
			if (m_Checkboxes.Get(i) == w)
			{
				HandleCheckboxClick(i);
				return true;
			}
		}

		return true;
	}

	void HandleCheckboxClick(int index)
	{
		if (m_OnlyOneOptionAllowed)
		{
			for (int i = 0; i < m_Checkboxes.Count(); i++)
			{
				if (i != index)
					m_Checkboxes.Get(i).SetChecked(false);
			}
		}
	}

	array<bool> GetSelectedOptions()
	{
		array<bool> returner = new array<bool>;
		for (int i = 0; i < m_Checkboxes.Count(); i++)
		{
			returner.Insert(m_Checkboxes.Get(i).IsChecked());
		}

		return returner;
	}


	// Send button
	private void OnSubmitBtnClick()
	{
		// Inform server player has read the reply
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_SendZenPollChoice", new Param1<array<bool>>(GetSelectedOptions()), true, NULL);
		
		// Close window
		Close();
	}

	private void OnCancelBtnClick()
	{
		Close();
	}

	override void OnShow()
	{
		super.OnShow();

		SetControls(false);
	}

	override void OnHide()
	{
		super.OnHide();

		SetControls(true);
	}

	void SetControls(bool on)
	{
		if (!on)
		{
			GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
			GetGame().GetMission().GetHud().Show(false);
		}
		else 
		{
			GetGame().GetMission().PlayerControlEnable(true);
			GetGame().GetMission().GetHud().Show(true);
		}
	}
}