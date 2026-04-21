modded class WeaponFSM 
{
    #ifdef SERVER
    override protected bool ValidateAndRepairHelper(Weapon_Base weapon, string name, bool stateCondition, bool gunCondition, WeaponEventBase e1, WeaponEventBase e2, out WeaponStableState state)
	{
        if (super.ValidateAndRepairHelper(weapon, name, stateCondition, gunCondition, e1, e2, state))
        {
            if (!weapon || !ZenModEnabled("ZenInformWeaponStateBug"))
                return true;

            PlayerBase player = PlayerBase.Cast(weapon.GetHierarchyRootPlayer());
            if (!player || !player.GetIdentity())
                return true;

            string msg = weapon.GetDisplayName() + ": #STR_ZenWeaponState_InvalidStateMsg";

            if (player.GetSimulationTimeStamp() < 200)
            {
                // Send confirm message popup:
                GetRPCManager().SendRPC("ZenMod_RPC", "RPC_ReceiveZenAdminMessage", new Param2<string, bool>("#STR_ZenWeaponState_Warning\n" + msg, true), true, player.GetIdentity());
            }
            else 
            {
                NotificationSystem.SendNotificationToPlayerIdentityExtended(player.GetIdentity(), 30.0, "#STR_ZenWeaponState_Warning", msg, "set:dayz_gui image:icon_bullet");
            }
            
            return true;
        }
       
        return false;
    }
    #endif
}