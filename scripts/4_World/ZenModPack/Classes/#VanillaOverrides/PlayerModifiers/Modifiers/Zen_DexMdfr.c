enum Zen_eModifiers : eModifiers
{
    MDF_DEX = 69237697
}

class Zen_DexMdfr extends ModifierBase
{
    static const int DURATION = 600;

    override void Init()
    {
        m_TrackActivatedTime = true;
        m_IsPersistent = true;
        m_ID = Zen_eModifiers.MDF_DEX;
        m_TickIntervalInactive = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive = DEFAULT_TICK_TIME_ACTIVE;

        DisableActivateCheck();
    }

    override bool ActivateCondition(PlayerBase player)
    {
        return false;
    }

    override void OnReconnect(PlayerBase player)
    {
        OnActivate(player);
    }

    override void OnActivate(PlayerBase player)
    {
        player.IncreaseHealingsCount();
        player.GetStaminaHandler().ActivateRecoveryModifier(Zen_EStaminaMultiplierTypes.DEX);
    }

    override void OnDeactivate(PlayerBase player)
    {
        player.DecreaseHealingsCount();
        player.GetStaminaHandler().DeactivateRecoveryModifier(Zen_EStaminaMultiplierTypes.DEX);
    }

    override bool DeactivateCondition(PlayerBase player)
    {
        float attached_time = GetAttachedTime();

        if (attached_time >= DURATION)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
