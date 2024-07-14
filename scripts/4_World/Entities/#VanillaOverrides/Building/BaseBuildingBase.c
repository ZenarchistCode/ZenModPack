modded class BaseBuildingBase
{	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (!GetZenUtilitiesConfig().ShouldLogRaids)
			return;

		// Check ammo config and log any players involved in a raid if the cfg ammo is found on the list.
		bool ammoFound = false;
		foreach (string s : GetZenUtilitiesConfig().RaidAmmo)
		{
			s.ToLower();
			ammo.ToLower();

			if (s.Contains(ammo))
			{
				ammoFound = true;
				break;
			}
		}

		if (!ammoFound)
			return;

		array<Man> players = new array<Man>;
		g_Game.GetWorld().GetPlayerList(players);
		if (!players || players.Count() <= 0)
			return;

		foreach (Man m : players)
		{
			if (vector.Distance(m.GetPosition(), GetPosition()) > 250)
				continue;

			PlayerBase pb = PlayerBase.Cast(m);
			if (pb)
			{
				float damage = damageResult.GetDamage(dmgZone, "Health");
				string identity = pb.GetIdentity().GetId() + " / " + pb.GetCachedID();
				ZenModLogger.Log(pb.GetCachedName() + " (" + identity + ") @ " + pb.GetPosition() + " involved in raid @ " + GetPosition() + " Object=" + GetType() + " ammo=" + ammo + " dmg=" + dmgZone + "/" + damage, "baseraid");
			}
		}
	}
}