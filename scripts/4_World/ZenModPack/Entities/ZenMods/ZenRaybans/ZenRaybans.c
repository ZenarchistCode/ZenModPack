class ZenRaybans_ColorBase extends Glasses_Base 
{
	override protected void InitGlobalExclusionValues()
	{
		super.InitGlobalExclusionValues();

		ClearSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_REGULAR_0);
		AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_TIGHT_0);
		AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_2);
	}
};

class ZenRaybans_Black extends ZenRaybans_ColorBase
{
	override int GetGlassesEffectID()
	{
		return PPERequesterBank.REQ_GLASSESSPORTBLACK;
	}
};

class ZenRaybans_Red extends ZenRaybans_ColorBase
{
	override int GetGlassesEffectID()
	{
		return PPERequesterBank.REQ_GLASSESSPORTORANGE;
	}
};

class ZenRaybans_Green extends ZenRaybans_ColorBase
{
	override int GetGlassesEffectID()
	{
		return PPERequesterBank.REQ_GLASSESSPORTGREEN;
	}
};

class ZenRaybans_Blue extends ZenRaybans_ColorBase
{
	override int GetGlassesEffectID()
	{
		return PPERequesterBank.REQ_GLASSESSPORTBLUE;
	}
};