class ZenDiscordAPI extends Managed
{
	static const string DISCORD_API = "https://discord.com/api/webhooks/";

	void ZenDiscordAPI()
	{
		if (!GetRestApi())
			CreateRestApi();
	}

	void SendMessage(ZenDiscordMessage msg)
	{
		ZenDiscordWebhookCB cb = new ZenDiscordWebhookCB();
		RestContext ctx = GetRestApi().GetRestContext(DISCORD_API);
		ctx.SetHeader("application/json");
		string json = msg.GetJSON();
		cb.SetJSON(json);

		for (int i = 0; i < msg.GetWebhooks().Count(); i++)
		{
			string hook = msg.GetWebhooks().Get(i);
			cb.SetWebhook(hook);
			hook.Replace(DISCORD_API, string.Empty);
			ctx.POST(cb, hook, json);
		}

		delete msg;
	}

	void SendAdminMessage(string title, string message)
	{
		ref ZenDiscordMessage msg = new ZenDiscordMessage(title);
		msg.SetTitle(title);
		msg.SetMessage(message);
		msg.SetColor(255, 255, 255);
		msg.AddWebhooks(GetZenDiscordConfig().AdminWebhooks);
		GetZenDiscordAPI().SendMessage(msg);
	}
}

static ref ZenDiscordAPI m_ZenDiscordAPI;

static ZenDiscordAPI GetZenDiscordAPI()
{
	if (!m_ZenDiscordAPI)
		m_ZenDiscordAPI = new ZenDiscordAPI;

	return m_ZenDiscordAPI;
}