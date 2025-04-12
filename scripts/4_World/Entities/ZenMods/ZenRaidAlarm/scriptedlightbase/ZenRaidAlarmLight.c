class ZenRaidAlarmLight extends PointLightBase
{
	void ZenRaidAlarmLight()
	{
		SetVisibleDuringDaylight(true);
		SetRadiusTo(0.5);
		SetBrightnessTo(100.0);
		FadeIn(1);
		SetFadeOutTime(1);
		SetFlareVisible(false);
		SetCastShadow(false);
		SetColor(1, 0, 0);
	}

	void SetColor(int r, int g, int b)
	{
		SetAmbientColor(r, g, b);
		SetDiffuseColor(r, g, b);
		SetBrightnessAdjusted(100);
	}

	void SetBrightnessAdjusted(float b)
	{
		if (GetGame().GetWorld().IsNight())
			 b = b * 0.1;

		SetBrightnessTo(b);
	}
}