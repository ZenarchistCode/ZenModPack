modded class DayZPlayerImplement
{
	//! IMMERSIVE SODAS 
	override void ProcessWeaponEvent(string pEventType, string pUserString, int pUserInt)
	{
		SodaCan_ColorBase sodaCan = SodaCan_ColorBase.Cast(GetHumanInventory().GetEntityInHands());
		if (sodaCan && sodaCan.IsOpened())
		{
			// Don't play open sound effect again if soda can has already been opened
			return;
		}

		super.ProcessWeaponEvent(pEventType, pUserString, pUserInt);
	}
}
