class Zen_ArtillerySmokeGrenade : M18SmokeGrenade_Red
{
	int m_BombCount = 0;

	void Zen_ArtillerySmokeGrenade()
	{
		SetParticleSmokeStart(ParticleList.GRENADE_M18_RED_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_RED_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_RED_END);
	}

	override void EOnInit(IEntity other, int extra)
	{
		super.EOnInit(other, extra);

		// VPP Admin Tools fix (spawning smoke grenade with 0 quantity = 0 energy = won't work)
		if (GetGame().IsDedicatedServer())
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SetEnergyToMax, 500, false);
	}

	void SetEnergyToMax()
	{
		if (GetCompEM())
			GetCompEM().SetEnergy0To1(1.0);
	}

	// Triggered when pin is pulled
	override void OnWorkStart()
	{
		super.OnWorkStart();

		if (GetGame().IsDedicatedServer())
		{
			SetLifetime(0); // Force this to despawn asap when players are not around - the bombs will trigger, and then if the server restarts the smoke will not trigger another airstrike on server startup.
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartBombs, Math.RandomInt(GetZenArtilleryConfig().MinStrikeSecs * 1000, GetZenArtilleryConfig().MaxStrikeSecs * 1000), false);
		}
	}

	string GetBombType()
	{
		return "Zen_ArtilleryBomb";
	}

	// Delayed - start artillery sounds
	void StartBombs()
	{
		// Get closest artillery firing position
		int i;
		float currentDist = 99999;
		vector firePos;
		vector smokePos = GetPosition();

		for (i = 0; i < GetGame().GetMission().GetWorldData().GetArtyFiringPos().Count(); i++)
		{
			vector v = GetGame().GetMission().GetWorldData().GetArtyFiringPos().Get(i);
			float dist = vector.Distance(v, smokePos);

			if (dist < currentDist)
			{
				currentDist = dist;
				firePos = v;
			}
		}

		// Send artillery sound playing to all players
		Param1<vector> fireParam = new Param1<vector>(firePos);
		GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveAirstrikeData", fireParam, true, null);

		// Schedule the big bang
		int randomDelay = Math.RandomInt(GetZenArtilleryConfig().MinBombSecs, GetZenArtilleryConfig().MaxBombSecs) * 1000;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartDroppingBombs, randomDelay, false);
	}

	// Drop random bombs all over the place
	void StartDroppingBombs()
	{
		if (GetBombType() == "Zen_ArtilleryBombChem")
			m_BombCount = Math.RandomIntInclusive(GetZenArtilleryConfig().MinChemBombs, GetZenArtilleryConfig().MaxChemBombs);
		else
			m_BombCount = Math.RandomIntInclusive(GetZenArtilleryConfig().MinBombs, GetZenArtilleryConfig().MaxBombs);

		DropBomb();
	}

	void DropBomb()
	{
		// Get smoke grenade position
		vector pos = ZenFunctions.GetRandomPointInCircle(GetPosition(), GetZenArtilleryConfig().BombDropRadius);

		// Create bomb
		Object bomb = GetGame().CreateObject(GetBombType(), pos);
		bomb.PlaceOnSurface();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DetonateBomb, Math.RandomInt(150, 300), false, bomb);

		m_BombCount = m_BombCount - 1;
		if (m_BombCount > 0)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DropBomb, Math.RandomInt(200, 3000), false);
		}
	}

	// When grenade_base is ruined it explodes
	void DetonateBomb(Object bomb)
	{
		bomb.SetHealth(0); 
	}
};

// Chem version
class Zen_ArtillerySmokeGrenadeChem : Zen_ArtillerySmokeGrenade
{
	void Zen_ArtillerySmokeGrenadeChem()
	{
		// Set chemical artillery colour
		SetParticleSmokeStart(ParticleList.GRENADE_M18_GREEN_START);
		SetParticleSmokeLoop(ParticleList.GRENADE_M18_GREEN_LOOP);
		SetParticleSmokeEnd(ParticleList.GRENADE_M18_GREEN_END);
	}

	override string GetBombType()
	{
		return "Zen_ArtilleryBombChem";
	}
}
