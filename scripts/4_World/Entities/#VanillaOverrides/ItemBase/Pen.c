modded class Pen_ColorBase : Inventory_Base
{
	//! SHARED
	override void SetActions()
	{
		super.SetActions();

		//! NOTES
		RemoveAction(ActionWritePaper);
		AddAction(ActionZenWritePaper);
	}
};