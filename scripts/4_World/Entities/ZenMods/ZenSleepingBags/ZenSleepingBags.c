/*
class ZenSleepingBag_PackedBase extends TentBase
{
    protected bool m_WasHologrammed;

    void SetHologrammed(bool hologram)
    {
        m_WasHologrammed = hologram;

        if (m_WasHologrammed)
        {
            string textureAlpha = "#(argb,8,8,3)color(1,1,1,0.1,ca)";
            SetObjectTexture(0, textureAlpha);
        }
        else
        {
            string textureNoAlpha = GetHiddenSelectionsTextures().Get(0);
            SetObjectTexture(0, textureNoAlpha);
        }
    }

    override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
    {
        super.OnPlacementComplete(player, position, orientation);

        if (!GetGame().IsDedicatedServer())
            return;

        // Place deployed version and carry over health
        string newType = GetType();
        newType.Replace("_Packed", "_Deployed");
        Object bagDeployed = GetGame().CreateObject(newType, position, false);
        if (!bagDeployed)
        {
            Error("ZenModPack :: SleepingBags - very bad error - couldn't create " + newType);
            return;
        }

        bagDeployed.SetHealth(GetHealth());
        bagDeployed.SetPosition(position);
        bagDeployed.SetOrientation(orientation);

        // Delete packed version
        GetGame().ObjectDelete(this);
    }

    override bool CanBeRepairedByCrafting()
    {
        return true;
    }

    override bool IsDeployable()
    {
        return true;
    }

    override string GetLoopDeploySoundset()
    {
        return "mediumtent_deploy_SoundSet";
    }

    override void SetActions()
    {
        super.SetActions();

        AddAction(ActionTogglePlaceObject);
        AddAction(ActionDeployObject);
    }
}

class ZenSleepingBag_DeployedBase extends ItemBase
{
    protected Object m_CC;

    override void DeferredInit()
	{
        super.DeferredInit();

        // Do this here because doing it on void ZenSleepingBagDeployedBase() is too soon - vector position is 0 0 0 upon object creation
        CreateClutterCutter();
    }

    void ~ZenSleepingBag_DeployedBase()
    {
        if (m_CC != NULL)
            GetGame().ObjectDelete(m_CC);
    }

    void CreateClutterCutter()
    {
        if (!m_CC)
            m_CC = GetGame().CreateObjectEx("ClutterCutterFireplace", GetPosition(), ECE_PLACE_ON_SURFACE);
    }

    override bool IsRefresherSignalingViable()
    {
        return !IsRuined();
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    override void SetActions()
    {
        super.SetActions();

        AddAction(ActionZenSleepOnBag);
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    // Drop inventory if ruined
    override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (!GetGame().IsDedicatedServer())
			return;

		if (newLevel == GameConstants.STATE_RUINED)
			MiscGameplayFunctions.DropAllItemsInInventoryInBounds(this, vector.Zero);
	}
}

// PACKED BAGS
class ZenSleepingBagBlackPacked extends ZenSleepingBag_PackedBase {};
class ZenSleepingBagBluePacked extends ZenSleepingBag_PackedBase {};
class ZenSleepingBagGreyPacked extends ZenSleepingBag_PackedBase {};
class ZenSleepingBagRedPacked extends ZenSleepingBag_PackedBase {};
class ZenSleepingBagGreenPacked extends ZenSleepingBag_PackedBase {};

// DEPLOYED BAGS
class ZenSleepingBagBlackDeployed extends ZenSleepingBag_DeployedBase {};
class ZenSleepingBagBlueDeployed extends ZenSleepingBag_DeployedBase {};
class ZenSleepingBagGreyDeployed extends ZenSleepingBag_DeployedBase {};
class ZenSleepingBagRedDeployed extends ZenSleepingBag_DeployedBase {};
class ZenSleepingBagGreenDeployed extends ZenSleepingBag_DeployedBase {};
*/