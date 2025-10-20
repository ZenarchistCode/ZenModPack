modded class ItemActionsWidget
{
	override protected void SetWeaponQuantity(int chamber, int mag, string itemWidget, string quantityPBWidget, string quantityTextWidget, bool enabled)
	{
		if (!GetZenModPackClientConfig().MagObfuscation)
		{
			super.SetWeaponQuantity(chamber, mag, itemWidget, quantityPBWidget, quantityTextWidget, enabled);
			return;
		}

		Widget widget = m_Root.FindAnyWidget(itemWidget);

		if (!enabled) 
		{
			widget.Show(false);
			return;
		}

		Weapon_Base weapon = Weapon_Base.Cast(m_EntityInHands);

		if (!weapon)
		{
			super.SetWeaponQuantity(chamber, mag, itemWidget, quantityPBWidget, quantityTextWidget, enabled);
			return;
		}

		ProgressBarWidget progressBar = ProgressBarWidget.Cast(widget.FindAnyWidget(quantityPBWidget));
		TextWidget textWidget = TextWidget.Cast(widget.FindAnyWidget(quantityTextWidget));

		string ammo_count = "";
		widget.Show(false);
		int mag_quantity;
		int i;

		progressBar.Show(false);

		if (weapon.IsInherited(Magnum)) 
		{
			mag_quantity = 0;
			for (i = 0; i < weapon.GetMuzzleCount(); i++) 
			{
				if (weapon.IsChamberFull(i) && !weapon.IsChamberFiredOut(i)) 
				{
					mag_quantity++;
				}
			}

			textWidget.SetText(mag_quantity.ToString());
			return;
		}

		if (weapon.IsJammed()) 
		{
			ammo_count = "X";
		}
		else 
		{
			for (i = 0; i < weapon.GetMuzzleCount(); i++) 
			{
				if (i > 0 && (weapon.GetMuzzleCount() < 3 || i % 2 == 0)) 
				{
					ammo_count += " ";
				}

				if (weapon.IsChamberEmpty(i)) 
				{
					ammo_count += "-";
				}
				else 
				if (weapon.IsChamberFiredOut(i)) 
				{
					ammo_count += "F";
				}
				else 
				{
					ammo_count += "+";
				}
			}
		}

		textWidget.SetText(ammo_count);
		textWidget.Show(true);
		textWidget.GetParent().Show(true);
	}
}