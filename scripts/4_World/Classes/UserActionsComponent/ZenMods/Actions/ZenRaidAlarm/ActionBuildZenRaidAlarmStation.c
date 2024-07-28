class ActionBuildZenRaidAlarmStationCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BASEBUILDING_REPAIR_FAST * 5);
	}
}

class ActionBuildZenRaidAlarmStation: ActionContinuousBase
{
	void ActionBuildZenRaidAlarmStation()
	{
		m_CallbackClass = ActionBuildZenRaidAlarmStationCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_FullBody = true;
		m_Text = "#STR_ZenGui_RaidAlarmBuild";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursorNoRuinCheck(UAMaxDistances.SMALL);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(target.GetObject());
		if (station)
		{
			BaseRadio radio = station.GetBaseRadio();
			if (radio != NULL && !radio.IsLockedInSlot())
			{
				return true;
			}
		}

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
		if (!action_data.m_Player || !action_data.m_Player.GetIdentity())
			return;

		if (GetZenDiscordConfig().RaidAlarmRequiresTerritory)
		{
			if (!Zen_RaidAlarmStation.GetTerritoryPermission(action_data.m_Player.GetCachedID(), action_data.m_Player.GetPosition()))
			{
				NotificationSystem.SendNotificationToPlayerIdentityExtended(action_data.m_Player.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, GetZenDiscordConfig().RaidAlarmRequiresTerritoryText, "set:ccgui_enforce image:MapShieldBooster");
				return;
			}
		}

		ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
		if (!plugin)
			return;

		if (plugin.GetNearestRaidStation(action_data.m_Target.GetObject().GetPosition()) != NULL)
		{
			NotificationSystem.SendNotificationToPlayerIdentityExtended(action_data.m_Player.GetIdentity(), 15.0, GetZenDiscordConfig().RaidAlarmMessageTitle, GetZenDiscordConfig().RaidAlarmTooClose, "set:ccgui_enforce image:MapShieldBooster");
			return;
		}

		MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, action_data.m_MainItem.GetMaxHealth() * 0.5);
		Zen_RaidAlarmStation station = Zen_RaidAlarmStation.Cast(action_data.m_Target.GetObject());
		if (station)
		{
			station.LockBaseRadio(true);
			station.InitBaseName();
		}
	}
}