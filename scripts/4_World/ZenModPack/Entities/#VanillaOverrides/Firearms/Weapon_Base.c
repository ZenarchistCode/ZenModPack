//! WEAPON ENGRAVE
modded class Weapon_Base
{
	//! ENGRAVE WEAPON 
	static const int ZEN_RIFLE_ENGRAVE_RPC_RESPONSE = -34827469;
	static const int ZEN_RIFLE_ENGRAVE_RPC_REQUEST = -34827468;

	protected string m_ZenPlayerName;
	protected bool m_HasReceivedName;
	protected bool m_ZenResetReceivedName;

	protected int m_ZenZombieKills;
	protected int m_ZenAnimalKills;
	protected int m_ZenPlayerKills;

	void Weapon_Base()
	{
		m_ZenPlayerName = "";
		m_ZenResetReceivedName = false;
		m_HasReceivedName = false;
		m_ZenZombieKills = 0;
		m_ZenAnimalKills = 0;
		m_ZenPlayerKills = 0;

		RegisterNetSyncVariableBool("m_ZenResetReceivedName");
		RegisterNetSyncVariableInt("m_ZenZombieKills");
		RegisterNetSyncVariableInt("m_ZenAnimalKills");
		RegisterNetSyncVariableInt("m_ZenPlayerKills");
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

		if (GetGame().IsClient())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ZenCheckEngraveSync, 1000, false);
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
		if (name == "Survivor")
			return false;

		if (name.Length() > 30)
			name = name.Substring(0, 30);

		m_ZenPlayerName = name;
		m_ZenResetReceivedName = true;
		SetSynchDirty();

		if (rpcId != NULL)
		{
			ZenSyncEngraveNameRPC(rpcId);
		}

		return true;
	}

	string GetZenEngravedPlayerName()
	{
		return m_ZenPlayerName;
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
		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			if (m_HasReceivedName && m_ZenPlayerName != "")
			{
				string displayName;
				GetGame().ConfigGetText("cfgWeapons " + GetType() + " displayName", displayName);
				string playerName = m_ZenPlayerName;
				playerName = playerName.Substring(0, playerName.IndexOf(" "));

				if (playerName.Length() > 12)
					playerName = playerName.Substring(0, 12);

				output = playerName + "'s " + displayName;
				return true;
			}
		}

		return false;
	}

	override bool DescriptionOverride(out string output)
	{
		if (ZenModEnabled("ZenWeaponEngrave") && m_HasReceivedName && m_ZenPlayerName != "")
		{
			string displayName;
			string description;
			GetGame().ConfigGetText("cfgWeapons " + GetType() + " displayName", displayName);
			GetGame().ConfigGetText("cfgWeapons " + GetType() + " descriptionShort", description);
			output = m_ZenPlayerName + "'s " + displayName + ". " + description + " | Kills: " + m_ZenZombieKills + " infected, " + m_ZenAnimalKills + " animals, " + m_ZenPlayerKills + " people.";
			return true;
		}

		return false;
	}

	int GetZenZombieKillCount()
	{
		return m_ZenZombieKills;
	}

	void IncreaseZenZombieKills()
	{
		m_ZenZombieKills++;
		SetSynchDirty();
	}

	void SetZenZombieKillCount(int count)
	{
		m_ZenZombieKills = count;
		SetSynchDirty();
	}

	int GetZenAnimalKillCount()
	{
		return m_ZenAnimalKills;
	}

	void IncreaseZenAnimalKills()
	{
		m_ZenAnimalKills++;
		SetSynchDirty();
	}

	void SetZenAnimalKillCount(int count)
	{
		m_ZenAnimalKills = count;
		SetSynchDirty();
	}

	int GetZenPlayerKillCount()
	{
		return m_ZenPlayerKills;
	}

	void IncreaseZenPlayerKills()
	{
		m_ZenPlayerKills++;
		SetSynchDirty();
	}

	void SetZenPlayerKillCount(int count)
	{
		m_ZenPlayerKills = count;
		SetSynchDirty();
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		super.OnStoreLoad(ctx, version);

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
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
		}

		return true;
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		if (ZenModEnabled("ZenWeaponEngrave"))
		{
			ctx.Write(m_ZenPlayerName);
			ctx.Write(m_ZenZombieKills);
			ctx.Write(m_ZenPlayerKills);
			ctx.Write(m_ZenAnimalKills);
		}
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_ZenResetReceivedName = true;
			SetSynchDirty();
	}

	#ifdef MAPLINK
	override void OnUApiSave(UApiEntityStore data)
	{
		super.OnUApiSave(data);

		data.Write("m_ZenPlayerName", m_ZenPlayerName);
		data.Write("m_ZenZombieKills", m_ZenZombieKills);
		data.Write("m_ZenPlayerKills", m_ZenPlayerKills);
		data.Write("m_ZenAnimalKills", m_ZenAnimalKills);

		//Print("[ZenModPack::MapLink] Saved weapon engrave data.");
	}
	
	override void OnUApiLoad(UApiEntityStore data)
	{
		super.OnUApiLoad(data);

		m_ZenPlayerName = data.GetString("m_ZenPlayerName");
		m_ZenZombieKills = data.GetInt("m_ZenZombieKills");
		m_ZenPlayerKills = data.GetInt("m_ZenPlayerKills");
		m_ZenAnimalKills = data.GetInt("m_ZenAnimalKills");
		SetSynchDirty();

		//Print("[ZenModPack::MapLink] Loaded weapon engrave data.");
	}
	#endif
}