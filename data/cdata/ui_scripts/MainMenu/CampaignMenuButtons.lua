local f0_local1 = function(f2_arg0)
    f2_arg0.ResumeButton:SetButtonDisabled(not Engine.CanResumeGame(f2_arg0._controllerIndex))
    if not CONDITIONS.IsTrialLicense(f2_arg0) then
        local f2_local0 = f2_arg0.MissionSelectButton
        local f2_local1 = f2_local0
        f2_local0 = f2_local0.SetButtonDisabled
        local f2_local2 = Engine.IsTrialLicense()
        if not f2_local2 then
            if not Engine.IsDevelopmentBuild() and not Engine.GetDvarBool("mis_cheat") then
                f2_local2 = not f0_local0(f2_arg0._controllerIndex)
            else
                f2_local2 = false
            end
        end
        f2_local0(f2_local1, f2_local2)
    end
end

local f0_local2 = function(f3_arg0, f3_arg1)
    LUI.FlowManager.RequestPopupMenu(nil, "FakeLoadingScreenOverlay", true, 0, false, {
        onLoadCompleteFunc = function()
            Engine.SetDvarString("ui_char_museum_mode", "credits_1")
            Engine.Exec("profile_difficultyLoad")
            Engine.Exec("profile_specialistModeLoad")
            Engine.Exec("profile_yoloModeLoad")
            Engine.Exec("loadgame_continue")
            Engine.Exec("updategamerprofile")
        end
    })
    LUI.FlowManager.RequestLeaveMenu(f3_arg0)
end

local f0_local3 = function(f5_arg0, f5_arg1)
    LUI.FlowManager.RequestPopupMenu(nil, "FakeLoadingScreenOverlay", true, 0, false, {
        onLoadCompleteFunc = function()
            Engine.Exec("set ui_play_credits 1; map shipcrib_epilogue")
        end
    })
end

-- MenuBuilder.registerType( "ResumeGamePopup", function ( f7_arg0, f7_arg1 )
--	return MenuBuilder.BuildRegisteredType( "PopupYesNo", {
--		message = Engine.Localize( "@MENU_RESUMEGAME_Q" ),
--		yesAction = f0_local2
--	} )
-- end )
local f0_local4 = function(f8_arg0, f8_arg1, f8_arg2)
    assert(f8_arg0.ResumeButton)
    assert(f8_arg0.NewButton)
    assert(f8_arg0.CreditsButton)
    if not CONDITIONS.IsTrialLicense(f8_arg0) then
        assert(f8_arg0.MissionSelectButton)
    end
    f8_arg0._controllerIndex = f8_arg1
    f8_arg0.ResumeButton:addEventHandler("button_action", function(f9_arg0, f9_arg1)
        Engine.SetDvarString("start", "")
        LUI.FlowManager.RequestPopupMenu(f9_arg0, "ResumeGamePopup", false, f9_arg1.controller, false)
    end)
    f8_arg0.NewButton:addEventHandler("button_action", function(f10_arg0, f10_arg1)
        Engine.SetDvarString("start", "")
        if Engine.CanResumeGame(f8_arg1) then
            LUI.FlowManager.RequestPopupMenu(f8_arg0, "overwrite_warning_menu", true, f10_arg1.controller)
        else
            LUI.FlowManager.RequestPopupMenu(f8_arg0, "popmenu_autosave_warning", true, f10_arg1.controller)
        end
    end)
    if not CONDITIONS.IsTrialLicense(f8_arg0) then
        f8_arg0.MissionSelectButton:addEventHandler("button_action", function(f11_arg0, f11_arg1)
            Engine.SetDvarString("start", "")
            LUI.FlowManager.RequestAddMenu("LevelSelectMenu", true, f11_arg1.controller, false)
        end)
    end
    f8_arg0.CreditsButton:addEventHandler("button_action", f0_local3)
    f0_local1(f8_arg0)
    f8_arg0:addEventHandler("update_save_game_available_complete", f0_local1)
    if Engine.GetDvarFloat("r_filmGrainAtten") == 0.25 then
        Engine.SetDvarFloat("r_filmGrainAtten", 1)
        Engine.ExecNow("profile_setFilmGrain " .. tostring(1), f8_arg1)
    end

    f8_arg0.ModsButton:addEventHandler("button_action", function(arg0, arg1)
        LUI.FlowManager.RequestAddMenu("ModSelectMenu", true, arg1.controller, false)
    end)
end

function CampaignMenuButtons(menu, controller)
    local self = LUI.UIVerticalList.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 440 * _1080p)
    self.id = "CampaignMenuButtons"
    local f12_local1 = controller and controller.controllerIndex
    if not f12_local1 and not Engine.InFrontend() then
        f12_local1 = self:getRootController()
    end
    assert(f12_local1)
    local f12_local2 = self
    self:SetSpacing(10 * _1080p)
    local ResumeButton = nil

    ResumeButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = f12_local1
    })
    ResumeButton.id = "ResumeButton"
    ResumeButton.buttonDescription = Engine.Localize("LUA_MENU_RESUME_GAME_DESC")
    ResumeButton.Text:setText(Engine.Localize("MENU_RESUMEGAME_CAPS"), 0)
    ResumeButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, 0, _1080p * 30)
    self:addElement(ResumeButton)
    self.ResumeButton = ResumeButton

    local NewButton = nil

    NewButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = f12_local1
    })
    NewButton.id = "NewButton"
    NewButton.buttonDescription = Engine.Localize("LUA_MENU_NEW_GAME_DESC")
    NewButton.Text:setText(Engine.Localize("MENU_NEWGAME_CAPS"), 0)
    NewButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
    self:addElement(NewButton)
    self.NewButton = NewButton

    local f12_local5 = nil
    if not CONDITIONS.IsTrialLicense(self) then
        f12_local5 = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = f12_local1
        })
        f12_local5.id = "MissionSelectButton"
        if not CONDITIONS.IsTrialLicense(self) then

        else

        end
        if not CONDITIONS.IsTrialLicense(self) then
            f12_local5.buttonDescription = Engine.Localize("LUA_MENU_MISSION_SELECT_DESC")
        end
        f12_local5.Text:setText(Engine.Localize("MENU_MISSION_SELECT_CAPS"), 0)
        f12_local5:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110)
        self:addElement(f12_local5)
        self.MissionSelectButton = f12_local5
    end
    local CreditsButton = nil

    CreditsButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = f12_local1
    })
    CreditsButton.id = "CreditsButton"
    CreditsButton.buttonDescription = Engine.Localize("LUA_MENU_CREDITS_DESC")
    CreditsButton.Text:setText(ToUpperCase(Engine.Localize("MENU_SP_CREDITS_CAPS")), 0)
    CreditsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150)
    self:addElement(CreditsButton)
    self.CreditsButton = CreditsButton

    local ModsButton = nil

    ModsButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = f12_local1
    })
    ModsButton.id = "ModsButton"
    ModsButton.buttonDescription = Engine.Localize("LUA_MENU_MODS_DESC")
    ModsButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MODS_CAPS")), 0)
    ModsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 160, _1080p * 190)
    self:addElement(ModsButton)
    self.ModsButton = ModsButton

    local ButtonDescription = nil

    ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = f12_local1
    })
    ButtonDescription.id = "ButtonDescription"
    ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 200, _1080p * 300)
    self:addElement(ButtonDescription)
    self.ButtonDescription = ButtonDescription

    f0_local4(self, f12_local1, controller)
    return self
end

MenuBuilder.m_types["CampaignMenuButtons"] = CampaignMenuButtons
