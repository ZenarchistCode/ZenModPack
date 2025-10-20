modded class CorpseData
{
	override void UpdateCorpseState(bool force_check = false)
	{
		super.UpdateCorpseState(force_check);

		//! GRAVES 
		// If bUpdate is false, the corpse is flagged to be deleted/despawned
		if (!m_bUpdate && GetZenGravesConfig().BurySkeleton)
		{
			// Save any leftover player items to bury on skeleton
			m_Player.SaveZenSkeletonItems();
		}
	};
};