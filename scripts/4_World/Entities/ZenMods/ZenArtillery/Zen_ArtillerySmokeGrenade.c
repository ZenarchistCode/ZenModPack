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
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(StartBombs, Math.RandomInt(GetZenArtilleryConfig().MinStrikeSecs * 1000, GetZenArtilleryConfig().MaxStrikeSecs * 1000), false);
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

		// Send artillery sound playing to all players (TODO - can't we just send an RPC to NULL to notify all players?)
		Param1<vector> fireParam = new Param1<vector>(firePos);
		array<Man> all_players = new array<Man>;
		GetGame().GetPlayers(all_players);
		for (i = 0; i < all_players.Count(); i++)
		{
			PlayerBase player;
			if (Class.CastTo(player, all_players.Get(i)))
			{
				GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveAirstrikeData", fireParam, true, player.GetIdentity(), player);
			}
		}

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
		vector pos = GetPosition();

		// Get radius & random x/y
		int radius = GetZenArtilleryConfig().BombDropRadius;
		float randomX = Math.RandomFloatInclusive(-radius, radius);
		float randomY = Math.RandomFloatInclusive(-radius, radius);

		// Set bomb position
		pos[0] = pos[0] + randomX;
		pos[2] = pos[2] + randomY;
		pos[1] = GetGame().SurfaceY(pos[0], pos[2]); // Get surface

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
