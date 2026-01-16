LUI.UIElement.debugDraw = function(f84_arg0, f84_arg1)
    if not f84_arg1 then
        f84_arg1 = 1
    end
    f84_arg0:addElement(LUI.UIImage.new({
        topAnchor = true,
        bottomAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        material = RegisterMaterial("white"),
        red = 0,
        blue = 0,
        green = 1,
        alpha = 0.2
    }))
    f84_arg0:addElement(LUI.UIImage.new({
        topAnchor = true,
        bottomAnchor = false,
        leftAnchor = true,
        rightAnchor = true,
        material = RegisterMaterial("white"),
        red = 1,
        blue = 0,
        green = 0,
        alpha = 1,
        bottom = f84_arg1
    }))
    f84_arg0:addElement(LUI.UIImage.new({
        topAnchor = true,
        bottomAnchor = true,
        leftAnchor = false,
        rightAnchor = true,
        material = RegisterMaterial("white"),
        red = 1,
        blue = 0,
        green = 0,
        alpha = 1,
        left = -1 * f84_arg1
    }))
    f84_arg0:addElement(LUI.UIImage.new({
        topAnchor = false,
        bottomAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        material = RegisterMaterial("white"),
        red = 1,
        blue = 0,
        green = 0,
        alpha = 1,
        top = -1 * f84_arg1
    }))
    f84_arg0:addElement(LUI.UIImage.new({
        topAnchor = true,
        bottomAnchor = true,
        leftAnchor = true,
        rightAnchor = false,
        material = RegisterMaterial("white"),
        red = 1,
        blue = 0,
        green = 0,
        alpha = 1,
        right = f84_arg1
    }))
end

local function draw_showlist(f29_arg0)
    if Engine.GetDvarBool("ui_showlist") then
        local _UIText = Engine.GetLuiRoot():getFirstDescendentById("uiShowlistText")
        if _UIText == nil then
            local f6_local1 = CoD.TextSettings.ButtonHelperFont
            _UIText = LUI.UIText.new({
                font = f6_local1.Font,
                topAnchor = false,
                bottomAnchor = true,
                leftAnchor = true,
                rightAnchor = true,
                top = -205,
                bottom = -205 + f6_local1.Height,
                right = 0,
                alpha = 1,
                color = COLORS.salmonRed
            })
            _UIText:setText("ui_showlist: ")
            _UIText.id = "uiShowlistText"
        end

        local rootElement = Engine.GetLuiRoot():getFirstDescendentById("uiShowlistRoot")
        if rootElement == nil then
            rootElement = LUI.UIElement.new(CoD.CreateState(70, nil, nil, nil, CoD.AnchorTypes.BottomLeft), 0)
            rootElement.width = 200
            rootElement.height = 200
            rootElement.alpha = 1
            rootElement.id = "uiShowlistRoot"
            Engine.GetLuiRoot():addElement(rootElement)
            rootElement:addElement(_UIText)
        end

        local ui_showlist_text = "ui_showlist:\n"
        for f29_local0 = #f29_arg0.menuInfoStack, 1, -1 do
            local f29_local3 = f29_arg0.menuInfoStack[f29_local0]
            ui_showlist_text = ui_showlist_text .. f29_local3.name .. ", "
        end

        _UIText:setText(ui_showlist_text)
    else
		-- remove ui show list if the dvar is turned off
        local rootElement = Engine.GetLuiRoot():getFirstDescendentById("uiShowlistRoot")
        if rootElement ~= nil then
            Engine.GetLuiRoot():removeElement(rootElement)
        end
    end
end

LUI.UIRoot.ProcessEvent = function(f12_arg0, f12_arg1)
    draw_showlist(Engine.GetLuiRoot().flowManager)

    -- original process event
    if f12_arg1.immediate == true then
        return LUI.UIRoot.ProcessEventNow(f12_arg0, f12_arg1)
    else
        table.insert(f12_arg0.eventQueue, f12_arg1)
        return false
    end
end
