modded class SodaCan_ColorBase
{
	//! IMMERSIVE SODAS
	protected EffectSound 	m_PouringLoopSound;
	protected EffectSound 	m_EmptyingLoopSound;

	override string GetDisplayName()
	{
		string displayName = super.GetDisplayName();

		if (IsOpened())
			displayName = "#open " + displayName;

		return displayName;
	}

	void ~SodaCan_ColorBase()
	{
		SEffectManager.DestroyEffect(m_PouringLoopSound);
		SEffectManager.DestroyEffect(m_EmptyingLoopSound);
	}

	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionZenEmptySodaCan);
	}

	override void OnVariablesSynchronized()
	{
		if (IsOpened())
			SetOpenedTexture();
	}

	void SetOpenedTexture()
	{
		// Didn't want to use this method as it's inefficient but can't figure out how to apply damaged rvmats
		string openTex = "ZenModPack/data/textures/immersivesodas/" + GetType() + "_opened_co.paa";
		if (FileExist(openTex))
		{
			SetObjectTexture(0, openTex);
		}
		
		// Very annoying... this func only works on Pristine soda cans for whatever reason, can't figure out how to override damaged rvmats:
		SetObjectMaterial(0, "ZenModPack/data/textures/immersivesodas/sodacan_opened.rvmat");
	}

	// Don't allow open soda cans to be placed in inventory because they'd spill all over your 762 and make it sticky, silly!
	override bool CanPutInCargo(EntityAI parent)
	{
		return !IsOpened();
	}

	bool IsOpened()
	{
		return GetQuantity() != GetQuantityMax();
	}

	override bool IsLiquidPresent()
	{
		return IsOpened() && GetQuantity() > 0.01;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx) 
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		Param1<bool> p = new Param1<bool>(false);
				
		if (!ctx.Read(p))
			return;
		
		bool play = p.param1;
		switch (rpc_type)
		{
			case SoundTypeBottle.POURING:
				if (play)
					PlayPouringLoopSound();
				else
					StopPouringLoopSound();			

				break;
			
			case SoundTypeBottle.EMPTYING:
				if (play)
					PlayEmptyingLoopSound();
				else
					StopEmptyingLoopSound();
			
				break;
		}
	}

	void PlayPouringLoopSound()
	{
		if (!m_PouringLoopSound || !m_PouringLoopSound.IsSoundPlaying())
		{
			m_PouringLoopSound = SEffectManager.PlaySoundOnObject(GetPouringSoundset(), this, 0, 0, true);
		}
	}
	
	void StopPouringLoopSound()
	{
		if (m_PouringLoopSound)
			m_PouringLoopSound.SoundStop();
	}

	void PlayEmptyingLoopSound()
	{
		if (!m_EmptyingLoopSound || !m_EmptyingLoopSound.IsSoundPlaying())
		{
			m_EmptyingLoopSound = SEffectManager.PlaySoundOnObject(GetEmptyingLoopSoundset(), this, 0, 0, true);
		}
	}
	
	void StopEmptyingLoopSound()
	{
		if (m_EmptyingLoopSound)
			m_EmptyingLoopSound.SoundStop();
				
		EffectSound sound =	SEffectManager.PlaySoundOnObject(GetEmptyingEndSoundset(), this);
		sound.SetAutodestroy(true);
	}

	string GetPouringSoundset()
	{
		return "emptyVessle_Canteen_SoundSet";
	}
	
	string GetEmptyingLoopSoundsetHard()
	{
		return "pour_HardGround_Canteen_SoundSet";
	}
	
	string GetEmptyingLoopSoundsetSoft()
	{
		return "pour_SoftGround_Canteen_SoundSet";
	}
	
	string GetEmptyingLoopSoundsetWater()
	{
		return "pour_Water_Canteen_SoundSet";
	}
	
	string GetEmptyingEndSoundsetHard()
	{
		return "pour_End_HardGround_Canteen_SoundSet";
	}
	
	string GetEmptyingEndSoundsetSoft()
	{
		return "pour_End_SoftGround_Canteen_SoundSet";
	}
	
	string GetEmptyingEndSoundsetWater()
	{
		return "pour_End_Water_Canteen_SoundSet";
	}

	string GetEmptyingLoopSoundset()
	{		
		vector pos = GetPosition();
		string surfaceType = GetGame().GetPlayer().GetSurfaceType();
		string soundSet = "";
		
		bool diggable = GetGame().IsSurfaceDigable(surfaceType);
		
		if (!diggable)
		{
			soundSet = GetEmptyingLoopSoundsetHard();
		}
		else if (diggable)
		{
			soundSet = GetEmptyingLoopSoundsetSoft();
		}
		else if (GetGame().SurfaceIsPond(pos[0], pos[2]) || GetGame().SurfaceIsSea(pos[0], pos[2]))
		{
			soundSet = GetEmptyingLoopSoundsetWater();
		}
		
		return soundSet;
	}
	
	string GetEmptyingEndSoundset()
	{		
		vector pos = GetPosition();
		string surfaceType = GetGame().GetPlayer().GetSurfaceType();
		string soundSet = "";
		
		bool diggable = GetGame().IsSurfaceDigable(surfaceType);
		
		if (!diggable)
		{
			soundSet = GetEmptyingEndSoundsetHard();
		}
		else if (diggable)
		{
			soundSet = GetEmptyingEndSoundsetSoft();
		}
		else if (GetGame().SurfaceIsPond(pos[0], pos[2]) || GetGame().SurfaceIsSea(pos[0], pos[2]))
		{
			soundSet = GetEmptyingEndSoundsetWater();
		}
		
		return soundSet;
	}
};
