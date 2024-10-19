class ActionPrepareZenFlintCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1);
	}
};

// This action sets the tool which the player wants to use to light fires with the Flint
class ActionPrepareZenFlint : ActionContinuousBase
{
	// List of valid tools for flint striking (anything with steel basically)
	static const ref array<string> TOOLS_FLINT =
	{
		"Hatchet",
		"WoodAxe",
		"Cleaver",
		"CombatKnife",
		"CrudeMachete",
		"FangeKnife",
		"HuntingKnife",
		"KitchenKnife",
		"KukriKnife",
		"Machete",
		"OrientalMachete",
		"Screwdriver",
		"AK_Bayonet",
		"M9A1_Bayonet",
		"Mosin_Bayonet",
		"SKS_Bayonet",
		"Crowbar",
		"PipeWrench",
		"Wrench",
		"Lugwrench",
		"CanOpener",
		"Pickaxe",
		"Iceaxe",
		"Pliers"
	};

	void ActionPrepareZenFlint()
	{
		m_CallbackClass = ActionPrepareZenFlintCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTNonRuined();
	}

	override string GetText()
	{
		return "#STR_ZenGui_PrepareFlint";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item.GetQuantity() == 0)
			return false;

		if (!target.GetObject())
		{
			return false;
		}

		foreach (string s : TOOLS_FLINT)
		{
			if (target.GetObject().IsKindOf(s))
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		PrepareZenFlint(action_data);
	}

	override void OnFinishProgressClient(ActionData action_data)
	{
		PrepareZenFlint(action_data);
	}

	void PrepareZenFlint(ActionData action_data)
	{
		ZenFlint flint = ZenFlint.Cast(action_data.m_MainItem);
		if (!flint)
			return;

		flint.SetZenIgniterItem(ItemBase.Cast(action_data.m_Target.GetObject()), action_data.m_Player);
	}

	override string GetSoundCategory(ActionData action_data)
	{
		return "Zen_FlintStrike";
	}
}
