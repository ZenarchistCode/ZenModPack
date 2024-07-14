class ReplaceZenSwissKnifeLambda extends ReplaceItemWithNewLambdaBase
{
	/*
	This whole thing is not ideal, but it is what it is and I can't think of a better way.

	In order to have each swiss tool work with all the inbuilt + modded recipes/actions, 
	ideally we need to make a separate Item for each tool which inherits from each tool class.

	That way ZenSwissKnife_Screwdriver works with all the vanilla Screwdriver recipes etc
	But that means transferring tool health between each item is extra complicated.

	Basically each swiss knife tool is its own item and we don't have the benefit of inheriting
	from one master class like you usually would, so we have to treat each tool as its own item.
	*/

	override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
	{
		super.CopyOldPropertiesToNew(old_item, new_item);

		if (!new_item)
		{
			Error("[ZenSwissKnife::ReplaceZenSwissKnifeLambda] VERY BAD ERROR: NEW ITEM DOESN'T EXIST!");
			return;
		}

		// We need to retrieve the current tool's health for each individual tool, and transfer them to the 'new' swiss knife item.
		float baseHP			= -1;
		float canOpenerHP		= -1;
		float screwdriverHP		= -1;
		float knifeHP			= -1;
		float sawHP				= -1;
		float lockpickHP		= -1;
		float scissorsHP		= -1;

		//! ZenSwissKnife BASE TOOL (BaseTool->Other)
		if (old_item.IsInherited(ZenSwissKnife))
		{
			ZenSwissKnife swissBase = ZenSwissKnife.Cast(old_item);
			swissBase.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, old_item.GetHealth01("",""), canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_CanOpener (CanOpener->BaseTool)
		if (old_item.IsInherited(ZenSwissKnife_CanOpener))
		{
			ZenSwissKnife_CanOpener canOpener = ZenSwissKnife_CanOpener.Cast(old_item);
			canOpener.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Screwdriver
		if (old_item.IsInherited(ZenSwissKnife_Screwdriver))
		{
			ZenSwissKnife_Screwdriver screwdriver = ZenSwissKnife_Screwdriver.Cast(old_item);
			screwdriver.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Knife
		if (old_item.IsInherited(ZenSwissKnife_Knife))
		{
			ZenSwissKnife_Knife knife = ZenSwissKnife_Knife.Cast(old_item);
			knife.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_SmallSaw
		if (old_item.IsInherited(ZenSwissKnife_SmallSaw))
		{
			ZenSwissKnife_SmallSaw saw = ZenSwissKnife_SmallSaw.Cast(old_item);
			saw.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Lockpick
		if (old_item.IsInherited(ZenSwissKnife_Lockpick))
		{
			ZenSwissKnife_Lockpick lockpick = ZenSwissKnife_Lockpick.Cast(old_item);
			lockpick.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Scissors
		if (old_item.IsInherited(ZenSwissKnife_Scissors))
		{
			ZenSwissKnife_Scissors scissors = ZenSwissKnife_Scissors.Cast(old_item);
			scissors.GetSwissToolHealthOut(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			SetNewToolHealthLevels(new_item, baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}
	}

	void SetNewToolHealthLevels(EntityAI new_item, float baseHP, float canOpenerHP, float screwdriverHP, float knifeHP, float sawHP, float lockpickHP, float scissorsHP)
	{
		ZenSwissKnife.DebugMessage("SetTo(" + new_item.GetType() + ") - baseHP=" + baseHP + " canOpenerHP=" + canOpenerHP + " screwdriverHP=" + screwdriverHP + " knifeHP=" + knifeHP + " sawHP=" + sawHP + " lockpickHP=" + lockpickHP + " scissorsHP=" + scissorsHP);

		//! ZenSwissKnife
		if (new_item.IsInherited(ZenSwissKnife))
		{
			ZenSwissKnife swissBase = ZenSwissKnife.Cast(new_item);
			swissBase.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_CanOpener
		if (new_item.IsInherited(ZenSwissKnife_CanOpener))
		{
			ZenSwissKnife_CanOpener canOpener = ZenSwissKnife_CanOpener.Cast(new_item);
			canOpener.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Screwdriver
		if (new_item.IsInherited(ZenSwissKnife_Screwdriver))
		{
			ZenSwissKnife_Screwdriver screwdriver = ZenSwissKnife_Screwdriver.Cast(new_item);
			screwdriver.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Knife
		if (new_item.IsInherited(ZenSwissKnife_Knife))
		{
			ZenSwissKnife_Knife knife = ZenSwissKnife_Knife.Cast(new_item);
			knife.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_SmallSaw
		if (new_item.IsInherited(ZenSwissKnife_SmallSaw))
		{
			ZenSwissKnife_SmallSaw saw = ZenSwissKnife_SmallSaw.Cast(new_item);
			saw.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Lockpick
		if (new_item.IsInherited(ZenSwissKnife_Lockpick))
		{
			ZenSwissKnife_Lockpick lockpick = ZenSwissKnife_Lockpick.Cast(new_item);
			lockpick.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}

		//! ZenSwissKnife_Lockpick
		if (new_item.IsInherited(ZenSwissKnife_Scissors))
		{
			ZenSwissKnife_Scissors scissors = ZenSwissKnife_Scissors.Cast(new_item);
			scissors.SetSwissToolHealthConvert(baseHP, canOpenerHP, screwdriverHP, knifeHP, sawHP, lockpickHP, scissorsHP);
			return;
		}
	}
}