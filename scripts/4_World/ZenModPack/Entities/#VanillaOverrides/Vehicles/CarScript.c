modded class CarScript
{
	//! CAR BATTERY ICON 
	protected int m_ZenBatteryEnergy;

	//! PIMP
	Chemlight_ColorBase		m_ZenChemlightAttached;
	ChemlightLight			m_ZenPimpLight;

	void CarScript()
	{
		RegisterNetSyncVariableInt("m_ZenBatteryEnergy");
	}

	void ~CarScript()
	{
	}

	//! CAR BATTERY ICON 
	override void OnUpdate(float dt)
    {
        super.OnUpdate(dt);

		#ifdef SERVER 
		if (ZenModEnabled("ZenCarBatteryIcon"))
		{
			ItemBase battery;

			if (EngineIsOn() || IsScriptedLightsOn())
			{
				battery = GetBattery();

				if (battery)
				{
					UpdateZenBatteryEnergy(battery.GetCompEM().GetEnergy0To100());
				}
			}
		}
		#endif
    }

	int GetZenBatteryEnergy0To100()
	{
		return m_ZenBatteryEnergy;
	}

	void UpdateZenBatteryEnergy(int percent)
	{
        if (!GetGame().IsDedicatedServer())
			return;

		if (!ZenModEnabled("ZenCarBatteryIcon"))
			return;

		if (m_ZenBatteryEnergy == percent)
		    return;

		m_ZenBatteryEnergy = percent;
		SetSynchDirty();
	}

	//! GLOVEBOX 
	override void EEInit()
	{
		super.EEInit();

		if (ZenModEnabled("ZenGlovebox"))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SpawnZenGlovebox, 2500, false);
		}
	}

	private void SpawnZenGlovebox()
	{
		if (!GetInventory())
			return;

		array<string> slot_names = new array<string>;
		ConfigGetTextArray("Attachments", slot_names);

		if (!slot_names)
			return;

		bool slotFound = false;
		foreach (string s : slot_names)
		{
			if (s == "ZenCarGlovebox")
			{
				slotFound = true;
				break;
			}
		}

		if (!slotFound)
			return;

		ZenGlovebox glovebox = ZenGlovebox.Cast(GetAttachmentByType(ZenGlovebox));
		if (!glovebox)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("ZenCarGlovebox"), false);
			ItemBase ib = ItemBase.Cast(GetInventory().CreateInInventory("ZenGlovebox"));
			if (ib)
				ib.LockToParent();
		}
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (GetGame().IsClient())
		{
			//! PIMP MY RIDE
			if (slot_name == "ZenChemlight")
			{
				CreateZenPimpLight(item);
			}
		}

		if (GetGame().IsDedicatedServer())
		{
			if (slot_name == "ZenJournal_Compass")
			{
				CreateZenCompass(Compass.Cast(item));
			} else
			if (slot_name == "CarBattery" || slot_name == "TruckBattery")
			{
				if (item.GetCompEM())
				{
					UpdateZenBatteryEnergy(item.GetCompEM().GetEnergy0To100());
				}
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (GetGame().IsClient())
		{
			if (slot_name == "ZenChemlight")
			{
				DestroyZenPimpLight();
			}
		}

		if (GetGame().IsDedicatedServer())
		{
			if (slot_name == "ZenJournal_Compass")
			{
				if (m_ZenCompassDashboard)
				{
					DeleteZenCompass(item);
				}
			} else 
			if (slot_name == "CarBattery" || slot_name == "TruckBattery")
			{
				UpdateZenBatteryEnergy(0);
			}
		}		
	}

	override bool CanDisplayCargo()
	{
		// If player inventory in cars is enabled, don't allow cargo displayed while inside car
		return super.CanDisplayCargo() && !Zen_PlayerInCar();
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
		if (category_name == "ZenMusic")
		{
			return false; // Moved ZenCassette slot to ZenInterior category for modpack.
		}
		else
		if (category_name == "ZenInterior")
		{
			return Zen_PlayerInCar();
		}
		else 
		if (category_name == "ZenCarAttachments")
		{
			if (ZenModEnabled("ZenCarAttachments"))
				return !Zen_PlayerInCar();

			if (ZenModEnabled("ZenPimpMyRide"))
				return !Zen_PlayerInCar();
		}

		return super.CanDisplayAttachmentCategory(category_name) && !Zen_PlayerInCar();
	}

	static const ref array<string> ZEN_CAR_ATTACHMENTS =
	{
		"Shoulder",
		"Back",
		"ZenTireGasoline",
		"ZenTireIron",
		"ZenTireRepair",
		"ZenBlowtorch"
	};

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		string slot_name = InventorySlots.GetSlotName(slot_id);
		if (super.CanDisplayAttachmentSlot(slot_id))
		{
			if (slot_name == "ZenCassette")
				return ZenModEnabled("ZenMusic");

			if (slot_name == "ZenJournal_Compass")
				return ZenModEnabled("ZenCarCompass");

			if (slot_name == "ZenCarGlovebox")
				return ZenModEnabled("ZenCarGlovebox");

			if (slot_name == "ZenChemlight")
				return ZenModEnabled("ZenPimpMyRide");	

			if (ZEN_CAR_ATTACHMENTS.Find(slot_name) != -1)
				return ZenModEnabled("ZenCarAttachments");

			return true;
		}
		
		return false;
	}

	//! GLOVEBOX / CLIENT-ONLY
	static bool Zen_PlayerInFrontSeat()
	{
		if (!GetGame() || !GetGame().IsClient())
			return false;

		PlayerBase clientPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (!clientPlayer)
			return false;

		if (!clientPlayer.IsInVehicle())
			return false;

		CarScript car = null;
		if (clientPlayer.GetCommand_Vehicle() && Class.CastTo(car, clientPlayer.GetCommand_Vehicle().GetTransport()))
		{
			int crewIdx = car.CrewMemberIndex(clientPlayer);
			if (crewIdx >= 0 && crewIdx <= 1)
			{
				return true;
			}
		}

		return false;
	}

	static bool Zen_PlayerInCar()
	{
		if (!GetGame() || !GetGame().IsClient())
			return false;

		PlayerBase clientPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (clientPlayer)
		{
			return clientPlayer.IsInVehicle();
		}

		return false;
	}

	//! COMPASS 
	protected ZenCarCompass m_ZenCompassDashboard;

	ZenCarCompass GetZenCarCompass()
	{
		return m_ZenCompassDashboard;
	}

	void CreateZenCompass(Compass item)
	{
		if (!GetGame().IsDedicatedServer())
			return;

		if (!item || !item.IsInherited(ItemCompass))
			return;

		int i;
		bool cfgFound = false;
		vector offsetPos;
		vector offsetOri;

		// Look for EXACT classname match first (as some modded cars inherit from vanilla scripts, but have different models)
		for (i = 0; i < GetZenCarCompassConfig().CarConfig.Count(); i++)
		{
			if (this.GetType() == GetZenCarCompassConfig().CarConfig.Get(i).CarTypeCfg)
			{
				cfgFound = true;
				offsetPos = GetZenCarCompassConfig().CarConfig.Get(i).Position;
				offsetOri = GetZenCarCompassConfig().CarConfig.Get(i).Orientation;
				break;
			}
		}

		// If not found, look for inherited child classnames
		if (!cfgFound)
		{
			for (i = 0; i < GetZenCarCompassConfig().CarConfig.Count(); i++)
			{
				if (this.IsKindOf(GetZenCarCompassConfig().CarConfig.Get(i).CarTypeCfg))
				{
					cfgFound = true;
					offsetPos = GetZenCarCompassConfig().CarConfig.Get(i).Position;
					offsetOri = GetZenCarCompassConfig().CarConfig.Get(i).Orientation;
					break;
				}
			}
		}

		if (!cfgFound)
		{
			Print("[ZenModPack] Compass attached to car: " + GetType() + " - but this modded car has no offset position config.");
			return;
		}

		if (!m_ZenCompassDashboard)
		{
			m_ZenCompassDashboard = ZenCarCompass.Cast(GetGame().CreateObject("ZenCarCompass", vector.Zero));

			if (!m_ZenCompassDashboard)
			{
				Error("[ZenModPack] Couldn't create duplicate proxy of ZenCarCompass for some reason?");
				return;
			}
		}

		if (!AddChild(m_ZenCompassDashboard, -1, false))
		{
			Error("[ZenModPack] Failed to attach " + m_ZenCompassDashboard.GetType() + " as child to " + GetType() + " for some reason?");
			m_ZenCompassDashboard.DeleteSafe();
			return;
		}

		m_ZenCompassDashboard.SetPosition(offsetPos);
		m_ZenCompassDashboard.SetOrientation(offsetOri);
		m_ZenCompassDashboard.Open();
		m_ZenCompassDashboard.Update();
		m_ZenCompassDashboard.SetHealth(item.GetHealth());
		m_ZenCompassDashboard.SetSynchDirty();

		item.Open();
		item.SetZenCarCompass(m_ZenCompassDashboard);
	}

	void DeleteZenCompass(EntityAI item)
	{
		if (!m_ZenCompassDashboard)
			return;

		this.RemoveChild(m_ZenCompassDashboard);
		m_ZenCompassDashboard.DeleteSafe();
		m_ZenCompassDashboard = NULL;

		Compass compassItem = Compass.Cast(item);
		if (compassItem)
		{
			compassItem.SetZenCarCompass(NULL);
		}
	}

	//! PIMP / CLIENT-ONLY
	vector GetZenChemlightHeightOffset() 
	{ 
		return "0 -0.01 0"; 
	}

	void CreateZenPimpLight(EntityAI item)
	{
		m_ZenChemlightAttached = Chemlight_ColorBase.Cast(item);
		if (!m_ZenChemlightAttached)
			return;

		if (!m_ZenChemlightAttached.GetCompEM() || !m_ZenChemlightAttached.GetCompEM().IsWorking())
		{
			DestroyZenPimpLight();
			return;
		}

		if (!m_ZenPimpLight)
		{
			m_ZenPimpLight = ChemlightLight.Cast(ScriptedLightBase.CreateLight(ChemlightLight, "0 -1 0"));
			m_ZenPimpLight.AttachOnObject(this, GetZenChemlightHeightOffset());

			switch(m_ZenChemlightAttached.GetType())
			{
				case "Chemlight_White": 
					m_ZenPimpLight.SetColorToWhite();
					break;
				case "Chemlight_Red": 
					m_ZenPimpLight.SetColorToRed();
					break;
				case "Chemlight_Green": 
					m_ZenPimpLight.SetColorToGreen();
					break;
				case "Chemlight_Blue": 
					m_ZenPimpLight.SetColorToBlue();
					break;
				case "Chemlight_Yellow": 
					m_ZenPimpLight.SetColorToYellow();
					break;
					
				default: { m_ZenPimpLight.SetColorToWhite(); };
			}
		}
	}

	void DestroyZenPimpLight()
	{
		m_ZenChemlightAttached = NULL;

		if (m_ZenPimpLight)
		{
			m_ZenPimpLight.FadeOut();
		}
	}

	void UpdateZenPimpLight()
	{
		if (m_ZenChemlightAttached != NULL)
		{
			if (!m_ZenChemlightAttached.GetCompEM() || !m_ZenChemlightAttached.GetCompEM().IsWorking())
			{
				DestroyZenPimpLight();
			} else 
			if (m_ZenChemlightAttached.GetCompEM() != NULL)
			{
				float efficiency = m_ZenChemlightAttached.GetEfficiency0To1();
				if (efficiency < 1)
				{
					m_ZenPimpLight.SetIntensity(efficiency, m_ZenChemlightAttached.GetCompEM().GetUpdateInterval());
				}
			}
		}
	}

	override void UpdateLightsClient(int newGear = -1)
	{
		super.UpdateLightsClient(newGear);

		UpdateZenPimpLight();
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
    {
        super.OnContact(zoneName, localPos, other, data);

		if (!ZenModEnabled("ZenCarsCutDownTrees"))
		{
			return;
		}

        if (!data || data.Impulse < 7500)
        {
            return;
        }

        Object tree = Object.Cast(other);
        if (!tree || tree.IsDamageDestroyed())
		{
            return;
        }

        if (!tree.GetType().Contains("TreeHard") && !tree.GetType().Contains("TreeSoft") && !tree.IsTree() && !tree.IsBush())
		{
            return;
        }

        PlayerBase driver;
        if (!Class.CastTo(driver, CrewDriver()))
        {
            return;
        }

        if (GetGame().IsDedicatedServer())
		{
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(ZenCutTreeProcess, tree, driver); // actually knock the tree over (runs on script main thread, *not* the physics thread)
        }

        if (GetGame().IsClient() || !GetGame().IsMultiplayer())
        {
            SoundHardTreeFallingPlay();
        }
    }

    static void ZenCutTreeProcess(Object tree, PlayerBase driver)
    {
        if (!tree || tree.IsDamageDestroyed())
            return;

        tree.DecreaseHealth("", "", 100, tree.CanBeAutoDeleted());
    }
}