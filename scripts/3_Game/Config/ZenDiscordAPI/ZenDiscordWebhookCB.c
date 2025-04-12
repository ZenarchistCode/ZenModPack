class ZenDiscordWebhookCB extends RestCallback
{
	protected string m_Webhook;
	protected string m_JSON;

	void SetWebhook(string s)
	{
		m_Webhook = s;
	}

	void SetJSON(string s)
	{
		m_JSON = s;
	}

	override void OnError(int errorCode)
	{
		if (errorCode == 8) // I get error code 8 even when msg sends ok
		{
			Print("[ZenDiscordWebhookCB] Discord message sent successfully.");
			return;
		}

		if (errorCode == 5) // 5 usually means error
		{
			Error("[ZenDiscordWebhookCB] Error code 5 - this usually means the JSON syntax is incorrect, or the webhook doesn't exist. Check your config:\n\nWebhook: " + m_Webhook);
			Error("\nJSON:\n" + m_JSON + "\n\n");
			return;
		}

		// Not sure what any other codes mean
		Error("[ZenDiscordWebhookCB] errorCode: " + errorCode);
	}

	override void OnTimeout()
	{
		Error("[ZenDiscordWebhookCB] Reponse timed out!");
	}
}