class ZenSplitItemUI extends ZenSplitItemUIBase
{
	static const string		LAYOUT_FILE = "ZenModPack/data/gui/layouts/splitui/split_item_ui.layout";

	ref ButtonWidget		m_SplitBtn;	
	ref ButtonWidget		m_CancelBtn;	
	ref EditBoxWidget		m_QuantityEditBox;
	ref SliderWidget		m_QuantitySlider;
	ref TextWidget			m_HeaderTitle;
	ref TextWidget			m_SplitText;
	ref ItemPreviewWidget	m_PreviewWidget;

	ItemBase				m_ItemToSplit;
	int						m_SplitQuantity;
	
    override Widget Init()
    {
		layoutRoot			= GetGame().GetWorkspace().CreateWidgets(LAYOUT_FILE);

		m_HeaderTitle		= TextWidget.Cast(layoutRoot.FindAnyWidget("HeaderWidget"));
		m_SplitText			= TextWidget.Cast(layoutRoot.FindAnyWidget("SplitTextWidget"));
		m_SplitBtn			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("SplitButton"));
		m_CancelBtn			= ButtonWidget.Cast(layoutRoot.FindAnyWidget("ExitButton"));
		m_QuantityEditBox	= EditBoxWidget.Cast(layoutRoot.FindAnyWidget("SplitEditBox"));
		m_QuantitySlider	= SliderWidget.Cast(layoutRoot.FindAnyWidget("SplitSlider"));
		m_PreviewWidget		= ItemPreviewWidget.Cast(layoutRoot.FindAnyWidget("ItemPreview"));

        return layoutRoot;
    }

	override void SetItem(notnull EntityAI entity)
	{
		m_ItemToSplit = ItemBase.Cast(entity);

		if (!m_ItemToSplit)
		{
			CancelAndHideUI();
			return;
		}

		// Init values
		m_SplitQuantity = Math.Floor(GetItemQuantity() * 0.5);
		m_QuantitySlider.SetMinMax(1, GetItemQuantity());
		m_HeaderTitle.SetText(m_ItemToSplit.GetDisplayName());
		m_PreviewWidget.SetItem(m_ItemToSplit);

		UpdateQuantity(m_SplitQuantity);
	}

	int GetItemQuantity()
	{
		if (m_ItemToSplit.IsAmmoPile())
		{
			Ammunition_Base ammo = Ammunition_Base.Cast(m_ItemToSplit);
			if (ammo)
				return ammo.GetAmmoCount();
		}

		return m_ItemToSplit.GetQuantity();
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		m_SplitQuantity += wheel;
		UpdateQuantity(m_SplitQuantity);
		return true;
	}

	void UpdateQuantity(int quantity)
	{
		m_SplitQuantity = Math.Clamp(quantity, 0, GetItemQuantity() - 1);
		m_QuantitySlider.SetCurrent(m_SplitQuantity);

		// This lil bit makes it possible to backspace to empty to enter a first number higher than 1.
		string txt = "";
		if (m_SplitQuantity > 0)
			txt = m_SplitQuantity.ToString();

		m_SplitText.SetText("#STR_split0: " + txt + "/" + GetItemQuantity().ToString());
		m_QuantityEditBox.SetText(txt);
	}
	
	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (KeyState(KeyCode.KC_ESCAPE) == 1)
			CancelAndHideUI();

		if (KeyState(KeyCode.KC_RETURN) == 1 || KeyState(KeyCode.KC_NUMPADENTER) == 1)
			SendSplitRPC();
	}

	override void OnShow()
	{
		super.OnShow();

		GetGame().GetInput().ChangeGameFocus(1);
		SetFocus(layoutRoot);
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) 
			player.GetInputController().SetDisabled(true);
		
		GetGame().GetUIManager().ShowCursor(true);
	}

	override void OnHide()
	{
		super.OnHide();

		GetGame().GetInput().ResetGameFocus();
		GetGame().GetUIManager().ShowCursor(false);

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player) 
			player.GetInputController().SetDisabled(false);
	}

	void CancelAndHideUI()
	{
		m_ItemToSplit = NULL;
		Close();
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{			
			if (w == m_SplitBtn)
			{
				SendSplitRPC();
				return true;
			}

			if (w == m_CancelBtn)
			{
				CancelAndHideUI();
				return true;
			}
		}
		
		return super.OnClick(w, x, y, button);
	}

	void SendSplitRPC()
	{
		if (!m_ItemToSplit)
			return;

		// Get split quantity
		UpdateQuantity(m_QuantityEditBox.GetText().ToInt());
		m_SplitQuantity = Math.Clamp(m_SplitQuantity, 1, GetItemQuantity() - 1);

		// Send split request to server
		m_ItemToSplit.Zen_SplitItemQuantityRPC(m_SplitQuantity);

		// Close window and nullify item ref.
		CancelAndHideUI();
	}
	
	// Detect change in values from editbox/slider.
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		bool result = super.OnChange(w, x, y, finished);

		if (w == m_QuantityEditBox) 
		{
			m_SplitQuantity = m_QuantityEditBox.GetText().ToInt();
			UpdateQuantity(m_SplitQuantity);
			return true;
		}

		if (w == m_QuantitySlider)
		{
			UpdateQuantity(Math.Round(m_QuantitySlider.GetCurrent()));
			return true;
		}
		
		return result;
	}
	
	override bool OnKeyPress(Widget w, int x, int y, int key) 
	{
		if (w == m_QuantityEditBox) 
		{
			// Only allow numbers/editing keys
			switch (key)
			{
				case KeyCode.KC_0:
				case KeyCode.KC_1:
				case KeyCode.KC_2:
				case KeyCode.KC_3:
				case KeyCode.KC_4:
				case KeyCode.KC_5:
				case KeyCode.KC_6:
				case KeyCode.KC_7:
				case KeyCode.KC_8:
				case KeyCode.KC_9:

				case KeyCode.KC_NUMPAD0:
				case KeyCode.KC_NUMPAD1:
				case KeyCode.KC_NUMPAD2:
				case KeyCode.KC_NUMPAD3:
				case KeyCode.KC_NUMPAD4:
				case KeyCode.KC_NUMPAD5:
				case KeyCode.KC_NUMPAD6:
				case KeyCode.KC_NUMPAD7:
				case KeyCode.KC_NUMPAD8:
				case KeyCode.KC_NUMPAD9:

				case KeyCode.KC_BACK:
				case KeyCode.KC_DELETE:
				case KeyCode.KC_LEFT:
				case KeyCode.KC_RIGHT:
					return true;
			}
		}

		return false;
	}
}
