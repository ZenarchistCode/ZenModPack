//! WEAPON ENGRAVE
modded class Weapon_Base
{
	//! ENGRAVE WEAPON 
	static const int RIFLE_ENGRAVE_RPC_RESPONSE = -34827469;
	static const int RIFLE_ENGRAVE_RPC_REQUEST = -34827468;

	protected string m_ZenPlayerName = "";
	protected bool m_HasReceivedName = false;
	protected bool m_ZenResetReceivedName = false;

	protected int m_ZenZombieKills = 0;
	protected int m_ZenPlayerKills = 0;

	void Weapon_Base()
	{
		RegisterNetSyncVariableBoolSignal("m_ZenResetReceivedName");
		RegisterNetSyncVariableInt("m_ZenZombieKills");
		RegisterNetSyncVariableInt("m_ZenPlayerKills");
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_ZenResetReceivedName)
		{
			RPCSingleParam(RIFLE_ENGRAVE_RPC_REQUEST, new Param1<bool>(true), true, NULL);
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();

#ifndef SERVER
		if (!m_HasReceivedName)
		{
			if (!ZenModEnabled("ZenWeaponEngrave"))
				return;

			// Client load - request player name
			RPCSingleParam(RIFLE_ENGRAVE_RPC_REQUEST, new Param1<bool>(true), true, NULL);
		}
#endif
	}

	bool SetZenEngravedName(string name)
	{
		if (name == "Survivor")
			return false;

		m_ZenPlayerName = name;
		m_HasReceivedName = false;
		m_ZenResetReceivedName = true;
		SetSynchDirty();
		return true;
	}

	string GetZenEngravedPlayerName()
	{
		return m_ZenPlayerName;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		Param1<string> params = new Param1<string>(m_ZenPlayerName);

#ifdef SERVER
		// Server-side receiver
		if (rpc_type == RIFLE_ENGRAVE_RPC_REQUEST)
		{
			if (m_ZenPlayerName != "")
			{
				RPCSingleParam(RIFLE_ENGRAVE_RPC_RESPONSE, new Param1<string>(m_ZenPlayerName), true, sender);
			}
		}
#else
		// Client-side receiver
		if (rpc_type == RIFLE_ENGRAVE_RPC_RESPONSE)
		{
			if (!ctx.Read(params) || !params.param1)
				return;

			m_ZenPlayerName = params.param1;
			m_HasReceivedName = true;
		}
#endif
	}

	override bool NameOverride(out string output)
	{
		if (ZenModEnabled("ZenWeaponEngrave") && m_HasReceivedName && m_ZenPlayerName != "")
		{
			string displayName;
			GetGame().ConfigGetText("cfgWeapons " + GetType() + " displayName", displayName);
			string playerName = m_ZenPlayerName;
			playerName = playerName.Substring(0, playerName.IndexOf(" "));
			output = playerName + "'s " + displayName;
			return true;
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
			output = m_ZenPlayerName + "'s " + displayName + ". " + description + " | Kills: " + m_ZenZombieKills + " infected, " + m_ZenPlayerKills + " people.";
			return true;
		}

		return false;
	}

	void IncreaseZenZombieKills()
	{
		m_ZenZombieKills++;
		SetSynchDirty();
	}

	void IncreaseZenPlayerKills()
	{
		m_ZenPlayerKills++;
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
		}
	}
}