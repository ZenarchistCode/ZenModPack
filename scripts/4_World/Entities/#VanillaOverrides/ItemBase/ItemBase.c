modded class ItemBase
{
	//! SHARED
	static int ZENMOD_ITEM_COUNT = 0;
	protected bool m_WasZenHologrammed;

	void SetZenHologrammed(bool hologram)
	{
		m_WasZenHologrammed = hologram;

		if (m_WasZenHologrammed)
		{
			string textureAlpha = "#(argb,8,8,3)color(1,1,1,0.1,ca)";
			SetObjectTexture(0, textureAlpha);
		}
		else
		{
			string textureNoAlpha = GetHiddenSelectionsTextures().Get(0);
			SetObjectTexture(0, textureNoAlpha);
		}
	}

	bool IsZenHologrammed()
	{
		return m_WasZenHologrammed;
	}

	bool ShouldZenHologram()
	{
		return false;
	}

	//! LEFTOVERS
	override bool SetQuantity(float value, bool destroy_config = true, bool destroy_forced = false, bool allow_client = false, bool clamp_to_stack_max = true)
	{
		#ifdef SERVER
		if (ZenModEnabled("ZenLeftovers"))
		{
			LeftoverItem li = GetLeftoversConfig().GetLeftoverItem(this.GetType());

			if (value <= 0.01 && li && !li.DropToGround && li.LeftoverItemType != "")
				return super.SetQuantity(0.01, destroy_config, destroy_forced, allow_client, clamp_to_stack_max); // Prevent edible item from being deleted if config says the junk item should be replaced in the player's hands instead of dropped to the ground.
		}
		#endif

		return super.SetQuantity(value, destroy_config, destroy_forced, allow_client, clamp_to_stack_max);
	}

	//! OPEN CANS ROCK - play soundset sent from server
	override void OnRPC(PlayerIdentity sender, int rpc_type,ParamsReadContext ctx) 
	{
		super.OnRPC(sender, rpc_type,ctx);
			
		switch (rpc_type)
		{
			#ifndef SERVER
			case ZenRPCs.PLAY_SOUND:
				Param1<string> p = new Param1<string>("");
					
				if (!ctx.Read(p))
					return;

				EffectSound sound = SEffectManager.PlaySound(p.param1, GetPosition());
				if (sound)
					sound.SetAutodestroy(true);
			
			break;
			#endif
		}
	}

	//! ZEN UTILITIES
#ifdef SERVER
	override void DeferredInit()
	{
		super.DeferredInit();


		if (ZenItemStatsLogger.ALREADY_PRINTED )
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLogItemStuff, 1000);
	}

	void ZenLogItemStuff()
	{
		string type = GetType();
		type.ToLower();

		if (GetZenUtilitiesConfig().FindItemLog(type))
		{
			ZenItemStatsLogger.IncreaseItemCount(type, GetQuantity());
		}
	}

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		ZENMOD_ITEM_COUNT++;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZENMOD_ITEM_COUNT--;
	}
