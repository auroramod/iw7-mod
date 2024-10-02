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

local postLoadGameSetupButtonsSubMenu = function(self, f2_arg1, f2_arg2)
	assert(self.MapsButton)
	assert(self.MapsButton.DynamicText)
	assert(self.OptionsButton)
	assert(self.OptionsButton.DynamicText)
	assert(self.BotSetup)

	self.MapsButton.DynamicText:setText(ToUpperCase(Lobby.GetMapName()))
	self.OptionsButton:SetButtonDisabled(1) -- TODO: game settings are not applied and get rest on menu change.
	self.BotSetup:SetButtonDisabled(1) -- TODO: support custom bot management (gsc + engine changes required)
end

local GameSetupButtonsSubMenu = function(menu, controller)
	local self = LUI.UIVerticalList.new()
	self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 600 * _1080p, 0, 190 * _1080p)
	self.id = "GameSetupButtonsSubMenu"
	local rootController = controller and controller.controllerIndex
	if not rootController and not Engine.InFrontend() then
		rootController = self:getRootController()
	end
	assert(rootController)
	local f3_local2 = self
	self:SetSpacing(10 * _1080p)

	local MapsButton = MenuBuilder.BuildRegisteredType("GenericDualLabelButton", {
		controllerIndex = rootController,
	})
	MapsButton.id = "MapsButton"
	MapsButton.buttonDescription = Engine.Localize("LUA_MENU_DESC_MAP")
	MapsButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MAP_CAPS")), 0)
	MapsButton.DynamicText:setText("", 0)
	MapsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, 0, _1080p * 30)
	self:addElement(MapsButton)
	self.MapsButton = MapsButton

	local OptionsButton = MenuBuilder.BuildRegisteredType("GenericDualLabelButton", {
		controllerIndex = rootController,
	})
	OptionsButton.id = "OptionsButton"
	OptionsButton.buttonDescription = Engine.Localize("LUA_MENU_DESC_OPTIONS")
	OptionsButton.Text:setText(ToUpperCase(Engine.Localize("MENU_CHANGE_GAME_RULES_CAPS")), 0)
	OptionsButton.DynamicText:setText("", 0)
	OptionsButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 80, _1080p * 110)
	self:addElement(OptionsButton)
	self.OptionsButton = OptionsButton

	local BotSetup = MenuBuilder.BuildRegisteredType("GenericButton", {
		controllerIndex = rootController,
	})
	BotSetup.id = "BotSetup"
	BotSetup.buttonDescription = Engine.Localize("LUA_MENU_DESC_BOTS")
	BotSetup.Text:setText(Engine.Localize("LUA_MENU_BOT_SETUP_CAPS"), 0)
	BotSetup:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 120, _1080p * 150)
	self:addElement(BotSetup)
	self.BotSetup = BotSetup

	local ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
		controllerIndex = rootController,
	})
	ButtonDescription.id = "ButtonDescription"
	ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 160, _1080p * 200)
	self:addElement(ButtonDescription)
	self.ButtonDescription = ButtonDescription

	MapsButton:addEventHandler("button_action", function(f4_arg0, f4_arg1)
		ACTIONS.OpenMenu("Maps", true, f4_arg1.controller or rootController)
	end)
	OptionsButton:addEventHandler("button_action", function(f6_arg0, f6_arg1)
		ACTIONS.OpenMenu("GameSetupOptionsMenu", true, f6_arg1.controller or rootController)
	end)
	BotSetup:addEventHandler("button_action", function(f7_arg0, f7_arg1)
		ACTIONS.OpenMenu("GameSetupBots", true, f7_arg1.controller or rootController)
	end)
	postLoadGameSetupButtonsSubMenu(self, rootController, controller)
	return self
end

MenuBuilder.m_types["GameSetupButtonsSubMenu"] = GameSetupButtonsSubMenu

local f0_local0 = function(f1_arg0, f1_arg1)
	f1_arg0.MenuTitle.MenuBreadcrumbs:setText(LocalizeString("COMBAT TRAINING"), 0)
end

local CombatTrainingGameSetup = function(menu, controller)
	local self = LUI.UIVerticalNavigator.new()
	self.id = "CombatTrainingGameSetup"
	local rootController = controller and controller.controllerIndex
	if not rootController and not Engine.InFrontend() then
		rootController = self:getRootController()
	end
	assert(rootController)
	self:playSound("menu_open")
	local f2_local2 = self
	local ButtonHelperBar = nil

	ButtonHelperBar = MenuBuilder.BuildRegisteredType("ButtonHelperBar", {
		controllerIndex = rootController,
	})
	ButtonHelperBar.id = "ButtonHelperBar"
	ButtonHelperBar:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -85, 0)
	self:addElement(ButtonHelperBar)
	self.ButtonHelperBar = ButtonHelperBar

	local MenuTitle = MenuBuilder.BuildRegisteredType("MenuTitle", {
		controllerIndex = rootController,
	})
	MenuTitle.id = "MenuTitle"
	MenuTitle.MenuTitle:setText(ToUpperCase(Engine.Localize("LUA_MENU_GAME_SETUP_CAPS")), 0)
	MenuTitle.MenuBreadcrumbs:setText(ToUpperCase(Engine.Localize("EXE_LOCAL_PLAY")), 0)
	MenuTitle.Icon:SetTop(_1080p * -28.5, 0)
	MenuTitle.Icon:SetBottom(_1080p * 61.5, 0)
	MenuTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134)
	self:addElement(MenuTitle)
	self.MenuTitle = MenuTitle

	local GameSetupButtons = MenuBuilder.BuildRegisteredType("GameSetupButtonsSubMenu", {
		controllerIndex = rootController,
	})
	GameSetupButtons.id = "GameSetupButtonsSubMenu"
	GameSetupButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 406)
	self:addElement(GameSetupButtons)
	self.GameSetupButtons = GameSetupButtons

	self.addButtonHelperFunction = function(self, f3_arg1)
		self:AddButtonHelperText({
			helper_text = Engine.Localize("MENU_BACK"),
			button_ref = "button_secondary",
			side = "left",
			priority = 10,
			clickable = true,
		})
		self:AddButtonHelperText({
			helper_text = Engine.Localize("LUA_MENU_SELECT"),
			button_ref = "button_primary",
			side = "left",
			priority = -10,
			clickable = true,
		})
	end

	self:addEventHandler("menu_create", self.addButtonHelperFunction)

	local bindButton = LUI.UIBindButton.new()
	bindButton.id = "selfBindButton"
	self:addElement(bindButton)
	self.bindButton = bindButton

	self.bindButton:addEventHandler("button_secondary", function(f4_arg0, f4_arg1)
		local f4_local0 = f4_arg1.controller or rootController
		ACTIONS.LeaveMenu(self)
	end)
	f0_local0(self, rootController, controller)
	return self
end

-- don't do local play things in public lobby
local SetupPrivateMatchLobbyScene_original = Lobby.SetupPrivateMatchLobbyScene
local SetupPrivateMatchLobbyScene_stub = function(a1)
	if CONDITIONS.InFrontendPublicMP then
		LUI.FlowManager.RegisterStackPopBehaviour("Maps", function() end)
		LUI.FlowManager.RegisterStackPopBehaviour("GameSetupOptionsMenu", function() end)
	else
		SetupPrivateMatchLobbyScene_original(a1)
	end
end
Lobby.SetupPrivateMatchLobbyScene = SetupPrivateMatchLobbyScene_stub

MenuBuilder.registerType("CombatTrainingGameSetup", CombatTrainingGameSetup)
