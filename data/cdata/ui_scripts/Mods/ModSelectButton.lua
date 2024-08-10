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
        dataSource = dataSource.modName
    end)
    
    buttonElement.GenericButton:addEventHandler("button_up", function(unhoveredElement, eventArgs)
        local dataSource = buttonElement:GetDataSource()
        dataSource.buttonOnHoverFunction(unhoveredElement, eventArgs)
        dataSource = buttonElement:GetDataSource()
        dataSource = dataSource.modName
    end)
    
    buttonElement:registerEventHandler("grid_anim", function(element, event)
        element:SetAlpha(event.value)
    end)
    
    buttonElement:SubscribeToDataSourceThroughElement(buttonElement, nil, function()
        local dataSource = buttonElement:GetDataSource()
        dataSource = dataSource.modName
    end)
end

function ModSelectButton(menu, controller)
    local ModSelectButton = LUI.UIButton.new()
    ModSelectButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 30 * _1080p)
    ModSelectButton.id = "ModSelectButton"
    ModSelectButton._animationSets = {}
    ModSelectButton._sequences = {}
    
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = ModSelectButton:getRootController()
    end
    assert(controllerIndex)
    
    local buttonContainer = ModSelectButton
    local GenericButton = nil

    GenericButton = MenuBuilder.BuildRegisteredType("GenericButton", {
        controllerIndex = controllerIndex
    })
    GenericButton.id = "GenericButton"
    GenericButton:SetAlpha(0, 0)
    GenericButton:SetAnchorsAndPosition(0, 1, 0, 0, 0, _1080p * 500, 0, 0)
    
    GenericButton:SubscribeToModelThroughElement(ModSelectButton, "buttonLabel", function()
        local dataSource = ModSelectButton:GetDataSource()
        local buttonLabel = dataSource.buttonLabel:GetValue(controllerIndex)
        if buttonLabel ~= nil then
            GenericButton.Text:setText(LocalizeString(ToUpperCase(buttonLabel)), 0)
        end
    end)
    
    ModSelectButton:addElement(GenericButton)
    ModSelectButton.GenericButton = GenericButton

    local GenericListArrowButtonBackground = nil

    GenericListArrowButtonBackground = MenuBuilder.BuildRegisteredType("GenericListArrowButtonBackground", {
        controllerIndex = controllerIndex
    })
    GenericListArrowButtonBackground.id = "GenericListButtonBackground"
    GenericListArrowButtonBackground:SetAnchorsAndPosition(0, 0, 0, 0, 0, 0, 0, 0)
    
    ModSelectButton:addElement(GenericListArrowButtonBackground)
    ModSelectButton.GenericListButtonBackground = GenericListArrowButtonBackground

    local Text = nil

    Text = LUI.UIStyledText.new()
    Text.id = "Text"
    Text:SetRGBFromInt(14277081, 0)
    Text:SetFontSize(22 * _1080p)
    Text:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Text:SetAlignment(LUI.Alignment.Left)
    Text:SetStartupDelay(2000)
    Text:SetLineHoldTime(400)
    Text:SetAnimMoveTime(300)
    Text:SetEndDelay(1500)
    Text:SetCrossfadeTime(750)
    Text:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    Text:SetMaxVisibleLines(1)
    Text:SetOutlineRGBFromInt(0, 0)
    Text:SetAnchorsAndPosition(0, 0, 0.5, 0.5, _1080p * 44, _1080p * -41, _1080p * -11, _1080p * 11)
    
    Text:SubscribeToModelThroughElement(ModSelectButton, "buttonLabel", function()
        local dataSource = ModSelectButton:GetDataSource()
        local buttonLabel = dataSource.buttonLabel:GetValue(controllerIndex)
        if buttonLabel ~= nil then
            Text:setText(LocalizeString(ToUpperCase(buttonLabel)), 0)
        end
    end)
    
    ModSelectButton:addElement(Text)
    ModSelectButton.Text = Text

    local Lock = nil

    Lock = LUI.UIImage.new()
    Lock.id = "Lock"
    Lock:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
    Lock:SetAlpha(0, 0)
    Lock:setImage(RegisterMaterial("icon_slot_locked"), 0)
    Lock:SetAnchorsAndPosition(1, 0, 0.5, 0.5, _1080p * -32, _1080p * -6, _1080p * -12, _1080p * 14)
    
    ModSelectButton:addElement(Lock)
    ModSelectButton.Lock = Lock

    ModSelectButton._animationSets.DefaultAnimationSet = function()
        ModSelectButton._sequences.DefaultSequence = function()

        end

        Text:RegisterAnimationSequence("ButtonOver", {{function()
            return ModSelectButton.Text:SetRGBFromInt(0, 0)
        end}, {function()
            return ModSelectButton.Text:SetAlpha(1, 0)
        end}})
        
        Lock:RegisterAnimationSequence("ButtonOver", {{function()
            return ModSelectButton.Lock:SetAlpha(0, 0)
        end}, {function()
            return ModSelectButton.Lock:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
        end}})
        
        ModSelectButton._sequences.ButtonOver = function()
            Text:AnimateSequence("ButtonOver")
            Lock:AnimateSequence("ButtonOver")
        end

        Text:RegisterAnimationSequence("ButtonUp", {{function()
            return ModSelectButton.Text:SetRGBFromInt(14277081, 0)
        end}})
        
        Lock:RegisterAnimationSequence("ButtonUp", {{function()
            return ModSelectButton.Lock:SetAlpha(0, 0)
        end}})
        
        ModSelectButton._sequences.ButtonUp = function()
            Text:AnimateSequence("ButtonUp")
            Lock:AnimateSequence("ButtonUp")
        end

        Text:RegisterAnimationSequence("ButtonOverDisabled", {{function()
            return ModSelectButton.Text:SetRGBFromInt(0, 0)
        end}, {function()
            return ModSelectButton.Text:SetAlpha(1, 0)
        end}})
        
        Lock:RegisterAnimationSequence("ButtonOverDisabled", {{function()
            return ModSelectButton.Lock:SetAlpha(1, 0)
        end}, {function()
            return ModSelectButton.Lock:SetRGBFromInt(0, 0)
        end}})
        
        ModSelectButton._sequences.ButtonOverDisabled = function()
            Text:AnimateSequence("ButtonOverDisabled")
            Lock:AnimateSequence("ButtonOverDisabled")
        end

        Text:RegisterAnimationSequence("ButtonUpDisabled", {{function()
            return ModSelectButton.Text:SetRGBFromInt(14277081, 0)
        end}})
        
        Lock:RegisterAnimationSequence("ButtonUpDisabled", {{function()
            return ModSelectButton.Lock:SetAlpha(1, 0)
        end}, {function()
            return ModSelectButton.Lock:SetRGBFromInt(12566463, 0)
        end}})
        
        ModSelectButton._sequences.ButtonUpDisabled = function()
            Text:AnimateSequence("ButtonUpDisabled")
            Lock:AnimateSequence("ButtonUpDisabled")
        end

    end

    ModSelectButton._animationSets.DefaultAnimationSet()
    PostLoadFunc(ModSelectButton, controllerIndex, controller)
    return ModSelectButton
end

MenuBuilder.registerType("ModSelectButton", ModSelectButton)