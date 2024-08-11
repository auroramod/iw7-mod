local modSelectPath = "frontEnd.ModSelect"

modSelectPathCleanup = function()
    WipeGlobalModelsAtPath(modSelectPath)
end

local function getModName(path)
    local modName = path
    local modDesc = Engine.Localize("LUA_MENU_MOD_DESC_DEFAULT", modName)

    return modName, modDesc
end

local function setMod(modName)
    Engine.SetDvarString("fs_game", modName)
    Engine.Exec("vid_restart")
end

local unloadMod = function(buttonElement, controllerIndex)
    setMod("")
end

local leaveMenu = function(menuElement, controllerIndex)
    LUI.FlowManager.RequestLeaveMenu(menuElement)
end

local updateModInfo = function(menuElement, controllerIndex)
    local scopedData = LUI.FlowManager.GetScopedData(menuElement)
    if not scopedData.currentLabel then
        scopedData.currentLabel = ""
    end
    if not scopedData.currentDesc then
        scopedData.currentDesc = ""
    end
    menuElement:processEvent({
        name = "menu_refresh"
    })
    local currentMenu = menuElement:GetCurrentMenu()
    assert(currentMenu.ModInfoTitle)
    currentMenu.ModInfoTitle:setText(scopedData.currentLabel)
    assert(currentMenu.ModInfoText)
    currentMenu.ModInfoText:setText(scopedData.currentDesc)
end

local applyMod = function(buttonElement, controllerIndex, modName)
    setMod(modName)
end

local updateModSelection = function(menuElement, controllerIndex, modData)
    local scopedData = LUI.FlowManager.GetScopedData(menuElement)
    scopedData.currentLabel = modData.buttonLabel
    scopedData.currentName = modData.modName
    scopedData.currentDesc = modData.objectiveText
    updateModInfo(menuElement, controllerIndex)
    Engine.PlaySound(CoD.SFX.SPMinimap)
end

