modded class BleedingCheckMdfr
{
	override void OnActivate(PlayerBase player)
	{
		super.OnActivate(player);

		//! CAUSE OF DEATH
		player.m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("bleeding").CauseMessage;
	}
};

modded class HungerMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0 && player.GetStatEnergy().Get() <= 0)
		{
			player.m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("hunger").CauseMessage;
		}
	}
};

modded class ThirstMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0 && player.GetStatWater().Get() <= 0)
		{
			player.m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("thirst").CauseMessage;
		}
	}
};

modded class WoundInfectStage2Mdfr
{
	override protected void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0)
		{
			player.m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("woundinfection").CauseMessage;
		}
	}
};

modded class ContaminationStage3Mdfr
{
	override protected void OnTick(PlayerBase player, float deltaT)
	{
		super.OnTick(player, deltaT);

		//! CAUSE OF DEATH
		if (player.GetHealth() <= 0)
		{
			player.m_CauseOfDeath = GetCauseOfDeathConfig().GetCauseOfDeath("contamination").CauseMessage;
		}
	}
};