local f0_local0 = function(f1_arg0, f1_arg1, f1_arg2)
    assert(f1_arg0.CreateAClassButton)
    assert(f1_arg0.MissionSelect)
    local f1_local0 = LUI.DataSourceInGlobalModel.new("frontEnd.lobby.findMatchButtonWaitStatus")
    f1_arg0:SubscribeToModel(f1_local0:GetModel(f1_arg1), function()
        local f2_local0 = f1_local0:GetValue(f1_arg1)
        local f2_local1 = "Browse for Custom Servers"
        if f2_local0 == "" then
            f1_arg0.MissionSelect:SetButtonDisabled(false)
        else
            f1_arg0.MissionSelect:SetButtonDisabled(true)
            f2_local1 = f2_local0
        end
        f1_arg0.MissionSelect.buttonDescription = f2_local1
        f1_arg0.ButtonDescription:processEvent({
            name = "update_button_description",
            text = f2_local1
        })
    end)
    if not Onboarding.RigTutorial:WasCompleted(f1_arg1) then
        f1_arg0.CreateAClassButton.listDefaultFocus = 0
    end
    if not CONDITIONS.IsQuarterMasterAllowed(f1_arg0) then
        f1_arg0.Barracks:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, _1080p * 120, _1080p * 150)
        f1_arg0.ButtonDescription:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, _1080p * 160, _1080p * 190)
    end
end

function MissionsVerticalLayout(menu, controller)
    local self = LUI.UIVerticalList.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 330 * _1080p)
    self.id = "MissionsVerticalLayout"
    self._animationSets = {}
    self._sequences = {}
    local f3_local1 = controller and controller.controllerIndex
    if not f3_local1 and not Engine.InFrontend() then
        f3_local1 = self:getRootController()
    end
    assert(f3_local1)
    local f3_local2 = self
    self:SetSpacing(10 * _1080p)
    local MissionSelect = nil

    MissionSelect = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = f3_local1
    })
    MissionSelect.id = "MissionSelect"
    MissionSelect.buttonDescription = "Browse for Custom Servers"
    MissionSelect.Text:setText(ToUpperCase("Server Browser"), 0)
    MissionSelect:SetAnchorsAndPosition(0, 0, 0, 1, 0, _1080p * -4, 0, _1080p * 30)
    self:addElement(MissionSelect)
    self.MissionSelect = MissionSelect

    local MissionTeams = nil

    MissionTeams = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = f3_local1
    })
    MissionTeams.id = "MissionTeams"
    MissionTeams.buttonDescription = Engine.Localize("LUA_MENU_DESC_MISSION_TEAMS")
    MissionTeams.Text:setText(ToUpperCase(Engine.Localize("MENU_MISSION_TEAMS")), 0)
    MissionTeams:SetAnchorsAndPosition(0, 0, 0, 1, 0, _1080p * -4, _1080p * 40, _1080p * 70)
    self:addElement(MissionTeams)
    self.MissionTeams = MissionTeams

    local CreateAClassButton = nil

    CreateAClassButton = MenuBuilder.BuildRegisteredType("CreateAClassButton", {
        controllerIndex = f3_local1
    })
    CreateAClassButton.id = "CreateAClassButton"
    CreateAClassButton:SetAnchorsAndPosition(0, 0, 0, 1, 0, _1080p * -4, _1080p * 80, _1080p * 110)
    self:addElement(CreateAClassButton)
    self.CreateAClassButton = CreateAClassButton

    local f3_local6 = nil
    if CONDITIONS.IsQuarterMasterAllowed(self) then
        f3_local6 = MenuBuilder.BuildRegisteredType("QuartermasterButton", {
            controllerIndex = f3_local1
        })
        f3_local6.id = "Armory"
        f3_local6:SetAnchorsAndPosition(0, 0, 0, 1, 0, _1080p * -4, _1080p * 120, _1080p * 150)
        self:addElement(f3_local6)
        self.Armory = f3_local6
    end
    local Barracks = nil

    Barracks = MenuBuilder.BuildRegisteredType("BarracksButton", {
        controllerIndex = f3_local1
    })
    Barracks.id = "Barracks"
    Barracks:SetAnchorsAndPosition(0, 0, 0, 1, 0, _1080p * -4, _1080p * 160, _1080p * 190)
    self:addElement(Barracks)
    self.Barracks = Barracks

    local f3_local8 = nil
    if CONDITIONS.AreContractsEnabled(self) then
        f3_local8 = MenuBuilder.BuildRegisteredType("ContractsButton", {
            controllerIndex = f3_local1
        })
        f3_local8.id = "Contracts"
        f3_local8:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 200, _1080p * 260)
        self:addElement(f3_local8)
        self.Contracts = f3_local8
    end
    local ButtonDescription = nil

    ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = f3_local1
    })
    ButtonDescription.id = "ButtonDescription"
    ButtonDescription.Description:SetRight(_1080p * 415, 0)
    ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 270, _1080p * 370)
    self:addElement(ButtonDescription)
    self.ButtonDescription = ButtonDescription

    self._animationSets.DefaultAnimationSet = function()
        self._sequences.DefaultSequence = function()

        end

        CreateAClassButton:RegisterAnimationSequence("ContractsKillswitched", {{function()
            return self.CreateAClassButton:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, _1080p * 80, _1080p * 110, 0)
        end}})
        if CONDITIONS.IsQuarterMasterAllowed(self) then
            f3_local6:RegisterAnimationSequence("ContractsKillswitched", {{function()
                return self.Armory:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, _1080p * 120, _1080p * 150, 0)
            end}})
        end
        Barracks:RegisterAnimationSequence("ContractsKillswitched", {{function()
            return self.Barracks:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, _1080p * 160, _1080p * 190, 0)
        end}})
        ButtonDescription:RegisterAnimationSequence("ContractsKillswitched", {{function()
            return self.ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 201, _1080p * 301,
                0)
        end}})
        self._sequences.ContractsKillswitched = function()
            CreateAClassButton:AnimateSequence("ContractsKillswitched")
            if CONDITIONS.IsQuarterMasterAllowed(self) then
                f3_local6:AnimateSequence("ContractsKillswitched")
            end
            Barracks:AnimateSequence("ContractsKillswitched")
            ButtonDescription:AnimateSequence("ContractsKillswitched")
        end

    end

    self._animationSets.DefaultAnimationSet()
    MissionSelect:addEventHandler("button_action", function(f11_arg0, f11_arg1)
        local f11_local0 = f11_arg1.controller or f3_local1
        ACTIONS.OpenMenu("SystemLinkMenu", true, f11_local0)
    end)
    MissionTeams:addEventHandler("button_action", function(f12_arg0, f12_arg1)
        ACTIONS.OpenMenu("MissionTeamSelect", true, f12_arg1.controller or f3_local1)
    end)
    f0_local0(self, f3_local1, controller)
    return self
end

MenuBuilder.m_types["MissionsVerticalLayout"] = MissionsVerticalLayout
