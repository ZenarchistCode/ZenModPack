class Zen_DebugProxyItemsTool extends ItemBase
{
	override void SetActions()
    {
        super.SetActions();

        AddAction(ActionZenDumpNearbyObjects);
    }
}