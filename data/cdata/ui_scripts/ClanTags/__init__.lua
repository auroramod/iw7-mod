Engine.TruncateToGlyphCount = function(str, len)
    return str
end

Game.GetPlayerClanTag = function(arg0)
    return game:getplayerclantag(arg0)
end

if not Engine.InFrontend() then
    return
end

function PostLoadFunc(f1_arg0, f1_arg1)
    assert(f1_arg0.ClanTagButton)
    local f1_local0 = f1_arg0.ClanTagButton
    local f1_local1 = f1_local0
    f1_local0 = f1_local0.SetButtonDisabled
    local f1_local2 = Engine.IsXB3()
    if f1_local2 then
        f1_local2 = not Engine.IsUserSignedInToLive(f1_arg1)
    end
    f1_local0(f1_local1, f1_local2)
    f1_arg0.ClanTagButton:addEventHandler("button_action", function(f2_arg0, f2_arg1)
        local f2_local0 = f2_arg1.controller or f1_arg1
        Engine.AntiCheatBanCheck(f2_local0, CoD.AntiCheat.Ban.FEATURE_BAN_GLOBAL_CLAN_TAG)
        OSK.OpenScreenKeyboard(f2_local0, Engine.Localize("@LUA_MENU_CUSTOM_CLAN_TAG"),
            Engine.GetCustomClanTag(f2_local0) or "", 4, true, true, true, function(f3_arg0, f3_arg1, f3_arg2)
                if f3_arg1 then
                    Engine.SetAndEnableCustomClanTag(f3_arg0, f3_arg1)
                    Engine.ExecNow("uploadstats", f3_arg0)
                end
            end)
    end)
    assert(f1_arg0.patchEmblem)
    f1_arg0.patchEmblem:addEventHandler("button_action", function(f4_arg0, f4_arg1)
        if not Engine.IsUserAGuest(f1_arg1) then
            ACTIONS.OpenMenu("EmblemSelectMenu", true, f1_arg1)
        elseif CONDITIONS.IsXboxOne() then
            LUI.FlowManager.RequestPopupMenu(f1_arg0, "mp_no_guest_popup", false, f1_arg1, false)
        end
    end)
    assert(f1_arg0.CallingCardButton)
    f1_arg0.CallingCardButton:addEventHandler("button_action", function(f5_arg0, f5_arg1)
        if not Engine.IsUserAGuest(f1_arg1) then
            ACTIONS.OpenMenu("CallingCardCategoryMenu", true, f1_arg1)
        elseif CONDITIONS.IsXboxOne() then
            LUI.FlowManager.RequestPopupMenu(f1_arg0, "mp_no_guest_popup", false, f1_arg1, false)
        end
    end)
    if f1_arg0.HeadquartersAnnouncerButton then
        f1_arg0.HeadquartersAnnouncerButton:addEventHandler("button_action", function(f6_arg0, f6_arg1)
            LUI.FlowManager.RequestPopupMenu(f1_arg0, "AnnouncerVoiceSelectPopup", false, f1_arg1, false, {
                controllerIndex = f1_arg1
            })
        end)
    end
    if CONDITIONS.UseCACBreadCrumbs() and CONDITIONS.IsPublicMatch() then
        local f1_local0 = Rewards.IsTypeNew(f1_arg1, "callingCard")
        if not f1_local0 then
            f1_local0 = CallingCardUtils.HasAnyNewLootCards(f1_arg1)
        end
        if f1_local0 then
            assert(f1_arg0.CallingCardButton)
            local f1_local1 = MenuBuilder.BuildRegisteredType("NewItemNotification", {
                controllerIndex = f1_arg1
            })
            f1_local1.id = "newWidget"
            if CONDITIONS.IsArabic(f1_arg0) then
                f1_local1:SetAnchorsAndPosition(1, 0, 0, 0, _1080p * -490, _1080p * -426, _1080p * -16, _1080p * 16)
            else
                f1_local1:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * 385, _1080p * -51, _1080p * -17, _1080p * 47)
            end
            f1_arg0.CallingCardButton:addElement(f1_local1)
        end
    end
