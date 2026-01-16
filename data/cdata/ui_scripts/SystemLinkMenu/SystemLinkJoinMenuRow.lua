local function PostLoadFunc(element, controllerIndex, controller)
    element:addEventHandler("button_action", function(arg0, arg1)
        local dataSource = element:GetDataSource()
        Lobby.JoinServer(controllerIndex, dataSource.index)
    end)
end

function SystemLinkJoinMenuRow(menu, controller)
    local self = LUI.UIButton.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 1790 * _1080p, 0, 35 * _1080p)
    self.id = "SystemLinkJoinMenuRow"
    self._animationSets = {}
    self._sequences = {}
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)

    local DropShadow = nil

    DropShadow = LUI.UIImage.new()
    DropShadow.id = "DropShadow"
    DropShadow:SetRGBFromInt(0, 0)
    DropShadow:SetAlpha(0.3, 0)
    DropShadow:setImage(RegisterMaterial("widg_gradient_top_to_bottom"), 0)
    DropShadow:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, 0, _1080p * 4)
    self:addElement(DropShadow)
    self.DropShadow = DropShadow

    local RowBG = nil

    RowBG = LUI.UIImage.new()
    RowBG.id = "RowBG"
    RowBG:SetRGBFromInt(0, 0)
    RowBG:SetAlpha(0.5, 0)
    self:addElement(RowBG)
    self.RowBG = RowBG

    local Host = nil

    Host = LUI.UIStyledText.new()
    Host.id = "Host"
    Host:SetRGBFromInt(14277081, 0)
    Host:SetFontSize(24 * _1080p)
    Host:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Host:SetAlignment(LUI.Alignment.Left)
    Host:SetStartupDelay(1000)
    Host:SetLineHoldTime(400)
    Host:SetAnimMoveTime(3000)
    Host:SetEndDelay(1000)
    Host:SetCrossfadeTime(400)
    Host:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    Host:SetMaxVisibleLines(1)
    Host:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 9, _1080p * 275, _1080p * -12, _1080p * 12)
    Host:SubscribeToModelThroughElement(self, "host", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.host:GetValue(controllerIndex)
        if dataSource ~= nil then
            Host:setText(dataSource, 0)
        end
    end)
    self:addElement(Host)
    self.Host = Host

    local Players = nil

    Players = LUI.UIText.new()
    Players.id = "Players"
    Players:SetRGBFromInt(14277081, 0)
    Players:SetFontSize(24 * _1080p)
    Players:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Players:SetAlignment(LUI.Alignment.Center)
    Players:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 283, _1080p * 418, _1080p * -12, _1080p * 12)
    Players:SubscribeToModelThroughElement(self, "players", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.players:GetValue(controllerIndex)
        if dataSource ~= nil then
            Players:setText(dataSource, 0)
        end
    end)
    self:addElement(Players)
    self.Players = Players

    local Status = nil

    Status = LUI.UIText.new()
    Status.id = "Status"
    Status:SetRGBFromInt(14277081, 0)
    Status:SetFontSize(24 * _1080p)
    Status:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Status:SetAlignment(LUI.Alignment.Left)
    Status:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 426, _1080p * 620, _1080p * -12, _1080p * 12)
    Status:SubscribeToModelThroughElement(self, "status", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.status:GetValue(controllerIndex)
        if dataSource ~= nil then
            Status:setText(ToUpperCase(dataSource), 0)
        end
    end)
    self:addElement(Status)
    self.Status = Status

    local Map = nil

    Map = LUI.UIStyledText.new()
    Map.id = "Map"
    Map:SetRGBFromInt(14277081, 0)
    Map:SetFontSize(24 * _1080p)
    Map:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Map:SetAlignment(LUI.Alignment.Left)
    Map:SetStartupDelay(1250)
    Map:SetLineHoldTime(400)
    Map:SetAnimMoveTime(3000)
    Map:SetEndDelay(1000)
    Map:SetCrossfadeTime(500)
    Map:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    Map:SetMaxVisibleLines(1)
    Map:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 627, _1080p * 853, _1080p * -12, _1080p * 12)
    Map:SubscribeToModelThroughElement(self, "map", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.map:GetValue(controllerIndex)
        if dataSource ~= nil then
            Map:setText(dataSource, 0)
        end
    end)
    self:addElement(Map)
    self.Map = Map

    local Type = nil

    Type = LUI.UIText.new()
    Type.id = "Type"
    Type:SetRGBFromInt(14277081, 0)
    Type:SetFontSize(24 * _1080p)
    Type:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Type:SetAlignment(LUI.Alignment.Left)
    Type:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 862, _1080p * 1115, _1080p * -12, _1080p * 12)
    Type:SubscribeToModelThroughElement(self, "type", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.type:GetValue(controllerIndex)
        if dataSource ~= nil then
            Type:setText(dataSource, 0)
        end
    end)
    self:addElement(Type)
    self.Type = Type

    local Mod = nil

    Mod = LUI.UIStyledText.new()
    Mod.id = "Mod"
    Mod:SetRGBFromInt(14277081, 0)
    Mod:SetFontSize(24 * _1080p)
    Mod:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Mod:SetAlignment(LUI.Alignment.Left)
    Mod:SetStartupDelay(1250)
    Mod:SetLineHoldTime(400)
    Mod:SetAnimMoveTime(3000)
    Mod:SetEndDelay(1000)
    Mod:SetCrossfadeTime(500)
    Mod:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    Mod:SetMaxVisibleLines(1)
    Mod:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1125, _1080p * 1400, _1080p * -12, _1080p * 12)
    Mod:SubscribeToModelThroughElement(self, "mod", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.mod:GetValue(controllerIndex)
        if dataSource ~= nil then
            Mod:setText(dataSource, 0)
        end
    end)
    self:addElement(Mod)
    self.Mod = Mod

    local Ping = nil

    Ping = LUI.UIText.new()
    Ping.id = "Ping"
    Ping:SetRGBFromInt(14277081, 0)
    Ping:SetFontSize(24 * _1080p)
    Ping:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Ping:SetAlignment(LUI.Alignment.Center)
    Ping:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1410, _1080p * 1460, _1080p * -12, _1080p * 12)
    Ping:SubscribeToModelThroughElement(self, "ping", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.ping:GetValue(controllerIndex)
        if dataSource ~= nil then
            Ping:setText(dataSource, 0)
        end
    end)
    self:addElement(Ping)
    self.Ping = Ping

    local PrivateS = nil

    PrivateS = LUI.UIText.new()
    PrivateS.id = "PrivateS"
    PrivateS:SetRGBFromInt(14277081, 0)
    PrivateS:SetFontSize(24 * _1080p)
    PrivateS:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    PrivateS:SetAlignment(LUI.Alignment.Center)
    PrivateS:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1470, _1080p * 1550, _1080p * -12, _1080p * 12)
    PrivateS:SubscribeToModelThroughElement(self, "priv", function()
        local dataSource = self:GetDataSource()
        dataSource = dataSource.priv:GetValue(controllerIndex)
        if dataSource ~= nil then
            PrivateS:setText(dataSource, 0)
        end
    end)
    self:addElement(PrivateS)
    self.PrivateS = PrivateS

    local Cap = nil

    Cap = LUI.UIImage.new()
    Cap.id = "Cap"
    Cap:SetRGBFromInt(14277081, 0)
    Cap:SetAnchorsAndPosition(0, 1, 0, 0, 0, _1080p * 6, 0, 0)
    self:addElement(Cap)
    self.Cap = Cap

    self._animationSets.DefaultAnimationSet = function()
        self._sequences.DefaultSequence = function()

        end

        RowBG:RegisterAnimationSequence("ButtonOver", {{function()
            return self.RowBG:SetAlpha(1, 0)
        end}, {function()
            return self.RowBG:SetRGBFromInt(14277081, 0)
        end}})
        Host:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Host:SetRGBFromInt(0, 0)
        end}})
        Players:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Players:SetRGBFromInt(0, 0)
        end}})
        Status:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Status:SetRGBFromInt(0, 0)
        end}})
        Map:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Map:SetRGBFromInt(0, 0)
        end}})
        Type:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Type:SetRGBFromInt(0, 0)
        end}})
        Mod:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Mod:SetRGBFromInt(0, 0)
        end}})
        Ping:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Ping:SetRGBFromInt(0, 0)
        end}})
        PrivateS:RegisterAnimationSequence("ButtonOver", {{function()
            return self.PrivateS:SetRGBFromInt(0, 0)
        end}})
        Cap:RegisterAnimationSequence("ButtonOver", {{function()
            return self.Cap:SetRGBFromInt(0, 0)
        end}})
        self._sequences.ButtonOver = function()
            RowBG:AnimateSequence("ButtonOver")
            Host:AnimateSequence("ButtonOver")
            Players:AnimateSequence("ButtonOver")
            Status:AnimateSequence("ButtonOver")
            Map:AnimateSequence("ButtonOver")
            Type:AnimateSequence("ButtonOver")
            Mod:AnimateSequence("ButtonOver")
            Ping:AnimateSequence("ButtonOver")
            PrivateS:AnimateSequence("ButtonOver")
            Cap:AnimateSequence("ButtonOver")
        end

        RowBG:RegisterAnimationSequence("ButtonUp", {{function()
            return self.RowBG:SetAlpha(0.5, 0)
        end}, {function()
            return self.RowBG:SetRGBFromInt(0, 0)
        end}})
        Host:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Host:SetRGBFromInt(14277081, 0)
        end}})
        Players:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Players:SetRGBFromInt(14277081, 0)
        end}})
        Status:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Status:SetRGBFromInt(14277081, 0)
        end}})
        Map:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Map:SetRGBFromInt(14277081, 0)
        end}})
        Type:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Type:SetRGBFromInt(14277081, 0)
        end}})
        Mod:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Mod:SetRGBFromInt(14277081, 0)
        end}})
        Ping:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Ping:SetRGBFromInt(14277081, 0)
        end}})
        PrivateS:RegisterAnimationSequence("ButtonUp", {{function()
            return self.PrivateS:SetRGBFromInt(14277081, 0)
        end}})
        Cap:RegisterAnimationSequence("ButtonUp", {{function()
            return self.Cap:SetRGBFromInt(14277081, 0)
        end}})
        self._sequences.ButtonUp = function()
            RowBG:AnimateSequence("ButtonUp")
            Host:AnimateSequence("ButtonUp")
            Players:AnimateSequence("ButtonUp")
            Status:AnimateSequence("ButtonUp")
            Map:AnimateSequence("ButtonUp")
            Type:AnimateSequence("ButtonUp")
            Mod:AnimateSequence("ButtonUp")
            Ping:AnimateSequence("ButtonUp")
            PrivateS:AnimateSequence("ButtonUp")
            Cap:AnimateSequence("ButtonUp")
        end

    end

    self._animationSets.DefaultAnimationSet()
    self:addEventHandler("button_over", function(arg0, arg1)
        ACTIONS.AnimateSequence(self, "ButtonOver")
    end)
    self:addEventHandler("button_up", function(arg0, arg1)
        ACTIONS.AnimateSequence(self, "ButtonUp")
    end)
    PostLoadFunc(self, controllerIndex, controller)
    return self
end

MenuBuilder.m_types["SystemLinkJoinMenuRow"] = SystemLinkJoinMenuRow
