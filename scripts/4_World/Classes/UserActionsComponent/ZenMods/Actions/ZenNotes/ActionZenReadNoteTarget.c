class ActionZenReadNoteTarget : ActionInteractBase
{
	void ActionZenReadNote()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "#read";
	}

	override string GetText()
	{
		return "#read";
	}

	override bool IsInstant()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!ZenModEnabled("ZenNotes"))
			return false;

		if (target && target.GetObject().IsInherited(ZenNote))
			return !player.IsPlacingLocal();

		return false;
	}

	override void OnStartServer(ActionData action_data)
	{
		ZenNote note = ZenNote.Cast(action_data.m_Target.GetObject());

		if (note)
		{
			auto data = new Param1<ref ZenNoteData>(note.GetNoteData());
			note.RPCSingleParam(ZenRPCs.RECEIVE_WRITTEN_NOTE, data, true, action_data.m_Player.GetIdentity());
		}
	}
}