end
function HeadquartersCustomizationButtons(menu, controller)
    local self = LUI.UIVerticalList.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 319 * _1080p)
    self.id = "HeadquartersCustomizationButtons"
    self._animationSets = {}
    self._sequences = {}
    local f7_local1 = controller and controller.controllerIndex
    if not f7_local1 and not Engine.InFrontend() then
        f7_local1 = self:getRootController()
    end
    assert(f7_local1)
    local f7_local2 = self
    self:SetSpacing(10 * _1080p)
    local Spacer = nil
    Spacer = LUI.UIImage.new()
    Spacer.id = "Spacer"
    Spacer:SetAlpha(0, 0)
    Spacer:SetAnchorsAndPosition(0, 1, 0, 1, 0, 0, 0, _1080p * 9)
    self:addElement(Spacer)
    self.Spacer = Spacer
    local CallingCardButton = nil
    CallingCardButton = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = f7_local1
    })
    CallingCardButton.id = "CallingCardButton"
    CallingCardButton.buttonDescription = Engine.Localize("MENU_DESC_PLAYERCARD_BARRACKS")
    CallingCardButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MP_CHANGE_PLAYERCARD")), 0)
    CallingCardButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 19, _1080p * 49)
    self:addElement(CallingCardButton)
    self.CallingCardButton = CallingCardButton
    local patchEmblem = nil
    patchEmblem = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = f7_local1
    })
    patchEmblem.id = "patchEmblem"
    patchEmblem.buttonDescription = Engine.Localize("MENU_DESC_EMBLEM")
    patchEmblem.Text:setText(ToUpperCase(Engine.Localize("MENU_EMBLEM_CAPS")), 0)
    patchEmblem:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 59, _1080p * 89)
    self:addElement(patchEmblem)
    self.patchEmblem = patchEmblem
    local f7_local6 = nil
    f7_local6 = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = f7_local1
    })
    f7_local6.id = "ClanTagButton"
    f7_local6.buttonDescription = Engine.Localize("LUA_MENU_CUSTOMIZATION_FIELD_DESC_CLAN_TAG")
    f7_local6.Text:setText(ToUpperCase(Engine.Localize("MENU_CLAN_TAG_CAPS")), 0)
    f7_local6:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 99, _1080p * 129)
    self:addElement(f7_local6)
    self.ClanTagButton = f7_local6
    local f7_local7 = nil
    if Engine.IsAliensMode() then
        f7_local7 = MenuBuilder.BuildRegisteredType("GenericButton", {
            controllerIndex = f7_local1
        })
        f7_local7.id = "LobbyMusicButton"
        if Engine.IsAliensMode() then
        else
        end
        if Engine.IsAliensMode() then
            f7_local7.buttonDescription = Engine.Localize("LUA_MENU_ZM_LOBBY_MUSIC_DESC")
        end
        f7_local7.Text:setText(ToUpperCase(Engine.Localize("MENU_LOBBY_MUSIC")), 0)
        f7_local7:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 139, _1080p * 169)
        self:addElement(f7_local7)
        self.LobbyMusicButton = f7_local7
    end
    local f7_local8 = nil
    if CONDITIONS.ShouldShowAnnouncersMenu(self) then
        f7_local8 = MenuBuilder.BuildRegisteredType("HeadquartersAnnouncerButton", {
            controllerIndex = f7_local1
        })
        f7_local8.id = "HeadquartersAnnouncerButton"
        f7_local8:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 179, _1080p * 209)
        self:addElement(f7_local8)
        self.HeadquartersAnnouncerButton = f7_local8
    end
    local ButtonDescription = nil
    ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = f7_local1
    })
    ButtonDescription.id = "ButtonDescription"
    ButtonDescription.Description:SetRight(_1080p * 415, 0)
    ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 219, _1080p * 319)
    self:addElement(ButtonDescription)
    self.ButtonDescription = ButtonDescription
    self._animationSets.DefaultAnimationSet = function()
        self._sequences.DefaultSequence = function()
        end
        ButtonDescription:RegisterAnimationSequence("CPinit", {{function()
            return self.ButtonDescription:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
        end}})
        self._sequences.CPinit = function()
            ButtonDescription:AnimateSequence("CPinit")
        end
    end
    self._animationSets.DefaultAnimationSet()
    if Engine.IsAliensMode() then
        f7_local7:addEventHandler("button_action", function(f12_arg0, f12_arg1)
            ACTIONS.OpenMenu("CPLobbyMusicMenu", true, f12_arg1.controller or f7_local1)
        end)
    end
    PostLoadFunc(self, f7_local1, controller)
    return self
end
MenuBuilder.registerType("HeadquartersCustomizationButtons", HeadquartersCustomizationButtons)
