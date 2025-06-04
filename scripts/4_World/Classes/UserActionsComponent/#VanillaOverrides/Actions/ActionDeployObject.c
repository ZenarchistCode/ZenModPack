modded class ActionDeployObject
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!item)
		{
			return super.ActionCondition(player, target, item);
		}

		//! CAMO SHELTER
		if (item.IsKindOf("Zen_CamoShelterKit"))
		{
			Zen_CamoShelterKit shelter = Zen_CamoShelterKit.Cast(item);
			return shelter && shelter.HasAllMaterials();
		}

		//! NOTES 
		#ifndef SERVER
		if (player.IsPlacingLocal() && item.IsKindOf("Paper"))
			return true;
		#endif

		if (!super.ActionCondition(player, target, item))
		{
			return false;
		}

		if (!ZenModEnabled("ZenBasebuldingConfig"))
		{
			return true;
		}

		if (GetGame().IsDedicatedServer())
		{
			// Check no build zones first.
			ZenNoBuildZone noBuildZone;
			vector noBuildPos;
			vector ourPos;

			for (int i = 0; i < GetZenBasebuildingConfig().NoBuildZones.Count(); i++)
			{
				noBuildZone = GetZenBasebuildingConfig().NoBuildZones.Get(i);
				noBuildPos = noBuildZone.Position;
				ourPos = player.GetPosition();

				if (noBuildZone.IgnoreHeight)
				{
					noBuildPos[1] = 0;
					ourPos[1] = 0;
				}

				if (vector.Distance(noBuildPos, ourPos) <= noBuildZone.MinimumDistance)
				{
					// Check whitelist
					if (GetZenBasebuildingConfig().Whitelist)
					{
						foreach (string s : GetZenBasebuildingConfig().Whitelist)
						{
							s.ToLower();
							if (item.IsKindOf(s))
							{
								return true;
							}
						}
					}
					
					string msg = GetZenBasebuildingConfig().NoBuildZoneDefaultMessage;

					if (noBuildZone.WarningMessage != "")
						msg = noBuildZone.WarningMessage;

					msg.Replace("%distance%", (Math.Round(noBuildZone.MinimumDistance - vector.Distance(noBuildPos, ourPos)) + 1).ToString() + "m");
					NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 10.0, noBuildZone.ZoneName, msg, "set:dayz_gui image:icon_hammer");

					player.PlacingCancelLocal();
					player.PlacingCancelServer();

					return false;
				}
			}
		}

		return true;
	}

	override void SetupAnimation(ItemBase item)
    {
		//! NOTES
        if (item.IsKindOf("Paper"))
        {
            m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
            return;
        }

        super.SetupAnimation(item);
    }
}
