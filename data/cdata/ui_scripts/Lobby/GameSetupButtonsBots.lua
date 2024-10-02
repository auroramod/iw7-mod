local Bot = {}
Bot.BotTeams = {
	Friendly = 0,
	Enemy = 1,
	FFA = 2,
}
Bot.BotLimit = 17
Bot.BotDifficulties = {
	Recruit = 0,
	Regular = 1,
	Hardened = 2,
	Veteran = 3,
	Mixed = 4,
}

Bot.GetMaxBotLimit = function()
	return 17
end

Bot.GetBotsTeamLimit = function(team)
	local botcount = 0
	if team == Bot.BotTeams.Friendly then
		botcount = Engine.GetDvarInt("bot_allies")
	elseif team == Bot.BotTeams.Enemy then
		botcount = Engine.GetDvarInt("bot_enemies")
	else
		botcount = Engine.GetDvarInt("bot_free")
	end
	if tonumber(botcount) <= 0 then
		botcount = 0
	end
	return tonumber(botcount)
end

Bot.SetBotsDifficulty = function(team, difficulty)
	if team == Bot.BotTeams.Friendly then
		Engine.Exec("set bot_difficulty_allies " .. tostring(difficulty))
	elseif team == Bot.BotTeams.Enemy then
		Engine.Exec("set bot_difficulty_enemies " .. tostring(difficulty))
	else
		Engine.Exec("set bot_difficulty_free " .. tostring(difficulty))
	end
end

Bot.GetBotsDifficulty = function(team)
	local difficulty = 4
	if team == Bot.BotTeams.Friendly then
		difficulty = Engine.GetDvarInt("bot_difficulty_allies")
	elseif team == Bot.BotTeams.Enemy then
		difficulty = Engine.GetDvarInt("bot_difficulty_enemies")
	else
		difficulty = Engine.GetDvarInt("bot_difficulty_free")
	end
	if not difficulty then
		difficulty = 4
	end
	return tonumber(difficulty)
end

Bot.SetBotsTeamLimit = function(team, size)
	Lobby.SetBotsTeamLimit(team, size)
	if team == Bot.BotTeams.Friendly then
		Engine.Exec("set bot_allies " .. tostring(size))
	elseif team == Bot.BotTeams.Enemy then
		Engine.Exec("set bot_enemies " .. tostring(size))
	else
		Engine.Exec("set bot_free " .. tostring(size))
	end
end

local update_button = function(f1_arg0, f1_arg1, f1_arg2)
	f1_arg0.currentValue = f1_arg1
	f1_arg0:UpdateContent()
	if f1_arg0.autoFunctionCall then
		f1_arg0.action(f1_arg0.currentValue, f1_arg2)
	end
end

