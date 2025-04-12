class Zen_CassetteCase extends Container_Base
{
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return item && item.IsInherited(Zen_Cassette_Base);
	}
};