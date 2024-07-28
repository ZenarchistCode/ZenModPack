modded class MissionGameplay
{
	//! SHARED
	override void OnInit()
    {
        super.OnInit();

		//! LOAD CLIENT CONFIG 
		GetZenModPackClientConfig();

		//! AUTO RUN
        m_AutoRunInterruptInputs = new array<string>;
        m_AutoRunInterruptInputs.Insert("UAZenAutoRunToggle");
        m_AutoRunInterruptInputs.Insert("UAMoveForward");
        m_AutoRunInterruptInputs.Insert("UAMoveBack");
        m_AutoRunInterruptInputs.Insert("UAMoveLeft");
        m_AutoRunInterruptInputs.Insert("UAMoveRight");
        m_AutoRunInterruptInputs.Insert("UAGetOver");
        m_AutoRunInterruptInputs.Insert("UAStance");
        m_AutoRunInterruptInputs.Insert("UAFire");
        m_AutoRunInterruptInputs.Insert("UATempRaiseWeapon");

		//! RADIO BADGE 
		m_ZenRadioWidget = new ZenRadioBadgeWidget;
		m_ZenRadioTime = 0;
    }

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice); 

		if (!GetGame())
			return;

		//! AUTO RUN 
		UpdateZenAutoRun();

		//! EAR PLUGS
		UpdateZenEarPlugs();

		//! BROKEN GLASSES
		UpdateZenBrokenGlasses();

		//! RADIO BADGE 
		UpdateZenRadioBadge(timeslice);
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);

		if (key == KeyCode.KC_ESCAPE)
		{
			UIScriptedMenu menu = m_UIManager.GetMenu();
			if (menu)
			{
				//! NOTES
				ZenNoteGUI noteMenu = ZenNoteGUI.Cast(menu);
				if (noteMenu)
				{
					noteMenu.OnExitBtnClick();
				}
			}
		}
	}

	//! RADIO BADGE (TODO: Find a better way to track the radio)
	private ref ZenRadioBadgeWidget m_ZenRadioWidget;
	private float m_ZenRadioTime;
	private ItemTransmitter m_Radio;

	void UpdateZenRadioBadge(float timeslice)
	{
		if (!GetGame() || !m_ZenRadioWidget)
			return;

		if (!m_Hud || !GetZenModPackClientConfig().ShowRadioBadge)
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (!GetGame().GetPlayer() || !GetGame().GetPlayer().IsAlive() || GetGame().GetPlayer().IsUnconscious())
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (m_Hud.HideZenRadioBadge())
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (GetGame().GetUIManager().GetMenu() != NULL && !GetGame().GetUIManager().FindMenu(MENU_INVENTORY) && !GetGame().GetUIManager().FindMenu(MENU_CHAT_INPUT))
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (m_Radio && !m_Radio.IsRuined() && m_Radio.IsReceiving() && m_Radio.GetCompEM().IsWorking())
		{
			m_ZenRadioWidget.SetChannel(m_Radio.GetTunedFrequency().ToString());
			m_ZenRadioWidget.ShowIcon();
		}

		// Only check for radio every 1 secs
		m_ZenRadioTime += timeslice;
		if (m_ZenRadioTime < 1)
			return;

		m_ZenRadioTime = 0;

		m_Radio = Zen_GetRadio(GetGame().GetPlayer());
		if (m_Radio)
		{
			m_ZenRadioWidget.ShowIcon();
		}
		else
		{
			m_ZenRadioWidget.HideIcon();
		}
	}

	private static ItemTransmitter Zen_GetRadio(DayZPlayer player)
	{
		array<EntityAI> itemsArray = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		for (int i = 0; i < itemsArray.Count(); i++)
		{
			if (itemsArray.Get(i) && itemsArray.Get(i).IsInherited(ItemTransmitter))
			{
				ItemTransmitter radio = ItemTransmitter.Cast(itemsArray.Get(i));
				if (radio && !radio.IsRuined() && radio.IsReceiving() && radio.GetCompEM() && radio.GetCompEM().IsWorking())
				{
					return radio;
				}
			}
		}

		return NULL;
	}

	//! AUTO RUN 
	protected ref array<string> m_AutoRunInterruptInputs;
    protected int m_SavedMovementState = DayZPlayerConstants.MOVEMENTIDX_IDLE;

	void UpdateZenAutoRun()
	{
		if (!ZenModEnabled("ZenAutoRun"))
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
			return;

        if (AutoRunShouldInterrupt(player))
        {
            AutoRunStop();
        }
        else 
        if (AutoRunShouldStart(player))
        {
            AutoRunStart(player);
        }
        else 
        if (player.IsAutoRunning())
        {
            int speed = GetAutoRunUpdatedSpeed(player);
            if (speed != DayZPlayerConstants.MOVEMENTIDX_IDLE)
            {
                AutoRunSync(true, speed, 1);
            }
        }
	}

    private bool CheckInput(string inputName)
    {
        if(GetUApi())
		{
            UAInput inp = GetUApi().GetInputByName(inputName);

            if (inp && inp.LocalPress())
            {
                return true;
            }
        }

        return false;
    }

    private bool AutoRunShouldInterrupt(PlayerBase player)
    {
        if (!player.IsAutoRunning())
            return false;

        if (player.IsInVehicle() || player.IsUnconscious() || player.IsRestrained() || !player.IsAlive())
            return true;

        foreach (string inputName : m_AutoRunInterruptInputs)
        {
            if (CheckInput(inputName))
            {
                return true;
            }
        }

        // Check for any menu other than inventory open (eg. map)
        if (GetGame().GetUIManager().GetMenu() != NULL && GetGame().GetUIManager().FindMenu(MENU_INVENTORY) == NULL)
            return true;

        return false;
    }

    private bool AutoRunShouldStart(PlayerBase player)
    {
        if (player.IsAutoRunning())
        {
            return false;
        }

        if (CheckInput("UAZenAutoRunToggle") && !player.IsAutoRunning() && !IsControlDisabled() && !AutoRunShouldInterrupt(player))
        {
            return true;
        }

        return false;
    }

    private int GetAutoRunUpdatedSpeed(PlayerBase player)
    {
        if (!GetGame() || !player || !GetGame().GetUIManager())
        {
            return DayZPlayerConstants.MOVEMENTIDX_IDLE;
        }

        if (GetGame().IsInventoryOpen() || player.GetInputController().LimitsIsSprintDisabled() || player.GetStaminaHandler().GetStamina() <= 0)
        {
            return DayZPlayerConstants.MOVEMENTIDX_RUN;
        }
        else
        if (player.CanSprint() && player.m_MovementState.m_iMovement != m_SavedMovementState)
        {
            return m_SavedMovementState;
        }

        return DayZPlayerConstants.MOVEMENTIDX_IDLE;
    }

    private void AutoRunStart(PlayerBase player)
    {
        m_SavedMovementState = player.m_MovementState.m_iMovement;

        if (m_SavedMovementState == DayZPlayerConstants.MOVEMENTIDX_IDLE)
        {
            m_SavedMovementState = DayZPlayerConstants.MOVEMENTIDX_SPRINT;
        }

        AutoRunSync(true, m_SavedMovementState, 1);
    }

    private void AutoRunStop()
    {
        AutoRunSync(false, DayZPlayerConstants.MOVEMENTIDX_IDLE, 0);
    }

	private void AutoRunSync(bool isRunning, int movementIdx, int angle)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;

        player.SetAutoRunning(isRunning);
        player.GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
        player.GetInputController().OverrideMovementAngle(isRunning, angle);

        auto params = new Param3<bool, int, int>(isRunning, movementIdx, angle);
        GetGame().RPCSingleParam(player, ZenRPCs.AUTORUN, params, true);
    }

	//! EAR PLUGS 
	protected bool m_HidEarPlugs = false;
	protected bool m_EarPlugsOn = false;
	protected int m_EarPlugsVolume = 5; // 50% volume by default
	protected ref EarPlugsWidget m_EarPlugsWidget;

	void UpdateZenEarPlugs()
	{
		if (!ZenModEnabled("ZenEarPlugs"))
			return;

		if (!m_EarPlugsWidget)
			m_EarPlugsWidget = new EarPlugsWidget;

		// No HUD - stop here.
		if (!m_Hud) 
		{
			m_EarPlugsWidget.HideAll();
			return;
		}

		// Player doesn't exist, died or went uncon. Reset ear plugs, stop here.
		if (!GetGame().GetPlayer() || !GetGame().GetPlayer().IsAlive() || GetGame().GetPlayer().IsUnconscious())
		{
			m_EarPlugsWidget.HideAll();
			m_EarPlugsOn = false;
			m_HidEarPlugs = false;
			return;
		}

		// If HUD is hidden, hide ear plugs too.
		if (m_Hud.HideEarPlugs())
		{
			if (m_EarPlugsOn && !m_HidEarPlugs)
			{
				m_EarPlugsWidget.HideAll();
				m_HidEarPlugs = true;
			}

			return;
		}

		// Hide ear plugs and don't allow ear plug input if a GUI menu is open.
		if (GetGame().GetUIManager().GetMenu() != NULL) 
		{
			if (m_EarPlugsOn && !m_HidEarPlugs)
			{
				m_EarPlugsWidget.HideAll();
				m_HidEarPlugs = true;
			}
			
			return;
		}

		// Show ear plugs if we hid them due to UI reasons and now those UI blocks have gone away.
		if (m_HidEarPlugs)
		{
			if (m_EarPlugsOn)
			{
				m_EarPlugsWidget.ShowIcon();
				m_HidEarPlugs = false;
			}
		}

		// Ear plugs toggle key
		if (GetGame().GetInput().LocalPress("UAZenEarPlugsToggle"))
		{
			if (!m_EarPlugsOn)
			{
				m_EarPlugsOn = true;
				GetGame().GetSoundScene().SetSoundVolume(m_EarPlugsVolume / 10, 1); // Set volume to X% over 1 second delay.
				m_EarPlugsWidget.ShowIcon();
				m_EarPlugsWidget.SetVolume(m_EarPlugsVolume);
			}
			else
			{
				m_EarPlugsOn = false;
				GetGame().GetSoundScene().SetSoundVolume(1, 1); // Set volume to 100% over 1 second delay.
				m_EarPlugsWidget.HideIcon();
			}
		}

		// If ear plugs are on, check for increase/decrease in volume
		if (m_EarPlugsOn)
		{
			if (GetGame().GetInput().LocalPress("UAZenEarPlugsIncreaseVol")) // Increase volume key (+)
			{
				m_EarPlugsVolume = m_EarPlugsVolume + 1;

				if (m_EarPlugsVolume > 9)
				{
					m_EarPlugsVolume = 9;
				}

				GetGame().GetSoundScene().SetSoundVolume(m_EarPlugsVolume / 10, 1);
				m_EarPlugsWidget.SetVolume(m_EarPlugsVolume);
			}

			if (GetGame().GetInput().LocalPress("UAZenEarPlugsDecreaseVol")) // Decrease volume key (-)
			{
				m_EarPlugsVolume = m_EarPlugsVolume - 1;

				if (m_EarPlugsVolume < 0)
				{
					m_EarPlugsVolume = 0;
				}

				GetGame().GetSoundScene().SetSoundVolume(m_EarPlugsVolume / 10, 1);
				m_EarPlugsWidget.SetVolume(m_EarPlugsVolume);
			}
		}
	}

	//! BROKEN GLASSES
	void UpdateZenBrokenGlasses()
	{
		if (!ZenModEnabled("ZenBrokenGlasses"))
			return;

		ZenBrokenGlassesWidget brokenGlassesWidget = ZenBrokenGlassesWidget.GetZenBrokenGlassesWidget();
		if (!brokenGlassesWidget)
			return;

		// No HUD - stop here.
		if (!m_Hud)
		{
			brokenGlassesWidget.HideFX();
			return;
		}

		if (!GetGame().GetPlayer() || !GetGame().GetPlayer().IsAlive() || GetGame().GetPlayer().IsUnconscious())
		{
			brokenGlassesWidget.HideFX();
			return;
		}

		if (GetGame().GetUIManager().GetMenu() != NULL && !GetGame().GetUIManager().FindMenu(MENU_INVENTORY) && !GetGame().GetUIManager().FindMenu(MENU_CHAT_INPUT))
		{
			brokenGlassesWidget.HideFX();
			return;
		}

		bool notNull = brokenGlassesWidget != NULL;
		brokenGlassesWidget.ShowFX(GetBrokenGlassesLevel());
	}

	// -1 means no glasses or no damage, >= 1 means damaged
	int GetBrokenGlassesLevel()
	{
		int damageLevel = -1;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return damageLevel;

		string type;

		// Check NVGs/visor
		if (player.IsNVGLowered())
		{
			NVGoggles nvg;
			if (player.FindAttachmentBySlotName("Eyewear"))
			{
				nvg = NVGoggles.Cast(player.FindAttachmentBySlotName("Eyewear").FindAttachmentBySlotName("NVG"));
			}
			else 
			if (player.FindAttachmentBySlotName("Headgear"))
			{
				nvg = NVGoggles.Cast(player.FindAttachmentBySlotName("Headgear").FindAttachmentBySlotName("NVG"));
			}
		
			if (nvg)
			{
				// If NVGs are lowered and damaged return their damage level first (doesn't matter if they're not turned on)
				damageLevel = nvg.GetHealthLevel();
			}
		}
		
		// Check visor
		if (player.FindAttachmentBySlotName("Headgear"))
		{
			ItemBase helmet = ItemBase.Cast(player.FindAttachmentBySlotName("Headgear"));
			if (helmet)
			{
				type = helmet.GetType();
				type.ToLower();

				// Check moto helm
				if (type.Contains("motohelmet"))
				{
					damageLevel = Math.Max(damageLevel, helmet.GetHealthLevel());
				}
				else 
				{
					// Check Gorka visor
					ItemBase visor = ItemBase.Cast(player.FindAttachmentBySlotName("Headgear").FindAttachmentBySlotName("Glass"));
					if (visor)
					{
						// If visor is damaged return its damage level first
						damageLevel = Math.Max(damageLevel, visor.GetHealthLevel());
					}
				}
			}
		}

		// Check gas mask
		if (player.FindAttachmentBySlotName("Mask"))
		{
			Clothing mask = Clothing.Cast(player.FindAttachmentBySlotName("Mask"));
			if (mask && mask.IsGasMask())
			{
				damageLevel = Math.Max(damageLevel, mask.GetHealthLevel());
			}
		}

		// Check glasses
		Clothing glasses = Clothing.Cast(player.FindAttachmentBySlotName("Eyewear"));
		if (!glasses)
			return damageLevel;

		// Final check for modded glasses without PPE effect
		type = glasses.GetType();
		type.ToLower();
		if (glasses.GetGlassesEffectID() == -1 && !type.Contains("glasses"))
			return damageLevel;

		// Return whatever is higher - our glasses damage or our visor damage
		return Math.Max(glasses.GetHealthLevel(), damageLevel);
	}
}