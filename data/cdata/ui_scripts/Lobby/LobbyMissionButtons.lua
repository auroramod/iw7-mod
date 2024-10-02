function CONDITIONS.IsMatchSimToolEnabled(self)
	return false
end

local set_matchmaking_dvars = function()
	Engine.Exec("xstopprivateparty")
	Engine.Exec("online_matchmaking_dedi_required 0")
	Engine.Exec("online_matchmaking_disband_on_update_failure 1")
	Engine.Exec("online_matchmaking_no_dedi_search 1")
	Engine.Exec("online_matchmaking_use_pruning 0")
	Engine.Exec("online_matchmaking_start_when_dw_is_ready 1")
	Engine.Exec("online_matchmaking_turn_on_broken_beta 0")
	Engine.Exec("online_matchmaking_allow_lobby_merging 0")
	Engine.Exec("online_matchmaking_updatehostdocs_check 0")
	Engine.Exec("online_matchmaking_do_not_use_dlc_mming 1")
	Engine.Exec("online_acquire_ds_during_intermission 0")
	Engine.Exec("pt_maxSearchTier 0")
	Engine.Exec("pt_searchConnectAttempts 0")
	Engine.Exec("mming_minplayers_to_lobby 1")
	Engine.Exec("scr_default_maxagents 24")
	Engine.Exec("pt_migrateBeforeAdvertise 0")
	Engine.Exec("party_minplayers 3")
end

local MatchSimulator = {}
MatchSimulator.ShowGameOverScreen = function()
	Engine.Exec("party_minplayers 1")
	LUI.UIRoot.BlockButtonInput(Engine.GetLuiRoot(), false, "TransitionToGame")
	Engine.Exec("exec start") -- TODO: figure out how to start public match correctly. This is terrible.
end

local LobbyMissionButtons = function(menu, controller)
	local self = LUI.UIVerticalNavigator.new()
	self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 600 * _1080p, 0, 643 * _1080p)
	self.id = "LobbyMissionButtons"
	local f2_local1 = controller and controller.controllerIndex
	if not f2_local1 and not Engine.InFrontend() then
		f2_local1 = self:getRootController()
	end
	assert(f2_local1)

	local LobbyMissionVerticalLayout = MenuBuilder.BuildRegisteredType("LobbyMissionVerticalLayout", {
		controllerIndex = f2_local1,
	})
	LobbyMissionVerticalLayout.id = "LobbyMissionVerticalLayout"
	LobbyMissionVerticalLayout:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 600, 0, _1080p * 564)
	self:addElement(LobbyMissionVerticalLayout)
	self.LobbyMissionVerticalLayout = LobbyMissionVerticalLayout

	local StartButton = MenuBuilder.BuildRegisteredType("GenericButton", {
		controllerIndex = f6_local1,
	})
	StartButton.id = "StartButton"
	StartButton.Text:setText(ToUpperCase(Engine.Localize("MENU_START_MATCH")), 0)
	StartButton.buttonDescription = Engine.Localize("MENU_START_MATCH")
	StartButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
	LUI.UIElement.addElementBefore(StartButton, LobbyMissionVerticalLayout:getChildById("CAC"))
	LobbyMissionVerticalLayout.StartButton = StartButton
	StartButton:addEventHandler("button_action", function(f14_arg0, f14_arg1)
		LUI.UIRoot.BlockButtonInput(Engine.GetLuiRoot(), false, "TransitionToGame")
		Engine.Exec("party_minplayers 1")
		Engine.Exec("exec start")
	end)

	local GameSetupButton = MenuBuilder.BuildRegisteredType("GenericButton", {
		controllerIndex = f6_local1,
	})
	GameSetupButton.id = "GameSetupButton"
	GameSetupButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_GAME_SETUP_CAPS")), 0)
	GameSetupButton.buttonDescription = Engine.Localize("LUA_MENU_DESC_GAME_SETUP")
	GameSetupButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70)
	LUI.UIElement.addElementBefore(GameSetupButton, LobbyMissionVerticalLayout:getChildById("CAC"))
	LobbyMissionVerticalLayout.GameSetupButton = GameSetupButton
	GameSetupButton:addEventHandler("button_action", function(f14_arg0, f14_arg1)
		LUI.FlowManager.RequestAddMenu("CombatTrainingGameSetup", true, f14_arg1.controller, false, {
			isSoloMode = false,
		})
	end)

	set_matchmaking_dvars()

	local CRMMain = MenuBuilder.BuildRegisteredType("CRMMain", {
		controllerIndex = f2_local1,
	})
	CRMMain.id = "CRMMain"
	CRMMain:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 6, _1080p * 317, _1080p * 529, _1080p * 723)
	self:addElement(CRMMain)
	self.CRMMain = CRMMain

	if CONDITIONS.AreContractsEnabled(self) then
		ACTIONS.AnimateSequenceByElement(self, {
			elementName = "CRMMain",
			sequenceName = "Minimize",
			elementPath = "CRMMain",
		})
	end
	return self
end

MenuBuilder.m_types["LobbyMissionButtons"] = LobbyMissionButtons
