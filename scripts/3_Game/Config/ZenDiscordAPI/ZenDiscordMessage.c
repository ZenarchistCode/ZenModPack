class ZenDiscordMessage
{
	static const string DEFAULT_RAID_ALARM = "https://i.imgur.com/dRcPIht.png";
	static const string DEFAULT_AVATAR_URL = "https://i.imgur.com/bUbr3MG.png";

	protected ref array<string> m_Webhooks;
	protected string m_Avatar;
	protected string m_Username;
	protected string m_Title;
	protected string m_Message;
	protected string m_Footer;
	protected int m_Color;

	void ZenDiscordMessage(string p_user, bool isRaid = false)
	{
		m_Username = p_user;
		m_Webhooks = new array<string>;

		if (isRaid)
			m_Avatar = DEFAULT_RAID_ALARM;
		else 
			m_Avatar = DEFAULT_AVATAR_URL;
	}

	void AddWebhook(string p_url)
	{
		m_Webhooks.Insert(p_url);
	}

	void AddWebhooks(notnull array<string> p_hooks)
	{
		foreach (string s : p_hooks)
		{
			if (s != "" && s.Contains(ZenDiscordAPI.DISCORD_API) && m_Webhooks.Find(s) == -1)
				AddWebhook(s);
		}
	}

	array<string> GetWebhooks()
	{
		return m_Webhooks;
	}

	string GetUsername()
	{
		return m_Username;
	}

	void SetAvatar(string p_url)
	{
		m_Avatar = p_url;
	}

	string GetAvatar()
	{
		return m_Avatar;
	}

	// RGB 0 - 1, 1 = max color
	void SetColorF(float p_r, float p_g, float p_b)
	{
		int r = (int)(p_r * 255);
		int g = (int)(p_g * 255);
		int b = (int)(p_b * 255);
		m_Color = (r << 16) | (g << 8) | b;
	}

	// RGB 255
	void SetColor(int p_r, int p_g, int p_b)
	{
		m_Color = (p_r << 16) | (p_g << 8) | p_b;
	}

	int GetColor()
	{
		return m_Color;
	}

	void SetTitle(string p_title)
	{
		m_Title = p_title;
	}

	string GetTitle()
	{
		return m_Title;
	}

	void SetMessage(string p_message)
	{
		m_Message = p_message;
	}

	string GetMessage()
	{
		return m_Message;
	}

	void SetFooter(string p_text)
	{
		m_Footer = p_text;
	}

	string GetFooter()
	{	
		return m_Footer;
	}

	string GetJSON()
	{
		string discordJSON;
		ZenDiscordMessageJSON message	= new ZenDiscordMessageJSON();
		ZenDiscordEmbedJSON embed		= new ZenDiscordEmbedJSON();
		JsonSerializer json				= new JsonSerializer();

		string footer = GetFooter();
		if (footer != "")
			footer = " | " + footer;

		embed.title			= GetTitle();
		embed.description	= GetMessage();
		embed.color			= GetColor();
		embed.footer		= new ZenDiscordEmbedFooterJSON(GetZenDiscordConfig().ServerName + footer);
		message.username	= GetUsername();
		message.avatar_url	= GetAvatar();
		message.embeds.Insert(embed);

		json.WriteToString(message, false, discordJSON);
		return discordJSON;
	}
}