//! WEAPON ENGRAVE
modded class Weapon_Base
{
	//! ENGRAVE WEAPON 
	static const int ZEN_RIFLE_ENGRAVE_RPC_RESPONSE = -34827469;
	static const int ZEN_RIFLE_ENGRAVE_RPC_REQUEST = -34827468;

	protected bool m_HasRequestedSync;
	protected string m_ZenPlayerName;
	protected bool m_HasReceivedName;
	protected bool m_ZenResetReceivedName;

	protected string m_ZenPlayerUID;
	protected int m_ZenZombieKills;
	protected int m_ZenAnimalKills;
	protected int m_ZenPlayerKills;
	protected float m_ZenLongestShot;

	void Weapon_Base()
	{
		m_ZenPlayerUID = "";
		m_ZenPlayerName = "";
		m_ZenResetReceivedName = false;
		m_HasReceivedName = false;
		m_ZenZombieKills = 0;
		m_ZenAnimalKills = 0;
		m_ZenPlayerKills = 0;
		m_ZenLongestShot = 0;

		RegisterNetSyncVariableBool("m_ZenResetReceivedName");
		RegisterNetSyncVariableInt("m_ZenZombieKills");
		RegisterNetSyncVariableInt("m_ZenAnimalKills");
		RegisterNetSyncVariableInt("m_ZenPlayerKills");
		RegisterNetSyncVariableFloat("m_ZenLongestShot");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_ZenResetReceivedName)
		{
			m_ZenResetReceivedName = false;
			m_HasReceivedName = false;
			RPCSingleParam(ZEN_RIFLE_ENGRAVE_RPC_REQUEST, new Param1<bool>(true), true, NULL);
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (g_Game.IsClient() && !m_HasRequestedSync)
		{
			m_HasRequestedSync = true;
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenCheckEngraveSync, 100);
		}
	}

	void ZenCheckEngraveSync()
	{
		if (!m_HasReceivedName)
		{
			if (!ZenModEnabled("ZenWeaponEngrave"))
				return;

			// Client load - request player name
			RPCSingleParam(ZEN_RIFLE_ENGRAVE_RPC_REQUEST, new Param1<bool>(true), true, NULL);
		}
	}

	bool SetZenEngravedName(string name, PlayerIdentity rpcId = NULL)
	{
		name.TrimInPlace();
	
		if (name.LengthUtf8() > 30)
			name = name.SubstringUtf8(0, 30);
	
		m_ZenPlayerUID = rpcId.GetId();
		m_ZenPlayerName = name;
		m_ZenResetReceivedName = true;
		SetSynchDirty();
	
		if (rpcId != NULL)
			ZenSyncEngraveNameRPC(rpcId);
	
		return true;
	}

	string GetZenEngravedPlayerName()
	{
		return m_ZenPlayerName;
	}
	
	string GetZenEngravedUID()
	{
		return m_ZenPlayerUID;
	}
	
	bool IsZenEngraved()
	{
		return m_ZenPlayerName != "";
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		// Server-side receiver
		if (rpc_type == ZEN_RIFLE_ENGRAVE_RPC_REQUEST)
		{
			if (m_ZenPlayerName != "")
			{
				ZenSyncEngraveNameRPC(sender);
			}
		}

		// Client-side receiver
		if (rpc_type == ZEN_RIFLE_ENGRAVE_RPC_RESPONSE)
		{
			Param1<string> params;

			if (!ctx.Read(params) || !params.param1)
				return;

			m_ZenPlayerName = params.param1;
			m_HasReceivedName = true;
		}
	}

	void ZenSyncEngraveNameRPC(PlayerIdentity id)
	{
		if (!id)
			return;

		RPCSingleParam(ZEN_RIFLE_ENGRAVE_RPC_RESPONSE, new Param1<string>(m_ZenPlayerName), true, id);
	}

	override bool NameOverride(out string output)
	{
		if (!ZenModEnabled("ZenWeaponEngrave"))
			return false;
	
		if (!m_HasReceivedName)
			return false;
	
		string fullName = m_ZenPlayerName;
		fullName.TrimInPlace(); // removes leading/trailing whitespace
	
		if (fullName == "")
			return false;
	
		// Prefer "first token", but safely fall back to full name if no space
		string shortName = fullName;
		int spaceIdx = fullName.IndexOf(" "); // returns -1 if not found
		if (spaceIdx > 0)
			shortName = fullName.Substring(0, spaceIdx);
	
		// UTF8-safe limit (avoids cutting multibyte chars)
		if (shortName.LengthUtf8() > 12)
			shortName = shortName.SubstringUtf8(0, 12);
		
		if (shortName == "")
			return false;
	
		string displayName;
		g_Game.ConfigGetText("cfgWeapons " + GetType() + " displayName", displayName);
		output = shortName + "'s " + displayName;
		return true;
	}

	override bool DescriptionOverride(out string output)
	{
		if (ZenModEnabled("ZenWeaponEngrave") && m_HasReceivedName && m_ZenPlayerName != "")
		{
			string displayName;
			string description;
			g_Game.ConfigGetText("cfgWeapons " + GetType() + " displayName", displayName);
			g_Game.ConfigGetText("cfgWeapons " + GetType() + " descriptionShort", description);
			output = m_ZenPlayerName + "'s " + displayName + ". " + description + "<br/><br/><color rgba='220,10,10,255'><b>#STR_ZenKills0:</b><br/>#STR_ZenKills1: " + m_ZenZombieKills + "<br/>#STR_ZenKills2: " + m_ZenAnimalKills + "<br/>#STR_ZenKills3: " + m_ZenPlayerKills + " <br/>#STR_ZenKills4: " + MiscGameplayFunctions.TruncateToS(m_ZenLongestShot) + "m</color>";
			return true;
		}

		return false;
	}
	
	bool IsWieldedByZenEngraver()
	{
		PlayerBase pb = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!pb)
			return false;
		
		if (!pb.GetIdentity())
			return false;
		
		return pb.GetIdentity().GetId() == m_ZenPlayerUID;
	}

	int GetZenZombieKillCount()
	{
		return m_ZenZombieKills;
	}

	void IncreaseZenZombieKills()
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenZombieKills++;
		SetSynchDirty();
	}

	void SetZenZombieKillCount(int count)
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenZombieKills = count;
		SetSynchDirty();
	}

	int GetZenAnimalKillCount()
	{
		return m_ZenAnimalKills;
	}

	void IncreaseZenAnimalKills()
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenAnimalKills++;
		SetSynchDirty();
	}

	void SetZenAnimalKillCount(int count)
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenAnimalKills = count;
		SetSynchDirty();
	}

	int GetZenPlayerKillCount()
	{
		return m_ZenPlayerKills;
	}

	void IncreaseZenPlayerKills()
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenPlayerKills++;
		SetSynchDirty();
	}

	void SetZenPlayerKillCount(int count)
	{
		if (!IsWieldedByZenEngraver())
			return;
		
		m_ZenPlayerKills = count;
		SetSynchDirty();
	}
	
	float GetZenLongestShot()
	{
		return m_ZenLongestShot;
	}

	void UpdateZenLongestShot(float meters)
	{
		if (!ZenModEnabled("ZenWeaponEngrave"))
			return;
		
		if (!IsWieldedByZenEngraver())
			return;

		if (meters > m_ZenLongestShot)
		{
			m_ZenLongestShot = meters;
			SetSynchDirty();
		}
	}

	// USE CF_Load/Save because this will NOT break server persistence if the mod is added/removed mid-wipe. 
	// NOTE: storage[] must refer to this mod's CfgMods classname EXACTLY or this won't work.
	
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage["ZenModPack"];
		if (!ctx) return;
		
		ctx.Write(m_ZenPlayerUID);
		ctx.Write(m_ZenPlayerName);
		ctx.Write(m_ZenZombieKills);
		ctx.Write(m_ZenPlayerKills);
		ctx.Write(m_ZenAnimalKills);
		ctx.Write(m_ZenLongestShot);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage)) return false;

		auto ctx = storage["ZenModPack"];
		if (!ctx) return true;

		if (ctx.GetVersion() >= 1)
		{
			if (!ctx.Read(m_ZenPlayerUID))
			{
				m_ZenPlayerUID = "";
				return false;
			}
			
			if (!ctx.Read(m_ZenPlayerName))
			{
				m_ZenPlayerName = "";
				return false;
			}

			if (!ctx.Read(m_ZenZombieKills))
			{
				m_ZenZombieKills = 0;
				return false;
			}

			if (!ctx.Read(m_ZenPlayerKills))
			{
				m_ZenPlayerKills = 0;
				return false;
			}

			if (!ctx.Read(m_ZenAnimalKills))
			{
				m_ZenAnimalKills = 0;
				return false;
			}
			
			if (!ctx.Read(m_ZenLongestShot))
			{
				m_ZenLongestShot = 0;
				return false;
			}
		}

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_ZenResetReceivedName = true;
			SetSynchDirty();
	}

	#ifdef MapLink
	override void OnUApiSave(UApiEntityStore data)
	{
		super.OnUApiSave(data);

		data.Write("m_ZenPlayerUID", m_ZenPlayerUID);
		data.Write("m_ZenPlayerName", m_ZenPlayerName);
		data.Write("m_ZenZombieKills", m_ZenZombieKills);
		data.Write("m_ZenPlayerKills", m_ZenPlayerKills);
		data.Write("m_ZenAnimalKills", m_ZenAnimalKills);
		data.Write("m_ZenLongestShot", m_ZenLongestShot);

		//Print("[ZenModPack::MapLink] Saved weapon engrave data.");
	}
	
	override void OnUApiLoad(UApiEntityStore data)
	{
		super.OnUApiLoad(data);

		m_ZenPlayerUID = data.GetString("m_ZenPlayerUID");
		m_ZenPlayerName = data.GetString("m_ZenPlayerName");
		m_ZenZombieKills = data.GetInt("m_ZenZombieKills");
		m_ZenPlayerKills = data.GetInt("m_ZenPlayerKills");
		m_ZenAnimalKills = data.GetInt("m_ZenAnimalKills");
		m_ZenLongestShot = data.GetFloat("m_ZenLongestShot");
		SetSynchDirty();

		//Print("[ZenModPack::MapLink] Loaded weapon engrave data.");
	}
	#endif
}