local populateModList = function(menuElement, controllerIndex)
    local modList = {}
    if io.directoryexists("mods") then
        local mods = io.listfiles("mods/")
        for i = 1, #mods do
            local name, desc = getModName(mods[i])
            modList[#modList + 1] = {
                buttonLabel = ToUpperCase(name),
                modName = name,
                objectiveText = desc
            }
        end
    end
    local dataSource = LUI.DataSourceFromList.new(#modList)
    dataSource.MakeDataSourceAtIndex = function(dataSource, index, controllerIndex)
        return {
            buttonLabel = LUI.DataSourceInGlobalModel.new(modSelectPath .. ".mods." .. index,
                modList[index + 1].buttonLabel),
            buttonOnClickFunction = function(buttonElement, controllerIndex)
                applyMod(buttonElement, controllerIndex, modList[index + 1].modName)
            end,
            buttonOnHoverFunction = function(buttonElement, controllerIndex)
                updateModSelection(buttonElement, controllerIndex, modList[index + 1])
            end,
            modName = modList[index + 1].modName
        }
    end

    assert(menuElement.ModSelectionList)
    menuElement.ModSelectionList:SetGridDataSource(dataSource, controllerIndex)
end

local function postLoadFunction(menuElement, controllerIndex, controller)
    assert(menuElement.bindButton)
    menuElement.bindButton:addEventHandler("button_secondary", leaveMenu)

    local fsGame = Engine.GetDvarString("fs_game")
    if fsGame ~= "" then
        menuElement.LoadedModName:setText("^3Loaded mod^7: " .. fsGame)
        menuElement.bindButton:addEventHandler("button_alt1", unloadMod)
    else
        menuElement.LoadedModName:setText("")
    end

    -- menuElement:addEventHandler("menu_create", f0_local3)
    populateModList(menuElement, controllerIndex)
    menuElement:addEventHandler("gain_focus", function(focusElement, controllerIndex)
        local modSelectionList = focusElement.ModSelectionList

        if modSelectionList:getNumChildren() == 0 then
            return
        end

        local contentOffset = modSelectionList:GetContentOffset(LUI.DIRECTION.vertical)
        modSelectionList:SetFocusedPosition({
            x = 0,
            y = contentOffset
        }, true)
        local focusedElement = modSelectionList:GetElementAtPosition(0, contentOffset)
        if focusedElement then
            focusedElement:processEvent({
                name = "gain_focus",
                controllerIndex = controllerIndex
            })
        end
    end)
end

function ModSelectMenu(arg0, controller)
    local menuElement = LUI.UIElement.new()
    menuElement.id = "ModSelectMenu"

    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = menuElement:getRootController()
    end
    assert(controllerIndex)

    menuElement:playSound("menu_open")

    if Engine.IsSingleplayer() then
        local backgroundImage = nil

        backgroundImage = LUI.UIImage.new()
        backgroundImage.id = "Background"
        backgroundImage:setImage(RegisterMaterial("sp_frontend_bink_background"), 0)
        menuElement:addElement(backgroundImage)
        menuElement.Background = backgroundImage

        local binkImage = nil

        binkImage = LUI.UIImage.new()
        binkImage.id = "Bink"
        binkImage:setImage(RegisterMaterial("cinematic"), 0)
        menuElement:addElement(binkImage)
        menuElement.Bink = binkImage
    end

    local buttonHelperBar = nil

    buttonHelperBar = MenuBuilder.BuildRegisteredType("ButtonHelperBar", {
        controllerIndex = controllerIndex
    })
    buttonHelperBar.id = "ButtonHelperBar"
    buttonHelperBar:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -85, 0)
    menuElement:addElement(buttonHelperBar)
    menuElement.ButtonHelperBar = buttonHelperBar

    local menuTitle = MenuBuilder.BuildRegisteredType("MenuTitle", {
        controllerIndex = controllerIndex
    })
    menuTitle.id = "MenuTitle"
    menuTitle.MenuTitle:setText(ToUpperCase(Engine.Localize("LUA_MENU_MODS")), 0)
    menuTitle.MenuBreadcrumbs:setText(ToUpperCase(""), 0)
    menuTitle.Icon:SetTop(_1080p * -28.5, 0)
    menuTitle.Icon:SetBottom(_1080p * 61.5, 0)
    menuTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134)
    menuElement:addElement(menuTitle)
    menuElement.MenuTitle = menuTitle

    local modInfoTitle = nil

    modInfoTitle = LUI.UIStyledText.new()
    modInfoTitle.id = "ModInfoTitle"
    modInfoTitle:setText("", 0)
    modInfoTitle:SetFontSize(30 * _1080p)
    modInfoTitle:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    modInfoTitle:SetAlignment(LUI.Alignment.Left)
    modInfoTitle:SetStartupDelay(2000)
    modInfoTitle:SetLineHoldTime(400)
    modInfoTitle:SetAnimMoveTime(300)
    modInfoTitle:SetEndDelay(1500)
    modInfoTitle:SetCrossfadeTime(750)
    modInfoTitle:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    modInfoTitle:SetMaxVisibleLines(1)
    modInfoTitle:SetDecodeLetterLength(15)
    modInfoTitle:SetDecodeMaxRandChars(6)
    modInfoTitle:SetDecodeUpdatesPerLetter(4)
    modInfoTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 1254, _1080p * 1824, _1080p * 216, _1080p * 246)
    menuElement:addElement(modInfoTitle)
    menuElement.ModInfoTitle = modInfoTitle

    local modInfoText = nil

    modInfoText = LUI.UIStyledText.new()
    modInfoText.id = "ModInfoText"
    modInfoText:setText("", 0)
    modInfoText:SetFontSize(20 * _1080p)
    modInfoText:SetFont(FONTS.GetFont(FONTS.MainCondensed.File))
    modInfoText:SetAlignment(LUI.Alignment.Left)
    modInfoText:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 1254, _1080p * 1824, _1080p * 248, _1080p * 268)
    menuElement:addElement(modInfoText)
    menuElement.ModInfoText = modInfoText

    local modSelectionList = nil

    modSelectionList = LUI.UIDataSourceGrid.new(nil, {
        maxVisibleColumns = 1,
        maxVisibleRows = 17,
        controllerIndex = controllerIndex,
        buildChild = function()
            return MenuBuilder.BuildRegisteredType("ModSelectButton", {
                controllerIndex = controllerIndex
            })
        end,
        wrapX = true,
        wrapY = true,
        spacingX = _1080p * 10,
        spacingY = _1080p * 10,
        columnWidth = _1080p * 500,
        rowHeight = _1080p * 30,
        scrollingThresholdX = 1,
        scrollingThresholdY = 1,
        adjustSizeToContent = false,
        horizontalAlignment = LUI.Alignment.Left,
        verticalAlignment = LUI.Alignment.Top,
        springCoefficient = 600,
        maxVelocity = 5000
    })
    modSelectionList.id = "ModSelectionList"
    modSelectionList:setUseStencil(false)
    modSelectionList:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 886)
    menuElement:addElement(modSelectionList)
    menuElement.ModSelectionList = modSelectionList

    local arrowUp = nil

    arrowUp = MenuBuilder.BuildRegisteredType("ArrowUp", {
        controllerIndex = controllerIndex
    })
    arrowUp.id = "ArrowUp"
    arrowUp:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 452.5, _1080p * 472.5, _1080p * 887, _1080p * 927)
    menuElement:addElement(arrowUp)
    menuElement.ArrowUp = arrowUp

    local arrowDown = nil

    arrowDown = MenuBuilder.BuildRegisteredType("ArrowDown", {
        controllerIndex = controllerIndex
    })
    arrowDown.id = "ArrowDown"
    arrowDown:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 287.5, _1080p * 307.5, _1080p * 886, _1080p * 926)
    menuElement:addElement(arrowDown)
    menuElement.ArrowDown = arrowDown

    local listCount = nil

    listCount = LUI.UIText.new()
    listCount.id = "ListCount"
    listCount:setText("1/15", 0)
    listCount:SetFontSize(24 * _1080p)
    listCount:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    listCount:SetAlignment(LUI.Alignment.Center)
    listCount:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 307.5, _1080p * 452.5, _1080p * 894, _1080p * 918)
    menuElement:addElement(listCount)
    menuElement.ListCount = listCount

    local loadedModName = nil

    loadedModName = LUI.UIText.new()
    loadedModName.id = "LoadedModName"
    loadedModName:setText("LOADED MOD NAME", 0)
    loadedModName:SetFontSize(20 * _1080p)
    loadedModName:SetFont(FONTS.GetFont(FONTS.MainBold.File))
    loadedModName:SetAlignment(LUI.Alignment.Left)
    loadedModName:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 942, _1080p * 966)
    menuElement:addElement(loadedModName)
    menuElement.LoadedModName = loadedModName

    modSelectionList:AddArrow(arrowUp)
    modSelectionList:AddArrow(arrowDown)
    modSelectionList:AddItemNumbers(listCount)

    menuElement.addButtonHelperFunction = function(arg0, arg1)
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("MENU_BACK"),
            button_ref = "button_secondary",
            side = "left",
            clickable = true
        })
        local fsGame = Engine.GetDvarString("fs_game")
        if fsGame ~= "" then
            arg0:AddButtonHelperText({
                helper_text = Engine.Localize("LUA_MENU_UNLOAD"),
                button_ref = "button_alt1",
                side = "left",
                clickable = true
            })
        end
    end

    menuElement:addEventHandler("menu_create", menuElement.addButtonHelperFunction)

    local bindButton = LUI.UIBindButton.new()
    bindButton.id = "selfBindButton"
    menuElement:addElement(bindButton)
    menuElement.bindButton = bindButton

    postLoadFunction(menuElement, controllerIndex, controller)

    return menuElement
end

MenuBuilder.registerType("ModSelectMenu", ModSelectMenu)
LUI.FlowManager.RegisterStackPushBehaviour("ModSelectMenu", PushFunc)
LUI.FlowManager.RegisterStackPopBehaviour("ModSelectMenu", modSelectPathCleanup)