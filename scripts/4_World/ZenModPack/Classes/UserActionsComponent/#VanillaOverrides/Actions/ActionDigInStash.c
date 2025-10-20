modded class ActionDigInStash
{
	// Don't allow players to re-bury a skeleton
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (target.GetObject() && target.GetObject().IsInherited(ZenGraves_DeadPlayerSkeleton))
			return false;

		return super.ActionCondition(player, target, item);
	};
};