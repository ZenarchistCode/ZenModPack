class ZenPetrolLighter extends PetrolLighter
{
	static const string START_SOUND = "Lighter_Soundset";

	protected ZenZippoLight m_Light;
	protected Particle m_FireParticle;
	protected EffectSound m_LighterStartSound;
	protected EffectSound m_LighterStopSound;
	protected bool m_PlayLightSound;

	void ZenPetrolLighter()
	{
		PrepareLighter();
	}

	// Set lighter to off state on object creation and have server sync its status
	void PrepareLighter()
	{
		RegisterNetSyncVariableBoolSignal("m_PlayLightSound");
		StopLighterFX();
	}

	// Stop light & fx on despawn
	void ~ZenPetrolLighter()
	{
		StopLighterFX();
	}

	// Turn off after server restart
	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsDedicatedServer())
			TurnOffOnServer();
	}

	void TurnOffOnServer()
	{
		if (GetCompEM() && GetCompEM().IsWorking())
			GetCompEM().SwitchOff();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_PlayLightSound)
		{
			PlaySoundSet(m_LighterStartSound, START_SOUND, 0, 0);
		}
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		if (GetGame().IsDedicatedServer()) // Server side - play sound only when lighter starts for first time
		{
			m_PlayLightSound = true;
			SetSynchDirty();
		}
	}

	override void OnWork(float consumed_energy) 
	{
		super.OnWork(consumed_energy);

		if (GetGame().IsClient()) // Client side
		{
			StartLighterFX();
		}
	}
	
	override void OnWorkStop()
	{
		super.OnWorkStop();

		if (GetGame().IsClient()) // Client side
		{
			StopLighterFX();
		}
	}

	//! CLIENT
	void StartLighterFX()
	{
		CreateFlameLight();
		CreateFlameParticle();
		AttachFlameParticle();
	}

	//! CLIENT
	void StopLighterFX()
	{
		if (m_Light)
			m_Light.FadeOut();

		if (m_FireParticle)
			m_FireParticle.StopParticle();
	}

	// Zippo can stay lit on ground
	bool StopLighterOnGround()
	{
		return true;
	}

	// Turn off if moved out of hands. autoSwitchOffWhenInCargo in .cpp takes care of turn off in cargo.
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		if (GetGame().IsDedicatedServer() && newLoc.GetType() != InventoryLocationType.HANDS && StopLighterOnGround())
		{
			TurnOffOnServer();
		}
	}

	// Turn off after server restart
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

		TurnOffOnServer();
	}

	protected void CreateFlameLight()
	{
		if (!m_Light)
		{
			m_Light = ZenZippoLight.Cast(ScriptedLightBase.CreateLight(ZenZippoLight, Vector(0,0,0), 1));
			m_Light.AttachOnObject(this, Vector(0,0,0));
			m_Light.SetFadeOutTime(0.1);
		}
	}

	protected void CreateFlameParticle()
	{
		if (!m_FireParticle)
			m_FireParticle = Particle.PlayOnObject(ParticleList.ZEN_ZIPPO_FLAME, this, Vector(0,0.02,-0.005), Vector(0,0,0), true);
	}

	protected void AttachFlameParticle()
	{
		if (!m_FireParticle)
			return;

		// Attach particle to light
		m_FireParticle.SetWiggle(2, 0.1);
		Object direct_particle = m_FireParticle.GetDirectParticleEffect();
		if (direct_particle && direct_particle != m_Light.GetAttachmentParent())
		{
			m_Light.AttachOnObject(direct_particle, Vector(0,0,0));
		}
	}

	override bool IsIgnited()
	{
		return GetCompEM().IsWorking();
	}

	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		if (IsIgnited())	
			return true;
		else
			return false;
	}

	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTurnOnWhileInHands);
		AddAction(ActionTurnOffWhileInHands);
	}
}