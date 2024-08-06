function SystemLinkMenuButtons(menu, controller)
    local SystemLinkMenuButtons = LUI.UIVerticalList.new()
    SystemLinkMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 300 * _1080p)
    SystemLinkMenuButtons.id = "SystemLinkMenuButtons"
    
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = SystemLinkMenuButtons:getRootController()
    end
    assert(controllerIndex)
    
    SystemLinkMenuButtons:SetSpacing(10 * _1080p)

    local casButton = nil
    if CONDITIONS.IsCoreMultiplayer(SystemLinkMenuButtons) then
        casButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        casButton.id = "CASButton"
        
        if CONDITIONS.IsCoreMultiplayer(SystemLinkMenuButtons) then
            casButton.buttonDescription = Engine.Localize("LUA_MENU_DESC_CREATE_A_CLASS")
        end
        
        casButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_CREATE_A_CLASS")), 0)
        casButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110)
        SystemLinkMenuButtons:addElement(casButton)
        SystemLinkMenuButtons.CASButton = casButton
    end
    
    local LoadoutButton = nil
    if CONDITIONS.IsThirdGameMode(SystemLinkMenuButtons) then
        LoadoutButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        LoadoutButton.id = "LoadoutButton"
        
        if CONDITIONS.IsThirdGameMode(SystemLinkMenuButtons) then
            LoadoutButton.buttonDescription = Engine.Localize("LUA_MENU_ZM_LOADOUT_DESC")
        end
        
        LoadoutButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_ZM_LOADOUT_CAPS")), 0)
        LoadoutButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150)
        SystemLinkMenuButtons:addElement(LoadoutButton)
        SystemLinkMenuButtons.LoadoutButton = LoadoutButton
    end

    if CONDITIONS.IsCoreMultiplayer(SystemLinkMenuButtons) then
        casButton:addEventHandler("button_action", function(button, event)
            local currentControllerIndex = event.controller or controllerIndex
            ClientWeapon.SetWeaponVisible(0, true)
            ClientCharacter.SetCharacterVisible(0, true)
            ACTIONS.OpenCreateAClass(SystemLinkMenuButtons, event)
        end)
    end
    
    if CONDITIONS.IsThirdGameMode(SystemLinkMenuButtons) then
        loadoutButton:addEventHandler("button_action", function(button, event)
            ACTIONS.OpenMenu("CPLoadoutMenu", true, event.controller or controllerIndex)
        end)
    end

    return SystemLinkMenuButtons
end

MenuBuilder.m_types["SystemLinkMenuButtons"] = SystemLinkMenuButtons