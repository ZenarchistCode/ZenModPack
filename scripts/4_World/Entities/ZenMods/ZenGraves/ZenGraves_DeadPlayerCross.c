class ZenGraves_DeadPlayerCross extends ItemBase
{
	// Used for cross identification (RPC)
	string m_PlayerName;
	string m_PlayerDescription;
	string m_CauseOfDeath;

	// Used for both identified graves and anonymous crosses (NetSync)
	int m_Day = -1;
	int m_Month = -1;
	int m_Year = -1;
	int m_DateFormat = 6;

	void ZenGraves_DeadPlayerCross() {}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

		if (GetGame().IsClient() && rpc_type == ZenRPCs.RECEIVE_GRAVE_RPC)
		{
			Param1<ref ZenGraves_RpcData> rpcData;

            if (!ctx.Read(rpcData))
            {
                Error("IMPORTANT ERROR: ZENGRAVESRPCs.RECEIVE_GRAVE_RPC failed to be read on client! Please tell Zenarchist he fucked up!");
                return;
            }

            // If cross is found, set player info
            m_PlayerName = rpcData.param1.PlayerName;
            m_PlayerDescription = rpcData.param1.PlayerDescription;
            m_Day = rpcData.param1.Day;
            m_Month = rpcData.param1.Month;
            m_Year = rpcData.param1.Year;
            m_DateFormat = rpcData.param1.DateFormat;
		}
	}

	override bool IsRefresherSignalingViable()
	{
		return false;
	}

	string GetZenGravePlayerName()
	{
		return m_PlayerName;
	}

	void SetDeathDate(int p_day, int p_month, int p_year, int p_format)
	{
		m_Day = p_day;
		m_Month = p_month;
		m_Year = p_year;
		m_DateFormat = p_format;
	}

	string GetDeathDate()
	{
		return GetZenGravesConfig().GetDate(m_DateFormat, m_Day, m_Month, m_Year);
	}

	override bool NameOverride(out string output)
	{
		output = m_PlayerName;

		if (GetDeathDate() != "")
			output = output + " - " + GetDeathDate();

		return true;
	};

	override bool DescriptionOverride(out string output)
	{
		output = m_PlayerDescription;
		output.Replace("#date", GetDeathDate());
		return true;
	};

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	void SpawnZenGravesRpcTrigger()
	{
		// If show player info is turned on, then spawn a trigger zone to RPC nearby players information about the grave cross. Otherwise stop here.
		if (!GetZenGravesConfig().ShowPlayerInfoOnCross)
		{
			GetZenGravesConfig().DebugMsg("SpawnZenGraves() - Don't spawn RPC trigger - ShowPlayerInfoOnCross=false");
			return;
		}

		// Spawn RPC trigger used to inform nearby clients of the cross's player name
		ZenGraves_RpcTrigger trigger = ZenGraves_RpcTrigger.Cast(GetGame().CreateObject("ZenGraves_RpcTrigger", GetPosition()));

		if (!trigger)
		{
			GetZenGravesConfig().DebugMsg("SpawnZenGraves() - Failed to spawn ZenGrave trigger!");
			return;
		}

		trigger.SetCross(this);
		trigger.SetCollisionSphere(10); // 10 meter radius to RPC cross data (TODO: CHANGE TO BOX)
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(m_Day);
		ctx.Write(m_Month);
		ctx.Write(m_Year);
		ctx.Write(m_DateFormat);
		ctx.Write(m_PlayerName);
		ctx.Write(m_PlayerDescription);
		ctx.Write(m_CauseOfDeath);
	};

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(m_Day))
			return false;

		if (!ctx.Read(m_Month))
			return false;

		if (!ctx.Read(m_Year))
			return false;

		if (!ctx.Read(m_DateFormat))
			return false;

		if (!ctx.Read(m_PlayerName))
			return false;

		if (!ctx.Read(m_PlayerDescription))
			return false;

		if (!ctx.Read(m_CauseOfDeath))
			return false;

		return true;
	};

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		SetAllowDamage(false);
		SpawnZenGravesRpcTrigger();
	}
};