local f0_local0 = function ( f1_arg0 )
	if Engine.HasCompletedAnyLevel( f1_arg0 ) then
		return true
	end
	local f1_local0 = {}
	for f1_local1 = 0, Engine.TableGetRowCount( CSV.levels.file ) - 1, 1 do
		local f1_local4 = CSV.ReadRow( CSV.levels, f1_local1 )
		local f1_local5 = f1_local4.name
		if not Engine.GetDvarBool( "lui_checkIfLevelInFileSystem" ) or Engine.IsLevelInFileSystem and Engine.IsLevelInFileSystem( f1_local5 ) then
			f1_local0[#f1_local0 + 1] = {
				buttonLabel = f1_local4.string,
				levelName = f1_local5,
				objectiveText = f1_local4.desc,
				levelNumber = f1_local4.ref,
				completedLevelIndex = f1_local4.completedRef,
				image = f1_local4.image
			}
		end
	end
	local f1_local1 = LUI.DataSourceFromPlayerData.new( CoD.ProgressionBlob.Gold, CoD.PlayMode.SP )
	local f1_local2 = f1_local1.spData
	for f1_local3 = 1, #f1_local0, 1 do
		local f1_local7 = f1_local0[f1_local3].levelName
		if f1_local7 ~= "europa" then
			local f1_local8 = ""
			if f1_local2.missionStateData[f1_local7] ~= nil then
				f1_local8 = f1_local2.missionStateData[f1_local7]:GetValue( f1_arg0 )
			end
			if f1_local8 ~= nil and (f1_local8 == "complete" or f1_local8 == "incomplete") then
				return true
			end
		end
	end
	return false
end

local f0_local1 = function(arg0)
    arg0.ResumeButton:SetButtonDisabled(not Engine.CanResumeGame(arg0._controllerIndex))
    if not CONDITIONS.IsTrialLicense(arg0) then
        local f2_local0 = arg0.MissionSelectButton
        local f2_local1 = f2_local0
        f2_local0 = f2_local0.SetButtonDisabled
        local f2_local2 = Engine.IsTrialLicense()
        if not f2_local2 then
            if not Engine.IsDevelopmentBuild() and not Engine.GetDvarBool("mis_cheat") then
                f2_local2 = not f0_local0(arg0._controllerIndex)
            else
                f2_local2 = false
            end
        end
        f2_local0(f2_local1, f2_local2)
    end
end

local f0_local3 = function(arg0, arg1)
    LUI.FlowManager.RequestPopupMenu(nil, "FakeLoadingScreenOverlay", true, 0, false, {
        onLoadCompleteFunc = function()
            Engine.Exec("set ui_play_credits 1; map shipcrib_epilogue")
        end
    })
end

local f0_local4 = function(arg0, arg1, arg2)
    assert(arg0.ResumeButton)
    assert(arg0.NewButton)
    assert(arg0.CreditsButton)
    if not CONDITIONS.IsTrialLicense(arg0) then
        assert(arg0.MissionSelectButton)
    end
    arg0._controllerIndex = arg1
    arg0.ResumeButton:addEventHandler("button_action", function(f9_arg0, f9_arg1)
        Engine.SetDvarString("start", "")
        LUI.FlowManager.RequestPopupMenu(f9_arg0, "ResumeGamePopup", false, f9_arg1.controller, false)
    end)
    arg0.NewButton:addEventHandler("button_action", function(f10_arg0, f10_arg1)
        Engine.SetDvarString("start", "")
        if Engine.CanResumeGame(arg1) then
            LUI.FlowManager.RequestPopupMenu(arg0, "overwrite_warning_menu", true, f10_arg1.controller)
        else
            LUI.FlowManager.RequestPopupMenu(arg0, "popmenu_autosave_warning", true, f10_arg1.controller)
        end
    end)
    if not CONDITIONS.IsTrialLicense(arg0) then
        arg0.MissionSelectButton:addEventHandler("button_action", function(f11_arg0, f11_arg1)
            Engine.SetDvarString("start", "")
            LUI.FlowManager.RequestAddMenu("LevelSelectMenu", true, f11_arg1.controller, false)
        end)
    end
    arg0.CreditsButton:addEventHandler("button_action", f0_local3)
    f0_local1(arg0)
    arg0:addEventHandler("update_save_game_available_complete", f0_local1)
    if Engine.GetDvarFloat("r_filmGrainAtten") == 0.25 then
        Engine.SetDvarFloat("r_filmGrainAtten", 1)
        Engine.ExecNow("profile_setFilmGrain " .. tostring(1), arg1)
    end

    arg0.ModsButton:addEventHandler("button_action", function(arg0, arg1)
        LUI.FlowManager.RequestAddMenu("ModSelectMenu", true, arg1.controller, false)
    end)
end

function CampaignMenuButtons(menu, controller)
    local self = LUI.UIVerticalList.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 440 * _1080p)
    self.id = "CampaignMenuButtons"
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)
    self:SetSpacing(10 * _1080p)

    local ResumeButton = nil

    ResumeButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    ResumeButton.id = "ResumeButton"
    ResumeButton.buttonDescription = Engine.Localize("LUA_MENU_RESUME_GAME_DESC")
    ResumeButton.Text:setText(Engine.Localize("MENU_RESUMEGAME_CAPS"), 0)
    ResumeButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, 0, _1080p * 30)
    self:addElement(ResumeButton)
    self.ResumeButton = ResumeButton

    local NewButton = nil

    NewButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    NewButton.id = "NewButton"
    NewButton.buttonDescription = Engine.Localize("LUA_MENU_NEW_GAME_DESC")
    NewButton.Text:setText(Engine.Localize("MENU_NEWGAME_CAPS"), 0)
    NewButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
    self:addElement(NewButton)
    self.NewButton = NewButton

    local MissionSelectButton = nil
    if not CONDITIONS.IsTrialLicense(self) then
        MissionSelectButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        MissionSelectButton.id = "MissionSelectButton"
        if not CONDITIONS.IsTrialLicense(self) then
            MissionSelectButton.buttonDescription = Engine.Localize("LUA_MENU_MISSION_SELECT_DESC")
        end
        MissionSelectButton.Text:setText(Engine.Localize("MENU_MISSION_SELECT_CAPS"), 0)
        MissionSelectButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110)
        self:addElement(MissionSelectButton)
        self.MissionSelectButton = MissionSelectButton
    end
    local CreditsButton = nil

    CreditsButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    CreditsButton.id = "CreditsButton"
    CreditsButton.buttonDescription = Engine.Localize("LUA_MENU_CREDITS_DESC")
    CreditsButton.Text:setText(ToUpperCase(Engine.Localize("MENU_SP_CREDITS_CAPS")), 0)
    CreditsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150)
    self:addElement(CreditsButton)
    self.CreditsButton = CreditsButton

    local ModsButton = nil

    ModsButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    ModsButton.id = "ModsButton"
    ModsButton.buttonDescription = Engine.Localize("LUA_MENU_MODS_DESC")
    ModsButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MODS_CAPS")), 0)
    ModsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 160, _1080p * 190)
    self:addElement(ModsButton)
    self.ModsButton = ModsButton

    local ButtonDescription = nil

    ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = controllerIndex
    })
    ButtonDescription.id = "ButtonDescription"
    ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 200, _1080p * 300)
    self:addElement(ButtonDescription)
    self.ButtonDescription = ButtonDescription

    f0_local4(self, controllerIndex, controller)
    return self
end

MenuBuilder.m_types["CampaignMenuButtons"] = CampaignMenuButtons
