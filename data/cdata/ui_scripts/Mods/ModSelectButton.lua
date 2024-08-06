local function PostLoadFunc(buttonElement, controllerIndex, controller)
    assert(buttonElement.GenericButton)

    buttonElement.GenericButton:addEventHandler("button_action", function(clickedElement, eventArgs)
        local dataSource = buttonElement:GetDataSource()
        dataSource.buttonOnClickFunction(clickedElement, eventArgs)
    end)
    
    buttonElement.GenericButton:addEventHandler("button_over", function(hoveredElement, eventArgs)
        local dataSource = buttonElement:GetDataSource()
        dataSource.buttonOnHoverFunction(hoveredElement, eventArgs)
        dataSource = buttonElement:GetDataSource()
        dataSource = dataSource.levelName
    end)
    
    buttonElement.GenericButton:addEventHandler("button_up", function(unhoveredElement, eventArgs)
        local dataSource = buttonElement:GetDataSource()
        dataSource.buttonOnHoverFunction(unhoveredElement, eventArgs)
        dataSource = buttonElement:GetDataSource()
        dataSource = dataSource.levelName
    end)
    
    buttonElement:registerEventHandler("grid_anim", function(element, event)
        element:SetAlpha(event.value)
    end)
    
    buttonElement:SubscribeToDataSourceThroughElement(buttonElement, nil, function()
        local dataSource = buttonElement:GetDataSource()
        dataSource = dataSource.levelName
    end)
end

