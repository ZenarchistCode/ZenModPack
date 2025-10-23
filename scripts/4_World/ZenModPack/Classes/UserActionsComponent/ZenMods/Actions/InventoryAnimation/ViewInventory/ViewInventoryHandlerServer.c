class ViewInventoryHandlerServer extends ViewInventoryHandlerBase
{
    void ViewInventoryHandlerServer(PlayerBase player)
    {
    }

    override bool OnInputUserDataProcess(ParamsReadContext ctx)
    {
        return ctx.Read(m_IsViewingInventorySynced);
    }
}