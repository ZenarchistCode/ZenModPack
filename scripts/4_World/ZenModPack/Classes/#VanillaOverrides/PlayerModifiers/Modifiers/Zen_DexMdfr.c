enum Zen_eModifiers : eModifiers
{
    MDF_DEX = 69237697
}

class Zen_DexMdfr extends ModifierBase
{
    static const int DELAY    = 300; // 5 min before effect starts
    static const int DURATION = 300; // 5 min of effect after delay

    protected bool m_EffectApplied;

    override void Init()
    {
        m_TrackActivatedTime   = true;
        m_IsPersistent         = true;
        m_ID                   = Zen_eModifiers.MDF_DEX;
        m_TickIntervalInactive = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive   = DEFAULT_TICK_TIME_ACTIVE;

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
        m_EffectApplied = false;

        player.IncreaseHealingsCount();
    }

    override void OnTick(PlayerBase player, float deltaT)
    {
        float attached_time = GetAttachedTime();

        // Start stamina effect after 10 minutes
        if (!m_EffectApplied && attached_time >= DELAY)
        {
            m_EffectApplied = true;

            #ifndef TerjeMedicine
            player.GetStaminaHandler().ActivateRecoveryModifier(Zen_EStaminaMultiplierTypes.DEX);
            #else 
            TerjeConsumableEffects medEffects = new TerjeConsumableEffects();
            medEffects.Apply(null, "CfgVehicles Zen_DexBottle_TerjeMedicine", player, 1);
            player.DecreaseHealingsCount();
            #endif
        }
    }

    override void OnDeactivate(PlayerBase player)
    {
        #ifndef TerjeMedicine
        // Only deactivate if we actually applied it
        if (m_EffectApplied)
        {
            player.GetStaminaHandler().DeactivateRecoveryModifier(Zen_EStaminaMultiplierTypes.DEX);
        }

        player.DecreaseHealingsCount();
        #endif
    }

    override bool DeactivateCondition(PlayerBase player)
    {
        float attached_time = GetAttachedTime();

        // total life = digest delay + effect duration
        return attached_time >= (DELAY + DURATION);
    }
}