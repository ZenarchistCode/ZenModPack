class ZenZippoLight extends PointLightBase
{
	void ZenZippoLight()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo(2.5);
		SetBrightnessTo(5.0);
		SetFadeOutTime(0);
		SetDiffuseColor(1.0, 0.45, 0.25);
		SetAmbientColor(1.0, 0.45, 0.25);
		SetFlickerAmplitude(0.45);
		SetFlickerSpeed(0.85);
		SetCastShadow(false);
		SetDancingShadowsMovementSpeed(0.25);
		SetDancingShadowsAmplitude(0.03);
		EnableHeatHaze(true);
		SetHeatHazeRadius(0.03);
		SetHeatHazePower(0.005);
		SetFlareVisible(false);
	}
}