modded class Icon
{
	override void RefreshMuzzleIcon()
	{
		if (!ZenModEnabled("ZenMagObfuscation"))
		{
			super.RefreshMuzzleIcon();
			return;
		}

		for (int i = 0; i < m_AmmoIcons.Count(); i++) 
		{
			if (m_AmmoIcons[i]) 
			{
				m_AmmoIcons[i].Show(false);
			}
		}
	}
}