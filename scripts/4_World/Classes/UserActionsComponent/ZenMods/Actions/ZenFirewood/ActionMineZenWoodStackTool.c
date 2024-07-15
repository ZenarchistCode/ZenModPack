class ActionMineZenWoodStackToolCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionMineZenWoodStackTool : ActionContinuousBase
{
	void ActionMineZenWoodStackTool()
	{
		m_CallbackClass = ActionMineZenWoodStackToolCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
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
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTObject(2.5); // 2.5m distance from object
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		return targetObject && targetObject.GetType() && targetObject.GetType().Contains("Zen_WoodReserve") && item && !item.IsRuined();
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		if (!action_data.m_Target || !action_data.m_Target.GetObject())
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

		// Create the firewood on the ground
		action_data.m_MainItem.DecreaseHealth("", "", GetZenFirewoodConfig().GetToolDamage(target.GetType()));
		ItemBase woodItem = ItemBase.Cast(GetGame().CreateObjectEx("FireWood", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));

		if (woodItem)
		{
			woodItem.SetHealth(Math.RandomIntInclusive(0, 500));
			woodReserve.ConsumeWood();
		}
	}
};