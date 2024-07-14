class ActionPackZenCamoShelter : ActionContinuousBase
{
	void ActionPackZenCamoShelter()
	{
		m_CallbackClass = ActionPackTentCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DEPLOY_2HD;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#pack_tent";
	}

	override void CreateConditionComponents()
	{
		m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasProgress()
	{
		return true;
	}

	override bool HasAlternativeInterrupt()
	{
		return true;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		return true;
	}

	override ActionData CreateActionData()
	{
		PlaceObjectActionData action_data = new PlaceObjectActionData;
		return action_data;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		//Action not allowed if player has broken legs
		if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			return false;

		Object targetObject = target.GetObject();

		if (player && targetObject)
		{
			Zen_ImprovisedShelter shelter = Zen_ImprovisedShelter.Cast(targetObject);
			if (shelter)
			{
				return shelter.IsEmpty();
				/*if (GetGame().IsDedicatedServer())
				{
					return shelter.IsEmpty();
				} 
				else
				{
					Zen_ImprovisedShelterInventory inventoryObj = shelter.GetInventoryObject();
					if (inventoryObj)
					{
						return inventoryObj.IsEmpty() && inventoryObj.GetInventory().AttachmentCount() == 0;
					}
				}*/
			}
		}

		return false;
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);
		action_data.m_Player.TryHideItemInHands(true);
	}

	override void OnEnd(ActionData action_data)
	{
		super.OnEnd(action_data);
		action_data.m_Player.TryHideItemInHands(false);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetObject();
		Zen_ImprovisedShelter shelter = Zen_ImprovisedShelter.Cast(targetObject);

		// Validate object existence
		if (!shelter)
			return;

		// Spawn kit
		Zen_CamoShelterKit kit = Zen_CamoShelterKit.Cast(GetGame().CreateObjectEx("Zen_CamoShelterKit", shelter.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS));

		// Create attachments
		EntityAI rope = EntityAI.Cast(kit.GetInventory().CreateInInventory("Rope"));
		EntityAI net = EntityAI.Cast(kit.GetInventory().CreateInInventory("CamoNet"));

		if (rope)
			rope.SetHealth(shelter.GetRopeHealth());

		if (net)
			net.SetHealth(shelter.GetCamoNetHealth());

		EntityAI stick = EntityAI.Cast(kit.GetInventory().CreateInInventory("LongWoodenStick"));

		if (stick)
			stick.SetHealth(Math.RandomIntInclusive(0, 1));

		// Delete shelter and drop kit
		GetGame().ObjectDelete(shelter);
	}
};