modded class Hologram
{
    override string ProjectionBasedOnParent()
    {
        //! CAMO SHELTER
        if (m_Parent.IsInherited(Zen_CamoShelterKit))
        {
            Zen_CamoShelterKit camoShelterKit = Zen_CamoShelterKit.Cast(m_Parent);
            return camoShelterKit.GetHologram();
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
            Zen_ImprovisedShelter shelter = Zen_ImprovisedShelter.Cast(g_Game.CreateObjectEx("Zen_ImprovisedShelter", m_Projection.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS));

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

    override bool IsBaseViable()
    {
        if (!ZenModEnabled("ZenBasebuildingConfig") || g_Game.IsDedicatedServer())
            return super.IsBaseViable();

        if (!super.IsBaseViable())
            return false;

        if (!CheckZenPlacementInsideSelf())
            return false;

        if (!CheckZenBuildingLimits())
            return false;

        return true;
    }

    bool CheckZenBuildingLimits()
    { 
        array<Object> nearest_objects = new array<Object>;

        foreach (ZenBasebuildingLimits limitCfg : GetZenBasebuildingConfig().BasebuildingLimits)
        {
            nearest_objects.Clear();
            g_Game.GetObjectsAtPosition3D(m_Projection.GetPosition(), limitCfg.MinSearchDistance, nearest_objects, NULL);

            for (int i = 0; i < nearest_objects.Count(); i++)
            {
                Object object = nearest_objects.Get(i);

                if (object.IsHologram())
                    continue;

                if (!object.IsKindOf(limitCfg.ClassName))
                    continue;

                // ChatGPT strikes again
                vector diff = m_Projection.GetPosition() - object.GetPosition();
                float heightDiff = Math.AbsFloat(diff[1]);

                vector dir = m_Projection.GetDirection();
                dir[1] = 0; // flatten to horizontal plane
                dir.Normalize();

                // Manually rotate dir 90 degrees around Y to get rightLeft vector
                vector rightLeft;
                rightLeft[0] = -dir[2];
                rightLeft[1] = 0;
                rightLeft[2] = dir[0];

                // Project diff into local space
                float forwardDist = vector.Dot(diff, dir);   // depth (front/back)
                float rightLeftDist = vector.Dot(diff, rightLeft);   // sideways (left/right)

                if (heightDiff < limitCfg.MinHeightSeparation)
                {
                    if (Math.AbsFloat(rightLeftDist) < limitCfg.MinRightLeftTolerance && Math.AbsFloat(forwardDist) < limitCfg.MinForwardTolerance)
                    {
                        if (GetZenBasebuildingConfig().DebugOn)
                            ZenFunctions.DebugMessage("Blocked: " + object.GetType() + "/" + m_Projection.GetType() + " rightLeftDist=" + rightLeftDist + " forwardDist=" + forwardDist + " heightDiff=" + heightDiff);
                        
                        return false;
                    }
                }
            }
        }

        return true;
    }

    bool CheckZenPlacementInsideSelf()
    {
        for (int i = 0; i < GetZenBasebuildingConfig().CantPlaceInsideOfSelf.Count(); i++)
        {
            string type = GetZenBasebuildingConfig().CantPlaceInsideOfSelf.GetKey(i);
            float dist = GetZenBasebuildingConfig().CantPlaceInsideOfSelf.GetElement(i);

            array<Object> nearest_objects = new array<Object>;
            g_Game.GetObjectsAtPosition3D(m_Projection.GetPosition(), dist, nearest_objects, NULL);

            foreach (Object obj : nearest_objects)
            {
                if (obj.IsHologram())
                    continue;

                if (obj.IsKindOf(type) && m_Projection.IsKindOf(type))
                {
                    if (GetZenBasebuildingConfig().DebugOn)
                        ZenFunctions.DebugMessage("Blocked: " + obj.GetType() + "/" + m_Projection.GetType() + " dist=" + vector.Distance(obj.GetPosition(), m_Projection.GetPosition()));
                    
                    return false;
                }
            }               
        }

        return true;
    }
}
