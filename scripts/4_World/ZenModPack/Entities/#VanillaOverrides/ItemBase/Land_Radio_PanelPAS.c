modded class Land_Radio_PanelPAS extends PASBroadcaster
{
	static ref array<Land_Radio_PanelPAS> Zen_PAS_List = new array<Land_Radio_PanelPAS>;

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		ItemBase itemBase = ItemBase.Cast(item);

		if (g_Game.IsDedicatedServer() && itemBase)
		{
            if (!ZenModEnabled("ZenBatteryPASPersistence"))
                return;

			if (!itemBase.IsRuined() && itemBase.GetHealth() > 0)
			{
				Zen_PAS_List.Insert(this);
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		int removeIdx = -1;
		for (int i = 0; i < Zen_PAS_List.Count(); i++)
		{
			if (vector.Distance(Zen_PAS_List.Get(i).GetPosition(), GetPosition()) < 1)
			{
				removeIdx = i;
				break;
			}
		}

		Zen_PAS_List.Remove(removeIdx);
	}

	void ZenCreateBattery(string type, float health, float energy)
	{
		ItemBase battery = ItemBase.Cast(GetInventory().CreateAttachment(type));

		if (!battery || !battery.GetCompEM())
			return;

		battery.SetHealth(health);
		battery.GetCompEM().SetEnergy(energy);
	}
}