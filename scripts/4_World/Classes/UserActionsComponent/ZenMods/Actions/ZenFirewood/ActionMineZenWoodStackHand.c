class ActionMineZenWoodStackHandCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(10);
	}
};

class ActionMineZenWoodStackHand : ActionContinuousBase
{
	void ActionMineZenWoodStackHand()
	{
		m_CallbackClass = ActionMineZenWoodStackHandCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
		string firewoodDisplayName = "Firewood";

		string firewoodConfigPath = "CfgVehicles Firewood displayName";
		if (GetGame().ConfigIsExisting(firewoodConfigPath))
		{
			GetGame().ConfigGetText(firewoodConfigPath, firewoodDisplayName);
		}

		m_Text = "#harvest" + " " + firewoodDisplayName;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTObject(2.5); // 2.5m distance from object
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// Only allow collecting firewood by hand if there is no item, or the item in hands is firewood
		if (item && item.GetType() != "Firewood")
			return false;

		Object targetObject = target.GetObject();
		return targetObject && targetObject.GetType() && targetObject.GetType().Contains("Zen_WoodReserve");
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data.m_Target || !action_data.m_Target.GetObject() || !action_data.m_Player)
			return;

		// Get target object
		Object target = action_data.m_Target.GetObject();
		int woodLeft = 0;

		// Get wood reserve/pile
		Zen_WoodReserve woodReserve = Zen_WoodReserve.Cast(target);
		if (woodReserve)
		{
			woodLeft = woodReserve.GetWoodLeft();
		}
		else
		{
			// No valid wood stack object found
			if (GetZenFirewoodConfig().DebugOn)
			{
				action_data.m_Player.Zen_SendMessage("ERROR: No valid wood object found - in debug mode getting firewood won't work!");
			}
			
			return;
		}

		if (woodLeft <= 0)
		{
			action_data.m_Target.GetObject().Delete();
			action_data.m_Player.Zen_SendMessage(GetZenFirewoodConfig().NoWoodLeftMessage);
			return;
		}

		// Attempt to create firewood in player hands, if not possible just drop it onto the ground
		ItemBase woodItem = ItemBase.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("FireWood"));
		if (!woodItem)
		{
			woodItem = ItemBase.Cast(GetGame().CreateObjectEx("FireWood", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
		}

		if (woodItem)
		{
			woodItem.SetHealth(Math.RandomIntInclusive(0, 500));
			woodReserve.ConsumeWood();

			// Check for gloves
			ItemBase gloves = ItemBase.Cast(action_data.m_Player.FindAttachmentBySlotName("Gloves"));
			if (gloves && !gloves.IsDamageDestroyed())
			{
				gloves.DecreaseHealth("", "", GetZenFirewoodConfig().GetGloveDamage(target.GetType()));
				return;
			}

			// Give player a bleed if enabled in config
			float chanceOfBleed = GetZenFirewoodConfig().GetChanceOfBleed(target.GetType());
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
	}
};