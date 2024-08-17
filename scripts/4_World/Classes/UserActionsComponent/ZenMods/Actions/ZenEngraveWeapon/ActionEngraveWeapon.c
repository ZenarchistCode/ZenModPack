class ActionZenEngraveWeaponCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(15);
	}
};

class ActionZenEngraveWeapon : ActionContinuousBase
{
	void ActionZenEngraveWeapon()
	{
		m_CallbackClass = ActionZenEngraveWeaponCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursor(UAMaxDistances.SMALL);
	}

	override string GetText()
	{
		return "#STR_ZenGui_EngraveWeapon";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenEngraveWeapon"))
			return false;

		Weapon_Base rifle = Weapon_Base.Cast(target.GetObject());
		if (!rifle)
		{
			return false;
		}

		if (rifle.IsRuined() || rifle.GetPlayerName() != "")
		{
			return false;
		}

		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		PlayerBase player = action_data.m_Player;

		if (!player || !action_data.m_Target || !action_data.m_Target.GetObject() || !action_data.m_MainItem)
			return;

		Weapon_Base rifle = Weapon_Base.Cast(action_data.m_Target.GetObject());
		if (!rifle)
			return;

		rifle.SetPlayerName(player.GetCachedName());
		action_data.m_MainItem.SetHealth(0);
	}

	override string GetSoundCategory(ActionData action_data)
	{
		return "Zen_CraftBolts";
	}
}
