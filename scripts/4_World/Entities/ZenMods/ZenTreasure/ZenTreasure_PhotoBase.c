class ZenTreasure_PhotoBase extends ItemBase
{
	int m_StashType = -1;
	vector m_StashPosition;

	// Called when Central Economy engine spawns this item into the world as loot
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		// This makes it so you don't gotta set cfgspawnabletypes.xml to spawn photos Pristine.
		// The photos are impossible to see once damaged, so let's always spawn them Pristine.
		// If they get damaged by zombies/combat/etc later.. well, tough luck survivor.
		SetHealth(GetMaxHealth());
	}

	void ZenTreasure_PhotoBase()
	{
		RegisterNetSyncVariableInt("m_StashType");

		// Get photo number for texture purposes (remove 'ZenTreasure_Photo' to get number text only)
		string className = GetType();
		string substringNumber = className.Substring(17, className.Length() - 17);
		int photoNumber = substringNumber.ToInt();
		if (photoNumber <= 9)
			substringNumber = "0" + photoNumber.ToString();
		else
			substringNumber = photoNumber.ToString();

		// Assign texture
		AsssignPhotoTexture(substringNumber);
	}

	void AsssignPhotoTexture(string photoNumber)
	{
		if (IsWinterPhoto())
		{
			if (GetHiddenSelectionsTextures().Count() > 0)
				SetObjectTexture(0, GetHiddenSelectionsTextures().Get(1));
			else 
				SetObjectTexture(0, "ZenTreasure/data/photos/photo" + photoNumber + "_winter.paa");
		}
		else 
		{
			if (GetHiddenSelectionsTextures().Count() > 0)
				SetObjectTexture(0, GetHiddenSelectionsTextures().Get(0));
			else 
				SetObjectTexture(0, "ZenTreasure/data/photos/photo" + photoNumber + ".paa");
		}
	}

	void AssignRandomStashType()
	{
		if (m_StashType != -1)
			return;

		// Assign a random loot type to this photo
		if (GetZenTreasureConfig().DebugAlwaysSpawnStashID == -1)
			m_StashType = Math.RandomIntInclusive(0, GetZenTreasureConfig().TreasureTypes.Count() - 1);
		else 
			m_StashType = GetZenTreasureConfig().DebugAlwaysSpawnStashID;

		SetSynchDirty();
	}

	override void EEInit()
	{
		super.EEInit();

		m_StashPosition = GetBuryPosition();
	}

	static bool IsWinterPhoto()
	{
		#ifdef WINTER // My winter define in @HardcoreDayZServerPack, used for season changes
		return true;
		#endif

		return GetZenTreasureConfig().IsWinterMap;
	}

	override bool DescriptionOverride(out string output)
	{
		string cfgPath = "CfgVehicles ZenTreasure_PhotoBase";
		string treasureTypeText = "";

		if (SpawnedStash())
		{
			cfgPath = "CfgVehicles " + GetType();
			treasureTypeText = ZenTreasureConfig.TreasureDescriptions.Get(m_StashType);
		}

		GetGame().ConfigGetText(cfgPath + " descriptionShort", output);
		output.Replace("TREASURETYPE", treasureTypeText);

		return true;
	}

	// Get bury location from *.cpp file config
	vector GetBuryPosition()
	{
		string cfgPath = "CfgVehicles " + GetType();

		float x = GetGame().ConfigGetFloat(cfgPath + " treasure_stash_x");
		float y = GetGame().ConfigGetFloat(cfgPath + " treasure_stash_z");
		float z = GetGame().SurfaceY(x, y);

		vector buryPos = vector.Zero;
		buryPos[0] = x;
		buryPos[1] = z;
		buryPos[2] = y;

		return buryPos;
	}

	int GetStashType()
	{
		return m_StashType;
	}

	void SetStashType(int type)
	{
		m_StashType = type;
		SetSynchDirty();
	}

	bool SpawnedStash()
	{
		return m_StashType != -1;
	}

	void SpawnStash(PlayerBase player)
	{
		AssignRandomStashType();

		// Spawn stash!
		SpawnTreasureTrigger(player);

		// Debug teleport?
		if (GetZenTreasureConfig().DebugTeleportToPhoto)
		{
			DeveloperTeleport.SetPlayerPosition(player, m_StashPosition);
		}
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionZenReadPhoto);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_StashType))
			return false;

		return true;
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_StashType);
	}

	void PhotoDebugPrint(string msg)
	{
		msg = "[ZenTreasure] " + msg;
		ZenFunctions.DebugMessage(msg);
		ZMPrint(msg);
	}

	void SpawnTreasureTrigger(PlayerBase player)
	{
		if (!player)
		{
			Error("[ZenTreasure] Treasure stashed was spawned with no valid Player responsible for it!");
			return;
		}

		ZenTreasureStashType treasureConfig = GetZenTreasureConfig().TreasureTypes.Get(m_StashType);
		if (!treasureConfig)
		{
			Error("[ZenTreasure] Could not spawn treasure loot trigger - config not found for m_StashType=" + m_StashType);
			return;
		}

		GetZenTreasure_Triggers().SpawnTrigger(GetZenTreasureConfig_SpawnTriggers().AddTreasureTrigger(m_StashPosition, player.GetCachedID(), m_StashType), player);
		ZenModLogger.Log(player.GetCachedID() + " spawned treasure stash TRIGGER @ " + m_StashPosition + " with config " + treasureConfig.ConfigName, "ZenTreasure");
	}
};
