class ActionZenDigIceFishingHoleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float time = 20; // 20 secs for anything other than iceaxe, iceaxe = 50% duration
		if (m_ActionData.m_MainItem.Type() == Iceaxe)
			time = time / 2;

		m_ActionData.m_ActionComponent = new CAContinuousTime(time);
	}
};

class ActionZenDigIceFishingHole : ActionContinuousBase
{
	void ActionZenDigIceFishingHole()
	{
		m_CallbackClass = ActionZenDigIceFishingHoleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGMANIPULATE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_ZenGui_DigIceHole";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTObject(UAMaxDistances.BASEBUILDING);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!target.GetObject())
			return false;

		return target.GetObject().Type() == Land_ZenIceSheet_4x4;
	}

	override void Start(ActionData action_data)
	{
		super.Start(action_data);

		if (action_data.m_Target.GetObject())
		{
			ZenFunctions.OrientObjectToPosition(action_data.m_Player, action_data.m_Target.GetObject().GetPosition());
		}
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		Object targetObject = action_data.m_Target.GetObject();
		if (!targetObject)
			return;

		// Replace with hole
		Object newIce = GetGame().CreateObjectEx(targetObject.GetType() + "_Hole", targetObject.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);
		newIce.SetOrientation(targetObject.GetOrientation());
		newIce.SetOrientation(newIce.GetOrientation());
		newIce.SetScale(targetObject.GetScale());
		newIce.SetPosition(targetObject.GetPosition() - "0 0.001 0"); // Not sure why... objects are identical .p3d's except hole but don't align properly
		newIce.Update();
		newIce.SetAffectPathgraph(true, false);
		GetGame().ObjectDelete(targetObject);

		if (newIce.CanAffectPathgraph())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, newIce);
		}

		float dmg = -0.4; // 40% for pickaxe/any other item
		if (action_data.m_MainItem.Type() == Iceaxe)
			dmg = -0.1; // 10% for iceaxe

		action_data.m_MainItem.AddHealth(action_data.m_MainItem.GetMaxHealth("","") * dmg);
	}
}
