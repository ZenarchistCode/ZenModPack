// I'm a big dummy who cbf learning how PPE fx work, so this is how I do things for this mod. Sue me.
class ZenBrokenGlassesWidget extends Managed
{
	static const string MOD_DIRECTORY = "";

	ref Widget				m_layout;
	ref Widget				m_glass1;
	ref Widget				m_glass2;
	ref Widget				m_glass3;
	ref Widget				m_glass4;
	ref Widget				m_glass5;
	ref Widget				m_glass6;

	static ref ZenBrokenGlassesWidget m_StaticRef;

	static ZenBrokenGlassesWidget GetZenBrokenGlassesWidget()
	{
		if (!m_StaticRef)
			m_StaticRef = new ZenBrokenGlassesWidget;

		return m_StaticRef;
	}

	void ZenBrokenGlassesWidget()
	{
		m_layout = GetGame().GetWorkspace().CreateWidgets("ZenModPack/data/gui/layouts/brokenglasses/brokenglasses.layout");

		m_glass1 = m_layout.FindAnyWidget("brokenglass1");
		m_glass2 = m_layout.FindAnyWidget("brokenglass2");
		m_glass3 = m_layout.FindAnyWidget("brokenglass3");
		m_glass4 = m_layout.FindAnyWidget("brokenglass4");
		m_glass5 = m_layout.FindAnyWidget("brokenglass5");
		m_glass6 = m_layout.FindAnyWidget("brokenglass6");
	}

	void ~ZenBrokenGlassesWidget()
	{
		HideFX();
	}

	void ShowFX(float dmg)
	{
		if (!ZenModEnabled("ZenBrokenGlasses"))
		{
			m_layout.Show(false);
			return;
		}

		m_layout.Show(true);

		if (dmg < GameConstants.STATE_DAMAGED)
		{
			HideFX();
			return;
		}

		if (dmg >= GameConstants.STATE_BADLY_DAMAGED)
		{
			m_glass1.Show(true);
			m_glass4.Show(true);
		} else
		{
			m_glass1.Show(false);
			m_glass4.Show(false);
		}

		m_glass2.Show(true);
		m_glass3.Show(true);
	}

	void ShowFX_Optics(float dmg)
	{
		if (!ZenModEnabled("ZenBrokenGlasses"))
		{
			m_layout.Show(false);
			return;
		}

		m_layout.Show(true);

		if (dmg < GameConstants.STATE_DAMAGED)
		{
			HideFX_Optics();
			return;
		}

		if (dmg >= GameConstants.STATE_BADLY_DAMAGED)
		{
			m_glass6.Show(true);
		} else
		{
			m_glass6.Show(false);
		}

		m_glass5.Show(true);
	}

	void HideFX()
	{
		m_glass1.Show(false);
		m_glass2.Show(false);
		m_glass3.Show(false);
		m_glass4.Show(false);
	}

	void HideFX_Optics()
	{
		m_glass5.Show(false);
		m_glass6.Show(false);
	}
}