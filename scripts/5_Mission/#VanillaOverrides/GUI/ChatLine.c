modded class ChatLine
{
	void ChatLine(Widget root_widget)
	{
		if (!GetZenModPackClientConfig().BetterChatFont)
			return;

		m_RootWidget = GetGame().GetWorkspace().CreateWidgets("ZenModPack/data/gui/ChatLine/zen_day_z_chat_item.layout", root_widget);
		m_NameWidget = TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderWidget"));
		m_TextWidget = TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemTextWidget"));
	}
}