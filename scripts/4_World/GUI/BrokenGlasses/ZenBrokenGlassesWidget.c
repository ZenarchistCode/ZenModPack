// I'm a big dummy who cbf learning how PPE fx work, so this is how I do things. Sue me.
class ZenBrokenGlassesWidget extends Managed
{
	static const string MOD_DIRECTORY = "ZenModPack/data/gui/layouts/brokenglasses/";

	ref Widget				m_layout;
	ref ImageWidget			m_glass1;
	ref ImageWidget			m_glass2;
	ref ImageWidget			m_glass3;
	ref ImageWidget			m_glass4;
	ref ImageWidget			m_glass5;
	ref ImageWidget			m_glass6;

	static ref ZenBrokenGlassesWidget m_StaticRef;

	static ZenBrokenGlassesWidget GetZenBrokenGlassesWidget()
	{
		if (!m_StaticRef)
			m_StaticRef = new ZenBrokenGlassesWidget;

		return m_StaticRef;
	}

	void ZenBrokenGlassesWidget()
	{
		m_layout = GetGame().GetWorkspace().CreateWidgets(MOD_DIRECTORY + "brokenglasses.layout");

		m_glass1 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass1"));
		m_glass2 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass2"));
		m_glass3 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass3"));
		m_glass4 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass4"));
		m_glass5 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass5"));
		m_glass6 = ImageWidget.Cast(m_layout.FindAnyWidget("brokenglass6"));

		m_glass1.LoadImageFile(0, MOD_DIRECTORY + "brokenglass1.edds");
		m_glass2.LoadImageFile(0, MOD_DIRECTORY + "brokenglass2.edds");
		m_glass3.LoadImageFile(0, MOD_DIRECTORY + "brokenglass3.edds");
		m_glass4.LoadImageFile(0, MOD_DIRECTORY + "brokenglass3.edds");
		m_glass5.LoadImageFile(0, MOD_DIRECTORY + "brokenglass2.edds");
		m_glass6.LoadImageFile(0, MOD_DIRECTORY + "brokenglass1.edds");
	}

	void ~ZenBrokenGlassesWidget()
	{
		HideFX();
	}

	void ShowFX(float dmg)
	{
		if (!ZenModEnabled("ZenBrokenGlasses"))
			return;

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
			return;

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