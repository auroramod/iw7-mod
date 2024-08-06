function SystemLinkMenuButtons(menu, controller)
    local SystemLinkMenuButtons = LUI.UIVerticalList.new()
    SystemLinkMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, 0, 450 * _1080p, 0, 300 * _1080p)
    SystemLinkMenuButtons.id = "SystemLinkMenuButtons"
    
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = SystemLinkMenuButtons:getRootController()
    end
    assert(controllerIndex)
    
    SystemLinkMenuButtons:SetSpacing(10 * _1080p)

    local CASButton = nil
    if CONDITIONS.IsCoreMultiplayer(SystemLinkMenuButtons) then
        CASButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        CASButton.id = "CASButton"
        CASButton.buttonDescription = Engine.Localize("LUA_MENU_DESC_CREATE_A_CLASS")
        CASButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_CREATE_A_CLASS")), 0)
        CASButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 450, _1080p * 80, _1080p * 110)
        SystemLinkMenuButtons:addElement(CASButton)
        SystemLinkMenuButtons.CASButton = CASButton
    end
    
    local LoadoutButton = nil
    if CONDITIONS.IsThirdGameMode(SystemLinkMenuButtons) then
        LoadoutButton = MenuBuilder.BuildRegisteredType("MenuButton", {
            controllerIndex = controllerIndex
        })
        LoadoutButton.id = "LoadoutButton"
        LoadoutButton.buttonDescription = Engine.Localize("LUA_MENU_ZM_LOADOUT_DESC")
        LoadoutButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_ZM_LOADOUT_CAPS")), 0)
        LoadoutButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 450, _1080p * 80, _1080p * 110)
        SystemLinkMenuButtons:addElement(LoadoutButton)
        SystemLinkMenuButtons.LoadoutButton = LoadoutButton
    end

    if CONDITIONS.IsCoreMultiplayer(SystemLinkMenuButtons) then
        CASButton:addEventHandler("button_action", function(button, event)
            ClientWeapon.SetWeaponVisible(0, true)
            ClientCharacter.SetCharacterVisible(0, true)
            ACTIONS.OpenCreateAClass(SystemLinkMenuButtons, event)
        end)
    end
    
    if CONDITIONS.IsThirdGameMode(SystemLinkMenuButtons) then
        LoadoutButton:addEventHandler("button_action", function(button, event)
            ACTIONS.OpenMenu("CPLoadoutMenu", true, event.controller or controllerIndex)
        end)
    end

    return SystemLinkMenuButtons
end

MenuBuilder.m_types["SystemLinkMenuButtons"] = SystemLinkMenuButtons