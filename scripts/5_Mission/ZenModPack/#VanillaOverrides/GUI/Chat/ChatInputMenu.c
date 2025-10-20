modded class ChatInputMenu
{
	//! ADMIN COMMANDS - chat seems to be handled internally by game files I'm too dumb to understand or can't access as a modder
	//					 so I hijack the text chat: RPC any messages beginning with '!' to server, and if the message isn't identified
	//					 as an admin command then the server sends the text back to the client which then processes it as a normal chat msg.
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (finished)
		{
			string text = m_edit_box.GetText();

			// Admin messages
			if (text.IndexOf("!") == 0)
			{
				GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminCommandMessage", new Param1<string>(text.Trim()), true, NULL);

				m_close_timer.Run(0.1, this, "Close");
				GetUApi().GetInputByID(UAPersonView).Supress();	

				// Don't send on to vanilla handling - this is to be handled as a potential admin command
				return true;
			}
		}

		return super.OnChange(w, x, y, finished);
	}
}