class Zen_EternalFireplace : Fireplace
{
	protected ref Timer m_ZenBurningCheckTimer;

	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsDedicatedServer())
		{
			PlaceOnSurface();
			SetAllowDamage(false);
			
			m_FuelBurnRateMP	= 0.000001;
			m_TemperatureLossMP = m_FuelBurnRateMP;
			m_ZenBurningCheckTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
			m_ZenBurningCheckTimer.Run(300, this, "FireStarta", null, true);

			if (FindAttachmentBySlotName("Stone") == NULL)
			{
				ItemBase stones = ItemBase.Cast(GetInventory().CreateAttachment("Stone"));
				if (stones)
				{
					stones.SetQuantity(8);
					stones.LockToParent();
					SetStoneCircleState(true);
				}
			}

			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(FireStarta, 1000, false);
		}
	};

	void ~Zen_EternalFireplace()
	{
		if (GetGame() && m_ClutterCutter)
			GetGame().ObjectDelete(m_ClutterCutter);
	}

	static const ref array<string> BANNED_ITEMS =
	{
		"Grenade_Base",
		"ExplosivesBase",
		"ExpansionSatchel",
		"Ammunition_Base",
		"GasCanister"
	};

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		#ifndef SERVER
		if (item)
		{
			foreach (string s : BANNED_ITEMS)
			{
				if (item.IsKindOf(s))
					return false;

				if (item.GetType().Contains(s)) // case sensitive
					return false;
			}
		}

		if (item.GetInventory())
		{
			array<EntityAI> itemsArray = new array<EntityAI>;
			item.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);
            	    
			for (int i = 0; i < itemsArray.Count(); i++)
			{
				item = itemsArray.Get(i);
				foreach (string s2 : BANNED_ITEMS)
				{
					if (item.IsKindOf(s2))
						return false;

					if (item.GetType().Contains(s2)) // case sensitive
						return false;
				}
			}
		}
		#endif
		
		return super.CanReceiveItemIntoCargo(item);
	}

	void FireStarta()
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
			GetInventory().SetSlotLock(InventorySlots.GetSlotIdFromString("Paper"), false);
			fuel = ItemBase.Cast(GetInventory().CreateAttachment("Zen_FireFuel"));
		}

		// Set tinder & fuel
		if (wood)
		{
			wood.SetQuantity(6);
		}

		if (bark)
		{
			bark.SetQuantity(8);
		}

		if (stick)
		{
			stick.SetQuantity(10);
		}

		if (!m_ClutterCutter)
			m_ClutterCutter = GetGame().CreateObjectEx("ClutterCutterFireplace", GetPosition(), ECE_PLACE_ON_SURFACE);

		// Ignite fire
		if (!IsBurning())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartEternalFire, 1000, false);
	}

	void StartEternalFire()
	{
		StartFire(true);
		Synchronize();
		SetSynchDirty();
	}

	override void StopFire( FireplaceFireState fire_state = FireplaceFireState.END_FIRE )
	{
		return;
	}

	override void StopHeating()
	{
		return;
	}

	override void StartCooling()
	{
		return;
	}
}