#endif

	//! SPLIT UI 
	//! Vanilla
	override void OnRightClick()
	{
		if (!ZenModEnabled("ZenSplitUI"))
		{
			super.OnRightClick();
			return;
		}

		// Hijack vanilla and don't send to super yet... I've got plans for you motherfucker.
		Zen_OnRightClick();
	}

	//! Client: Check if ctrl is down and item can be split, open custom split UI to allow player to select quantity to split off.
	void Zen_OnRightClick()
	{
		if (GetDayZGame().IsLeftCtrlDown() && CanBeSplit() && !GetGame().GetPlayer().GetInventory().HasInventoryReservation(this, NULL))
		{
			// Right click + ctrl is down - open UI
			if (GetGame().IsClient() && GetGame().GetUIManager() && !GetGame().GetUIManager().FindMenu(ZenMenus.SPLIT_GUI))
			{
				ZenSplitItemUIBase gui = ZenSplitItemUIBase.Cast(GetGame().GetUIManager().EnterScriptedMenu(ZenMenus.SPLIT_GUI, GetGame().GetUIManager().GetMenu()));
				if (gui)
					gui.SetItem(this);
			}
		}
		else 
		{
			// These are not the droids we're looking for - just pass to super.
			super.OnRightClick();
		}
	}

	//! Client: Send split quantity request from GUI to server.
	void Zen_SplitItemQuantityRPC(int quantity)
	{
		// Copied from vanilla, only change is adding split quantity int from GUI to the RPC send and assigning a custom RPC id (ZEN_SPLIT_ITEM)
		if (ScriptInputUserData.CanStoreInputUserData())
		{
			vector m4[4];
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
					
			EntityAI root = GetHierarchyRoot();
					
			InventoryLocation dst = new InventoryLocation;
			if (!player.GetInventory().FindFirstFreeLocationForNewEntity(GetType(), FindInventoryLocationType.CARGO, dst))
			{						
				if (root)
				{
					root.GetTransform(m4);
					dst.SetGround(this, m4);
				}
				else
					GetInventory().GetCurrentInventoryLocation(dst);
			}
			else
			{
				dst.SetCargo(dst.GetParent(), this, dst.GetIdx(), dst.GetRow(), dst.GetCol(), dst.GetFlip());
				if (GetGame().GetPlayer().GetInventory().HasInventoryReservation(null, dst))
				{
					if (root)
					{
						root.GetTransform(m4);
						dst.SetGround(this, m4);
					}
					else
						GetInventory().GetCurrentInventoryLocation(dst);
				}
				else
				{
					GetGame().GetPlayer().GetInventory().AddInventoryReservationEx(null, dst, GameInventory.c_InventoryReservationTimeoutShortMS);
				}
			}

			ScriptInputUserData ctx = new ScriptInputUserData;
			ctx.Write(ZenRPCs.ZEN_SPLIT_ITEM);
			ctx.Write(quantity);
			ItemBase thiz = this;
			ctx.Write(thiz);
			dst.WriteToContext(ctx);
			ctx.Send();
		}
	}

	//! Server - verify item can actually be split, then splizzle that shizzle
	void SplitItemToInventoryLocation_ZenSplit(notnull InventoryLocation dst, int quantity)
	{
		if (!CanBeSplit())
			return;

		ItemBase new_item = ItemBase.Cast(GameInventory.LocationCreateEntity(dst, GetType(), ECE_IN_INVENTORY, RF_DEFAULT));
		float split_quantity_new = Math.Clamp(quantity, 1, quantity);

		if (new_item)
		{
			// Ammo quantity is handled differently to ItemBase
			if (new_item.IsAmmoPile())
			{
				Ammunition_Base ammo = Ammunition_Base.Cast(new_item);
				if (ammo.GetAmmoMax() < split_quantity_new)
				{
					split_quantity_new = ammo.GetAmmoMax();
				}
			
				ammo.SetResultOfSplit(true);
				MiscGameplayFunctions.TransferItemProperties(this, ammo);
				ammo.ServerSetAmmoCount(split_quantity_new);

				Ammunition_Base ammo2 = Ammunition_Base.Cast(this);
				if (ammo2)
					ammo2.ServerAddAmmoCount(-split_quantity_new);
			}
			else 
			// Regular ItemBase
			{
				if (new_item.GetQuantityMax() < split_quantity_new)
				{
					split_quantity_new = new_item.GetQuantityMax();
				}
			
				new_item.SetResultOfSplit(true);
				MiscGameplayFunctions.TransferItemProperties(this, new_item);
			
				if (dst.IsValid() && dst.GetType() == InventoryLocationType.ATTACHMENT && split_quantity_new > 1)
				{
					AddQuantity(-1);
					new_item.SetQuantity(1);
				}
				else
				{
					AddQuantity(-split_quantity_new);
					new_item.SetQuantity(split_quantity_new);				
				}
			}
		}	
	}

	/*
	//! UTILITIES - Loot cycling detection?
#ifdef SERVER
	bool m_ZenIsVirgin = false;

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_ZenIsVirgin = true; // :(
	}

	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);

		if (!GetZenUtilitiesConfig().ShouldLogLootCyclers)
			return;

		// Only count items spawned this session
		if (!m_ZenIsVirgin)
			return;

		// If new owner is not null, item was not dropped onto the ground.
		if (new_owner != NULL)
			return;

		// If old owner is null, it wasn't dropped by a player
		if (old_owner == NULL)
			return;

		if (IsRuined())
			return;

		PlayerBase player = PlayerBase.Cast(old_owner);
		if (!player)
			player = PlayerBase.Cast(old_owner.GetHierarchyRootPlayer());

		if (!player)
			return;

		// If lifetime doesn't match lifetime max, this is potentially a new item???
		ZenFunctions.DebugMessage("Lifetime=" + GetLifetime() + " Max=" + GetLifetimeMax() + " m_ZenIsVirgin=" + m_ZenIsVirgin);
		player.IncreaseZenLootCyclingCounter();
		m_ZenIsVirgin = false;
	}
#endif
*/
}