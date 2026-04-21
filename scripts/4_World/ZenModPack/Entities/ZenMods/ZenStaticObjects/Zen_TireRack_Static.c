class Zen_TireRack_Static extends Zen_TireRack
{
    override void DeferredInit()
    {
        super.DeferredInit();

        if (!g_Game.IsDedicatedServer())
            return;

        SetAllowDamage(false);
        SetLifetimeMax(999999);
        SetLifetime(999999);
        IncreaseLifetime();

        // Get objects within 1 meter of the spawn
        array<Object> objectsNearby = new array<Object>;
        g_Game.GetObjectsAtPosition3D(GetPosition(), 0.1, objectsNearby, null);
        
        foreach (Object z_obj : objectsNearby)
        {
            if (z_obj != this && z_obj.GetType() == GetType())
            {
                g_Game.ObjectDelete(z_obj);
                Error("Duplicate " + GetType() + " found @ " + GetPosition() + " - this item should only be spawned ONCE! Comment out any spawn code and leave it uncommented until next server wipe.");
            }
        }

        if (GetZenStaticTireRackConfig().AttachmentTypes.Count() == 0)
            return;

        int attachmentsCount = GetInventory().AttachmentCount();
        int freeSlots = GetZenStaticTireRackConfig().MaxAttachmentCount - attachmentsCount;

        if (freeSlots <= 0)
            return;

        for (int i = 0; i < freeSlots; i++)
        {
            string className = GetZenStaticTireRackConfig().AttachmentTypes.GetKeyArray().GetRandomElement();
            float chance = GetZenStaticTireRackConfig().AttachmentTypes.Get(className);

            if (Math.RandomFloat01() > chance)
                continue;

            ItemBase newAttachment = ItemBase.Cast(GetInventory().CreateAttachment(className));
            if (!newAttachment && !ZenFunctions.GetItemOnSlotByType(this, className))
            {
                Error("[Zen_TireRack_Static] Failed to spawn item class - ensure it exists: " + className);
                continue;
            }

            if (newAttachment)
            {
                newAttachment.SetHealth(newAttachment.GetMaxHealth() * (Math.RandomFloat01() + 0.01));
            }
        }
    }

	override bool IsRefresherSignalingViable()
	{
		return false;
	}

    override bool DisableVicinityIcon()
	{
		return GetInventory().AttachmentCount() == 0;
	}

	override int GetHideIconMask()
	{
        if (GetInventory().AttachmentCount() == 0)
		    return EInventoryIconVisibility.HIDE_VICINITY;

        return super.GetHideIconMask();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
        return false;
	}

	override bool IsTakeable()
	{
        return false;
	}

	override bool CanBeActionTarget()
	{
        return false;
    }
}