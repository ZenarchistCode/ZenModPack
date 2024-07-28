modded class Hologram
{
    override void EvaluateCollision(ItemBase action_item = null)
	{
        //! NOTES
		if (action_item && action_item.IsInherited(ZenNote))
		{
			SetIsColliding(false);
			return;
		}

		super.EvaluateCollision(action_item);
	}

    override void SetProjectionPosition(vector position)
	{
        //! MUSIC
		Zen_BoomBox boombox = Zen_BoomBox.Cast(m_Parent);
		if (boombox)
		{
			m_Projection.SetPosition(position + "0 0.15 0");
			
			if (IsFloating())
			{
				m_Projection.SetPosition(SetOnGround(position + "0 0.15 0"));
			}

			return;
		}

        //! KIT BOX
        ZenKitBoxBase zen_kit = ZenKitBoxBase.Cast(m_Parent);
        if (zen_kit != NULL)
        {
            m_Projection.SetPosition(position + zen_kit.GetDeployPositionOffset());
            return;
        }

		super.SetProjectionPosition( position );
	}

    override void SetProjectionOrientation(vector orientation)
    {
        //! RAID ALARM
        ZenKitBoxBase zen_kit = ZenKitBoxBase.Cast(m_Parent);
        if (zen_kit != NULL)
        {
            m_Projection.SetOrientation(orientation + zen_kit.GetDeployOrientationOffset());
            return;
        }

        super.SetProjectionOrientation(orientation);
    }

    override string ProjectionBasedOnParent()
    {
        //! CAMO SHELTER
        if (m_Parent.IsInherited(Zen_CamoShelterKit))
        {
            Zen_CamoShelterKit camoShelterKit = Zen_CamoShelterKit.Cast(m_Parent);
            return camoShelterKit.GetHologram();
        }

        //! CAR WORKBENCH 
        Zen_CarWorkbenchKit kit = Zen_CarWorkbenchKit.Cast(m_Parent);
		if (kit != NULL)
			return kit.GetDeployedClassname();

        //! SLEEPING BAGS 
        if (m_Parent.IsKindOf("ZenSleepingBag_PackedBase"))
        {
            string newType = m_Parent.GetType();
            newType.Replace("_Packed", "_Deployed");
            return newType;
        }

        //! RAID ALARM
        Zen_RaidAlarmStationKit station_kit = Zen_RaidAlarmStationKit.Cast(m_Parent);
        if (station_kit != NULL)
            return station_kit.GetDeployedClassname();

        //! KIT BOX
        ZenKitBoxBase zen_kit = ZenKitBoxBase.Cast(m_Parent);
        if (zen_kit != NULL)
            return zen_kit.GetDeployedClassname();

        return super.ProjectionBasedOnParent();
    }

    override EntityAI PlaceEntity(EntityAI entity_for_placing)
    {
        //! RAID ALARM
        string itemType = entity_for_placing.GetType();
        itemType.ToLower();

        // Check if this item is on our raid config
        foreach(string s : GetZenDiscordConfig().ItemsDeployedTriggerRaidAlert)
        {
            s.ToLower();
            if (itemType == s || entity_for_placing.IsKindOf(s))
            {
                ZenRaidAlarmPlugin plugin = ZenRaidAlarmPlugin.Cast(GetPlugin(ZenRaidAlarmPlugin));
                if (plugin)
                    plugin.AlertNearestRaidStation(entity_for_placing.GetPosition());

                break;
            }
        }

        //! CAMO SHELTER
        if (m_Parent.IsInherited(Zen_CamoShelterKit))
        {
            // Get kit
            Zen_CamoShelterKit kit = Zen_CamoShelterKit.Cast(m_Parent);

            // Create shelter on the ground in location of hologram
            Zen_ImprovisedShelter shelter = Zen_ImprovisedShelter.Cast(GetGame().CreateObjectEx("Zen_ImprovisedShelter", m_Projection.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS));

            // Validate object existence
            if (!shelter)
                return entity_for_placing;

            // Set orientation
            shelter.SetPosition(m_Projection.GetPosition());
            shelter.SetOrientation(m_Projection.GetOrientation());
            shelter.SetSynchDirty();

            // Save ingredient health
            if (kit)
            {
                EntityAI rope = EntityAI.Cast(kit.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Rope")));
                EntityAI net = EntityAI.Cast(kit.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("CamoNet")));

                if (rope && net)
                    shelter.SetIngredientHealth(rope.GetHealth(), net.GetHealth());
            }

            return shelter;
        }

        return super.PlaceEntity(entity_for_placing);
    }

	override void RefreshVisual()
    {
        super.RefreshVisual();

        if (!m_Parent)
            return;

        //! SHARED
        if (m_Parent.ShouldZenHologram() && m_Player.IsPlacingLocal())
        {
            m_Parent.SetZenHologrammed(m_Projection != NULL);
        }
    }

	void ~Hologram()
    {
        if (!GetGame())
            return;

        if (!m_Parent)
            return;

        //! SHARED
        if (m_Parent.IsZenHologrammed())
        {
            m_Parent.SetZenHologrammed(false);
        }
    }
}
