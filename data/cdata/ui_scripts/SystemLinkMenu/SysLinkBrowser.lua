local serverBrowserPath = "frontEnd.systemLinkMenu.serverBrowser"
local serverBrowserOffsets = {10, 240, 360, 450}

local leaveMenuHandler = function(menu, controller)
    -- Engine.StopGameMode()
    LUI.FlowManager.RequestLeaveMenu(menu, true)
end

local updateServerDisplayListAndCreateModels

updateServerDisplayListAndCreateModels = function(menu, controller, index)
    local waitDuration = menu:Wait(250)
    waitDuration.onComplete = function()
        return updateServerDisplayListAndCreateModels(menu, controller)
    end

    Lobby.UpdateServerDisplayList(controller)
    Lobby.CreateServersDataModels(controller)
end

local setupSystemLinkMenu = function(menu, controller, index)
    assert(menu.ServerBrowser)
    assert(menu.ServerBrowser.ServerList)
    menu:setPriority(-1)
    menu.isSignInMenu = true
    local isAliensMode = Engine.IsAliensMode()

    if isAliensMode then
        Engine.SetFrontEndSceneSection("zm_main", 1)
    else
        menu.MenuTitle.MenuBreadcrumbs:setText(ToUpperCase(
            Engine.Localize("LUA_MENU_BREADCRUMB_2_ITEMS", "IW7-Mod", "MULTIPLAYER")), 0)
    end

    menu.addButtonHelperFunction = function(helperMenu, helperController)
        menu:AddButtonHelperText({
            helper_text = Engine.Localize("MENU_SB_TOOLTIP_BTN_REFRESH"),
            button_ref = "button_alt1",
            side = "left",
            priority = 2,
            clickable = true
        })
    end

    menu:addEventHandler("menu_create", menu.addButtonHelperFunction)

    assert(Engine.IsUserSignedIn(controller))
    if isAliensMode then
        Engine.ExecNow(CPConfig.default_systemlink, controller)
    else
        Engine.ExecNow(MPConfig.default_systemlink, controller)
    end

    Engine.Exec(MPConfig.default_dvars)

    menu:addEventHandler("menu_create", function(menuHandler, eventHandler)
        assert(menu.bindButton)

        if CONDITIONS.IsThirdGameMode(menu) then
            ACTIONS.AnimateSequence(menu, "DisplayServerBrowser")
        end

        menu.bindButton:addEventHandler("button_secondary", function(buttonHandler, buttonEvent)
            local scopedData = LUI.FlowManager.GetScopedData(menu)
            if scopedData.serverBrowserFocused then
                scopedData.serverBrowserFocused = false
                ACTIONS.LoseFocus(menu, "ServerBrowser", controller)
                ACTIONS.GainFocus(menu.SystemLinkMenuButtons, "JoinGameButton", controller)
            else
                leaveMenuHandler(buttonHandler, buttonEvent)
            end
        end)

        menu.bindButton:addEventHandler("button_alt1", function(buttonHandler, buttonEvent)
            Lobby.RefreshServerList(controller)
            ACTIONS.PlaySoundSetSound(menu, "selectAlt", false)
        end)
    end)

    menu.SystemLinkMenuButtons:addEventHandler("gain_focus", function(menuHandler, eventHandler)
        local scopedData = LUI.FlowManager.GetScopedData(menu)
        if scopedData.serverBrowserFocused then
            scopedData.serverBrowserFocused = false
            ACTIONS.LoseFocus(menu, "ServerBrowser", controller)
            if CONDITIONS.IsThirdGameMode(menu) then
                ACTIONS.AnimateSequence(menu, "DisplayServerBrowser")
                menu:RemoveButtonHelperText("button_alt1", "left")
            end
        end
    end)

    Engine.PLMRefreshData()
    if Engine.IsCoreMode() then
        ClientWeapon.SetWeaponVisible(0, false)
        ClientCharacter.SetCharacterVisible(0, false)
    end

    Lobby.SetPartyUIRoot(PartyUIRoot.SYSTEM_LINK)
    local scopedData = LUI.FlowManager.GetScopedData(menu)
    if scopedData.optionsMenuOpened then
        scopedData.optionsMenuOpened = nil
        if scopedData.serverBrowserFocused then
            scopedData.serverBrowserFocused = false
            menu:clearSavedState()
            if isAliensMode then
                LUI.FlowManager.ClearSavedMenuState("CPSystemLinkMenu")
            else
                LUI.FlowManager.ClearSavedMenuState("MPSystemLinkMenu")
            end
        end
    else
        scopedData.serverBrowserFocused = false
    end

    Lobby.BuildServerList(controller)
    Lobby.RefreshServerList(controller)

    assert(menu.ServerBrowser)
    assert(menu.ServerBrowser.ServerList)
    local serverCountDataSource = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. ".serverCount")
    menu:SubscribeToModel(serverCountDataSource:GetModel(controller), function()
        local serverCount = serverCountDataSource:GetValue(controller)
        if serverCount ~= nil then
            if serverCount == 0 then
                local scopedData = LUI.FlowManager.GetScopedData(menu)
                if scopedData.serverBrowserFocused then
                    scopedData.serverBrowserFocused = false
                    ACTIONS.LoseFocus(menu, "ServerBrowser", controller)
                end
            end
            menu.ServerBrowser:SetBlurHeight(serverCount)
        end
    end)

    local serverListDataSource = LUI.DataSourceFromList.new(serverCountDataSource)
    serverListDataSource.MakeDataSourceAtIndex = function(dataSource, index, controller)
        local color = index % 2 == 0 and COLORS.white or COLORS.black
        local statusDataSource = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".status")
        return {
            index = index,
            color = color,
            host = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".host"),
            map = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".map"),
            players = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".players"),
            type = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".type"),
            status = statusDataSource:Filter("status", function(value, model)
                if value == 1 then
                    return Engine.Localize("MENU_LOBBY")
                else
                    return Engine.Localize("MENU_IN_GAME_CAPS")
                end
            end)
        }
    end

    menu.ServerBrowser.ServerList:SetGridDataSource(serverListDataSource)

    assert(menu.bindButton)
    menu.bindButton:addEventHandler("button_start", function(buttonHandler, buttonEvent)
        local scopedData = LUI.FlowManager.GetScopedData(menu)
        scopedData.optionsMenuOpened = true
        ACTIONS.OpenMenu("OptionsMenu", true, buttonEvent.controller)
        ACTIONS.PlaySoundSetSound(menu, "selectAlt", false)
    end)

    local waitDuration = menu:Wait(250)
    waitDuration.onComplete = function()
        return updateServerDisplayListAndCreateModels(menu, controller)
    end

    Engine.StopMenuVideo()
