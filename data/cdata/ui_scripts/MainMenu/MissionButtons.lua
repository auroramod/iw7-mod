function MissionsButtons(menu, controller)
	local self = LUI.UIVerticalNavigator.new()
	self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 380 * _1080p)
	self.id = "MissionsButtons"
	local f1_local1 = controller and controller.controllerIndex
	if not f1_local1 and not Engine.InFrontend() then
		f1_local1 = self:getRootController()
	end
	assert(f1_local1)
	local MissionsVerticalLayout = MenuBuilder.BuildRegisteredType("MissionsVerticalLayout", {
		controllerIndex = f1_local1,
	})
	MissionsVerticalLayout.id = "MissionsVerticalLayout"
	MissionsVerticalLayout:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, 0, _1080p * 340)
	self:addElement(MissionsVerticalLayout)
	self.MissionsVerticalLayout = MissionsVerticalLayout

	local FindMatchButton = MissionsVerticalLayout:getChildById("MissionSelect")
	FindMatchButton.Text:setText(ToUpperCase(Engine.Localize("Combat Training")), 0)
	FindMatchButton.buttonDescription = Engine.Localize("Rank up against bots.")

	local ServerBrowserButton = MenuBuilder.BuildRegisteredType("GenericButton", {
		controllerIndex = f6_local1,
	})
	ServerBrowserButton.id = "ServerBrowserButton"
	ServerBrowserButton.Text:setText(ToUpperCase(Engine.Localize("Server Browser")), 0)
	ServerBrowserButton.buttonDescription = Engine.Localize("Browse a list of dedicated servers.")
	ServerBrowserButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
	LUI.UIElement.addElementBefore(ServerBrowserButton, MissionsVerticalLayout:getChildById("MissionSelect"))
	MissionsVerticalLayout.ServerBrowserButton = ServerBrowserButton
	ServerBrowserButton:addEventHandler("button_action", function(f14_arg0, f14_arg1)
		LUI.FlowManager.RequestAddMenu("SystemLinkMenu", true, f14_arg1.controller, false, {})
	end)

	local CRMMain = MenuBuilder.BuildRegisteredType("CRMMain", {
		controllerIndex = f1_local1,
	})
	CRMMain.id = "CRMMain"
	CRMMain:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 6, _1080p * 317, _1080p * 530, _1080p * 854)
	self:addElement(CRMMain)
	self.CRMMain = CRMMain

	ACTIONS.AnimateSequenceByElement(self, {
		elementName = "CRMMain",
		sequenceName = "Opening",
		elementPath = "CRMMain",
	})
	return self
end

MenuBuilder.m_types["MissionsButtons"] = MissionsButtons
