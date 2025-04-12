// This spawns an event trigger that is used to RPC the cross data to any player who walks nearby
class ZenGraves_RpcTrigger extends Trigger
{
	private ZenGraves_DeadPlayerCross m_Cross;
	private ref ZenGraves_RpcData m_CrossData;
	private ref Param1<ref ZenGraves_RpcData> m_rpcData;

	void SetCross(ZenGraves_DeadPlayerCross p_cross)
	{
		m_Cross = p_cross;

		if (p_cross && !m_CrossData)
		{
			m_CrossData = new ZenGraves_RpcData;
			m_CrossData.Position = m_Cross.GetPosition();
			m_CrossData.PlayerName = m_Cross.m_PlayerName;
			m_CrossData.PlayerDescription = m_Cross.m_PlayerDescription;
			m_CrossData.Day = m_Cross.m_Day;
			m_CrossData.Month = m_Cross.m_Month;
			m_CrossData.Year = m_Cross.m_Year;
			m_CrossData.DateFormat = m_Cross.m_DateFormat;
			m_rpcData = new Param1<ref ZenGraves_RpcData>(m_CrossData);
			ZMPrint("[ZenModPack] ZenGraves::ZenGraves_RpcTrigger - Spawned dead player grave @ " + m_Cross.GetPosition());
		}
	}

	override void OnEnter(Object obj)
	{
		if (!obj.IsMan() || !m_Cross || !m_rpcData || !GetGame().IsDedicatedServer())
			return;

		PlayerBase player = PlayerBase.Cast(obj);
		if (!player || !player.GetIdentity())
			return;

		Param1<bool> params = new Param1<bool>(true);
		m_Cross.RPCSingleParam(ZenRPCs.RECEIVE_GRAVE_RPC, m_rpcData, true, player.GetIdentity());
	};
};