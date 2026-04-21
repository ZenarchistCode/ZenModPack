modded class ChatLine
{
	void ChatLine(Widget root_widget)
	{
		if (!ZenModEnabled("ZenBetterChatFont"))
			return;

		m_RootWidget = g_Game.GetWorkspace().CreateWidgets("ZenModPack/data/gui/ChatLine/zen_day_z_chat_item.layout", root_widget);
		m_NameWidget = TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemSenderWidget"));
		m_TextWidget = TextWidget.Cast(m_RootWidget.FindAnyWidget("ChatItemTextWidget"));
	}
}