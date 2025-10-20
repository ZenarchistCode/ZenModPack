#ifdef SERVER

modded class BleedingCheckMdfr
{
	override void OnActivate(PlayerBase player)
	{
		super.OnActivate(player);

		//! CAUSE OF DEATH
		player.SetCauseOfDeath(GetCauseOfDeathConfig().GetCauseOfDeath("bleeding").CauseMessage);
	}
}

modded class HungerMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0 && player.GetStatEnergy().Get() <= 0)
		{
			player.SetCauseOfDeath(GetCauseOfDeathConfig().GetCauseOfDeath("hunger").CauseMessage);
		}
	}
}

modded class ThirstMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0 && player.GetStatWater().Get() <= 0)
		{
			player.SetCauseOfDeath(GetCauseOfDeathConfig().GetCauseOfDeath("thirst").CauseMessage);
		}
	}
}

modded class WoundInfectStage2Mdfr
{
	override protected void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0)
		{
			player.SetCauseOfDeath(GetCauseOfDeathConfig().GetCauseOfDeath("woundinfection").CauseMessage);
		}
	}
}

modded class ContaminationStage3Mdfr
{
	override protected void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0)
		{
			player.SetCauseOfDeath(GetCauseOfDeathConfig().GetCauseOfDeath("contamination").CauseMessage);
		}
	}
}

#endif