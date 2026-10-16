modded class ItemBase
{	
	//! SHARED
	static int ZENMOD_ITEM_COUNT = 0;

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

		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ZenLogItemStuff, 1000);
	}

	void ZenLogItemStuff()
	{
		string type = GetType();
		type.ToLower();

		if (GetZenUtilitiesConfig().FindItemLog(type))
		{
			float qty = GetQuantity();
			if (GetType().Contains("HDSN_BreachingCharge"))
				qty = 1;

			ZenItemStatsLogger.IncreaseItemCount(type, qty);
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

	//! NBC CASE 
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		if (GetHierarchyParent() != NULL)
		{
			// Dont allow cargo if we're attached to NBC case.
			if (GetHierarchyParent().IsInherited(Zen_NBCCase))
				return false;
		}

		return true;
	}
	
	//! AUTO-STACK
	private int m_Zen_LastAutoStackMS;

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		if (!m_CanThisBeSplit)
			return;

		if (GetCleanness() > 0)
			return;

		// client only
		if (g_Game.IsDedicatedServer())
			return;
		
		if (!ZenModEnabled("ZenAutoStackItems"))
			return;
		
		int settingType = ZenOptions.GetIndex("ZEN.ZenAutoStack", 0);
		if (settingType == 0)
			return;
		
		// only react to local player
		PlayerBase localPlayer = PlayerBase.Cast(g_Game.GetPlayer());
		if (!localPlayer || GetHierarchyRootPlayer() != localPlayer)
			return;

		// only when picking up from ground into -> inventory cargo
		if (oldLoc.GetType() != InventoryLocationType.GROUND)
			return;

		if (newLoc.GetType() != InventoryLocationType.CARGO)
			return;

		// throttle a bit (prevents spamming callqueues if multiple events fire)
		int now = g_Game.GetTime();
		if (now - m_Zen_LastAutoStackMS < 150)
			return;

		m_Zen_LastAutoStackMS = now;

		// let the take/juncture finish first
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Zen_TryAutoStackClient);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Zen_TryAutoStackClient, 100, false);
	}

	private void Zen_TryAutoStackClient()
	{
		if (g_Game.IsDedicatedServer())
			return;

		PlayerBase localPlayer = PlayerBase.Cast(g_Game.GetPlayer());
		if (!localPlayer || GetHierarchyRootPlayer() != localPlayer)
			return;
		
		int settingType = ZenOptions.GetIndex("ZEN.ZenAutoStack", 0);

		string myType = GetType();

		array<EntityAI> items = new array<EntityAI>;
		localPlayer.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		// simplest + most robust: first compatible stack, then request combine from server
		foreach (EntityAI ent : items)
		{
			ItemBase other = ItemBase.Cast(ent);
			if (!other || other == this)
				continue;

			if (other.GetType() != myType)
				continue;

			// reservation_check=true helps avoid fighting active inventory ops
			if (!other.CanBeCombined(this, true))
				continue;

			if (other.GetCleanness() != GetCleanness())
				continue;
			
			if (settingType == 1 && other.GetHealthLevel() != GetHealthLevel())
				continue;

			other.CombineItemsClient(this, true);
		}
	}
	
	//! MAPS ON BACKPACK SLOT
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		string slot_name = InventorySlots.GetSlotName(slotId);

		if (super.CanReceiveAttachment(attachment, slotId))
		{
			if (!attachment || !attachment.IsInherited(ChernarusMap))
				return true;

			if (slot_name == "WalkieTalkie" || slot_name == "Chemlight" || slot_name.Contains("VestGrenade"))
				return ZenModEnabled("ZenAllowMapBagSlot");

			return true;
		}

		return false;
	}
	
	// Use CF_Load/Save because this will NOT break server persistence if the mod is added/removed mid-wipe. 
	// NOTE: storage[] must refer to this mod's CfgMods classname EXACTLY or this won't work.
	
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage["ZenModPack"];
		if (!ctx) return;
		
		ctx.Write(m_ZenFirstTouchedTimestamp);
		ctx.Write(m_ZenCreationTimestamp);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage)) return false;

		auto ctx = storage["ZenModPack"];
		if (!ctx) return true;

		if (ctx.GetVersion() >= 1)
		{
			if (!ctx.Read(m_ZenFirstTouchedTimestamp))
				return false;
		
			if (!ctx.Read(m_ZenCreationTimestamp))
				return false;
		}

		return true;
	}	
}