local serverBrowserPath = "frontEnd.systemLinkMenu.serverBrowser"

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

local function setupSystemLinkMenu(menu, controller, index)
    assert(menu.ServerBrowser)
    assert(menu.ServerBrowser.ServerList)
    menu:setPriority(-1)
    menu.isSignInMenu = true
    local isAliensMode = Engine.IsAliensMode()

    if isAliensMode then
        Engine.SetFrontEndSceneSection("zm_main", 1)
        menu.MenuTitle.MenuBreadcrumbs:setText(ToUpperCase(
            Engine.Localize("LUA_MENU_BREADCRUMB_2_ITEMS", "IW7-Mod", "ZOMBIES")), 0)
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

    menu:addEventHandler("menu_create", function(menuHandler, a2)
        assert(menu.bindButton)

        if CONDITIONS.IsThirdGameMode(menu) then
            ACTIONS.AnimateSequence(menu, "DisplayServerBrowser")
        end

        menu.bindButton:addEventHandler("button_secondary", function(buttonHandler, buttonEvent)
            leaveMenuHandler(buttonHandler, buttonEvent)
        end)

        menu.bindButton:addEventHandler("button_alt1", function(buttonHandler, buttonEvent)
            Lobby.RefreshServerList(controller)
            ACTIONS.PlaySoundSetSound(menu, "selectAlt", false)
        end)
    end)

    Engine.PLMRefreshData()
    if Engine.IsCoreMode() then
        ClientWeapon.SetWeaponVisible(0, false)
        ClientCharacter.SetCharacterVisible(0, false)
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
            else
                ACTIONS.GainFocus(menu, "ServerBrowser", controller)
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
            mod = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".mod"),
            ping = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".ping"),
            priv = LUI.DataSourceInGlobalModel.new(serverBrowserPath .. "." .. index .. ".priv"),
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

    local MenuTitle = nil
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

    local ServerBrowser = nil

    ServerBrowser = MenuBuilder.BuildRegisteredType("ServerBrowser", {
        controllerIndex = controllerIndex
    })
    ServerBrowser.id = "ServerBrowser"
    ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 1790, _1080p * 0, _1080p * 0)
    self:addElement(ServerBrowser)
    self.ServerBrowser = ServerBrowser

    self._animationSets.DefaultAnimationSet = function()
        ServerBrowser:RegisterAnimationSequence("DefaultSequence", {{function()
            return self.ServerBrowser:SetAlpha(1, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 1790, _1080p * 216, _1080p * 885, 0)
        end}})
        self._sequences.DefaultSequence = function()
            ServerBrowser:AnimateSequence("DefaultSequence")
        end

        ServerBrowser:RegisterAnimationSequence("IntroCore", {{function()
            return self.ServerBrowser:SetAlpha(1, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 1790, _1080p * 216, _1080p * 885, 0)
        end}})
        self._sequences.IntroCore = function()
            ServerBrowser:AnimateSequence("IntroCore")
        end
    end

    self._animationSets.ThirdGameModeAnimationSet = function()
        self._sequences.DefaultSequence = function()
        end
        
        ServerBrowser:RegisterAnimationSequence("Intro", {{function()
            return self.ServerBrowser:SetAlpha(0, 0)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 1790, _1080p * 216, _1080p * 885, 0)
        end}})

        self._sequences.Intro = function()
            ServerBrowser:AnimateSequence("Intro")
        end

        ServerBrowser:RegisterAnimationSequence("DisplayServerBrowser", {{function()
            return self.ServerBrowser:SetAlpha(0, 0)
        end, function()
            return self.ServerBrowser:SetAlpha(1, 80)
        end}, {function()
            return self.ServerBrowser:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 1790, _1080p * 216, _1080p * 885, 0)
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
    WipeGlobalModelsAtPath(serverBrowserPath)
end

LUI.FlowManager.RegisterStackPushBehaviour("SystemLinkMenu", PushFunc)
LUI.FlowManager.RegisterStackPopBehaviour("SystemLinkMenu", PopFunc)
