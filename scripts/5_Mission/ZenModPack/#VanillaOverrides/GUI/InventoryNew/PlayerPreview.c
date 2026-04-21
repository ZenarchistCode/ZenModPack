modded class PlayerPreview
{
    void PlayerPreview( LayoutHolder parent )
	{
        // Set character preview zoomed out when it is initially created instead of zoomed in on their face
		m_CharacterPanelWidget.SetSize( 0.62, 0.62 );
        UpdateScale();
	}
}