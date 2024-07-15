class Zen_CassetteCase extends ItemBase
{
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return item && item.IsInherited(Zen_Cassette_Base);
	}

	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		if (!super.CanLoadItemIntoCargo(item))
			return false;

		return item && item.IsInherited(Zen_Cassette_Base);
	}
};