if not Engine.InFrontend() then
	return
end

local MenuOverride = function(oldmenu, postLoad)
	local newmenu = function(menu, controller)
		local RootController = controller and controller.controllerIndex
		if not RootController and not Engine.InFrontend() then
			RootController = self:getRootController()
		end
		assert(RootController)

		local self = oldmenu(menu, controller)
		postLoad(self)
		return self
	end
	return newmenu
end

local GetAnchorsAndPosition = function(element)
	local l_left, l_top, l_right, l_bottom = element:getLocalRect()
	local a_left, a_top, a_right, a_bottom = element:GetAnchorData()
	return { a_left, a_right, a_top, a_bottom, l_left, l_right, l_bottom, l_top }
end

local customServerButton = function(self, override, preLoad)
	if not CONDITIONS.IsServerBrowserAllowed(self) then
		return
	end

	self.ServerBrowserButton = override
	assert(self.ServerBrowserButton)

	self.ServerBrowserButton:registerEventHandler("button_action", function(f5_arg0, f5_arg1)
		preLoad()
		LUI.FlowManager.RequestAddMenu("SystemLinkMenu", false, f5_arg1.controller, false, {}, true)
	end)

	if self.ServerBrowserButton then
		self.ServerBrowserButton.id = "ServerBrowser"
		self.ServerBrowserButton.buttonDescription = Engine.Localize("Browse custom servers")
		self.ServerBrowserButton.Text:setText(ToUpperCase(Engine.Localize("Server Browser")), 0)
	end
end

local customModsButton = function(self, info)
	if not CONDITIONS.IsServerBrowserAllowed(self) then
		return
	end

	if not (type(info.position) == "table" and #info.position == 8) then
		return
	end

	local ModsButton = MenuBuilder.BuildRegisteredType("MenuButton", { controllerIndex = controllerIndex })
	ModsButton.id = "ModsButton"
	ModsButton.buttonDescription = Engine.Localize("LUA_MENU_MODS_DESC")
	ModsButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MODS_CAPS")), 0)
	if CONDITIONS.InFrontendPublicMP and CONDITIONS.IsStoreAllowed() then
		self:removeElement(self.StoreButton)
	end
	ModsButton:SetAnchorsAndPosition(table.unpack(info.position))

	ModsButton:registerEventHandler("button_action", function(f5_arg0, f5_arg1)
		if info.preLoad then
			info.preLoad()
		end
		LUI.FlowManager.RequestAddMenu("ModSelectMenu", false, f5_arg1.controller, false, {}, true)
	end)

	ModsButton:SetAlignment(LUI.Alignment.Left)
	self:addElement(ModsButton)
	self.ModsButton = ModsButton
end

local postLoadMP = function(self)
	local preLoadMP = function()
		local ActiveController = Engine.GetFirstActiveController()
		if Engine.GetDvarBool("xblive_competitionmatch") then
			Engine.SetDvarBool("xblive_competitionmatch", false)
			Engine.Exec("set remove_mlg_rules 1")
		end
		Engine.ExecNow(MPConfig.default_xboxlive, ActiveController)
	end

	local modButtonInfo = {}
	modButtonInfo.preLoad = function() end
	modButtonInfo.position = { 0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * (120 + 30) }
	customModsButton(self, modButtonInfo)
	customServerButton(self, self.MLGGameBattlesButton, preLoadMP)
end

local postLoadCP = function(self)
	local preLoadCP = function()
		local ActiveController = Engine.GetFirstActiveController()
		Engine.ExecNow(MPConfig.default_xboxlive, ActiveController)
	end

	local modButtonInfo = {}
	modButtonInfo.preLoad = function() end
	modButtonInfo.position = { 0, 1, 0, 1, 0, _1080p * 500, _1080p * 300, _1080p * (300 + 30) }
	customModsButton(self, modButtonInfo)
	customServerButton(self, self:getChildById("PublicMatch"), preLoadCP)
end

local MPMainMenuButtons = MenuOverride(package.loaded["frontEnd.mp.MPMainMenuButtons"].MPMainMenuButtons, postLoadMP)
local CPMainMenuButtons = MenuOverride(package.loaded["frontEnd.cp.CPMainMenuButtons"].CPMainMenuButtons, postLoadCP)

MenuBuilder.registerType("MPMainMenuButtons", MPMainMenuButtons)
MenuBuilder.registerType("CPMainMenuButtons", CPMainMenuButtons)
