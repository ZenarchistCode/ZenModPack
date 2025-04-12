class ActionDismantleZenRaidAlarmRadar: ActionContinuousBase
{
	void ActionDismantleZenRaidAlarmRadar()
	{
		m_CallbackClass = ActionBuildZenRaidAlarmStationCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_Text = "#dismantle";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.SMALL);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Zen_RaidAlarmRadar radar = Zen_RaidAlarmRadar.Cast(target.GetObject());
		if (radar != NULL)
			return !radar.GetCompEM().IsPlugged();

		return false;
	}
	
	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);

#ifdef SERVER
		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(true, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
#endif
	}

	override void OnEnd(ActionData action_data)
	{
#ifdef SERVER
		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(false, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
#endif
	}

	override void OnEndAnimationLoop(ActionData action_data)
	{
		super.OnEndAnimationLoop(action_data);

#ifdef SERVER
		ItemBase target_IB = ItemBase.Cast(action_data.m_Target.GetObject());
		Param2<bool, string> play = new Param2<bool, string>(false, action_data.m_MainItem.GetLockSoundSet());
		GetGame().RPCSingleParam(target_IB, ERPCs.RPC_SOUND_LOCK_ATTACH, play, true);
#endif
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		if (GetZenDiscordConfig().RaidAlarmRequiresTerritory)
		{
			if (!Zen_RaidAlarmStation.GetTerritoryPermission(action_data.m_Player.GetCachedID(), action_data.m_Player.GetPosition()))
			{
				NotificationSystem.SendNotificationToPlayerIdentityExtended(action_data.m_Player.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, GetZenDiscordConfig().RaidAlarmRequiresTerritoryText, "set:ccgui_enforce image:MapShieldBooster");
				return;
			}
		}

		MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, action_data.m_MainItem.GetMaxHealth() * 0.5);

		Zen_RaidAlarmRadar radar = Zen_RaidAlarmRadar.Cast(action_data.m_Target.GetObject());
		if (radar)
		{
			if (radar.GetCompEM().IsPlugged())
				return;

			EntityAI newItem = EntityAI.Cast(GetGame().CreateObjectEx("Zen_RaidAlarmRadarKit", action_data.m_Player.GetPosition(), ECE_PLACE_ON_SURFACE));
			newItem.SetHealth01("", "", radar.GetHealth01("", "Health"));
			radar.DeleteSafe();
		}
	}
}