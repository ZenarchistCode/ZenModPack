modded class ActionDrinkCan
{
	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);

		SodaCan_ColorBase sodaCan = SodaCan_ColorBase.Cast(action_data.m_MainItem);
		if (!sodaCan || sodaCan.IsRuined())
			return;

		// Convert vanilla soda can to my open soda can texture
		sodaCan.SetOpenedTexture();
	}
}
