modded class Grenade_ChemGas
{
	override void EOnContact(IEntity other, Contact extra)
	{
		super.EOnContact(other, extra);

		#ifdef SERVER
		//! ANTI-COMBAT LOG
		TriggerCombatPlayersInRadius(GetZenAntiCombatLogoutConfig().TriggerOnExplosiveRadius);
		#endif
	}
};
