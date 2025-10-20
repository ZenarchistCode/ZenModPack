class ZenFunctions
{
	// ACTION CONDITIONS
	static const int	CAMERA_PITCH_TO_CATCH_RAIN = 45; // Camera pitch angle to catch rain
	static const float	REQUIRED_RAIN = 0.2;			 // Weather rain level required to catch rain
	static const float	REQUIRED_OVERCAST = 0.2;		 // Weather overcast level required to catch rain

	//! Returns true/false if the given item is found inside the given player's inventory
	static bool HasItemType(DayZPlayer player, string item)
	{
		item.ToLower();
		array<EntityAI> itemsArray = new array<EntityAI>;
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, itemsArray);

		for (int i = 0; i < itemsArray.Count(); i++)
		{
			if (itemsArray.Get(i))
			{
				string foundType = itemsArray.Get(i).GetType();
				foundType.ToLower();
				if (foundType == item)
					return true;
			}
		}

		return false;
	}

	//! Moves the entire inventory of oldItem into newItem, and optionally deletes the old item if deleteOldItem = true
	//! used for things like painting/dying items - ideally should be used on an item with identical cargo/attachments only
	static bool MoveInventory(notnull EntityAI oldItem, notnull EntityAI newItem, bool deleteOldItem = true)
	{
		if (!oldItem.GetInventory() || !newItem.GetInventory())
		{
			if (deleteOldItem)
			{
				GetGame().ObjectDelete(oldItem);
			}

			return false;
		}

		ItemBase oldItemBase = ItemBase.Cast(oldItem);
		ItemBase newItemBase = ItemBase.Cast(newItem);

		if (oldItemBase != NULL && newItemBase != NULL)
		{
			if (oldItemBase.IsMagazine() && newItemBase.IsMagazine()) 
			{
				Magazine magOld = Magazine.Cast(oldItemBase);
				Magazine magNew = Magazine.Cast(newItemBase);

				if (magOld == NULL || magNew == NULL)
					return false;

				magNew.ServerSetAmmoCount(magOld.GetAmmoCount());
			} else 
			if (oldItemBase.IsAmmoPile() && newItemBase.IsAmmoPile()) 
			{
				Ammunition_Base ammoOld = Ammunition_Base.Cast(oldItemBase);
				Ammunition_Base ammoNew = Ammunition_Base.Cast(newItemBase);

				if (ammoOld == NULL || ammoNew == NULL)
					return false;

				ammoNew.ServerSetAmmoCount(ammoOld.GetAmmoCount());
			} else
			if (oldItemBase.IsWeapon() && newItemBase.IsWeapon())
			{
				Weapon_Base source_wpn = Weapon_Base.Cast(oldItemBase);
				Weapon_Base target_wpn = Weapon_Base.Cast(newItemBase);

				if (source_wpn != NULL && target_wpn != NULL)
				{
					float damage = 0.0;
					string type;

					//target_wpn.CopyWeaponStateFrom(source_wpn);
					for (int mi = 0; mi < source_wpn.GetMuzzleCount(); ++mi)
					{
						if (!source_wpn.IsChamberEmpty(mi))
						{
							if (source_wpn.GetCartridgeInfo(mi, damage, type))
							{
								target_wpn.PushCartridgeToChamber(mi, damage, type);
							}
						}
			
						for (int ci = 0; ci < source_wpn.GetInternalMagazineCartridgeCount(mi); ++ci)
						{
							if (source_wpn.GetInternalMagazineCartridgeInfo(mi, ci, damage, type))
							{
								target_wpn.PushCartridgeToInternalMagazine(mi, damage, type);
							}
						}
					}
				}
			} else
			{
				newItemBase.SetQuantity(oldItemBase.GetQuantity(), true, true);
			}
		}

		array<EntityAI> children = new array<EntityAI>;
		oldItem.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
		int count = children.Count();
		float itemHealth;
		bool isRuined, isLocked;
		for (int i = 0; i < count; i++)
		{
			EntityAI child = children.Get(i);
			if (child)
			{
				isRuined = false;
				isLocked = false;
				if (child.IsRuined())
				{
					itemHealth = child.GetHealth("", "");
					child.SetHealthMax("", "");
					isRuined = true;

				}

				if (child.IsLockedInSlot())
				{
					isLocked = true;
				}

				InventoryLocation child_src = new InventoryLocation;
				child.GetInventory().GetCurrentInventoryLocation(child_src);

				InventoryLocation child_dst = new InventoryLocation;
				child_dst.Copy(child_src);
				child_dst.SetParent(newItem);

				if (GameInventory.LocationCanAddEntity(child_dst))
				{
					newItem.GetInventory().TakeToDst(InventoryMode.SERVER, child_src, child_dst);

					if (isRuined)
					{
						child_dst.GetItem().SetHealth("", "", itemHealth);
					}

					if (isLocked)
					{
						ItemBase ib = ItemBase.Cast(child_dst.GetItem());
						if (ib)
							ib.LockToParent();
					}
				}
			}
		}

		if (deleteOldItem)
		{
			GetGame().ObjectDelete(oldItem);
		}

		newItem.SetSynchDirty();
		return true;
	}

	//! Set the given item's quantity regardless of its type (itembase, weapon, ammo, magazine etc)
	static bool SetQuantity(notnull ItemBase item, float quantity)
	{
		if (item.IsMagazine())
		{
			Magazine magNew = Magazine.Cast(item);

			if (magNew == NULL)
				return false;

			magNew.ServerSetAmmoCount((int)Math.Round(quantity));
			return true;
		}

		if (item.IsAmmoPile())
		{
			Ammunition_Base ammoNew = Ammunition_Base.Cast(item);

			if (ammoNew == NULL)
				return false;

			ammoNew.ServerSetAmmoCount((int)Math.Round(quantity));
			return true;
		}

		return item.SetQuantity(quantity, true, true);
	}

	//! Debug message - sends a server-side player message to all online players
	static void SendGlobalMessage(string msg)
	{
		SendGlobalMessageEx(msg, "[SERVER] ");
	}

	//! Debug message - sends a server-side player message to all online players
	static void SendGlobalMessageEx(string msg, string prefix = "")
	{
		#ifdef SERVER
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		for (int x = 0; x < players.Count(); x++)
		{
			PlayerBase pb = PlayerBase.Cast(players.Get(x));
			if (pb)
			{
				SendPlayerMessage(pb, prefix + msg);
			}
		}
		#endif
	}

	//! Display client message ONLY on client
	static void ZenClientMessage(string message)
	{
#ifndef SERVER
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
		}
