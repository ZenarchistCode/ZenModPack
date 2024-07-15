class ActionMineCoopHandsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(10);
	}
};

class ActionMineCoopHands : ActionContinuousBase
{
	void ActionMineCoopHands()
	{
		m_CallbackClass = ActionMineCoopHandsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
		string featherDisplayName = "Feather";

		// Get feather display name (doing it this way automatically displays local language translation)
		string featherConfigPath = "CfgVehicles ChickenFeather displayName";
		if (GetGame().ConfigIsExisting(featherConfigPath))
		{
			GetGame().ConfigGetText(featherConfigPath, featherDisplayName);
		}

		m_Text = "#harvest" + " " + featherDisplayName;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// Only allow collecting feathers by hand if there is no item, or the item in hands is a feather
		if (item && !item.IsInherited(ChickenFeather))
			return false;

		Object targetObject = target.GetObject();
		return targetObject && targetObject.GetType() && targetObject.GetType().Contains("Zen_ChickenCoop");
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data.m_Target || !action_data.m_Target.GetObject() || !action_data.m_Player)
			return;

		// Get target object
		Object target = action_data.m_Target.GetObject();
		int feathersLeft = 0;

		// Get chicken coop
		Zen_ChickenCoop chickenCoop = Zen_ChickenCoop.Cast(target);
		if (chickenCoop)
		{
			feathersLeft = chickenCoop.GetFeathersLeft();
		}
		else
		{
			// No valid coop object found
			if (GetZenChickenCoopsConfig().DebugOn)
			{
				action_data.m_Player.Zen_SendMessage("ERROR: No valid chicken coop object found - in debug mode getting feathers won't work!");
			}
			
			return;
		}

		if (feathersLeft <= 0)
		{
			action_data.m_Target.GetObject().Delete();
			action_data.m_Player.Zen_SendMessage(GetZenChickenCoopsConfig().NoFeathersLeftMessage);
			return;
		}

		// Attempt to create feather in player hands, if not possible just drop it onto the ground
		ItemBase chickenFeather = ItemBase.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("ChickenFeather"));
		if (!chickenFeather)
		{
			chickenFeather = ItemBase.Cast(GetGame().CreateObjectEx("ChickenFeather", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
		}

		if (!chickenFeather)
		{
			ZenChickenCoopsLogger.Log("ERROR: Failed to create feathers for object @ " + chickenCoop.GetPosition());
			return;
		}

		int featherQuantity = Math.RandomIntInclusive(1, GetZenChickenCoopsConfig().GetMaxFeathersCollection(chickenCoop.GetType()));
		chickenFeather.SetHealth(Math.RandomIntInclusive(1, 5));
		chickenFeather.SetQuantity(featherQuantity);
		chickenCoop.ConsumeFeathers(featherQuantity);

		// Check for gloves
		ItemBase gloves = ItemBase.Cast(action_data.m_Player.FindAttachmentBySlotName("Gloves"));
		if (gloves && !gloves.IsDamageDestroyed())
		{
			gloves.DecreaseHealth("", "", GetZenChickenCoopsConfig().GetGloveDamage(target.GetType()));
			return;
		}

		// Give player a bleed if enabled in config
		float chanceOfBleed = GetZenChickenCoopsConfig().GetChanceOfBleed(target.GetType());
		if (Math.RandomFloat01() < chanceOfBleed)
		{
			int rand = Math.RandomIntInclusive(0, 1);
			if (rand == 0 && !action_data.m_Player.GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("LeftForeArmRoll"))
			{
				action_data.m_Player.GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("RightForeArmRoll");
			}
			else 
			if (rand == 1 && !action_data.m_Player.GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("RightForeArmRoll"))
			{
				action_data.m_Player.GetBleedingManagerServer().AttemptAddBleedingSourceBySelection("LeftForeArmRoll");
			}
		}
	}
};