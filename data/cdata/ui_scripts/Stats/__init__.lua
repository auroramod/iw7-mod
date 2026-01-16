if not Engine.InFrontend() then
	return
end

local GetRankIcon = Rank.GetRankIcon
local GetRankIcon_stub = function(rank, prestige)
	local ranked = GetRankIcon(rank, prestige)
	return Engine.IsAliensMode() and prestige > 0 and string.format("zm_icon_rank_prestige_%d", prestige) or ranked
end
Rank.GetRankIcon = GetRankIcon_stub

local function GenericArrowButton(controllerIndex, id, title, desc)
	local self = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
		controllerIndex = controllerIndex
	})

	self.id = id
	self:SetAnchorsAndPosition(0, 0, 0, 1, 0, 0, 0, _1080p * 30, 0)
	self.Title:setText(ToUpperCase(title))
	self.buttonDescription = desc

	return self
end

function popmenu_unlockstats()
	return {
		type = "generic_yesno_popup",
		properties = {
			message_text = "Do you want to unlock all stats, items and progression?",
			popup_title = "Unlock Stats",
			yes_text = "Yes, Unlock",
			no_text = "No, Cancel",
			yes_action = function()
				Engine.Exec("unlockstats")
				Engine.Exec("unlockstatsEE")
                Engine.Exec("luiReload") --fixme
			end
		}
	}
end

function popmenu_resetstats()
	return {
		type = "generic_yesno_popup",
		properties = {
			message_text = "Are you sure you want to reset all stats?",
			popup_title = "Reset Stats",
			yes_text = "Yes, Reset",
			no_text = "No, Keep Stats",
			yes_action = function()
				Engine.Exec("freshStart")
				Engine.Exec("confirmStatsReset")
			end
		}
	}
end

MenuBuilder.registerDef("popmenu_unlockstats", popmenu_unlockstats)
MenuBuilder.registerDef("popmenu_resetstats", popmenu_resetstats)