local f0_local0 = function(f1_arg0, f1_arg1, f1_arg2)
	local f1_local0 = {
		[Bot.BotTeams.FFA] = f1_arg0.FFABots,
		[Bot.BotTeams.Enemy] = f1_arg0.EnemyBots,
		[Bot.BotTeams.Friendly] = f1_arg0.FriendlyBots,
	}
	local f1_local1 = function(team, teambased)
		local f2_local0 = {}
		local MaxBotLimit = Bot.GetMaxBotLimit()
		for f2_local2 = 0, MaxBotLimit, 1 do
			table.insert(f2_local0, tostring(f2_local2))
		end
		local f2_local2 = 1 + Bot.GetBotsTeamLimit(team)
		if #f2_local0 < f2_local2 then
			Bot.SetBotsTeamLimit(team, #f2_local0 - 1)
			f2_local2 = #f2_local0
		end
		if not teambased then
			local MaxBotLimit = Bot.GetMaxBotLimit()
			if MaxBotLimit < f2_local2 then
				f2_local2 = MaxBotLimit + 1
			end
		end
		return {
			labels = f2_local0,
			action = function(f3_arg0)
				Bot.SetBotsTeamLimit(team, f3_arg0 - 1)
				if teambased and (Bot.GetBotsTeamLimit(0) + Bot.GetBotsTeamLimit(1)) >= Bot.GetMaxBotLimit() then
					local adjustteam = 0
					local otherside = 1
					if team == Bot.BotTeams.Friendly then
						adjustteam = Bot.BotTeams.Enemy
						otherside = 0
					end
					if team == Bot.BotTeams.Enemy then
						adjustteam = Bot.BotTeams.Friendly
						otherside = 1
					end
					local adjustval = Bot.GetMaxBotLimit() - Bot.GetBotsTeamLimit(otherside)
					if adjustval <= 0 then
						adjustval = 1
					end
					f1_local0[adjustteam].currentValue = adjustval

					f1_local0[adjustteam]:UpdateContent()
					Bot.SetBotsTeamLimit(adjustteam, adjustval - 1)
				end
			end,
			defaultValue = f2_local2,
			wrapAround = false,
		}
	end

	local f1_local2 = function(f4_arg0)
		local f4_local0 = {}
		table.insert(f4_local0, Engine.Localize("LUA_MENU_BOTS_RECRUIT"))
		table.insert(f4_local0, Engine.Localize("LUA_MENU_BOTS_REGULAR"))
		table.insert(f4_local0, Engine.Localize("LUA_MENU_BOTS_HARDENED"))
		table.insert(f4_local0, Engine.Localize("LUA_MENU_BOTS_VETERAN"))
		table.insert(f4_local0, Engine.Localize("LUA_MENU_BOTS_MIXED"))
		return {
			labels = f4_local0,
			action = function(f5_arg0)
				Bot.SetBotsDifficulty(f4_arg0, f5_arg0 - 1)
			end,
			defaultValue = 1 + Bot.GetBotsDifficulty(f4_arg0),
			wrapAround = true,
		}
	end

	if
		f1_arg0.FriendlyBots
		and f1_arg0.FriendlyBotsDifficulty
		and f1_arg0.EnemyBots
		and f1_arg0.EnemyBotsDifficulty
	then
		LUI.AddUIArrowTextButtonLogic(
			f1_arg0.FriendlyBots,
			f1_arg1,
			f1_local1(Bot.BotTeams.Friendly, Bot.BotTeams.Enemy)
		)
		LUI.AddUIArrowTextButtonLogic(f1_arg0.EnemyBots, f1_arg1, f1_local1(Bot.BotTeams.Enemy, Bot.BotTeams.Friendly))
		LUI.AddUIArrowTextButtonLogic(f1_arg0.FriendlyBotsDifficulty, f1_arg1, f1_local2(Bot.BotTeams.Friendly))
		LUI.AddUIArrowTextButtonLogic(f1_arg0.EnemyBotsDifficulty, f1_arg1, f1_local2(Bot.BotTeams.Enemy))
	end
	if f1_arg0.FFABots and f1_arg0.FFABotsDifficulty then
		LUI.AddUIArrowTextButtonLogic(f1_arg0.FFABots, f1_arg1, f1_local1(Bot.BotTeams.FFA))
		LUI.AddUIArrowTextButtonLogic(f1_arg0.FFABotsDifficulty, f1_arg1, f1_local2(Bot.BotTeams.FFA))
	end
	local f1_local3 = LUI.DataSourceInGlobalModel.new("frontEnd.Bot.areWeGameHost")
	f1_arg0:SubscribeToModel(f1_local3:GetModel(f1_arg1), function(f6_arg0)
		local f6_local0 = DataModel.GetModelValue(f6_arg0)
		local f6_local1 = IsSystemLink()
		if f1_arg0.FriendlyBots and f1_arg0.FriendlyBotsDifficulty then
			local f6_local2 = f1_arg0.FriendlyBots
			local f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			local f6_local4
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
			f6_local2 = f1_arg0.FriendlyBotsDifficulty
			f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
		end
		if f1_arg0.EnemyBots and f1_arg0.EnemyBotsDifficulty then
			local f6_local2 = f1_arg0.EnemyBots
			local f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			local f6_local4
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
			f6_local2 = f1_arg0.EnemyBotsDifficulty
			f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
		end
		if f1_arg0.FFABots and f1_arg0.FFABotsDifficulty then
			local f6_local2 = f1_arg0.FFABots
			local f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			local f6_local4
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
			f6_local2 = f1_arg0.FFABotsDifficulty
			f6_local3 = f6_local2
			f6_local2 = f6_local2.SetButtonDisabled
			if not f6_local0 then
				f6_local4 = not f6_local1
			else
				f6_local4 = false
			end
			f6_local2(f6_local3, false)
		end
	end)
end

local GameSetupButtonsBotsCombatTraining = function(menu, controller)
	local self = LUI.UIVerticalList.new()
	self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 600 * _1080p, 0, 280 * _1080p)
	self.id = "GameSetupButtonsBotsCombatTraining"
	local f7_local1 = controller and controller.controllerIndex
	if not f7_local1 and not Engine.InFrontend() then
		f7_local1 = self:getRootController()
	end
	assert(f7_local1)
	self:SetSpacing(10 * _1080p)
	local f7_local3 = nil
	if CONDITIONS.IsTeamBasedGameType(self) then
		f7_local3 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local3.id = "FriendlyBots"
		if CONDITIONS.IsTeamBasedGameType(self) then
			f7_local3.buttonDescription = Engine.Localize("LUA_MENU_FRIENDLY_BOTS_DESC")
		end
		f7_local3.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_FRIENDLY_BOTS_CAPS")), 0)
		f7_local3.Text:setText(ToUpperCase(""), 0)
		f7_local3:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, 0, _1080p * 30)
		self:addElement(f7_local3)
		self.FriendlyBots = f7_local3
	end
	local f7_local4 = nil
	if CONDITIONS.IsTeamBasedGameType(self) then
		f7_local4 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local4.id = "FriendlyBotsDifficulty"
		if CONDITIONS.IsTeamBasedGameType(self) then
			f7_local4.buttonDescription = Engine.Localize("LUA_MENU_FRIENDLY_BOTDIFFICULTY_DESC")
		end
		f7_local4.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_BOTDIFFICULTY_CAPS")), 0)
		f7_local4.Text:setText(ToUpperCase(""), 0)
		f7_local4:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 40, _1080p * 70)
		self:addElement(f7_local4)
		self.FriendlyBotsDifficulty = f7_local4
	end
	local f7_local5 = nil
	if CONDITIONS.IsTeamBasedGameType(self) then
		f7_local5 = LUI.UIImage.new()
		f7_local5.id = "SpacerImage"
		f7_local5:SetAlpha(0, 0)
		f7_local5:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 80, _1080p * 90)
		self:addElement(f7_local5)
		self.SpacerImage = f7_local5
	end
	local f7_local6 = nil
	if CONDITIONS.IsTeamBasedGameType(self) then
		f7_local6 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local6.id = "EnemyBots"
		if CONDITIONS.IsTeamBasedGameType(self) then
			f7_local6.buttonDescription = Engine.Localize("LUA_MENU_ENEMY_BOTS_DESC")
		end
		f7_local6.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_ENEMY_BOTS_CAPS")), 0)
		f7_local6.Text:setText(ToUpperCase(""), 0)
		f7_local6:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 100, _1080p * 130)
		self:addElement(f7_local6)
		self.EnemyBots = f7_local6
	end
	local f7_local7 = nil
	if CONDITIONS.IsTeamBasedGameType(self) then
		f7_local7 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local7.id = "EnemyBotsDifficulty"
		if CONDITIONS.IsTeamBasedGameType(self) then
			f7_local7.buttonDescription = Engine.Localize("LUA_MENU_ENEMY_BOTDIFFICULTY_DESC")
		end
		f7_local7.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_BOTDIFFICULTY_CAPS")), 0)
		f7_local7.Text:setText(ToUpperCase(""), 0)
		f7_local7:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 140, _1080p * 170)
		self:addElement(f7_local7)
		self.EnemyBotsDifficulty = f7_local7
	end
	local f7_local8 = nil
	if not CONDITIONS.IsTeamBasedGameType(self) then
		f7_local8 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local8.id = "FFABots"
		if not CONDITIONS.IsTeamBasedGameType(self) then
			f7_local8.buttonDescription = Engine.Localize("LUA_MENU_BOTS_DESC")
		end
		f7_local8.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_BOTS_CAPS")), 0)
		f7_local8.Text:setText(ToUpperCase(""), 0)
		f7_local8:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 180, _1080p * 210)
		self:addElement(f7_local8)
		self.FFABots = f7_local8
	end
	local f7_local9 = nil
	if not CONDITIONS.IsTeamBasedGameType(self) then
		f7_local9 = MenuBuilder.BuildRegisteredType("GenericArrowButton", {
			controllerIndex = f7_local1,
		})
		f7_local9.id = "FFABotsDifficulty"
		if not CONDITIONS.IsTeamBasedGameType(self) then
			f7_local9.buttonDescription = Engine.Localize("LUA_MENU_BOTDIFFICULTY_DESC")
		end
		f7_local9.Title:setText(ToUpperCase(Engine.Localize("LUA_MENU_BOTDIFFICULTY_CAPS")), 0)
		f7_local9.Text:setText(ToUpperCase(""), 0)
		f7_local9:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, _1080p * 220, _1080p * 250)
		self:addElement(f7_local9)
		self.FFABotsDifficulty = f7_local9
	end
	local ButtonDescriptionText = nil

	ButtonDescriptionText = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
		controllerIndex = f7_local1,
	})
	ButtonDescriptionText.id = "ButtonDescriptionText"
	ButtonDescriptionText:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 420, _1080p * 260, _1080p * 292)
	self:addElement(ButtonDescriptionText)
	self.ButtonDescriptionText = ButtonDescriptionText

	f0_local0(self, f7_local1, controller)
	return self
end

local GameSetupButtonsBotsCustomGames = package.loaded["frontEnd.mp.GameSetupButtonsBots"].GameSetupButtonsBots
GameSetupButtonsBotsOverride = function(menu, controller)
	if CONDITIONS.InFrontendPublicMP then
		return GameSetupButtonsBotsCombatTraining(menu, controller)
	else
		return GameSetupButtonsBotsCustomGames(menu, controller)
	end
end

MenuBuilder.registerType("GameSetupButtonsBots", GameSetupButtonsBotsOverride)
