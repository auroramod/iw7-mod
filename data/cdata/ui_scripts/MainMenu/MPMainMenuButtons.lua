local buttonSpacing = 40
local f0_local1 = 10
local f0_local2 = function(f1_arg0, f1_arg1, f1_arg2)
    if 0 < f1_arg2 then
        local f1_local0, f1_local1, f1_local2, f1_local3 = f1_arg0:getLocalRect()
        local f1_local4 = f1_local3 - f1_local1
        f1_arg0:SetTop(f1_local1 - (f1_local4 + f1_arg1) * f1_arg2)
        f1_arg0:SetBottom(f1_local3 - (f1_local4 + f1_arg1) * f1_arg2)
    end
end

local f0_local3 = function(f2_arg0, f2_arg1, f2_arg2)
    assert(f2_arg0.ConquestButton)
    if CONDITIONS.IsStoreAllowed(f2_arg0) then
        assert(f2_arg0.StoreButton)
    end
    local f2_local0 = not CONDITIONS.IsTrialLicense(f2_arg0)
    if f2_local0 then
        assert(f2_arg0.CustomGameButton)
    end
    f2_arg0.buttonSpacing = _1080p * buttonSpacing
    local f2_local1 = function()
        return Lobby.IsInPrivateParty() and not Lobby.IsPrivatePartyHost()
    end

    local f2_local2 = function()
        local f4_local0 = f2_local1()
        f2_arg0.ConquestButton:SetButtonDisabled(f4_local0)
        if f2_arg0.MLGGameBattlesButton ~= nil then
            f2_arg0.MLGGameBattlesButton:SetButtonDisabled(f4_local0)
        end
        if f2_local0 then
            f2_arg0.CustomGameButton:SetButtonDisabled(f4_local0)
        end
    end

    local f2_local3 = LUI.DataSourceInGlobalModel.new("frontEnd.lobby.areWeGameHost")
    local f2_local4 = DataSources.frontEnd.lobby.memberCount
    f2_arg0:SubscribeToModel(f2_local3:GetModel(f2_arg1), f2_local2)
    f2_arg0:SubscribeToModel(f2_local4:GetModel(f2_arg1), f2_local2)
    f2_arg0.ConquestButton:addEventHandler("button_action", function(f5_arg0, f5_arg1)
        Engine.SetDvarBool("cg_mlg_gamebattles_match", false)
        local f5_local0 = function()
            LUI.FlowManager.RequestAddMenu("Missions", false, f5_arg1.controller, false, {}, true)
        end

        if not Onboarding:BeginFlow(Onboarding.RigTutorial, f2_arg1) then
            f5_local0()
        else
            LUI.FlowManager.RequestPopupMenu(nil, "MPFullScreenVideoOverlay", true, f2_arg1, nil, {
                videoRef = "mp_wolverines_mission_commander",
                allowSkip = true,
                doIntroFadeOut = false,
                doIntroFadeIn = false,
                doOutroFadeIn = true,
                doOutroFadeOut = true,
                fadeColor = COLORS.black
            }, nil, true, true)
            local f5_local1 = f2_arg0:Wait(500)
            f5_local1.onComplete = f5_local0
        end
    end)
    if CONDITIONS.IsGameBattlesAllowed(f2_arg0) then
        f2_arg0.MLGGameBattlesButton:addEventHandler("button_action", function(f7_arg0, f7_arg1)
            if Engine.GetDvarBool("splitscreen") then
                LUI.FlowManager.RequestPopupMenu(f2_arg0, "MLGGamebattlesSplitscreenPopup", true, f7_arg1.controller,
                    false, {
                        controllerIndex = f2_arg1
                    })
            elseif Lobby.IsNotAloneInPrivateParty() then
                LUI.FlowManager.RequestPopupMenu(f2_arg0, "DisbandPartyEnterGameBattlesLobbyPopup", true,
                    f7_arg1.controller, false, {
                        controllerIndex = f2_arg1
                    })
            else
                OpenGameBattlesLobby(f7_arg1.controller)
            end
        end)
    end
    if f2_local0 then
        f2_arg0.CustomGameButton:addEventHandler("button_action", function(f8_arg0, f8_arg1)
            OpenPrivateMatchLobby(f8_arg1)
        end)
    end

    f2_arg0.ModsButton:addEventHandler("button_action", function(arg0, arg1)
        LUI.FlowManager.RequestAddMenu("ModSelectMenu", true, arg1.controller, false)
    end)

    if CONDITIONS.IsStoreAllowed(f2_arg0) then
        f2_arg0.StoreButton:addEventHandler("button_action", function(f9_arg0, f9_arg1)
            local f9_local0 = STORE.GoToStore
            local f9_local1 = f9_arg1.controller
            local f9_local2 = f9_arg0:GetCurrentMenu()
            f9_local0(f9_local1, f9_local2.id, f9_arg0.id)
        end)
    end
    local f2_local5 = _1080p * f0_local1
    local f2_local6 = 0
    if f2_arg0.MLGGameBattlesButton == nil then
        f2_local6 = 1
    end
    if f2_arg0.CustomGameButton then
        f0_local2(f2_arg0.CustomGameButton, f2_local5, f2_local6)
    else
        f2_local6 = f2_local6 + 1
    end
    if f2_arg0.ModsButton then
        f0_local2(f2_arg0.ModsButton, f2_local5, f2_local6)
    else
        f2_local6 = f2_local6 + 1
    end
    if CONDITIONS.IsStoreAllowed(f2_arg0) then
        f0_local2(f2_arg0.StoreButton, f2_local5, f2_local6)
    else
        f2_local6 = f2_local6 + 1
    end
    if f2_arg0.StoreButton then
        f2_arg0.StoreButton:SetButtonDescription(STORE.GetStoreDescription())
        if CONDITIONS.IsTrialLicense() then
            f2_arg0.StoreButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_BUY_NOW")))
        end
    end
