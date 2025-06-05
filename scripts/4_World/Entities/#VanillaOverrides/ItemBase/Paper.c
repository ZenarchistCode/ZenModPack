modded class Paper
{
    //! SHARED
    override void SetActions()
	{
		super.SetActions();

        //! NOTES
		AddAction(ActionZenWritePaper);
	}

    //! NOTES
    private int m_NotePickupTries = 0;
    Pen_ColorBase m_Pen;

    void SetPen(Pen_ColorBase pen)
    {
        m_Pen = pen;
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        // Client-side receiver for server-formatted date
        if (rpc_type == ZenRPCs.RECEIVE_NOTE_DATE)
        {
            Param2<string, bool> data_from_server;

            if (ctx.Read(data_from_server))
            {
                // Set static variables (date text formatted by server & whether or not font can be changed)
                ZenNoteGUI.DATE_TEXT = data_from_server.param1;
                ZenNoteGUI.CAN_CHANGE_FONTS = data_from_server.param2;

                // UI will be open before the above data is set, so we need to update it after data is received
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateGUI, 50, false);
            }

            return;
        }

        // Server-side receiver for note data written by client/player
        if (rpc_type == ZenRPCs.SEND_WRITTEN_NOTE)
        {
            int highBits, lowBits;
            GetGame().GetPlayerNetworkIDByIdentityID(sender.GetPlayerId(), lowBits, highBits);
            PlayerBase player = PlayerBase.Cast(GetGame().GetObjectByNetworkId(lowBits, highBits));

            if (!player) // If we can't identify the player who sent this note data, stop here.
                return;

            Param1<ref ZenNoteData> data_from_client;

            if (ctx.Read(data_from_client))
            {
                if (data_from_client.param1)
                {
                    // Check if the note contains any blacklisted words
                    if (GetZenNotesConfig().IsBlacklisted(data_from_client.param1.m_NoteText))
                    {
                        // Log the blacklisted note for server admins
                        ZenModLogger.Log("[BLACKLIST] " + player.GetCachedName() + " (" + sender.GetId() + ") @ " + this.GetPosition() + " tried to write: " + data_from_client.param1.m_NoteText, "notes");

                        // If player warning is set, send it
                        if (GetZenNotesConfig().SendPlayerWarning != "")
                        {
                            Zen_NoteSendMessage(player, GetZenNotesConfig().SendPlayerWarning);
                        }

                        return;
                    }

                    // Convert to written note item
                    ZenNoteData noteData = new ZenNoteData;
                    noteData.m_FontIndex = data_from_client.param1.m_FontIndex;
                    noteData.m_DateText = data_from_client.param1.m_DateText; // Server date
                    noteData.m_NoteText = data_from_client.param1.m_NoteText;
                    noteData.m_ARGBColour = data_from_client.param1.m_ARGBColour;

                    // If quantity of this paper is 1, swap the actual item
                    if (this.GetQuantity() == 1)
                    {
                        // Prepare item replacement lambda
                        ReplacePaperWithNoteLambda lambda = new ReplacePaperWithNoteLambda(this, "ZenNote", noteData);

                        // If player is holding the note, replace it in their hands - otherwise replace the item elsewhere
                        if (player.GetItemInHands() == this)
                            player.ServerReplaceItemInHandsWithNew(lambda);
                        else
                            player.ServerReplaceItemWithNew(lambda);
                    }
                    else
                    {
                        this.SetQuantity(this.GetQuantity() - 1);

                        // Spawn a note on the ground
                        ZenNote noteGround = ZenNote.Cast(GetGame().CreateObjectEx("ZenNote", this.GetPosition(), ECE_PLACE_ON_SURFACE));

                        // If note did not spawn, stop here.
                        if (!noteGround)
                            return;

                        // Copy note data to the written note object
                        noteGround.SetNoteData(noteData);
                        MiscGameplayFunctions.TransferItemProperties(this, noteGround);

                        // If player is holding the paper stack, replace it in their hands with a note and put paper back in inventory
                        if (player.GetItemInHands() == this && noteGround)
                        {
                            // Put the paper stack back in its reserved location, and place note into player's hands
                            HumanInventory inventory = player.GetHumanInventory();
                            if (!inventory)
                                return;

                            // Get reserved paper location in inventory (dst)
                            InventoryLocation loc_dst = new InventoryLocation;
                            int reservedIndex = inventory.FindUserReservedLocationIndex(this);
                            inventory.GetUserReservedLocation(reservedIndex, loc_dst);

                            // Get current paper inventory location (src)
                            InventoryLocation loc_src = new InventoryLocation;
                            this.GetInventory().GetCurrentInventoryLocation(loc_src);

                            // If player has moved an item into the paper's reserved slot, or ServerTakeToDst fails, just take paper to anywhere in inventory.
                            if (reservedIndex == -1 || !player.ServerTakeToDst(loc_src, loc_dst))
                            {
                                // If we can't take the paper stack to inventory for whatever reason, do nothing, leave it in player's hands and spawn note on ground
                                player.ServerTakeEntityToInventory(FindInventoryLocationType.ANY_CARGO, this);
                            }

                            // Used to loop & try to take note to hands a few times (necessary because it takes a moment to update hand state when paper is put away)
                            m_NotePickupTries = 0;

                            // Attempt to pick up the note from the ground
                            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(TakeNoteToHands, 200, true, player, noteGround);
                        } 
                    }

                    // Consume pen quantity
                    if (GetZenNotesConfig().PenConsumeQuantity > 0 && m_Pen)
                    {
                        m_Pen.SetQuantity(m_Pen.GetQuantity() - GetZenNotesConfig().PenConsumeQuantity);
                    }

                    // Log note for server admins
                    string noteText = noteData.m_NoteText;
                    noteText.Replace("\n", "");
                    ZenModLogger.Log(player.GetCachedName() + " (" + sender.GetId() + ") @ " + this.GetPosition() + " wrote: " + noteText, "notes");
                }
            }
        }
    }

    // Pick up the note from the ground (TODO: Convoluted, there must be a better/more efficient way to do this!)
    private void TakeNoteToHands(PlayerBase player, EntityAI item)
    {
        if (item && player && player.IsAlive() && !player.IsUnconscious() && !player.IsPlayerDisconnected())
        {
            // For some reason this approach bugs the player's hands?? Seems to work in my other mods though, not sure what I'm doing wrong...
            //EntityAI note;
            //note = player.GetHumanInventory().CreateInHands("ZenNote");

            // Get inventory location of note (should be ground)
            InventoryLocation loc_src = new InventoryLocation;
            item.GetInventory().GetCurrentInventoryLocation(loc_src);

            // Create new inventory location with note (but set it to Hands location)
            InventoryLocation loc_dst = new InventoryLocation;
            loc_dst.Copy(loc_src);
            loc_dst.SetHands(player, item);

            // Take the ground note item to hands
            player.ServerTakeToDst(loc_src, loc_dst);

            // Increment pickup tries
            m_NotePickupTries++;

            // Try to pick up note for ~1 second, if failed just abandon picking up the note and leave it on the ground
            if (m_NotePickupTries >= 5)
            {
                GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.TakeNoteToHands);
            }
        }
        else
        {
            // If there is no note, no player object, or they're dead, or they're uncon, or they're disconnected, stop trying to pick it up and leave note on ground.
            GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(this.TakeNoteToHands);
        }
    }

    // Updates the text GUI if it's visible
    void UpdateGUI()
    {
        UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
        if (menu)
        {
            ZenNoteGUI noteMenu = ZenNoteGUI.Cast(menu);
            if (noteMenu)
            {
                noteMenu.SetDate();
                noteMenu.SetFontEnabled();
            }
        }
    }

    // For debug purposes / to send blacklist warnings
    void Zen_NoteSendMessage(PlayerBase player, string message)
    {
        Param1<string> m_MessageParam = new Param1<string>("");
        if (GetGame().IsDedicatedServer() && m_MessageParam && player.IsAlive() && !player.IsPlayerDisconnected() && message != "")
        {
            m_MessageParam.param1 = message;
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
        }
    }
};

// Lambda for replacing paper with written note object
class ReplacePaperWithNoteLambda extends ReplaceItemWithNewLambdaBase
{
    ref ZenNoteData m_NoteData;

    void ReplacePaperWithNoteLambda(EntityAI old_item, string new_item_type, ZenNoteData data)
    {
        m_NoteData = data;
    }

    override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
    {
        super.CopyOldPropertiesToNew(old_item, new_item);

        ZenNote note = ZenNote.Cast(new_item);
        if (!note)
            return;

        note.SetNoteData(m_NoteData);
        MiscGameplayFunctions.TransferItemProperties(old_item, note);

        if (!note.GetParent()) // If not in player's hands, copy paper location & orientation to new object
        {
            note.SetOrientation(old_item.GetOrientation());
            note.SetPosition(old_item.GetPosition());
        }
    }
}