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
				else 
				{
					ZenFunctions.DebugMessage("ITS NULL!");
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

	//! Debug message - sends a server-side player message to all online players
	static void SendGlobalMessage(string msg)
	{
		#ifdef SERVER
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		for (int x = 0; x < players.Count(); x++)
		{
			PlayerBase pb = PlayerBase.Cast(players.Get(x));
			if (pb)
			{
				pb.Zen_SendMessage("[SERVER] " + msg);
			}
		}
		#endif
	}

	//! Print a debug chat message both client-side & server-side
	static void DebugMessage(string message)
	{
		#ifndef SERVER
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "[CLIENT] " + message, ""));
		}
		#endif

		SendGlobalMessage(message);
	}

	//! For non-debug client messages
	static void ZenClientMessage(string message)
	{
		#ifndef SERVER
		if (GetGame().GetPlayer())
		{
			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
		}
		#endif
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
		if (!player || player.IsPlayerDisconnected() || !player.GetIdentity())
			return;

		Param1<string> m_MessageParam = new Param1<string>("");
		if (GetGame().IsDedicatedServer() && m_MessageParam && msg != "")
		{
			m_MessageParam.param1 = msg;
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
		}
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
		// Calculate direction vector from object to target
		vector startPos = object.GetPosition();
		vector direction = targetPos - startPos;

		// Calculate yaw angle in radians. Note that in Enfusion's coordinate system,
		// Z is typically the forward axis and X is the side axis.
		float yawRadians = Math.Atan2(direction[0], direction[2]); // [0] is X, [2] is Z

		// Convert radians to degrees
		float yawDegrees = yawRadians * Math.RAD2DEG;

		// Adjust the angle to be between 0 and 360 degrees if necessary
		if (yawDegrees < 0)
			yawDegrees += 360;

		// Set object orientation's yaw angle
		vector objectOri = object.GetOrientation();
		objectOri[0] = yawDegrees;

		object.SetOrientation(objectOri + oriOffset);
	}

	//! Generate a random point inside a circle. minDistance = minimum distance in meters from center (again thanks ChatGPT)
	static vector GetRandomPointInCircle(vector position, float radius, float minDistance = 0)
	{
		// Get a random angle between 0 and 2*PI
        float angle = Math.RandomFloatInclusive(0.0, Math.PI2);
        
        // Convert polar coordinates to Cartesian coordinates
        float x = (minDistance + Math.RandomFloatInclusive(0.0, radius - minDistance)) * Math.Cos(angle);
        float z = (minDistance + Math.RandomFloatInclusive(0.0, radius - minDistance)) * Math.Sin(angle);
        
        // Adjust new position and return it
		vector newPos = position;
		newPos[0] = newPos[0] + x;
		newPos[2] = newPos[2] + z;
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
}