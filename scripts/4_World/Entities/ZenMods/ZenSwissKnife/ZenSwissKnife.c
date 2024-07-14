// Base class - no tool selected
class ZenSwissKnife extends ToolBase
{
	static const bool SWISS_KNIFE_DEBUG = false; // Only turn on when debugging

	static const float DURATION_NERF = 2; // Actions take 2x longer with Swiss tool
	static const int TOOL_COUNT = 6;

	static const string RUINED_TEXTURE			= "ZenModPack/data/textures/swissknife/swissknife_destruct_co.paa";
	static const string DAMAGED_TEXTURE			= "ZenModPack/data/textures/swissknife/swissknife_damage_co.paa";
	static const string DAMAGED_BASE_TEXTURE	= "ZenModPack/data/textures/swissknife/swissknife_damage_base_co.paa";
	static const string PRISTINE_TEXTURE		= "ZenModPack/data/textures/swissknife/swissknife_co.paa";

	protected float m_CanOpenerHealth	= 1;
	protected float m_ScrewdriverHealth = 1;
	protected float m_KnifeHealth		= 1;
	protected float m_SawHealth			= 1;
	protected float m_LockpickHealth	= 1;
	protected float m_ScissorsHealth	= 1;

	void ZenSwissKnife()
	{
		ShowToolType(this, "packed");
		RegisterSwissNetSync();
	}

	protected void RegisterSwissNetSync()
	{
		RegisterNetSyncVariableFloat("m_CanOpenerHealth");
		RegisterNetSyncVariableFloat("m_ScrewdriverHealth");
		RegisterNetSyncVariableFloat("m_KnifeHealth");
		RegisterNetSyncVariableFloat("m_SawHealth");
		RegisterNetSyncVariableFloat("m_LockpickHealth");
		RegisterNetSyncVariableFloat("m_ScissorsHealth");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionZenSwissKnifeConvert_Knife);
		AddAction(ActionZenSwissKnifeConvert_Screwdriver);
		AddAction(ActionZenSwissKnifeConvert_SmallSaw);
		AddAction(ActionZenSwissKnifeConvert_CanOpener);
		AddAction(ActionZenSwissKnifeConvert_Lockpick);
		AddAction(ActionZenSwissKnifeConvert_Scissors);
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (GetGame().IsDedicatedServer())
		{
			UpdateToolTexture(this, true);
			SetSynchDirty();
		}
	}

	void GetSwissToolHealthOut(out float base, out float canopener, out float screwdriver, out float knife, out float saw, out float lockpick, out float sewing)
	{
		base				= GetHealth01("","");
		canopener			= m_CanOpenerHealth;
		screwdriver			= m_ScrewdriverHealth;
		knife				= m_KnifeHealth;
		saw					= m_SawHealth;
		lockpick			= m_LockpickHealth;
		sewing				= m_ScissorsHealth;
	}

	void SetSwissToolHealthConvert(float base, float canopener, float screwdriver, float knife, float saw, float lockpick, float sewing)
	{
		m_CanOpenerHealth	= canopener;
		m_ScrewdriverHealth = screwdriver;
		m_KnifeHealth		= knife;
		m_SawHealth			= saw;
		m_LockpickHealth	= lockpick;
		m_ScissorsHealth	= sewing;

		// Set base tool health to average of all tools
		SetHealth01("", "", Math.Min(GetToolHealth01(), base));

		UpdateToolTexture(this, true);
		SetSynchDirty();
	}

	// Average of all tools
	float GetToolHealth01()
	{
		return (m_CanOpenerHealth + m_ScrewdriverHealth + m_KnifeHealth + m_SawHealth + m_LockpickHealth + m_ScissorsHealth) / TOOL_COUNT;
	}

	float GetSwissToolHealth(string toolType)
	{
		switch (toolType)
		{
			case "ZenSwissKnife_CanOpener":		return m_CanOpenerHealth;
			case "ZenSwissKnife_Screwdriver":	return m_ScrewdriverHealth;
			case "ZenSwissKnife_Knife":			return m_KnifeHealth;
			case "ZenSwissKnife_SmallSaw":		return m_SawHealth;
			case "ZenSwissKnife_Lockpick":		return m_LockpickHealth;
			case "ZenSwissKnife_Scissors":		return m_ScissorsHealth;
		}

		return -1;
	}

	//! SAVE/LOAD TOOL HEALTH
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write(m_CanOpenerHealth);
		ctx.Write(m_ScrewdriverHealth);
		ctx.Write(m_KnifeHealth);
		ctx.Write(m_SawHealth);
		ctx.Write(m_LockpickHealth);
		ctx.Write(m_ScissorsHealth);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_CanOpenerHealth))
			m_CanOpenerHealth = 1;

		if (!ctx.Read(m_ScrewdriverHealth))
			m_ScrewdriverHealth = 1;

		if (!ctx.Read(m_KnifeHealth))
			m_KnifeHealth = 1;

		if (!ctx.Read(m_SawHealth))
			m_SawHealth = 1;

		if (!ctx.Read(m_LockpickHealth))
			m_LockpickHealth = 1;

		if (!ctx.Read(m_ScissorsHealth))
			m_ScissorsHealth = 1;

		UpdateToolTexture(this, true);
		return true;
	}

	static void UpdateToolTexture(EntityAI knife, bool isBase = false)
	{
		if (knife.IsRuined())
		{
			knife.SetObjectTexture(0, RUINED_TEXTURE);
		} else
		if (knife.GetHealth01() <= GameConstants.DAMAGE_DAMAGED_VALUE)
		{
			if (isBase)
				knife.SetObjectTexture(0, DAMAGED_BASE_TEXTURE);
			else
				knife.SetObjectTexture(0, DAMAGED_TEXTURE);
		} else
		{
			knife.SetObjectTexture(0, PRISTINE_TEXTURE);
		}
	}

	//! SHOW/HIDE TOOL SIMPLE SELECTIONS
	static void ShowToolType(ItemBase item, string selectionName)
    {
        TStringArray selectionNames = new TStringArray;
        item.ConfigGetTextArray("simpleHiddenSelections", selectionNames);

		if (!selectionNames)
			return;

        int selectionId;
		foreach (string name : selectionNames)
		{
			selectionId = selectionNames.Find(name);
			if (name == selectionName)
				item.SetSimpleHiddenSelectionState(selectionId, 1);
			else 
				item.SetSimpleHiddenSelectionState(selectionId, 0);
		}
    }

	//! TEST FOR KNIFE OBJECT IN ACTION DATA 
	static bool IsSwissKnifeUsed(ActionData action_data)
	{
		if (action_data == NULL)
			return false;

		if (action_data.m_MainItem != NULL && action_data.m_MainItem.GetType().Contains("ZenSwissKnife"))
		{
			return true;
		}

		if (action_data.m_Target != NULL)
		{
			Object targetObj = action_data.m_Target.GetObject();
			if (targetObj != NULL && targetObj.GetType().Contains("ZenSwissKnife"))
				return true;
		}

		return false;
	}

	//! CONVERT KNIFE TOOL
	static void ConvertKnife(notnull EntityAI old_item, string new_item = "ZenSwissKnife")
	{
		old_item.GetInventory().ReplaceItemWithNew(InventoryMode.SERVER, new ReplaceZenSwissKnifeLambda(old_item, new_item));
	}

	static void DebugMessage(string msg)
	{
		if (SWISS_KNIFE_DEBUG)
			ZenFunctions.DebugMessage(msg);
	}
}
