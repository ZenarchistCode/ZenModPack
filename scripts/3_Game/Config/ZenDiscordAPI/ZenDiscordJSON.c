class ZenDiscordEmbedJSON
{
	string title;
	string description;
	int color;
	ref ZenDiscordEmbedFooterJSON footer = new ZenDiscordEmbedFooterJSON();
}

class ZenDiscordEmbedFooterJSON
{
    string text;

	void ZenDiscordEmbedFooterJSON(string p_text = "")
	{
		text = p_text;
	}
}

class ZenDiscordMessageJSON
{
	string username;
	string avatar_url;
	ref array<ref ZenDiscordEmbedJSON> embeds;

	void ZenDiscordMessageJSON()
	{
		embeds = new array<ref ZenDiscordEmbedJSON>;
	}
}