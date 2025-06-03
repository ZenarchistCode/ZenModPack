//! ANTI-COMBAT LOG + WEAPON ENGRAVE

// (Client-side) Detects when the player shoots at another player and informs the server.
// I do this client-side because otherwise all these raycasts are likely to affect server performance.
modded class Weapon_Base
{
	// Used to temporarily disable gunshot RPC's to prevent spamming the server with mag dumps
	bool m_ZenPauseCombatRPC = false;
	static const float ZRPC_DELAY_TIMER = 3000;

	// Set default weapon combat log trigger distance
	float GetAntiCombatLogWeaponDistance()
	{
		return 250;
	}

	// Detect gun fired
	override void OnFire(int muzzle_index)
	{
		super.OnFire(muzzle_index);

		//! ANTI-COMBAT LOGOUT
		if (!ZenModEnabled("ZenAntiCombatLogout"))
			return;

		// Don't run raycast on server, probably too resource intensive especially for mag dumps. Run raycast on client instead and RPC server when a player is detected
		#ifdef SERVER
		// Check if trigger on gunshot is enabled
		if (GetZenAntiCombatLogoutConfig().TriggerOnGunshot)
		{
			PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
			if (player)
			{
				player.SetCombatLogTimer(player, null);
			}
		}
		return;
		#endif

		// Check on next frame if a player was shot at
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(DetectPlayerShot, 1, false);
	}

	// Detect if a player was shot at
	private void DetectPlayerShot()
	{
		// Check if we sent a gunshot RPC in the past 3 secs, if so, stop here to avoid spamming the server
		if (m_ZenPauseCombatRPC)
			return;

		// If weapon is not fired by our player, stop here.
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (!player || !player.IsControlledPlayer())
			return;

		// Prepare raycast vars
		float distance = GetAntiCombatLogWeaponDistance();
		vector start;
		vector direction;

		// Freelook raycast - taken from vanilla LiftWeaponCheck()
		if (player.GetInputController().CameraIsFreeLook())
		{
			if (player.m_DirectionToCursor != vector.Zero)
			{
				direction = player.m_DirectionToCursor;
			}
			// If player raised weapon in freelook
			else
			{
				direction = MiscGameplayFunctions.GetHeadingVector(player);
			}
		}
		else
		{
			direction = GetGame().GetCurrentCameraDirection(); // Exception for freelook. Much better this way!
		}

		int idx = player.GetBoneIndexByName("Neck");
		if (idx == -1)
		{
			start = player.GetPosition()[1] + 1.5;
		}
		else
		{
			start = player.GetBonePositionWS(idx);
		}

		vector end = start + (direction * distance);

		// Perform raycast
		float raycastRadius = 5.0;
		RaycastRVParams rayInput = new RaycastRVParams(start, end, player, raycastRadius);
		rayInput.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
		array<PlayerBase> informedPlayers = new array<PlayerBase>;

		if (DayZPhysics.RaycastRVProxy(rayInput, results))
		{
			if (results.Count() > 0)
			{
				bool skip = false;
				bool sentRPC = false;

				for (int i = 0; i < results.Count(); i++)
				{
					RaycastRVResult res = results.Get(i);

					/* //DEBUG
					EntityAI entity = EntityAI.Cast(res.obj);
					if (entity)
						entity = EntityAI.Cast(res.parent);

					if (!entity)
						continue;

					ZenFunctions.DebugMessage("ENTITY=" + entity.GetType());
					//END DEBUG */ 

					// Skip any objects that are not human
					if ((results[i].obj && !results[i].obj.IsMan()) && (!res.parent || !res.parent.IsMan()))
						continue;

					PlayerBase otherPlayer = PlayerBase.Cast(results[i].obj);

					if (!otherPlayer && res.parent && res.parent.IsMan())
						otherPlayer = PlayerBase.Cast(res.parent);

					// Allow shooting dead players to trigger shooter's combat log timer? 
					// Might assist with meta gaming (ie. shooting up a building and then checking logout timer to see if someone is still alive inside)
					if (otherPlayer /* && otherPlayer.IsAlive()*/)
					{
						// Raycast with a 5m radius can detect the same object multiple times, so filter results array
						skip = false;
						for (int x = 0; x < informedPlayers.Count(); x++)
						{
							if (informedPlayers.Get(x) == otherPlayer)
								skip = true;
						}

						if (!skip)
						{
							player.InformServerThatWeShotAt(otherPlayer);
							informedPlayers.Insert(otherPlayer);
							sentRPC = true;
						}
					}
				}

				// If we sent an RPC to the server tagging any player(s) we shot at, delay the next RPC event by 3 secs to avoid spamming the server with mag dumps
				if (sentRPC)
				{
					m_ZenPauseCombatRPC = true;
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ResetZenCombatRPC, ZRPC_DELAY_TIMER, false);
				}
			}
		}
	}

	// Reset RPC timer
	private void ResetZenCombatRPC()
	{
		m_ZenPauseCombatRPC = false;
	}

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