MenuBuilder.registerType("StatsMenuButtons", function(menu, controller)
	local self = LUI.UIVerticalNavigator.new({
		spacing = 10 * _1080p
	})

	local controllerIndex = controller and controller.controllerIndex
	if not controllerIndex and not Engine.InFrontend() then
		controllerIndex = self:getRootController()
	end
	assert(controllerIndex)

	self:SetupStackedLayout({})

	local UnlockItems = GenericArrowButton(
		controllerIndex,
		"UnlockItems",
		"Unlock All Items",
		"Whether items should be locked based on the player's stats or always unlocked."
	)

	local labels = {Engine.Localize("LUA_MENU_ENABLED"), Engine.Localize("LUA_MENU_DISABLED")}

	LUI.AddUIArrowTextButtonLogic(UnlockItems, controllerIndex, {
		labels = labels,
		wrapAround = true,
		defaultValue = Engine.GetDvarBool("cg_unlockall_items") and 1 or 2,
		action = function(valueIndex)
			Engine.SetDvarBool("cg_unlockall_items", valueIndex == 1)
		end
	})

	self.UnlockItems = UnlockItems
	self:addElement(UnlockItems)

	local UnlockLoot = GenericArrowButton(
		controllerIndex,
		"UnlockLoot",
		"Unlock All Loot",
		"Whether loot should be locked based on the player's stats or always unlocked."
	)

	LUI.AddUIArrowTextButtonLogic(UnlockLoot, controllerIndex, {
		labels = labels,
		wrapAround = true,
		defaultValue = Engine.GetDvarBool("cg_unlockall_loot") and 1 or 2,
		action = function(valueIndex)
			local enabled = (valueIndex == 1)
			Engine.SetDvarBool("cg_unlockall_loot", enabled)
			Engine.SetDvarInt("cg_loot_count", enabled and 999 or 1)
		end
	})

	self.UnlockLoot = UnlockLoot
	self:addElement(UnlockLoot)

	self:addSpacer(10)

	local isAliens = Engine.IsAliensMode()
	local prestigeTitle = isAliens and "Icon" or "Prestige"
	local prestigeDesc = isAliens and "Edit icon." or "Edit prestige level."

	local SetPrestige = GenericArrowButton(
		controllerIndex,
		"SetPrestige",
		prestigeTitle,
		prestigeDesc
	)

	labels = {}
	for i = 1, (isAliens and 49 or 31) do
		labels[i] = i - 1
	end

	--if isAliens then
	--	labels[1] = "Default"
	--end

	LUI.AddUIArrowTextButtonLogic(SetPrestige, controllerIndex, {
		labels = labels,
		wrapAround = true,
		defaultValue = Cac.GetPrestigeLevel(controllerIndex) + 1,
		action = function(valueIndex)
			local statsGroup = isAliens and CoD.StatsGroup.Coop or CoD.StatsGroup.Ranked
			Engine.SetPlayerDataEx(
				controllerIndex,
				statsGroup,
				"progression",
				"playerLevel",
				"prestige",
				valueIndex - 1
			)
		end
	})

	SetPrestige:addEventHandler("button_action", function()
		OSK.OpenScreenKeyboard(
			controllerIndex,
			"Edit Prestige",
			"",
			4,
			false,
			true,
			false,
			function(arg0, arg1, arg2)
				if not arg1 or arg2 == CoD.KeyboardResult.UI_KEYBOARD_RESULT_CANCELLED then
					return
				end

				local input = tonumber(arg1)
				if input then
					if input < 0 then
						input = 0
					end
					if input >= #SetPrestige.labels then
						input = #SetPrestige.labels - 1
					end

					SetPrestige.action(input + 1)
					SetPrestige:SetCurrentValue(input + 1)
				else
					LUI.FlowManager.RequestPopupMenu(arg0, "PopupOK", true, arg1.controller, false, {
						title = "ERROR",
						message = ""
					})
				end
			end,
			CoD.KeyboardInputTypes.Normal
		)
	end)

	self.SetPrestige = SetPrestige
	self:addElement(SetPrestige)

	local SetRank = GenericArrowButton(
		controllerIndex,
		"SetRank",
		"Rank",
		"Edit rank."
	)

	labels = {}
	for i = 1, Rank.GetMaxRank() + 1 do
		labels[i] = i
	end

	local rankStatsGroup = isAliens and CoD.StatsGroup.Coop or CoD.StatsGroup.Ranked
	local currentXP = Engine.GetPlayerDataEx(
		controllerIndex,
		rankStatsGroup,
		"progression",
		"playerLevel",
		"xp"
	)

	LUI.AddUIArrowTextButtonLogic(SetRank, controllerIndex, {
		labels = labels,
		wrapAround = true,
		defaultValue = Engine.GetRankForXP(currentXP) + 1,
		action = function(valueIndex)
			local statsGroup = isAliens and CoD.StatsGroup.Coop or CoD.StatsGroup.Ranked
			local newXP = Rank.GetRankMinXP(valueIndex - 1) + 123
			Engine.SetPlayerDataEx(
				controllerIndex,
				statsGroup,
				"progression",
				"playerLevel",
				"xp",
				newXP
			)
		end
	})

	SetRank:addEventHandler("button_action", function()
		OSK.OpenScreenKeyboard(
			controllerIndex,
			"Edit Rank",
			"",
			4,
			false,
			true,
			false,
			function(arg0, arg1, arg2)
				if not arg1 or arg2 == CoD.KeyboardResult.UI_KEYBOARD_RESULT_CANCELLED then
					return
				end

				local input = tonumber(arg1)
				if input then
					if input < 1 then
						input = 1
					end
					if input > #SetRank.labels then
						input = #SetRank.labels
					end

					SetRank.action(input)
					SetRank:SetCurrentValue(input)
				else
					LUI.FlowManager.RequestPopupMenu(arg0, "PopupOK", true, arg1.controller, false, {
						title = "ERROR",
						message = ""
					})
				end
			end,
			CoD.KeyboardInputTypes.Normal
		)
	end)

	self.SetRank = SetRank
	self:addElement(SetRank)

	local ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
		controllerIndex = controllerIndex
	})
	ButtonDescription.id = "ButtonDescription"
	self.ButtonDescription = ButtonDescription
	self:addElement(ButtonDescription)

	return self
end)