end

function MPMainMenuButtons(menu, controller)
    local self = LUI.UIVerticalNavigator.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 190 * _1080p)
    self.id = "MPMainMenuButtons"
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)
    local ConquestButton = nil

    ConquestButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    ConquestButton.id = "ConquestButton"
    ConquestButton.buttonDescription = Engine.Localize("LUA_MENU_PUBLIC_MATCH_DESC")
    ConquestButton.Text:setText(Engine.Localize("LUA_MENU_PUBLIC_MATCH_CAPS"), 0)
    ConquestButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, 0, _1080p * 30)
    self:addElement(ConquestButton)
    self.ConquestButton = ConquestButton

    local MLGGameBattlesButton = nil
    if CONDITIONS.IsGameBattlesAllowed(self) then
        MLGGameBattlesButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        MLGGameBattlesButton.id = "MLGGameBattlesButton"
        if CONDITIONS.IsGameBattlesAllowed(self) then

        else

        end
        if CONDITIONS.IsGameBattlesAllowed(self) then
            MLGGameBattlesButton.buttonDescription = Engine.Localize("LUA_MENU_MLG_GAMEBATTLES_DESC")
        end
        MLGGameBattlesButton.Text:setText(Engine.Localize("LUA_MENU_MLG_GAMEBATTLES_CAPS"), 0)
        MLGGameBattlesButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
        self:addElement(MLGGameBattlesButton)
        self.MLGGameBattlesButton = MLGGameBattlesButton
    end

    local CustomGameButton = nil

    CustomGameButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    CustomGameButton.id = "CustomGameButton"
    CustomGameButton.buttonDescription = Engine.Localize("LUA_MENU_CUSTOM_GAME_DESC")
    CustomGameButton.Text:setText(Engine.Localize("LUA_MENU_CUSTOM_GAME_CAPS"), 0)
    CustomGameButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110)
    self:addElement(CustomGameButton)
    self.CustomGameButton = CustomGameButton

    local ModsButton = nil

    ModsButton = MenuBuilder.BuildRegisteredType("MenuButton", {
        controllerIndex = controllerIndex
    })
    ModsButton.id = "ModsButton"
    ModsButton.buttonDescription = Engine.Localize("LUA_MENU_MODS_DESC")
    ModsButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MODS_CAPS")), 0)
    ModsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150)
    self:addElement(ModsButton)
    self.ModsButton = ModsButton

    local StoreButton = nil
    if CONDITIONS.IsStoreAllowed(self) then
        StoreButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        StoreButton.id = "StoreButton"
        StoreButton.buttonDescription = Engine.Localize("LUA_MENU_STORE_DESC")
        StoreButton.Text:setText(Engine.Localize("LUA_MENU_STORE_CAPS"), 0)
        StoreButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 160, _1080p * 190)
        self:addElement(StoreButton)
        self.StoreButton = StoreButton
    end

    f0_local3(self, controllerIndex, controller)
    return self
end

MenuBuilder.m_types["MPMainMenuButtons"] = MPMainMenuButtons
