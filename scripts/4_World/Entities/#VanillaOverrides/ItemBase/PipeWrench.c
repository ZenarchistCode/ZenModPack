modded class PipeWrench extends Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();

		//! REPAIR WELLS 
		AddAction(ActionRepairWell);

		//! REPAIR PUMPS
		AddAction(ActionRepairPump);
	}
}