MenuBuilder.registerType("StatsMenu", function(menu, controller)
	local self = LUI.UIElement.new()
	self.id = "StatsMenu"

	local controllerIndex = controller and controller.controllerIndex
	if not controllerIndex and not Engine.InFrontend() then
		controllerIndex = self:getRootController()
	end
	assert(controllerIndex)

	self:playSound("menu_open")

	local ButtonHelperBar = MenuBuilder.BuildRegisteredType("ButtonHelperBar", {
		controllerIndex = controllerIndex
	})
	ButtonHelperBar.id = "ButtonHelperBar"
	ButtonHelperBar:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -85, 0)
	self:addElement(ButtonHelperBar)
	self.ButtonHelperBar = ButtonHelperBar

	local MenuTitle
	if Engine.IsAliensMode() then
		MenuTitle = MenuBuilder.BuildRegisteredType("CPMenuTitle", {
			controllerIndex = controllerIndex
		})
	else
		MenuTitle = MenuBuilder.BuildRegisteredType("MenuTitle", {
			controllerIndex = controllerIndex
		})
		MenuTitle.MenuBreadcrumbs:setText(ToUpperCase("IW7-Mod"))
		MenuTitle.Icon:SetTop(_1080p * -28.5, 0)
		MenuTitle.Icon:SetBottom(_1080p * 61.5, 0)
	end

	MenuTitle.id = "MenuTitle"
	MenuTitle.MenuTitle:setText(ToUpperCase("Stats"), 0)
	MenuTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134)
	self:addElement(MenuTitle)
	self.MenuTitle = MenuTitle

	local MissionTeamInfo = MenuBuilder.BuildRegisteredType("MissionTeamInfo", {
		controllerIndex = controllerIndex
	})
	MissionTeamInfo.id = "MissionTeamInfo"
	MissionTeamInfo:SetAnchorsAndPosition(1, 0, 0, 1, _1080p * -700, 0, _1080p * 54, _1080p * 142)
	self:addElement(MissionTeamInfo)
	self.MissionTeamInfo = MissionTeamInfo

	local StatsMenuButtons = MenuBuilder.BuildRegisteredType("StatsMenuButtons", {
		controllerIndex = controllerIndex
	})
	StatsMenuButtons.id = "StatsMenuButtons"
	StatsMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 906, _1080p * 216, _1080p * 917, 0)
	self:addElement(StatsMenuButtons)
	self.StatsMenuButtons = StatsMenuButtons

	local SocialFeed = MenuBuilder.BuildRegisteredType("SocialFeed", {
		controllerIndex = controllerIndex
	})
	SocialFeed.id = "SocialFeed"
	SocialFeed:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 1920, _1080p * 965, _1080p * 995)
	self:addElement(SocialFeed)
	self.SocialFeed = SocialFeed

	self.addButtonHelperFunction = function(arg0)
		arg0:AddButtonHelperText({
            helper_text = Engine.Localize("MENU_BACK"),
            button_ref = "button_secondary",
            side = "left",
            clickable = true
        })
        arg0:AddButtonHelperText({
			helper_text = "UNLOCK STATS",
			button_ref = "button_alt1",
			side = "left",
			clickable = true
		})
		arg0:AddButtonHelperText({
			helper_text = "RESET STATS",
			button_ref = "button_alt2",
			side = "left",
			clickable = true
		})
	end
	self:addEventHandler("menu_create", self.addButtonHelperFunction)

	local BindButton = LUI.UIBindButton.new()
	BindButton.id = "selfBindButton"
	self:addElement(BindButton)
	self.BindButton = BindButton

	self.BindButton:addEventHandler("button_secondary", function()
		Engine.Exec("uploadStats")
		ACTIONS.LeaveMenu(self)
	end)

    self.BindButton:addEventHandler("button_alt1", function(arg0, arg1)
	LUI.FlowManager.RequestPopupMenu(self, "popmenu_unlockstats", true, controllerIndex, false)
    end)
    
	self.BindButton:addEventHandler("button_alt2", function(arg0, arg1)
		LUI.FlowManager.RequestPopupMenu(self, "popmenu_resetstats", false, controllerIndex, false)
	end)

	if not Engine.IsAliensMode() then
		local Blur = LUI.UIElement.new({
			worldBlur = 5
		})
		Blur:setupWorldBlur()
		Blur.id = "blur"
		self:addElement(Blur)
	end

	return self
end)

local HeadquartersCustomizationButtons = MenuBuilder.m_types["HeadquartersCustomizationButtons"]
local HeadquartersCustomizationButtons_stub = function(menu, controller)
	local self = HeadquartersCustomizationButtons(menu, controller)

	self:SetupStackedLayout({})

	local Stats = OPTIONS.BuildGenericButton("Stats", "Stats", "Edit stats.", 0)
	Stats:registerEventHandler("button_action", function()
		ACTIONS.OpenMenu("StatsMenu", true, 0)
	end)

	self.Stats = Stats
	self.Stats:addElementBefore(self.ButtonDescription)

	return self
end

MenuBuilder.m_types["HeadquartersCustomizationButtons"] = HeadquartersCustomizationButtons_stub