#endif
	}

	//! Print a debug chat message both client-side & server-side
	static void DebugMessage(string message)
	{
		ZenClientMessage("[CLIENT] " + message);
		SendGlobalMessage(message);
	}

	//! For client-only error message
	static void ZenClientError(string message)
	{
		ZenClientMessage("[CLIENT ERROR] " + message);
		Error("[ZENMODPACK CLIENT ERROR] " + message);
	}

	//! Send a message to the given player
	static void SendPlayerMessage(PlayerBase player, string msg)
	{
#ifdef SERVER
		if (msg == "" || msg == string.Empty)
			return;

		if (!player || player.IsPlayerDisconnected() || !player.GetIdentity())
			return;

		Param1<string> m_MessageParam = new Param1<string>("");
		if (m_MessageParam && msg != "")
		{
			m_MessageParam.param1 = msg;
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
		}
#endif
	}

	//! Is it currently raining heavily and overcast?
	static bool IsRaining()
	{
		return GetGame().GetWeather().GetRain().GetActual() >= REQUIRED_RAIN && GetGame().GetWeather().GetOvercast().GetActual() >= REQUIRED_OVERCAST;
	}

	//! Client-side only. Gets camera's vertical angle up/down. Useful for checking if player is looking at ground or sky.
	static int GetCameraPitch()
	{
		if (!GetGame())
			return 0;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		float pitch = 0;

		if (player)
		{
			DayZPlayerCamera camera = player.GetCurrentCamera();

			if (camera)
				pitch = camera.GetCurrentPitch();
		}

		return pitch;
	}

	//! Aligns given object to underlying terrain
	static void AlignToTerrain(Object obj)
	{
		vector transform[4];
		obj.GetTransform(transform);
		vector ground_position, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground_position, ground_dir, component, null, null, null, false, true);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		vector local_ori = obj.GetDirection();
		transform[0] = surface_normal * local_ori;
		transform[1] = surface_normal;
		transform[2] = surface_normal * (local_ori * vector.Up);
		obj.SetTransform(transform);
		obj.Update();
	}

	//! Orientates given object to vector pos. Thanks ChatGPT ;) I might have failed math, but I still know how to write a good prompt
	static void OrientObjectToPosition(Object object, vector targetPos, vector oriOffset = "0 0 0")
	{
		vector startPos = object.GetPosition();
		vector direction = targetPos - startPos;

		float yawRadians = Math.Atan2(direction[0], direction[2]);
		float yawDegrees = yawRadians * Math.RAD2DEG;

		if (yawDegrees < 0)
			yawDegrees += 360;

		// Build orientation vector directly
		vector newOri = Vector(yawDegrees + oriOffset[0], oriOffset[1], oriOffset[2]);

		object.SetOrientation(newOri);
	}

	//! Generate a random point inside a circle. minDistance = minimum distance in meters from center (again thanks ChatGPT)
	static vector GetRandomPointInCircle(vector position, float radius, float minDistance = 0, bool placeOnSurface = true)
	{
		float angle = Math.RandomFloatInclusive(0.0, Math.PI2);

		// Uniform distribution by using sqrt of random float
		float rand = Math.RandomFloatInclusive(0.0, 1.0);
		float distance = Math.Sqrt(rand) * (radius - minDistance) + minDistance;

		float x = distance * Math.Cos(angle);
		float z = distance * Math.Sin(angle);

		vector newPos = position;
		newPos[0] = newPos[0] + x;
		newPos[2] = newPos[2] + z;

		if (placeOnSurface)
			newPos[1] = GetGame().SurfaceY(newPos[0], newPos[2]);

		return newPos;
	}

	//! "Pulls" backwards the entity's position slightly from its current location towards the player.
	//! Useful for adjusting holograms which place themselves too far inside walls etc. (Thanks yet again ChatGPT)
	static vector MovePositionCloserToThisPosition(vector originalPosition, vector playerOrientation, float offsetDistance)
	{
		// Get the player's orientation (yaw, pitch, roll)
		vector orientation = playerOrientation;

		// Convert orientation from degrees to radians
		float yaw = Math.DEG2RAD * orientation[0];

		// Calculate the offset vector based on player's yaw
		vector offset = Vector(-Math.Sin(yaw), 0, Math.Cos(yaw)) * offsetDistance;

		// Calculate the new position by adding the offset to the original position adjusted for angle
		return originalPosition + offset;
	}

	//! Returns a readable time string based on the seconds
	static string GetTimeToString(int totalSeconds, bool showHours = true, bool showMinutes = true, bool showSeconds = true)
	{
		int days = totalSeconds / 86400;        // 86400 seconds in a day
		int remainingSeconds = totalSeconds - (days * 86400);

		int hours = remainingSeconds / 3600;    // 3600 seconds in an hour
		remainingSeconds = remainingSeconds - (hours * 3600);

		int minutes = remainingSeconds / 60;    // 60 seconds in a minute
		int seconds = remainingSeconds - (minutes * 60);

		string formattedTime = "";

		if (days > 0)
		{
			formattedTime += days.ToString() + " #STR_ZenGui_Days";
		}

		if (showHours && hours > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += hours.ToString() + " #STR_ZenGui_Hours";
		}

		// Optionally include minutes and seconds
		if (showMinutes && minutes > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += minutes.ToString() + " #STR_ZenGui_Minutes";
		}

		if (showSeconds && seconds > 0)
		{
			if (formattedTime != "")
				formattedTime += " ";

			formattedTime += seconds.ToString() + " #STR_ZenGui_Seconds";
		}

		return formattedTime;
	}

	//! Check for roof above given position (no entity required)
	static bool IsUnderRoof(vector from, float height = GameConstants.ROOF_CHECK_RAYCAST_DIST, int geometry = ObjIntersectView)
	{
		vector to = from;
		to[1] = to[1] + height;

		vector contact_pos;
		vector contact_dir;

		int contact_component;
		bool boo = DayZPhysics.RaycastRV(from, to, contact_pos, contact_dir, contact_component, NULL, NULL, NULL, false, false, geometry, 0.25);

		return boo;
	}

	//! Return a list of objects the player is aiming at within X meters distance
	static array<Object> GetObjectsRayCastCamera(float distance = UAMaxDistances.BASEBUILDING, PlayerBase ourPlayer = NULL)
	{
		array<Object> aimedObjects = new array<Object>;

		if (!ourPlayer)
			ourPlayer = PlayerBase.Cast(GetGame().GetPlayer());

		if (!ourPlayer)
			return aimedObjects;

		int hitComponentIndex;
		vector playerPos = ourPlayer.GetPosition();
		vector headingDirection = MiscGameplayFunctions.GetHeadingVector(ourPlayer);

		vector m_RayStart = GetGame().GetCurrentCameraPosition();
		vector m_RayEnd = m_RayStart + GetGame().GetCurrentCameraDirection() * distance;

		RaycastRVParams rayInput = new RaycastRVParams(m_RayStart, m_RayEnd, ourPlayer);
		rayInput.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
		RaycastRVResult res;

		if (DayZPhysics.RaycastRVProxy(rayInput, results))
		{
			for (int i = 0; i < results.Count(); i++)
			{
				if (results.Get(i).obj == ourPlayer)
					continue;

				aimedObjects.Insert(results.Get(i).obj);
			}
		}

		return aimedObjects;
	}

	//! Return a list of objects the player is standing on
	static array<Object> GetObjectsRayCastBeneath(float distance = 2.0, PlayerBase ourPlayer = NULL)
	{
		array<Object> aimedObjects = new array<Object>;

		if (!ourPlayer)
			ourPlayer = PlayerBase.Cast(GetGame().GetPlayer());

		if (!ourPlayer)
			return aimedObjects;

		int hitComponentIndex;
		vector playerPos = ourPlayer.GetPosition();
		vector headingDirection = MiscGameplayFunctions.GetHeadingVector(ourPlayer);

		vector distanceVector = ourPlayer.GetPosition();
		distanceVector[1] = distanceVector[1] - distance;

		vector m_RayStart = ourPlayer.GetPosition();
		vector m_RayEnd = m_RayStart + (ourPlayer.GetPosition() - distanceVector);

		RaycastRVParams rayInput = new RaycastRVParams(m_RayStart, m_RayEnd, ourPlayer);
		rayInput.flags = CollisionFlags.ALLOBJECTS;
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>;
		RaycastRVResult res;

		if (DayZPhysics.RaycastRVProxy(rayInput, results))
		{
			for (int i = 0; i < results.Count(); i++)
			{
				if (results.Get(i).obj == ourPlayer)
					continue;

				aimedObjects.Insert(results.Get(i).obj);
			}
		}

		return aimedObjects;
	}

	//! Enable/disable player controls - WARNING: This function does not check if controls should be enabled given what the player is doing at the time when it's called, so use carefully and thoughtfully
	static void SetPlayerControl(bool isEnabled = true, bool hideHud = false)
	{
		if (!GetGame())
			return;

		if (!GetGame().GetMission())
			return;
			
		if (!GetGame().IsClient())
			return;

		if (!isEnabled)
		{
			GetGame().GetMission().AddActiveInputExcludes({"menu"});

			if (hideHud)
				GetGame().GetMission().GetHud().Show(false);
		}
		else
		{
			GetGame().GetMission().RemoveActiveInputExcludes({"menu"});

			if (hideHud)
				GetGame().GetMission().GetHud().Show(true);
		}
	}

	static void EnablePlayerControl()
	{
		SetPlayerControl(true, false);
	}

	static void DisablePlayerControl()
	{
		SetPlayerControl(false, false);
	}

	// Sends a GameLabs CFTools Cloud deployed message @ the given position.
	// !NOTE: Will use a random player to "deploy" the message and position,
	//		  so disregard the player in whatever message is sent.
	static void GameLabs_SendServerDeployed(vector pos, string message)
	{
#ifdef SERVER
#ifdef GAMELABS
		array<Man> players = new array<Man>;
		g_Game.GetWorld().GetPlayerList(players);
		if (!players || players.Count() == 0)
			return;

		message.ToUpper();

		_LogPlayerEx logObjectPlayer = new _LogPlayerEx(PlayerBase.Cast(players.Get(0)));
		logObjectPlayer.position = pos;
		_Payload_ItemPlace payload = new _Payload_ItemPlace(logObjectPlayer, message);
		GetGameLabs().GetApi().ItemPlace(new _Callback(), payload);
#endif
#endif
	}
}