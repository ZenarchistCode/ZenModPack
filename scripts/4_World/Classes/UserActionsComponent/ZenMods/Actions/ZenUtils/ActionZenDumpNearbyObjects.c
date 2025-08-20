class ActionZenDumpNearbyObjectsCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(0.1);
	}
}

class ActionZenDumpNearbyObjects: ActionContinuousBase
{	
	void ActionZenDumpNearbyObjects()
	{
		m_CallbackClass = ActionZenDumpNearbyObjectsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENITEM;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_OPENITEM;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_Text = "Dump nearby objects to logs";
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool HasTarget()
	{
		return false;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		array<Object> objectsOnMap = new array<Object>;
		GetGame().GetObjectsAtPosition3D(action_data.m_Player.GetPosition(), 5, objectsOnMap, null);

		// Cycle through all map objects
		for (int x = 0; x < objectsOnMap.Count(); x++)
		{
			// Prepare classname(s)
			string debugName = "";
			string className = "";

			// Get classname(s)
			debugName = objectsOnMap[x].GetDebugNameNative();
			className = objectsOnMap[x].ClassName();

			Print("Object Found [DEBUGNAME] " + debugName);
			Print("Object Found [CLASSNAME] " + className);

			ZenFunctions.DebugMessage("DUMP OBJECT: " + debugName + ", " + className);
		}
	}
}