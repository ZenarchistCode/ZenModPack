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
        //! MUSIC (TODO: Figure out why this 3d model doesn't place correctly as hologram, broken p3d LODs?)
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

		super.SetProjectionPosition( position );
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

        return super.ProjectionBasedOnParent();
    }

    override EntityAI PlaceEntity(EntityAI entity_for_placing)
    {
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

    //! SLEEPING BAG
	override void RefreshVisual()
    {
        super.RefreshVisual();

        if (m_Parent && m_Parent.IsInherited(ZenSleepingBag_PackedBase) && m_Player.IsPlacingLocal())
        {
            ZenSleepingBag_PackedBase bag = ZenSleepingBag_PackedBase.Cast(m_Parent);
            if (bag != NULL)
            {
                bag.SetHologrammed(m_Projection != NULL);
            }
        }
    }

	void ~Hologram()
    {
        if (GetGame())
        {
            if (m_Parent && m_Parent.IsInherited(ZenSleepingBag_PackedBase))
            {
                ZenSleepingBag_PackedBase bag = ZenSleepingBag_PackedBase.Cast(m_Parent);
                if (bag != NULL)
                {
                    bag.SetHologrammed(false);
                }
            }
        }
    }
}
