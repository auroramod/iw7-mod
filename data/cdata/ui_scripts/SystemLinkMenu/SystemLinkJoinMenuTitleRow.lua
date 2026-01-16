local function PostLoadFunc(element, controllerIndex, controller)
    assert(element.Host)
    assert(element.Players)
    assert(element.Status)
    assert(element.Map)
    assert(element.Type)
    assert(element.Mod)
    assert(element.Ping)
    assert(element.PrivateS)
    if IsLanguageOversizedFont() then
        element.Host:SetTop(-11 * _1080p, 0)
        element.Host:SetBottom(11 * _1080p, 0)
        element.Players:SetTop(-11 * _1080p, 0)
        element.Players:SetBottom(11 * _1080p, 0)
        element.Status:SetTop(-11 * _1080p, 0)
        element.Status:SetBottom(11 * _1080p, 0)
        element.Map:SetTop(-11 * _1080p, 0)
        element.Map:SetBottom(11 * _1080p, 0)
        element.Type:SetTop(-11 * _1080p, 0)
        element.Type:SetBottom(11 * _1080p, 0)
        element.Mod:SetTop(-11 * _1080p, 0)
        element.Mod:SetBottom(11 * _1080p, 0)
        element.Ping:SetTop(-11 * _1080p, 0)
        element.Ping:SetBottom(11 * _1080p, 0)
        element.PrivateS:SetTop(-11 * _1080p, 0)
        element.PrivateS:SetBottom(11 * _1080p, 0)
    end
end

function SystemLinkJoinMenuTitleRow(menu, controller)
    local self = LUI.UIElement.new()
    self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 1790 * _1080p, 0, 40 * _1080p)
    self.id = "SystemLinkJoinMenuTitleRow"
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)

    local TitleBG = nil

    TitleBG = LUI.UIImage.new()
    TitleBG.id = "TitleBG"
    TitleBG:SetRGBFromInt(0, 0)
    self:addElement(TitleBG)
    self.TitleBG = TitleBG

    local Host = nil

    Host = LUI.UIText.new()
    Host.id = "Host"
    Host:setText(Engine.Localize("MENU_HOST_NAME"), 0)
    Host:SetFontSize(24 * _1080p)
    Host:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Host:SetAlignment(LUI.Alignment.Left)
    Host:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 9, _1080p * 275, _1080p * -12, _1080p * 12)
    self:addElement(Host)
    self.Host = Host

    local Players = nil

    Players = LUI.UIText.new()
    Players.id = "Players"
    Players:setText(Engine.Localize("MENU_PLAYERS"), 0)
    Players:SetFontSize(24 * _1080p)
    Players:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Players:SetAlignment(LUI.Alignment.Center)
    Players:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 283, _1080p * 418, _1080p * -12, _1080p * 12)
    self:addElement(Players)
    self.Players = Players

    local Status = nil

    Status = LUI.UIText.new()
    Status.id = "Status"
    Status:setText(Engine.Localize("MENU_STATUS_NAME"), 0)
    Status:SetFontSize(24 * _1080p)
    Status:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Status:SetAlignment(LUI.Alignment.Left)
    Status:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 426, _1080p * 620, _1080p * -12, _1080p * 12)
    self:addElement(Status)
    self.Status = Status

    local Map = nil

    Map = LUI.UIText.new()
    Map.id = "Map"
    Map:setText(Engine.Localize("MENU_MAP"), 0)
    Map:SetFontSize(24 * _1080p)
    Map:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Map:SetAlignment(LUI.Alignment.Left)
    Map:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 627, _1080p * 853, _1080p * -12, _1080p * 12)
    self:addElement(Map)
    self.Map = Map

    local Type = nil

    Type = LUI.UIText.new()
    Type.id = "Type"
    Type:setText(Engine.Localize("MENU_TYPE1"), 0)
    Type:SetFontSize(24 * _1080p)
    Type:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Type:SetAlignment(LUI.Alignment.Left)
    Type:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 862, _1080p * 1115, _1080p * -12, _1080p * 12)
    self:addElement(Type)
    self.Type = Type

    local Mod = nil

    Mod = LUI.UIText.new()
    Mod.id = "Mod"
    Mod:setText(Engine.Localize("MENU_MOD"), 0)
    Mod:SetFontSize(24 * _1080p)
    Mod:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Mod:SetAlignment(LUI.Alignment.Left)
    Mod:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1125, _1080p * 1400, _1080p * -12, _1080p * 12)
    self:addElement(Mod)
    self.Mod = Mod

    local Ping = nil

    Ping = LUI.UIText.new()
    Ping.id = "Ping"
    Ping:setText(Engine.Localize("MENU_PING"), 0)
    Ping:SetFontSize(24 * _1080p)
    Ping:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    Ping:SetAlignment(LUI.Alignment.Center)
    Ping:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1410, _1080p * 1460, _1080p * -12, _1080p * 12)
    self:addElement(Ping)
    self.Ping = Ping

    local PrivateS = nil

    PrivateS = LUI.UIText.new()
    PrivateS.id = "PrivateS"
    PrivateS:setText(Engine.Localize("MENU_PRIVATE"), 0)
    PrivateS:SetFontSize(24 * _1080p)
    PrivateS:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    PrivateS:SetAlignment(LUI.Alignment.Center)
    PrivateS:SetAnchorsAndPosition(0, 1, 0.5, 0.5, _1080p * 1470, _1080p * 1550, _1080p * -12, _1080p * 12)
    self:addElement(PrivateS)
    self.PrivateS = PrivateS

    PostLoadFunc(self, controllerIndex, controller)
    return self
end

MenuBuilder.m_types["SystemLinkJoinMenuTitleRow"] = SystemLinkJoinMenuTitleRow
