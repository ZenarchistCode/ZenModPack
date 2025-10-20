#ifdef EXPANSIONMODAI
modded class eAIBase
{
	static int ZENMOD_AI_COUNT = 0;

	override void Init()
	{
		super.Init();

		ZENMOD_AI_COUNT++;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		ZENMOD_AI_COUNT--;
	}
}
#endif