end

MenuBuilder.m_types["SystemLinkMenu"] = function(menu, controller)
    local self = LUI.UIElement.new()
    self.id = "SystemLinkMenu"
    self._animationSets = {}
    self._sequences = {}
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)
    self.soundSet = "cpMainMenuDefault"
    self:playSound("menu_open")
    local OuterMaskTop = nil
    if CONDITIONS.IsThirdGameMode(self) then
        OuterMaskTop = MenuBuilder.BuildRegisteredType("CPMenuOuterMask", {
            controllerIndex = controllerIndex
        })
        OuterMaskTop.id = "OuterMaskTop"
        OuterMaskTop:SetZRotation(-168, 0)
        OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320, _1080p * 100)
        self:addElement(OuterMaskTop)
        self.OuterMaskTop = OuterMaskTop
    end
    local OuterMaskBottom = nil
    if CONDITIONS.IsThirdGameMode(self) then
        OuterMaskBottom = MenuBuilder.BuildRegisteredType("CPMenuOuterMask", {
            controllerIndex = controllerIndex
        })
        OuterMaskBottom.id = "OuterMaskBottom"
        OuterMaskBottom:SetZRotation(-168, 0)
        OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205, _1080p * 215)
        self:addElement(OuterMaskBottom)
        self.OuterMaskBottom = OuterMaskBottom
    end
    local MenuTitle = nil
    if CONDITIONS.IsCoreMultiplayer(self) then
        MenuTitle = MenuBuilder.BuildRegisteredType("MenuTitle", {
            controllerIndex = controllerIndex
        })
        MenuTitle.id = "MenuTitle"
        MenuTitle.MenuTitle:setText(ToUpperCase(Engine.Localize("SERVER BROWSER")), 0)
        MenuTitle.MenuBreadcrumbs:setText(ToUpperCase(Engine.Localize("EXE_LOCAL_PLAY")), 0)
        MenuTitle.Icon:SetTop(_1080p * -28.5, 0)
        MenuTitle.Icon:SetBottom(_1080p * 61.5, 0)
        MenuTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134)
        self:addElement(MenuTitle)
        self.MenuTitle = MenuTitle
    end
    local ButtonHelperBar = nil

    ButtonHelperBar = MenuBuilder.BuildRegisteredType("ButtonHelperBar", {
        controllerIndex = controllerIndex
    })
    ButtonHelperBar.id = "ButtonHelperBar"
    ButtonHelperBar:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -85, 0)
    self:addElement(ButtonHelperBar)
    self.ButtonHelperBar = ButtonHelperBar

    local SocialFeed = nil

    SocialFeed = MenuBuilder.BuildRegisteredType("SocialFeed", {
        controllerIndex = controllerIndex
    })
    SocialFeed.id = "SocialFeed"
    SocialFeed:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -115, _1080p * -85)
    self:addElement(SocialFeed)
    self.SocialFeed = SocialFeed

    local SystemLinkMenuButtons = nil

    SystemLinkMenuButtons = MenuBuilder.BuildRegisteredType("SystemLinkMenuButtons", {
        controllerIndex = controllerIndex
    })
    SystemLinkMenuButtons.id = "SystemLinkMenuButtons"
    SystemLinkMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 438)
    self:addElement(SystemLinkMenuButtons)
    self.SystemLinkMenuButtons = SystemLinkMenuButtons

    local LobbyMembers = nil
    if CONDITIONS.IsThirdGameMode(self) then
        f22_local9 = MenuBuilder.BuildRegisteredType("LobbyMembers", {
            controllerIndex = controllerIndex
        })
        LobbyMembers.id = "LobbyMembers"
        LobbyMembers:SetAlpha(0, 0)
        LobbyMembers:SetAnchorsAndPosition(1, 0, 0, 1, _1080p * -1790, _1080p * -1090, _1080p * 791.5, _1080p * 1435.5)
        self:addElement(LobbyMembers)
        self.LobbyMembers = LobbyMembers
    end
    local ServerBrowser = nil

    ServerBrowser = MenuBuilder.BuildRegisteredType("ServerBrowser", {
        controllerIndex = controllerIndex
    })
    ServerBrowser.id = "ServerBrowser"
    ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216, _1080p * 885)
    self:addElement(ServerBrowser)
    self.ServerBrowser = ServerBrowser

    local BloodSplat = nil
    if CONDITIONS.IsThirdGameMode(self) then
        BloodSplat = LUI.UIImage.new()
        BloodSplat.id = "BloodSplat"
        BloodSplat:SetAlpha(0.5, 0)
        BloodSplat:setImage(RegisterMaterial("cp_menu_zombies_blood_splats_large"), 0)
        BloodSplat:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 48, _1080p * 678, _1080p * -71, _1080p * 441)
        self:addElement(BloodSplat)
        self.BloodSplat = BloodSplat
    end
    local ZombiesLogo = nil
    if CONDITIONS.IsThirdGameMode(self) then
        ZombiesLogo = LUI.UIImage.new()
        ZombiesLogo.id = "ZombiesLogo"
        ZombiesLogo:setImage(RegisterMaterial("cp_menu_zombies_logo"), 0)
        ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 60, _1080p * 740, _1080p * 34, _1080p * 434)
        self:addElement(ZombiesLogo)
        self.ZombiesLogo = ZombiesLogo
    end
    local ZomLanPartyShadow = nil
    if CONDITIONS.IsThirdGameMode(self) then
        ZomLanPartyShadow = LUI.UIStyledText.new()
        ZomLanPartyShadow.id = "ZomLanPartyShadow"
        ZomLanPartyShadow:SetRGBFromInt(0, 0)
        ZomLanPartyShadow:setText(Engine.Localize("Server Browser"), 0)
        ZomLanPartyShadow:SetFontSize(36 * _1080p)
        ZomLanPartyShadow:SetFont(FONTS.GetFont(FONTS.ZmClean.File))
        ZomLanPartyShadow:SetAlignment(LUI.Alignment.Center)
        ZomLanPartyShadow:SetOptOutRightToLeftAlignmentFlip(true)
        ZomLanPartyShadow:SetShadowRGBFromInt(0, 0)
        ZomLanPartyShadow:SetOutlineRGBFromInt(0, 0)
        ZomLanPartyShadow:SetDecodeLetterLength(25)
        ZomLanPartyShadow:SetDecodeMaxRandChars(3)
        ZomLanPartyShadow:SetDecodeUpdatesPerLetter(4)
        ZomLanPartyShadow:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 272, _1080p * 597.5, _1080p * 302, _1080p * 338)
        self:addElement(ZomLanPartyShadow)
        self.ZomLanPartyShadow = ZomLanPartyShadow
    end
    local ZomLanParty = nil
    if CONDITIONS.IsThirdGameMode(self) then
        ZomLanParty = LUI.UIStyledText.new()
        ZomLanParty.id = "ZomLanParty"
        ZomLanParty:SetRGBFromInt(14042667, 0)
        ZomLanParty:setText(Engine.Localize("Server Browser"), 0)
        ZomLanParty:SetFontSize(36 * _1080p)
        ZomLanParty:SetFont(FONTS.GetFont(FONTS.ZmClean.File))
        ZomLanParty:SetAlignment(LUI.Alignment.Center)
        ZomLanParty:SetOptOutRightToLeftAlignmentFlip(true)
        ZomLanParty:SetShadowRGBFromInt(0, 0)
        ZomLanParty:SetOutlineRGBFromInt(0, 0)
        ZomLanParty:SetDecodeLetterLength(25)
        ZomLanParty:SetDecodeMaxRandChars(3)
        ZomLanParty:SetDecodeUpdatesPerLetter(4)
        ZomLanParty:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 272, _1080p * 597.5, _1080p * 302, _1080p * 338)
        self:addElement(ZomLanParty)
        self.ZomLanParty = ZomLanParty
    end
    local MPLobbyMembersPlayercards = nil

    MPLobbyMembersPlayercards = MenuBuilder.BuildRegisteredType("MPLobbyMembersPlayercards", {
        controllerIndex = controllerIndex
    })
    MPLobbyMembersPlayercards.id = "MPLobbyMembersPlayercards"
    MPLobbyMembersPlayercards:SetAnchorsAndPosition(0, 1, 1, 0, _1080p * 130, _1080p * 630, _1080p * -255, _1080p * -189)
    self:addElement(MPLobbyMembersPlayercards)
    self.MPLobbyMembersPlayercards = MPLobbyMembersPlayercards

    local ButtonDescriptionText = nil

    ButtonDescriptionText = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = controllerIndex
    })
    ButtonDescriptionText.id = "ButtonDescriptionText"
    ButtonDescriptionText:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 550, _1080p * 334, _1080p * 366)
    self:addElement(ButtonDescriptionText)
    self.ButtonDescriptionText = ButtonDescriptionText

    self._animationSets.DefaultAnimationSet = function()
        ServerBrowser:RegisterAnimationSequence("DefaultSequence", {{function()
            return self.ServerBrowser:SetAlpha(1, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216,
                _1080p * 885, 0)
        end}})
        self._sequences.DefaultSequence = function()
            ServerBrowser:AnimateSequence("DefaultSequence")
        end

        SystemLinkMenuButtons:RegisterAnimationSequence("IntroCore", {{function()
            return self.SystemLinkMenuButtons:SetAlpha(1, 0)
        end}, {function()
            return self.SystemLinkMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630,
                _1080p * 216, _1080p * 438, 0)
        end}})
        ServerBrowser:RegisterAnimationSequence("IntroCore", {{function()
            return self.ServerBrowser:SetAlpha(1, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216,
                _1080p * 885, 0)
        end}})
        self._sequences.IntroCore = function()
            SystemLinkMenuButtons:AnimateSequence("IntroCore")
            ServerBrowser:AnimateSequence("IntroCore")
        end

    end

    self._animationSets.ThirdGameModeAnimationSet = function()
        self._sequences.DefaultSequence = function()

        end

        if CONDITIONS.IsThirdGameMode(self) then
            CPMenuOuterMask:RegisterAnimationSequence("Intro", {{function()
                return self.OuterMaskTop:SetAlpha(0, 0)
            end, function()
                return self.OuterMaskTop:SetAlpha(0.6, 500, LUI.EASING.outBack)
            end}, {function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -635,
                    _1080p * -215, 0)
            end, function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -635,
                    _1080p * -215, 500, LUI.EASING.outBack)
            end, function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320,
                    _1080p * 100, 500, LUI.EASING.outQuadratic)
            end, function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320,
                    _1080p * 100, 139)
            end, function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -280,
                    _1080p * 140, 9)
            end, function()
                return self.OuterMaskTop:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320,
                    _1080p * 100, 200, LUI.EASING.outQuadratic)
            end}})
        end
        if CONDITIONS.IsThirdGameMode(self) then
            f22_local4:RegisterAnimationSequence("Intro", {{function()
                return self.OuterMaskBottom:SetAlpha(0, 0)
            end, function()
                return self.OuterMaskBottom:SetAlpha(0.6, 500, LUI.EASING.outBack)
            end}, {function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * 225,
                    _1080p * 645, 0)
            end, function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * 225,
                    _1080p * 645, 500, LUI.EASING.outBack)
            end, function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205,
                    _1080p * 215, 500, LUI.EASING.outQuadratic)
            end, function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205,
                    _1080p * 215, 139)
            end, function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -245,
                    _1080p * 175, 9)
            end, function()
                return self.OuterMaskBottom:SetAnchorsAndPosition(0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205,
                    _1080p * 215, 200, LUI.EASING.outQuadratic)
            end}})
        end
        SystemLinkMenuButtons:RegisterAnimationSequence("Intro", {{function()
            return self.SystemLinkMenuButtons:SetAlpha(0, 0)
        end, function()
            return self.SystemLinkMenuButtons:SetAlpha(0, 1220)
        end, function()
            return self.SystemLinkMenuButtons:SetAlpha(1, 490)
        end}, {function()
            return self.SystemLinkMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630,
                _1080p * 406, _1080p * 628, 0)
        end}})
        if CONDITIONS.IsThirdGameMode(self) then
            f22_local9:RegisterAnimationSequence("Intro", {{function()
                return self.LobbyMembers:SetAlpha(0, 0)
            end, function()
                return self.LobbyMembers:SetAlpha(0, 2000)
            end}})
        end
        ServerBrowser:RegisterAnimationSequence("Intro", {{function()
            return self.ServerBrowser:SetAlpha(0, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 789, _1080p * 1920, _1080p * 133,
                _1080p * 802, 0)
        end}})
        if CONDITIONS.IsThirdGameMode(self) then
            controllerIndex1:RegisterAnimationSequence("Intro", {{function()
                return self.BloodSplat:SetAlpha(0, 0)
            end, function()
                return self.BloodSplat:SetAlpha(0, 1000)
            end, function()
                return self.BloodSplat:SetAlpha(0, 140)
            end, function()
                return self.BloodSplat:SetAlpha(0.5, 10)
            end, function()
                return self.BloodSplat:SetAlpha(0.5, 200, LUI.EASING.outQuadratic)
            end, function()
                return self.BloodSplat:SetAlpha(0.4, 650, LUI.EASING.outQuadratic)
            end}, {function()
                return self.BloodSplat:SetZRotation(-5, 1000)
            end, function()
                return self.BloodSplat:SetZRotation(-5, 150)
            end, function()
                return self.BloodSplat:SetZRotation(0, 850, LUI.EASING.outQuadratic)
            end}, {function()
                return self.BloodSplat:playSound("splat", false, 1150)
            end}, {function()
                return self.BloodSplat:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 218.92, _1080p * 547.08,
                    _1080p * 51.65, _1080p * 318.35, 1149)
            end, function()
                return self.BloodSplat:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 88, _1080p * 718, _1080p * -71,
                    _1080p * 441, 70)
            end, function()
                return self.BloodSplat:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 80, _1080p * 710, _1080p * -71,
                    _1080p * 441, 129, LUI.EASING.outQuadratic)
            end}})
        end
        if CONDITIONS.IsThirdGameMode(self) then
            controllerIndex2:RegisterAnimationSequence("Intro", {{function()
                return self.ZombiesLogo:SetAlpha(0, 0)
            end, function()
                return self.ZombiesLogo:SetAlpha(0, 1000)
            end, function()
                return self.ZombiesLogo:SetAlpha(1, 150)
            end}, {function()
                return self.ZombiesLogo:SetZRotation(5, 1000)
            end, function()
                return self.ZombiesLogo:SetZRotation(5, 150)
            end, function()
                return self.ZombiesLogo:SetZRotation(0, 850, LUI.EASING.outQuadratic)
            end}, {function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * -429.8, _1080p * 1249.8,
                    _1080p * -260, _1080p * 728, 1000)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 80, _1080p * 720, _1080p * 51.65,
                    _1080p * 416.35, 149)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 34,
                    _1080p * 434, 200)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 36,
                    _1080p * 436, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 68, _1080p * 748, _1080p * 35.68,
                    _1080p * 435.68, 50)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70.35, _1080p * 750.35,
                    _1080p * 33.39, _1080p * 433.39, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70.28, _1080p * 750.28,
                    _1080p * 31.51, _1080p * 431.51, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 68.22, _1080p * 748.22,
                    _1080p * 34.03, _1080p * 434.03, 50)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70.64, _1080p * 750.64,
                    _1080p * 34.02, _1080p * 434.02, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70.47, _1080p * 750.47,
                    _1080p * 36.01, _1080p * 436.01, 50)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 68.33, _1080p * 748.33,
                    _1080p * 35.4, _1080p * 435.4, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 68.93, _1080p * 748.93,
                    _1080p * 32.9, _1080p * 432.9, 59)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 69.4, _1080p * 749.4, _1080p * 35.38,
                    _1080p * 435.38, 40)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 69.73, _1080p * 749.73,
                    _1080p * 36.61, _1080p * 436.61, 49)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 71.93, _1080p * 751.93,
                    _1080p * 34.65, _1080p * 434.65, 50)
            end, function()
                return self.ZombiesLogo:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 34,
                    _1080p * 434, 49, LUI.EASING.outQuadratic)
            end}})
        end
        if CONDITIONS.IsThirdGameMode(self) then
            controllerIndex3:RegisterAnimationSequence("Intro", {{function()
                return self.ZomLanPartyShadow:SetAlpha(0, 0)
            end, function()
                return self.ZomLanPartyShadow:SetAlpha(0, 1710)
            end, function()
                return self.ZomLanPartyShadow:SetAlpha(1, 290)
            end}})
        end
        if CONDITIONS.IsThirdGameMode(self) then
            controllerIndex4:RegisterAnimationSequence("Intro", {{function()
                return self.ZomLanParty:SetAlpha(0, 0)
            end, function()
                return self.ZomLanParty:SetAlpha(0, 1220)
            end, function()
                return self.ZomLanParty:SetAlpha(1, 490)
            end}, {function()
                return self.ZomLanParty:SetRGBFromInt(14042667, 0)
            end}})
        end
        MPLobbyMembersPlayercards:RegisterAnimationSequence("Intro", {{function()
            return self.MPLobbyMembersPlayercards:SetAlpha(0, 0)
        end, function()
            return self.MPLobbyMembersPlayercards:SetAlpha(0, 700)
        end, function()
            return self.MPLobbyMembersPlayercards:SetAlpha(0, 1010)
        end, function()
            return self.MPLobbyMembersPlayercards:SetAlpha(1, 290)
        end}})
        ButtonDescriptionText:RegisterAnimationSequence("Intro", {{function()
            return self.ButtonDescriptionText:SetAlpha(0, 0)
        end, function()
            return self.ButtonDescriptionText:SetAlpha(0, 1220)
        end, function()
            return self.ButtonDescriptionText:SetAlpha(1, 490)
        end}, {function()
            return self.ButtonDescriptionText:SetRGBFromTable(SWATCHES.text.primaryText, 0)
        end}, {function()
            return self.ButtonDescriptionText:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 132, _1080p * 552,
                _1080p * 524, _1080p * 556, 0)
        end}})
        self._sequences.Intro = function()
            if CONDITIONS.IsThirdGameMode(self) then
                CPMenuOuterMask:AnimateSequence("Intro")
            end
            if CONDITIONS.IsThirdGameMode(self) then
                f22_local4:AnimateSequence("Intro")
            end
            SystemLinkMenuButtons:AnimateSequence("Intro")
            if CONDITIONS.IsThirdGameMode(self) then
                f22_local9:AnimateSequence("Intro")
            end
            ServerBrowser:AnimateSequence("Intro")
            if CONDITIONS.IsThirdGameMode(self) then
                controllerIndex1:AnimateSequence("Intro")
            end
            if CONDITIONS.IsThirdGameMode(self) then
                controllerIndex2:AnimateSequence("Intro")
            end
            if CONDITIONS.IsThirdGameMode(self) then
                controllerIndex3:AnimateSequence("Intro")
            end
            if CONDITIONS.IsThirdGameMode(self) then
                controllerIndex4:AnimateSequence("Intro")
            end
            MPLobbyMembersPlayercards:AnimateSequence("Intro")
            ButtonDescriptionText:AnimateSequence("Intro")
        end

        ServerBrowser:RegisterAnimationSequence("DisplayServerBrowser", {{function()
            return self.ServerBrowser:SetAlpha(0, 0)
        end, function()
            return self.ServerBrowser:SetAlpha(1, 80)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 727, _1080p * 1858, _1080p * 216,
                _1080p * 885, 0)
        end}})
        self._sequences.DisplayServerBrowser = function()
            ServerBrowser:AnimateSequence("DisplayServerBrowser")
        end

        ServerBrowser:RegisterAnimationSequence("HideServerBrowser", {{function()
            return self.ServerBrowser:SetAlpha(1, 0)
        end, function()
            return self.ServerBrowser:SetAlpha(0, 80)
        end}})
        self._sequences.HideServerBrowser = function()
            ServerBrowser:AnimateSequence("HideServerBrowser")
        end

    end

    self._animationSets.DefaultAnimationSet()
    self.addButtonHelperFunction = function(arg0, arg1)
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("LUA_MENU_SELECT"),
            button_ref = "button_primary",
            side = "left",
            clickable = true
        })
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("LUA_MENU_BACK"),
            button_ref = "button_secondary",
            side = "left",
            priority = 1,
            clickable = true
        })
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("LUA_MENU_OPTIONS_CAPS"),
            button_ref = "button_start",
            side = "left",
            priority = 4,
            clickable = true
        })
    end

    self:addEventHandler("menu_create", self.addButtonHelperFunction)

    local bindButton = LUI.UIBindButton.new()
    bindButton.id = "selfBindButton"
    self:addElement(bindButton)
    self.bindButton = bindButton

    setupSystemLinkMenu(self, controllerIndex, controller)
    ACTIONS.AnimateSequence(self, "DefaultSequence")
    if CONDITIONS.IsThirdGameMode(self) then
        ACTIONS.SetAnimationSet(self, "ThirdGameModeAnimationSet")
        ACTIONS.AnimateSequence(self, "Intro")
    end
    if CONDITIONS.IsCoreMultiplayer(self) then
        ACTIONS.SetAnimationSet(self, "DefaultAnimationSet")
        ACTIONS.AnimateSequence(self, "IntroCore")
    end

    return self
end

local PopFunc = function()
    clearMatchData()
    WipeGlobalModelsAtPath(f0_local0)
end

LUI.FlowManager.RegisterStackPushBehaviour("SystemLinkMenu", PushFunc)
LUI.FlowManager.RegisterStackPopBehaviour("SystemLinkMenu", PopFunc)
