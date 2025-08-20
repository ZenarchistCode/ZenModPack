class ZenRadioPlugin extends PluginBase
{
    protected ref array<TransmitterBase> m_Radios;
    protected ref map<string, string> m_NotificationSoundsets;

	override void OnInit()
    {
        ZMPrint("[ZenRadioPlugin] :: OnInit - Load soundsets.");

        m_Radios = new array<TransmitterBase>;
        m_NotificationSoundsets = new map<string, string>;

        // For some reason, I can't access NotificationUI's pre-cached soundsets, so just reload them here.
        for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgSoundSets"); i++)
		{
			string soundset_name = "";
			GetGame().ConfigGetChildName("CfgSoundSets", i, soundset_name);
			if (soundset_name == "")
				continue;

			string keywords = "";
			GetGame().ConfigGetText("CfgSoundSets " + soundset_name + " soundKeywords", keywords);
			if (keywords == "")
				continue;

			// Valid mission keyword found - add to soundset map
			m_NotificationSoundsets.Insert(keywords, soundset_name);
		}

        ZMPrint("[ZenRadioPlugin] :: Soundsets loaded.");
    }

    void AddRadio(notnull TransmitterBase radio)
    {
        if (!m_Radios)
            m_Radios = new array<TransmitterBase>;

        if (m_Radios.Find(radio) == -1)
            m_Radios.Insert(radio);
    }

    void RemoveRadio(notnull TransmitterBase radio)
    {
        m_Radios.RemoveItem(radio);
    }

    ItemBase GetNearestWorkingRadio(vector location, float maxDist = 10.0)
    {
        ItemBase radio;
        float lastDist = 99999;

        foreach (TransmitterBase tb : m_Radios)
        {
            ItemBase radioTemp = tb;
            if (radioTemp && radioTemp.GetCompEM() && radioTemp.GetCompEM().IsWorking())
            {
                float dist = vector.Distance(location, radioTemp.GetPosition());
                if (dist <= maxDist && dist < lastDist)
                {
                    lastDist = dist;
                    radio = radioTemp;
                }
            }
        }

        return radio;
    }

    array<TransmitterBase> GetRadios()
    {
        return m_Radios;
    }

    void PlaySoundSetMessage(string message)
    {
        string soundset_name = GetZenSoundSetForKeywords(message);
		if (soundset_name != "")
		{
			int playOnRadio = GetGame().ConfigGetInt("CfgSoundSets " + soundset_name + " playOnRadio");
			if (playOnRadio == 1)
			{
				PlaySoundset(soundset_name);
			}
		}
    }

    void PlaySoundset(string soundset_name)
    {
        if (!GetGame().ConfigIsExisting("CfgSoundSets " + soundset_name))
            return;

        Param1<string> data = new Param1<string>(soundset_name);
        int sentSounds = 0;

        foreach (TransmitterBase radio : m_Radios)
        {
            if (radio && radio.GetCompEM() && radio.GetCompEM().IsWorking())
            {
                radio.RPCSingleParam(TransmitterBase.RPC_PLAY_ZEN_RADIO_SOUNDSET, data, true);
                sentSounds++;
            }
        }

        Print("[ZenRadioPlugin] Played " + soundset_name + " on " + sentSounds + " working radios.");
    }

    string GetZenSoundSetForKeywords(string keywords)
	{
		if (!m_NotificationSoundsets)
			return "";

		for (int i = 0; i < m_NotificationSoundsets.Count(); i++)
		{
			string key = m_NotificationSoundsets.GetKey(i);
			if (keywords.Contains(key))
			{
				string soundset_name = m_NotificationSoundsets.GetElement(i);
				if (GetGame().ConfigIsExisting("CfgSoundSets " + soundset_name))
				{
					return soundset_name;
				}
			}
		}

		return "";
	}
}