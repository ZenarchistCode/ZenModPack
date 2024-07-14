class ZenRainWaterConstants
{
	// RAIN WATER FILL SPEEDS
	static const float FILL_RAIN_SPEED	 = 5;			// Base fill quantity per second for bottles when jogging - make it slow so it's not OP.
	static const float FILL_MULTI_POT	 = 2;			// Fill pots faster given it has a much larger surface area
	static const float FILL_MULTI_IDLE	 = 3;			// Fill mulitiplier when idle (not moving)
	static const float FILL_MULTI_WALK	 = 2;			// Fill multiplier when walking slowly
	static const float FILL_GROUND_SPEED = 20.0;		// How fast to fill on ground

	// ACTION CONDITIONS
	static const int	CAMERA_PITCH_TO_CATCH_RAIN = 45; // Camera pitch angle to catch rain
	static const float	REQUIRED_RAIN = 0.2;			 // Weather rain level required to catch rain
	static const float	REQUIRED_OVERCAST = 0.6;		 // Weather overcast level required to catch rain
};