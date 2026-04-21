class ZenYieldItemFishConfigurable : FishYieldItemBase
{
	protected float m_ChanceMult = 1.0;

	void ZenYieldItemFishConfigurable(int baseWeight, ZenFishingFishEntry e, array<float> defaultCoefs, float defaultQuality)
	{
		// core identity (used by selection and for your duplicate-removal)
		m_Type = e.Type;
		m_EnviroMask = e.EnviroMask;
		m_MethodMask = e.MethodMask;

		// optional chance tuning (multiplies normal logic)
		m_ChanceMult = e.ChanceMult;

		// quality
		if (e.QualityBase >= 0)
			m_QualityBase = e.QualityBase;
		else
			m_QualityBase = defaultQuality;

		// hourly cycle coefs: entry override or config default
		array<float> src = e.HourlyCycleCoefs;
		if (!src || src.Count() != 24)
			src = defaultCoefs;

		if (src && src.Count() == 24)
		{
			for (int i = 0; i < 24; i++)
				m_HourlyCycleLengthCoefs[i] = src[i];
		}
	}

	override float GetChanceForYieldItem(CatchingContextBase ctx)
	{
		return super.GetChanceForYieldItem(ctx) * m_ChanceMult;
	}
}
