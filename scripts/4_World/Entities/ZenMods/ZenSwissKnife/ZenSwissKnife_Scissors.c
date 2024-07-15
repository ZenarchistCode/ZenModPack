class ZenSwissKnife_Scissors extends KitchenKnife
{
	protected float m_BaseHealth;
	protected float m_ScrewdriverHealth;
	protected float m_CanOpenerHealth;
	protected float m_KnifeHealth;
	protected float m_LockpickHealth;
	protected float m_SawHealth;

	void ZenSwissKnife_Scissors()
	{
		ZenSwissKnife.ShowToolType(this, "scissors");
	}

	override bool NameOverride(out string output)
	{
		output = GetGame().ConfigGetTextOut("CfgVehicles " + GetType() + " displayName") + " (#STR_CfgVehicles_SewingKit0)";
		return true;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionZenSwissKnifeConvert_BaseTool);

		RemoveAction(ActionBurnSewTarget);
		RemoveAction(ActionSkinning);
		RemoveAction(ActionBurnSewSelf);
		RemoveAction(ActionDigWorms);
		RemoveAction(ActionShaveTarget);
		RemoveAction(ActionDisarmMine);
		RemoveAction(ActionDisarmExplosive);
		RemoveAction(ActionShave);
		RemoveAction(ActionCraftBolts);
	}

	override bool IsMeleeFinisher()
	{
		return false;
	}

	// If tool is ruined, set it back to packed tool state and make this tool no longer usable
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (GetGame().IsDedicatedServer())
		{
			if (newLevel == GameConstants.STATE_RUINED)
				ZenSwissKnife.ConvertKnife(this);
			else
			{
				UpdateToolTexture();
				SetSynchDirty();
			}
		}
	}
	
	void GetSwissToolHealthOut(out float base, out float canopener, out float screwdriver, out float knife, out float saw, out float lockpick, out float sewing)
	{
		base				= m_BaseHealth;
		canopener			= m_CanOpenerHealth;
		screwdriver			= m_ScrewdriverHealth;
		knife				= m_KnifeHealth;
		saw					= m_SawHealth;
		lockpick			= m_LockpickHealth;
		sewing				= GetHealth01();
	}

	void SetSwissToolHealthConvert(float base, float canopener, float screwdriver, float knife, float saw, float lockpick, float sewing)
	{
		SetHealth01("","", sewing);

		m_BaseHealth		= base;
		m_CanOpenerHealth	= canopener;
		m_ScrewdriverHealth = screwdriver;
		m_KnifeHealth		= knife;
		m_LockpickHealth	= lockpick;
		m_SawHealth			= sewing;

		UpdateToolTexture();
		SetSynchDirty();

		//ZenSwissKnife.DebugMessage("base=" + base + " canopener=" + canopener + " screwdriver=" + screwdriver + " knife=" + knife + " lockpick=" + lockpick + " sewing=" + sewing);
	}

	float GetSwissToolHealth(string toolType)
	{
		switch (toolType)
		{
			case "ZenSwissKnife_CanOpener":		return m_CanOpenerHealth;
			case "ZenSwissKnife_Screwdriver":	return m_ScrewdriverHealth;
			case "ZenSwissKnife_Knife":			return m_KnifeHealth;
			case "ZenSwissKnife_Saw":			return m_SawHealth;
			case "ZenSwissKnife_Lockpick":		return m_LockpickHealth;
			case "ZenSwissKnife_Scissors":		return GetHealth01();
		}

		return -1;
	}

	//! SAVE/LOAD TOOL HEALTH
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write(m_BaseHealth);
		ctx.Write(m_ScrewdriverHealth);
		ctx.Write(m_CanOpenerHealth);
		ctx.Write(m_KnifeHealth);
		ctx.Write(m_LockpickHealth);
		ctx.Write(m_SawHealth);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_BaseHealth))
			m_BaseHealth = 1;

		if (!ctx.Read(m_ScrewdriverHealth))
			m_ScrewdriverHealth = 1;

		if (!ctx.Read(m_CanOpenerHealth))
			m_CanOpenerHealth = 1;

		if (!ctx.Read(m_KnifeHealth))
			m_KnifeHealth = 1;

		if (!ctx.Read(m_LockpickHealth))
			m_LockpickHealth = 1;

		if (!ctx.Read(m_SawHealth))
			m_SawHealth = 1;

		UpdateToolTexture();
		return true;
	}

	void UpdateToolTexture()
	{
		ZenSwissKnife.UpdateToolTexture(this);
	}
}