function ModSelectButton(menu, controller)
    local modSelectButton = LUI.UIButton.new()
    modSelectButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 30 * _1080p)
    modSelectButton.id = "ModSelectButton"
    modSelectButton._animationSets = {}
    modSelectButton._sequences = {}
    
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = modSelectButton:getRootController()
    end
    assert(controllerIndex)
    
    local buttonContainer = modSelectButton
    local genericButton = nil

    genericButton = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = controllerIndex
    })
    genericButton.id = "GenericButton"
    genericButton:SetAlpha(0, 0)
    genericButton:SetAnchorsAndPosition(0, 1, 0, 0, 0, _1080p * 500, 0, 0)
    
    genericButton:SubscribeToModelThroughElement(modSelectButton, "buttonLabel", function()
        local dataSource = modSelectButton:GetDataSource()
        local buttonLabel = dataSource.buttonLabel:GetValue(controllerIndex)
        if buttonLabel ~= nil then
            genericButton.Text:setText(LocalizeString(ToUpperCase(buttonLabel)), 0)
        end
    end)
    
    modSelectButton:addElement(genericButton)
    modSelectButton.GenericButton = genericButton

    local listButtonBackground = nil

    listButtonBackground = MenuBuilder.BuildRegisteredType("GenericListArrowButtonBackground", {
        controllerIndex = controllerIndex
    })
    listButtonBackground.id = "GenericListButtonBackground"
    listButtonBackground:SetAnchorsAndPosition(0, 0, 0, 0, 0, 0, 0, 0)
    
    modSelectButton:addElement(listButtonBackground)
    modSelectButton.GenericListButtonBackground = listButtonBackground

    local buttonText = nil

    buttonText = LUI.UIStyledText.new()
    buttonText.id = "Text"
    buttonText:SetRGBFromInt(14277081, 0)
    buttonText:SetFontSize(22 * _1080p)
    buttonText:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    buttonText:SetAlignment(LUI.Alignment.Left)
    buttonText:SetStartupDelay(2000)
    buttonText:SetLineHoldTime(400)
    buttonText:SetAnimMoveTime(300)
    buttonText:SetEndDelay(1500)
    buttonText:SetCrossfadeTime(750)
    buttonText:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    buttonText:SetMaxVisibleLines(1)
    buttonText:SetOutlineRGBFromInt(0, 0)
    buttonText:SetAnchorsAndPosition(0, 0, 0.5, 0.5, _1080p * 44, _1080p * -41, _1080p * -11, _1080p * 11)
    
    buttonText:SubscribeToModelThroughElement(modSelectButton, "buttonLabel", function()
        local dataSource = modSelectButton:GetDataSource()
        local buttonLabel = dataSource.buttonLabel:GetValue(controllerIndex)
        if buttonLabel ~= nil then
            buttonText:setText(LocalizeString(ToUpperCase(buttonLabel)), 0)
        end
    end)
    
    modSelectButton:addElement(buttonText)
    modSelectButton.Text = buttonText

    local lockIcon = nil

    lockIcon = LUI.UIImage.new()
    lockIcon.id = "Lock"
    lockIcon:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
    lockIcon:SetAlpha(0, 0)
    lockIcon:setImage(RegisterMaterial("icon_slot_locked"), 0)
    lockIcon:SetAnchorsAndPosition(1, 0, 0.5, 0.5, _1080p * -32, _1080p * -6, _1080p * -12, _1080p * 14)
    
    modSelectButton:addElement(lockIcon)
    modSelectButton.Lock = lockIcon

    modSelectButton._animationSets.DefaultAnimationSet = function()
        modSelectButton._sequences.DefaultSequence = function()

        end

        buttonText:RegisterAnimationSequence("ButtonOver", {{function()
            return modSelectButton.Text:SetRGBFromInt(0, 0)
        end}, {function()
            return modSelectButton.Text:SetAlpha(1, 0)
        end}})
        
        lockIcon:RegisterAnimationSequence("ButtonOver", {{function()
            return modSelectButton.Lock:SetAlpha(0, 0)
        end}, {function()
            return modSelectButton.Lock:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
        end}})
        
        modSelectButton._sequences.ButtonOver = function()
            buttonText:AnimateSequence("ButtonOver")
            lockIcon:AnimateSequence("ButtonOver")
        end

        buttonText:RegisterAnimationSequence("ButtonUp", {{function()
            return modSelectButton.Text:SetRGBFromInt(14277081, 0)
        end}})
        
        lockIcon:RegisterAnimationSequence("ButtonUp", {{function()
            return modSelectButton.Lock:SetAlpha(0, 0)
        end}})
        
        modSelectButton._sequences.ButtonUp = function()
            buttonText:AnimateSequence("ButtonUp")
            lockIcon:AnimateSequence("ButtonUp")
        end

        buttonText:RegisterAnimationSequence("ButtonOverDisabled", {{function()
            return modSelectButton.Text:SetRGBFromInt(0, 0)
        end}, {function()
            return modSelectButton.Text:SetAlpha(1, 0)
        end}})
        
        lockIcon:RegisterAnimationSequence("ButtonOverDisabled", {{function()
            return modSelectButton.Lock:SetAlpha(1, 0)
        end}, {function()
            return modSelectButton.Lock:SetRGBFromInt(0, 0)
        end}})
        
        modSelectButton._sequences.ButtonOverDisabled = function()
            buttonText:AnimateSequence("ButtonOverDisabled")
            lockIcon:AnimateSequence("ButtonOverDisabled")
        end

        buttonText:RegisterAnimationSequence("ButtonUpDisabled", {{function()
            return modSelectButton.Text:SetRGBFromInt(14277081, 0)
        end}})
        
        lockIcon:RegisterAnimationSequence("ButtonUpDisabled", {{function()
            return modSelectButton.Lock:SetAlpha(1, 0)
        end}, {function()
            return modSelectButton.Lock:SetRGBFromInt(12566463, 0)
        end}})
        
        modSelectButton._sequences.ButtonUpDisabled = function()
            buttonText:AnimateSequence("ButtonUpDisabled")
            lockIcon:AnimateSequence("ButtonUpDisabled")
        end

    end

    modSelectButton._animationSets.DefaultAnimationSet()
    PostLoadFunc(modSelectButton, controllerIndex, controller)
    return modSelectButton
end

MenuBuilder.registerType("ModSelectButton", ModSelectButton)