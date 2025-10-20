modded class ShelterSite
{
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		if (!ZenModEnabled("ZenShelterCargo"))
		{
			super.OnPartBuiltServer(player, part_name, action_id);
			return;
		}

		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		string shelter_type = "";
		switch (part_name)
		{
			case "leather": 
				shelter_type = "Zen_ShelterLeather";
			break;
			
			case "fabric": 
				shelter_type = "Zen_ShelterFabric";
			break;
			
			case "stick": 
				shelter_type = "Zen_ShelterStick";
			break;
				
			default: {};
		}
		
		if (shelter_type != "")
		{
			GetConstruction().DropNonUsableMaterialsServer( player, part_name );
			MiscGameplayFunctions.TurnItemIntoItem(this, shelter_type, PlayerBase.Cast(player));
			
			PluginAdminLog admin_log = PluginAdminLog.Cast( GetPlugin(PluginAdminLog) );
			if (admin_log)
			{
				string playerPrefix = admin_log.GetPlayerPrefix(PlayerBase.Cast(player), player.GetIdentity());
				admin_log.DirectAdminLogPrint(playerPrefix +" built " + shelter_type + " with Hands ");
			}
		}
	}
}

class Zen_ShelterStick extends ShelterStick {};
class Zen_ShelterFabric extends ShelterFabric {};
class Zen_ShelterLeather extends ShelterLeather {};