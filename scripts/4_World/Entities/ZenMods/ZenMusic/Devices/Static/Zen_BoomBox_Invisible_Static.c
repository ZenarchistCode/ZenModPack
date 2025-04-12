class Zen_BoomBox_Invisible_Static extends Zen_BoomBox_Static 
{
	override bool DisableVicinityIcon()
	{
		return true;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return false;
	}

	override bool CanDisplayAttachmentSlot( int slot_id )
	{
		return false;
	}

	override void SetActions()
	{
		return;
	}
};
