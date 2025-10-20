//! LEFTOVERS
static void DropJunk(ActionData action_data)
{
	if (!ZenModEnabled("ZenLeftovers"))
		return;

	PlayerBase player = PlayerBase.Cast(action_data.m_Player);
	if (!player || !action_data.m_MainItem)
		return;

	ItemBase item = action_data.m_MainItem;

	if (item && item.GetQuantity() <= 0.01)
	{
		LeftoverItem li = GetLeftoversConfig().GetLeftoverItem(item.GetType());

		// Namalsk frozen item support
		if (!li || li.OriginalItemType == "" || li.LeftoverItemType == "")
		{
			string itemType = item.GetType();
			itemType.ToLower();
			if (itemType.Contains("dzn_"))
			{
				itemType.Replace("dzn_", "");
				li = GetLeftoversConfig().GetLeftoverItem(itemType);
			}
		}

		if (li && li.OriginalItemType != "" && li.LeftoverItemType != "") // Junk item found!
		{
			float health = item.GetHealth();
			if (li.ItemHealth != 0)
				health = li.ItemHealth;

			if (li.DropToGround)
			{
				GetGame().ObjectDelete(player.GetItemInHands());
				ItemBase junk = ItemBase.Cast(GetGame().CreateObjectEx(li.LeftoverItemType, player.GetPosition(), ECE_PLACE_ON_SURFACE));

				if (junk) // Dropped item to the ground
				{
					junk.SetHealth("", "Health", health);
				}
			}
			else // Replacing item in hands
			{
				action_data.m_Player.GetHumanInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ReplaceJunkLambda(item, li.LeftoverItemType, health));
			}
		}
	}
}

// Drop junk food items
modded class ActionConsume : ActionContinuousBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		DropJunk(action_data);
	};
}

// Drop junk food items when feeding other players
modded class ActionForceConsume : ActionContinuousBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		DropJunk(action_data);
	};
}

// Drop junk saline bag on self
modded class ActionGiveSalineSelf
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		DropJunk(action_data);
	}
}

// Drop junk saline bag on other player
modded class ActionGiveSalineTarget
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		DropJunk(action_data);
	}
}

// Drop junk blood bag on self
modded class ActionGiveBloodSelf : ActionContinuousBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		DropJunk(action_data);
	};
}

// Drop junk blood bag on other player
modded class ActionGiveBloodTarget : ActionContinuousBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);
		DropJunk(action_data);
	};
}

// Drop junk syringe on injecting self
modded class ActionInjectSelf : ActionSingleUseBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_WasExecuted && action_data.m_MainItem)
		{
			DropJunk(action_data);
		}
	}
}

// Drop junk syringe on other player
modded class ActionInjectTarget : ActionSingleUseBase
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_WasExecuted && action_data.m_MainItem)
		{
			DropJunk(action_data);
		}
	}
}

// This lambda replaces the given item with a new item - custom lamda allows for copying health & quantity properties.
class ReplaceJunkLambda : ReplaceItemWithNewLambdaBase
{
	float m_NewItemHealth = 1;

	void ReplaceJunkLambda(EntityAI old_item, string new_item_type, float health)
	{
		m_OldItem = old_item;
		m_NewItemType = new_item_type;
		m_NewItemHealth = health;
	}

	override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
	{
		ItemBase newItem = ItemBase.Cast(new_item);

		if (newItem)
		{
			newItem.SetHealth(m_NewItemHealth);
			newItem.SetQuantity(0.01, false, false); // false, false overrides the game deleting some edibles with <= 0 quantity.
			newItem.SetLifetimeMax(GetLeftoversConfig().JunkLifetimeSecs);
			newItem.SetLifetime(GetLeftoversConfig().JunkLifetimeSecs);
		}
	}
};