enum Zen_EStaminaMultiplierTypes : EStaminaMultiplierTypes
{
    DEX = 6928469
}

modded class StaminaHandler
{
    override void Init()
    {
        super.Init();

        m_RegisteredRecoveryModifiers.Insert(Zen_EStaminaMultiplierTypes.DEX, 2.0);
    }
}