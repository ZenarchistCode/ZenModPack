/*
Vanilla handling of GUIInventoryAttachmentsProps is more cooked than I was 15 years ago when I used to 
smoke bongs all day. Whoever wrote this vanilla code was probably high at the time themselves.

Had to re-write it so that it doesn't crash log spam nullpointers and cause all kinds of in-game GUI
issues when there's too many attachment categories added through .cpp's GUIInventoryAttachmentsProps.

This is the error I'd get whenever an object with cargo had an incompatible number of attachment categories:

NULL pointer to instance
Class:      'AttachmentCategoriesContainer'
Function: 'InitGhostSlots'
Stack trace:
scripts/5_Mission/gui\inventorynew\inherited\attachmentcategoriescontainer.c:388 Function InitGhostSlots
scripts/5_Mission/gui\inventorynew\inherited\attachmentcategoriescontainer.c:36 Function SetEntity
*/
modded class AttachmentCategoriesContainer: CollapsibleContainer
{
	protected int m_ZenCargoSlotRowIndex = -1;
	protected int m_ZenCargoSlotColumnIndex = -1;

	override void InitIconsContainers()
	{
		if (ZenModEnabled("ZenFixCarAttachmentGUI"))
		{
			ZenInitIconsContainers();
		}
		else 
		{
			super.InitIconsContainers();
		}
	}

	override void InitGhostSlots()
	{
		if (ZenModEnabled("ZenFixCarAttachmentGUI"))
		{
			ZenInitGhostSlots();
		}
		else 
		{
			super.InitGhostSlots();
		}
	}

	void ZenInitIconsContainers()
	{
		m_SlotsCount = 0;
		m_ZenCargoSlotRowIndex = -1;
		m_ZenCargoSlotColumnIndex = -1;

		string type = m_Entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";
		int attachments_categories_count = GetAttachmentCategoriesCount(config_path_attachment_categories);

		bool has_cargo = m_Entity.GetInventory().GetCargo() != NULL;

		int slot_index = 0;
		int row_count = Math.Ceil(attachments_categories_count / ITEMS_IN_ROW);
		if (has_cargo)
		{
			int used_cols_last_row = attachments_categories_count % ITEMS_IN_ROW;
			if (used_cols_last_row == 0 && attachments_categories_count > 0)
				used_cols_last_row = ITEMS_IN_ROW;

			if (used_cols_last_row == ITEMS_IN_ROW)
			{
				row_count++; // add new row for cargo
				m_ZenCargoSlotColumnIndex = 0;
				m_ZenCargoSlotRowIndex = row_count - 1;
			}
			else
			{
				m_ZenCargoSlotRowIndex = row_count - 1;
				m_ZenCargoSlotColumnIndex = used_cols_last_row;
			}
		}

		for (int i = 0; i < row_count; i++)
		{
			ref AttachmentCategoriesSlotsContainer items_cont = new AttachmentCategoriesSlotsContainer(this, i);
			m_Body.Insert(items_cont);
			m_OpenedContainers.Insert(items_cont);

			int column_count = ITEMS_IN_ROW;
			if (i == (row_count - 1))
			{
				if (i == m_ZenCargoSlotRowIndex)
				{
					column_count = m_ZenCargoSlotColumnIndex + 1;
				}
				else
				{
					int remainder = attachments_categories_count % ITEMS_IN_ROW;
					if (remainder == 0)
					{
						column_count = ITEMS_IN_ROW;
					}
					else
					{
						column_count = remainder;
					}
				}
			}
			else
			{
				column_count = ITEMS_IN_ROW;
			}

			items_cont.GetSlotsContainer().SetColumnCount(column_count);
		}

		m_SlotsCount = row_count;
	}

	void ZenInitGhostSlots()
	{
		string type = m_Entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";

		int attachments_categories_count = GetAttachmentCategoriesCount(config_path_attachment_categories);

		SlotsContainer items_cont;
		SlotsIcon icon;
		string attachment_category;
		string icon_name;

		for (int i = 0; i < attachments_categories_count; i++)
		{
			int row_index = i / ITEMS_IN_ROW;
			int slot_number = i % ITEMS_IN_ROW;

			items_cont = GetSlotsContainer(row_index);

			if (!items_cont)
			{
				Error("[ZenMod] InitGhostSlots: NULL items_cont at row_index=" + row_index + " (i=" + i + ")");
				continue;
			}

			attachment_category = GetAttachmentCategory(config_path_attachment_categories, i);
			icon_name = GetIconName(config_path_attachment_categories, attachment_category);

			m_CategorySlotIndex.Insert(attachment_category, i);
			icon = items_cont.GetSlotIcon(slot_number);

			if (!icon)
			{
				Error("[ZenMod] InitGhostSlots: NULL slot icon at i=" + i + ", slot=" + slot_number);
				continue;
			}

			icon.GetGhostSlot().SetFlags(WidgetFlags.IGNOREPOINTER);
			LoadAttachmentCategoriesIcon(items_cont, icon_name, slot_number);

			string config = config_path_attachment_categories + " " + attachment_category + " name";
			string name;
			GetGame().ConfigGetText(config, name);
			icon.SetSlotDisplayName(name);

			AttachmentCategoriesRow ar = new AttachmentCategoriesRow(this, -1);
			ar.Init(attachments_categories_count, i, attachment_category, config_path_attachment_categories, m_Entity, m_Body.Count());
			ar.SetSlotIcon(icon);

			icon.GetRadialIconPanel().Show(true);
			ar.Open();
			icon.SetContainer(ar);
			Insert(ar);
		}

		if (m_Entity.GetInventory().GetCargo())
		{
			items_cont = GetSlotsContainer(m_ZenCargoSlotRowIndex);
			if (!items_cont)
			{
				Error("[ZenMod] InitGhostSlots: NULL cargo items_cont");
				return;
			}

			icon = items_cont.GetSlotIcon(m_ZenCargoSlotColumnIndex);
			if (!icon)
			{
				Error("[ZenMod] InitGhostSlots: NULL cargo icon at row=" + m_ZenCargoSlotRowIndex + ", column=" + m_ZenCargoSlotColumnIndex);
				return;
			}

			icon.GetGhostSlot().Show(true);
			icon.GetGhostSlot().LoadImageFile(0, StaticGUIUtils.VerifyIconImageString(StaticGUIUtils.IMAGESETGROUP_INVENTORY, m_Entity.ConfigGetString("GUIInventoryCargoIcon")));
			icon.SetSlotDisplayName(m_Entity.ConfigGetString("GUIInventoryCargoName"));
			icon.GetGhostSlot().SetFlags(WidgetFlags.IGNOREPOINTER);
			icon.GetRadialIconPanel().Show(true);
			icon.GetMainWidget().Show(true);

			ContainerWithCargo iwc = new ContainerWithCargo(this, -1);
			iwc.Get(0).GetRootWidget().ClearFlags(WidgetFlags.DRAGGABLE);
			iwc.SetEntity(m_Entity, 0, false);
			iwc.SetSlotIcon(icon);
			iwc.Open();

			icon.SetContainer(iwc);
			m_CargoSlotsIcon = icon;
		}

		RecomputeOpenedContainers();
	}
}