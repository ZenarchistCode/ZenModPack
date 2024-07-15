modded class ActionDigOutStashCB
{
	override void CreateActionComponent()
	{
		super.CreateActionComponent();

		if (m_ActionData.m_MainItem && m_ActionData.m_MainItem.IsInherited(ZenTreasure_DebugShovel))
			m_ActionData.m_ActionComponent = new CAContinuousTime(0.01);
	}
}
