modded class Wrench extends Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();

		//! REPAIR WELLS 
		AddAction(ActionZenRepairWell);

		//! REPAIR PUMPS
		AddAction(ActionZenRepairPump);
	}
}