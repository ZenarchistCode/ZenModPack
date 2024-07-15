class ZenZippoLighter extends ZenPetrolLighter
{
	// Show/hide components from P3D listed as 'sections' in model.cfg (ie. lighter lid)
	protected void ZippoShowSimpleSelection(string selectionName, bool hide = false)
    {
        TStringArray selectionNames = new TStringArray;
        ConfigGetTextArray("simpleHiddenSelections", selectionNames);

		if (!selectionNames)
			return;

        int selectionId = selectionNames.Find(selectionName);
        SetSimpleHiddenSelectionState(selectionId, hide);
    }

	//! CLIENT
	override void StartLighterFX()
	{
		super.StartLighterFX();

		ZippoShowSimpleSelection("top_closed", 0);
		ZippoShowSimpleSelection("top_open", 1);
	}

	//! CLIENT
	override void StopLighterFX()
	{
		super.StopLighterFX();

		ZippoShowSimpleSelection("top_closed", 1);
		ZippoShowSimpleSelection("top_open", 0);
	}

	override void CreateFlameParticle()
	{
		if (!m_FireParticle)
			m_FireParticle = Particle.PlayOnObject(ParticleList.ZEN_ZIPPO_FLAME, this, GetMemoryPointPos("flame"), Vector(0,0,0), true);
	}

	override bool StopLighterOnGround()
	{
		return false;
	}

	override void OnWork(float consumed_energy)
	{
		super.OnWork(consumed_energy);

		#ifdef SERVER
		float dmg = (GetMaxHealth() / ConfigGetInt("varTotalLifetime")) * -1;
		AddHealth(dmg); // Damage lighter slowly over time since it's refillable.
		#endif
	}
}
