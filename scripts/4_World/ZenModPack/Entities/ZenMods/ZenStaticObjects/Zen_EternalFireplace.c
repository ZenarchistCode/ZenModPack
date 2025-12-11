class Zen_EternalFireplace extends Fireplace
{
	static const ref array<string> BANNED_ITEMS =
	{
		"Grenade_Base",
		"ExplosivesBase",
		"ExpansionSatchel",
		"Ammunition_Base",
		"GasCanister"
	};

	protected ref Timer m_ZenWatchdog;

	override void DeferredInit()
	{
		super.DeferredInit();

		if (!GetGame().IsDedicatedServer())
			return;

		PlaceOnSurface();
		SetAllowDamage(false);

		// Ensure stone circle
		if (!FindAttachmentBySlotName("Stone"))
		{
			ItemBase stones = ItemBase.Cast(GetInventory().CreateAttachment("Stone"));
			if (stones)
			{
				stones.SetQuantity(8);
				stones.LockToParent();
				SetStoneCircleState(true);
			}
		}

		ZenFireStarta();

		if (!m_ZenWatchdog)
			m_ZenWatchdog = new Timer(CALL_CATEGORY_SYSTEM);

		m_ZenWatchdog.Run(60, this, "EnsureZenLit", null, true);
	}

	void ~Zen_EternalFireplace()
	{
		if (GetGame() && m_ClutterCutter)
			GetGame().ObjectDelete(m_ClutterCutter);
	}

	// Relight / unstick and keep minimal fuel+kindling present
	void EnsureZenLit()
	{
		if (!GetGame().IsDedicatedServer())
			return;

		// Kindling always present
		if (!FindAttachmentBySlotName("Paper"))
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Paper"), false);
			GetInventory().CreateAttachment("Paper");
		}

		if (!FindAttachmentBySlotName("Bark"))
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Bark"), false);
			GetInventory().CreateAttachment("Bark_Oak");
		}

		// Minimal fuel present / topped up
		ItemBase wood = ItemBase.Cast(FindAttachmentBySlotName("Firewood"));
		if (!wood)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Firewood"), false);
			wood = ItemBase.Cast(GetInventory().CreateAttachment("Firewood"));
		}
		if (wood && wood.GetQuantity() < 6)
			wood.SetQuantity(6);

		ItemBase stick = ItemBase.Cast(FindAttachmentBySlotName("WoodenStick"));
		if (!stick)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("WoodenStick"), false);
			stick = ItemBase.Cast(GetInventory().CreateAttachment("WoodenStick"));
		}
		if (stick && stick.GetQuantity() < 10)
			stick.SetQuantity(10);

		if (!FindAttachmentBySlotName("ZenFuel"))
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("ZenFuel"), false);
			GetInventory().CreateAttachment("Zen_FireFuel");
		}

		RebuildZenConsumables();
		SetWet(0);

		// Unstick "ghost" state (burning flag true but no timers)
		if (IsBurning() && (!m_HeatingTimer || !m_HeatingTimer.IsRunning()) && (!m_CoolingTimer || !m_CoolingTimer.IsRunning()))
			super.StopFire(FireplaceFireState.END_FIRE);

		if (!IsBurning())
			StartFire(true);
	}
	
	void RebuildZenConsumables()
	{
	    m_FireConsumables.Clear();
		
	    for (int i = 0; i < GetInventory().AttachmentCount(); i++)
	    {
	        ItemBase it = ItemBase.Cast(GetInventory().GetAttachmentFromIndex(i));
			
	        if (it && (IsKindling(it) || IsFuel(it)))
	            AddToFireConsumables(it);
	    }
		
	    CalcAndSetQuantity();
	}

	// Keep burn/cooling effectively frozen even after vanilla refreshes
	override void RefreshFireplaceVisuals()
	{
		super.RefreshFireplaceVisuals();

		SetFuelBurnRateMP(0.000001);
		SetTemperatureLossMP(0.000001);
	}

	// Server-authoritative ban on explosives/ammo in cargo
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (item)
		{
			foreach (string s : BANNED_ITEMS)
			{
				if (item.IsKindOf(s) || item.GetType().Contains(s))
				{
					return false;
				}
			}
		}

		if (item && item.GetInventory())
		{
			array<EntityAI> itemsArray = {};
			item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
			
			foreach (EntityAI e : itemsArray)
			{
				foreach (string s2 : BANNED_ITEMS)
				{
					if (e.IsKindOf(s2) || e.GetType().Contains(s2))
					{
						return false;
					}
				}
			}
		}

		return super.CanReceiveItemIntoCargo(item);
	}

	// Seed attachments and ignite
	void ZenFireStarta()
	{
		// Create tinder & fuel
		ItemBase wood = ItemBase.Cast(FindAttachmentBySlotName("Firewood"));
		if (!wood)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Firewood"), false);
			wood = ItemBase.Cast(GetInventory().CreateAttachment("Firewood"));
		}

		ItemBase stick = ItemBase.Cast(FindAttachmentBySlotName("WoodenStick"));
		if (!stick)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("WoodenStick"), false);
			stick = ItemBase.Cast(GetInventory().CreateAttachment("WoodenStick"));
		}

		ItemBase paper = ItemBase.Cast(FindAttachmentBySlotName("Paper"));
		if (!paper)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Paper"), false);
			paper = ItemBase.Cast(GetInventory().CreateAttachment("Paper"));
		}

		ItemBase bark = ItemBase.Cast(FindAttachmentBySlotName("Bark"));
		if (!bark)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Bark"), false);
			bark = ItemBase.Cast(GetInventory().CreateAttachment("Bark_Oak"));
		}

		ItemBase fuel = ItemBase.Cast(FindAttachmentBySlotName("ZenFuel"));
		if (!fuel)
		{
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("ZenFuel"), false);
			fuel = ItemBase.Cast(GetInventory().CreateAttachment("Zen_FireFuel"));
		}

		// Quantities
		if (wood)  wood.SetQuantity(6);
		if (bark)  bark.SetQuantity(8);
		if (stick) stick.SetQuantity(10);

		// Clutter cutter
		if (!m_ClutterCutter)
			m_ClutterCutter = GetGame().CreateObjectEx("ClutterCutterFireplace", GetPosition(), ECE_PLACE_ON_SURFACE);

		RebuildZenConsumables();
		SetWet(0);

		if (!IsBurning())
			StartFire(true);
	}

	// Eternal behavior: remove wind/wet early-returns while keeping vanilla timers/areas/sounds
	override protected void StartHeating()
	{
		SetObjectMaterial(0, MATERIAL_FIREPLACE_GLOW);

		// NO early return on wind/wet

		if (IsBaseFireplace() && !IsOven())
			CreateAreaDamage();

		if (!m_CookingProcess)
			m_CookingProcess = new Cooking();

		m_HeatingTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_HeatingTimer.Run(TIMER_HEATING_UPDATE_INTERVAL, this, "Heating", null, true);
		m_CookingProcess.SetCookingUpdateTime(TIMER_HEATING_UPDATE_INTERVAL);

		m_NoisePar = new NoiseParams();
		
		if (IsRoofAbove() || IsOven() || IsFireplaceIndoor())
			m_NoisePar.LoadFromPath("CfgVehicles FireplaceBase NoiseFireplaceSpecial");
		else
			m_NoisePar.LoadFromPath("CfgVehicles FireplaceBase NoiseFireplaceBase");
	}
}