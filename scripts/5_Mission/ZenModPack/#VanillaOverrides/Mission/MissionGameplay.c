modded class MissionGameplay
{
	//! UTILITIES 
	static bool SHOW_ZEN_ADMIN_DIALOG = false; 
	static string SHOW_ZEN_ADMIN_DIALOG_TEXT = "";
	static bool SHOW_ZEN_ADMIN_DIALOG_ISPOPUP = false;
	
	//! SHARED
	override void OnInit()
    {
        super.OnInit();

		//! LOAD CLIENT CONFIG 
		GetZenModPackClientConfig();

		//! AUTO RUN
        m_ZenAutoRunInterruptInputs = new array<string>;
        m_ZenAutoRunInterruptInputs.Insert("UAZenAutoRunToggle");
        m_ZenAutoRunInterruptInputs.Insert("UAMoveForward");
        m_ZenAutoRunInterruptInputs.Insert("UAMoveBack");
        m_ZenAutoRunInterruptInputs.Insert("UAMoveLeft");
        m_ZenAutoRunInterruptInputs.Insert("UAMoveRight");
        m_ZenAutoRunInterruptInputs.Insert("UAGetOver");
        m_ZenAutoRunInterruptInputs.Insert("UAStance");
        m_ZenAutoRunInterruptInputs.Insert("UAFire");
        m_ZenAutoRunInterruptInputs.Insert("UATempRaiseWeapon");

		//! RADIO BADGE 
		m_ZenRadioWidget = new ZenRadioBadgeWidget;
		m_ZenRadioTime = 0;
    }

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice); 

		if (!g_Game)
			return;
		
		//! UTILITIES
		UpdateZenAdminMessages();

		//! AUTO RUN 
		UpdateZenAutoRun();

		//! EAR PLUGS
		UpdateZenEarPlugs();

		//! BROKEN GLASSES
		UpdateZenBrokenGlasses();

		//! RADIO BADGE 
		UpdateZenRadioBadge(timeslice);
	}
	
	//! UTILITIES 
	void UpdateZenAdminMessages() 
	{
		if (!SHOW_ZEN_ADMIN_DIALOG)
			return;
		
		if (g_Game.GetUIManager() != NULL)
        {
            ZenAdminMessageGUI gui = ZenAdminMessageGUI.Cast(g_Game.GetUIManager().EnterScriptedMenu(ZenMenus.PLAYER_MESSAGE, NULL));
            if (gui)
			{
                gui.SetAdminMessage(SHOW_ZEN_ADMIN_DIALOG_TEXT, SHOW_ZEN_ADMIN_DIALOG_ISPOPUP);
				SHOW_ZEN_ADMIN_DIALOG = false;
			}
        }
	}

	//! RADIO BADGE (TODO: Find a better way to track the radio)
	private ref ZenRadioBadgeWidget m_ZenRadioWidget;
	private float m_ZenRadioTime;
	private ItemTransmitter m_ZenRadio;

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

		if (m_Hud.GetHudVisibility().IsContextFlagActive(IngameHudVisibility.HUD_HIDE_FLAGS))
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (GetGame().GetUIManager().GetMenu() != NULL && !GetGame().GetUIManager().FindMenu(MENU_INVENTORY) && !GetGame().GetUIManager().FindMenu(MENU_CHAT_INPUT))
		{
			m_ZenRadioWidget.HideAll();
			return;
		}

		if (m_ZenRadio && !m_ZenRadio.IsRuined() && m_ZenRadio.IsReceiving() && m_ZenRadio.GetCompEM().IsWorking())
		{
			m_ZenRadioWidget.SetChannel(m_ZenRadio.GetTunedFrequency().ToString());
			m_ZenRadioWidget.ShowIcon();
		}

		// Only check for radio every 1 secs
		m_ZenRadioTime += timeslice;
		if (m_ZenRadioTime < 1)
			return;

		m_ZenRadioTime = 0;

		m_ZenRadio = Zen_GetRadio(GetGame().GetPlayer());
		if (m_ZenRadio)
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
	protected ref array<string> m_ZenAutoRunInterruptInputs;
    protected int m_ZenSavedMovementState = DayZPlayerConstants.MOVEMENTIDX_IDLE;

	void UpdateZenAutoRun()
	{
		if (!ZenModEnabled("ZenAutoRun"))
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
			return;

        if (ZenAutoRunShouldInterrupt(player))
        {
            ZenAutoRunStop();
        }
        else 
        if (ZenAutoRunShouldStart(player))
        {
            ZenAutoRunStart(player);
        }
        else 
        if (player.IsZenAutoRunning())
        {
            int speed = GetZenAutoRunUpdatedSpeed(player);
            if (speed != DayZPlayerConstants.MOVEMENTIDX_IDLE)
            {
                ZenAutoRunSync(true, speed, 1);
            }
        }
	}

    private bool Zen_CheckInput(string inputName)
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

    private bool ZenAutoRunShouldInterrupt(PlayerBase player)
    {
        if (!player.IsZenAutoRunning())
            return false;

        if (player.IsInVehicle() || player.IsUnconscious() || player.IsRestrained() || !player.IsAlive())
            return true;

        foreach (string inputName : m_ZenAutoRunInterruptInputs)
        {
            if (Zen_CheckInput(inputName))
            {
                return true;
            }
        }

        // Check for any menu other than inventory open (eg. map)
        if (GetGame().GetUIManager().GetMenu() != NULL && GetGame().GetUIManager().FindMenu(MENU_INVENTORY) == NULL)
            return true;

        return false;
    }

    private bool ZenAutoRunShouldStart(PlayerBase player)
    {
        if (player.IsZenAutoRunning())
        {
            return false;
        }

        if (Zen_CheckInput("UAZenAutoRunToggle") && !player.IsZenAutoRunning() && !IsControlDisabled() && !ZenAutoRunShouldInterrupt(player))
        {
            return true;
        }

        return false;
    }

    private int GetZenAutoRunUpdatedSpeed(PlayerBase player)
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
        if (player.CanSprint() && player.m_MovementState.m_iMovement != m_ZenSavedMovementState)
        {
            return m_ZenSavedMovementState;
        }

        return DayZPlayerConstants.MOVEMENTIDX_IDLE;
    }

    private void ZenAutoRunStart(PlayerBase player)
    {
        m_ZenSavedMovementState = player.m_MovementState.m_iMovement;

        if (m_ZenSavedMovementState == DayZPlayerConstants.MOVEMENTIDX_IDLE)
        {
            m_ZenSavedMovementState = DayZPlayerConstants.MOVEMENTIDX_SPRINT;
        }

        ZenAutoRunSync(true, m_ZenSavedMovementState, 1);
    }

    private void ZenAutoRunStop()
    {
        ZenAutoRunSync(false, DayZPlayerConstants.MOVEMENTIDX_IDLE, 0);
    }

	private void ZenAutoRunSync(bool isRunning, int movementIdx, int angle)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;

        player.SetZenAutoRunning(isRunning);
        player.GetInputController().OverrideMovementSpeed(isRunning, movementIdx);
        player.GetInputController().OverrideMovementAngle(isRunning, angle);

        auto params = new Param3<bool, int, int>(isRunning, movementIdx, angle);
        GetGame().RPCSingleParam(player, ZenRPCs.AUTORUN, params, true);
    }

	//! EAR PLUGS 
	protected bool m_ZenHidEarPlugs = false;
	protected bool m_ZenEarPlugsOn = false;
	protected int m_ZenEarPlugsVolume = 50; // 50% volume by default
	protected ref ZenEarPlugsWidget m_ZenEarPlugsWidget;

	void UpdateZenEarPlugs()
	{
		if (!ZenModEnabled("ZenEarPlugs"))
			return;

		if (!m_ZenEarPlugsWidget)
			m_ZenEarPlugsWidget = new ZenEarPlugsWidget();

		// No HUD - stop here.
		if (!m_Hud) 
		{
			m_ZenEarPlugsWidget.HideAll();
			return;
		}

		// Player doesn't exist, died or went uncon. Reset ear plugs, stop here.
		if (!GetGame().GetPlayer() || !GetGame().GetPlayer().IsAlive() || GetGame().GetPlayer().IsUnconscious())
		{
			m_ZenEarPlugsWidget.HideAll();
			m_ZenEarPlugsOn = false;
			m_ZenHidEarPlugs = false;
			return;
		}

		// If HUD is hidden, hide ear plugs too.
		if (m_Hud.GetHudVisibility().IsContextFlagActive(IngameHudVisibility.HUD_HIDE_FLAGS) || m_Hud.GetHudVisibility().IsContextFlagActive(EHudContextFlags.INVENTORY_OPEN))
		{
			if (m_ZenEarPlugsOn && !m_ZenHidEarPlugs)
			{
				m_ZenEarPlugsWidget.HideAll();
				m_ZenHidEarPlugs = true;
			}

			return;
		}

		// Hide ear plugs and don't allow ear plug input if a GUI menu is open.
		if (GetGame().GetUIManager().GetMenu() != NULL) 
		{
			if (m_ZenEarPlugsOn && !m_ZenHidEarPlugs)
			{
				m_ZenEarPlugsWidget.HideAll();
				m_ZenHidEarPlugs = true;
			}

			return;
		}

		// Show ear plugs if we hid them due to UI reasons and now those UI blocks have gone away.
		if (m_ZenHidEarPlugs)
		{
			if (m_ZenEarPlugsOn)
			{
				m_ZenEarPlugsWidget.ShowIcon();
				m_ZenHidEarPlugs = false;
			}
		}

		// Ear plugs toggle key
		if (GetGame().GetInput().LocalPress("UAZenEarPlugsToggle"))
		{
			if (!m_ZenEarPlugsOn)
			{
				m_ZenEarPlugsOn = true;
				GetGame().GetSoundScene().SetSoundVolume(m_ZenEarPlugsVolume / 100, 0.5); // Set volume to X% over X second delay.
				m_ZenEarPlugsWidget.ShowIcon();
				m_ZenEarPlugsWidget.SetVolume(m_ZenEarPlugsVolume);
			}
			else
			{
				m_ZenEarPlugsOn = false;
				GetGame().GetSoundScene().SetSoundVolume(1, 0.5);
				m_ZenEarPlugsWidget.HideIcon();
			}
		}

		// If ear plugs are on, check for increase/decrease in volume
		if (m_ZenEarPlugsOn)
		{
			if (GetGame().GetInput().LocalPress("UAZenEarPlugsIncreaseVol")) // Increase volume key (+)
			{
				m_ZenEarPlugsVolume += 25;

				if (m_ZenEarPlugsVolume > 75)
				{
					m_ZenEarPlugsVolume = 75;
				}

				GetGame().GetSoundScene().SetSoundVolume(m_ZenEarPlugsVolume / 100, 0.5);
				m_ZenEarPlugsWidget.SetVolume(m_ZenEarPlugsVolume);
			}

			if (GetGame().GetInput().LocalPress("UAZenEarPlugsDecreaseVol")) // Decrease volume key (-)
			{
				m_ZenEarPlugsVolume -= 25;

				if (m_ZenEarPlugsVolume < 0)
				{
					m_ZenEarPlugsVolume = 0;
				}

				GetGame().GetSoundScene().SetSoundVolume(m_ZenEarPlugsVolume / 100, 0.5);
				m_ZenEarPlugsWidget.SetVolume(m_ZenEarPlugsVolume);
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
		brokenGlassesWidget.ShowFX(GetZenBrokenGlassesLevel());
	}

	// -1 means no glasses or no damage, >= 1 means damaged
	int GetZenBrokenGlassesLevel()
	{
		int damageLevel = -1;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return damageLevel;

		string type;

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