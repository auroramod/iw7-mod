local f0_local0 = function ()
	if Engine.SplitscreenPlayerCount() > 0 then
		local f1_local0 = Engine.GetFirstActiveController()
		if f1_local0 and Engine.IsUserSignedIn( f1_local0 ) then
			Engine.PLMRefreshData()
		end
	end
end

local f0_local1 = function ( f2_arg0, f2_arg1 )
	local f2_local0 = Engine.HasActiveLocalClient( f2_arg1.controller )
	if f2_local0 then
		f2_local0 = not Engine.IsActiveLocalClientPrimary( f2_arg1.controller )
	end
	if f2_local0 then
		local f2_local1 = LUI.FlowManager.GetScopedData( f2_arg0 )
		if f2_local1 and f2_local1.focusedPage and f2_local1.focusedPage > 2 then
			f2_arg0:dispatchEventToRoot( {
				name = "lobby_slide_left",
				immediate = true
			} )
		end
	elseif Lobby.IsNotAloneInPrivateParty() then
		LUI.FlowManager.RequestPopupMenu( f2_arg0, "LeaveCommonMPMainMenuPopup", true, f2_arg1.controller, false, {
            menu = f2_arg0
        } )
	else
		Lobby.LeaveCommonMPMainMenu( f2_arg0, f2_arg1.controller )
	end
	return true
end

local f0_local2 = function ( f3_arg0, f3_arg1 )
	local f3_local0 = Engine.GetFirstActiveController()
	if Engine.GetDvarBool( "xblive_competitionmatch" ) then
		Engine.SetDvarBool( "xblive_competitionmatch", false )
		Engine.Exec( "set remove_mlg_rules 1" )
	end
	Engine.ExecNow( MPConfig.default_xboxlive, f3_local0 )
end

local f0_local3 = function ( f4_arg0, f4_arg1 )
	if Lobby.IsInviteOnly( f4_arg1 ) then
		f4_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_LOBBY_ALLOW_JOIN" ),
			button_ref = "button_alt1",
			side = "left",
			clickable = true
		} )
	else
		f4_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_LOBBY_BLOCK_JOIN" ),
			button_ref = "button_alt1",
			side = "left",
			clickable = true
		} )
	end
end

local f0_local4 = function ( f5_arg0 )
	local f5_local0 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.Ranked, "postShipFlags", 3 )
	if f5_local0 == nil or f5_local0 == false then
		local f5_local1 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 1, "head" )
		local f5_local2 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 3, "head" )
		local f5_local3 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 1, "body" )
		local f5_local4 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 3, "body" )
		if f5_local1 ~= nil and f5_local1 == "head_160" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 1, "head", "head_287" )
		end
		if f5_local2 ~= nil and f5_local2 == "head_167" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 3, "head", "head_294" )
		end
		if f5_local3 ~= nil and f5_local3 == "body_90" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 1, "body", "body_217" )
		end
		if f5_local4 ~= nil and f5_local4 == "body_97" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", 3, "body", "body_224" )
		end
		local f5_local5 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head" )
		local f5_local6 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body" )
		if f5_local5 ~= nil and f5_local5 == 160 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head", 287 )
		end
		if f5_local5 ~= nil and f5_local5 == 167 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head", 294 )
		end
		if f5_local6 ~= nil and f5_local6 == 90 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body", 217 )
		end
		if f5_local6 ~= nil and f5_local6 == 97 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body", 224 )
		end
		local f5_local7 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 1, "head" )
		local f5_local8 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 3, "head" )
		local f5_local9 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 1, "body" )
		local f5_local10 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 3, "body" )
		if f5_local7 ~= nil and f5_local7 == "head_160" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 1, "head", "head_287" )
		end
		if f5_local8 ~= nil and f5_local8 == "head_167" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 3, "head", "head_294" )
		end
		if f5_local9 ~= nil and f5_local9 == "body_90" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 1, "body", "body_217" )
		end
		if f5_local10 ~= nil and f5_local10 == "body_97" then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", 3, "body", "body_224" )
		end
		local f5_local11 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "head" )
		local f5_local12 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "body" )
		if f5_local11 ~= nil and f5_local11 == 160 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "head", 287 )
		end
		if f5_local11 ~= nil and f5_local11 == 167 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "head", 294 )
		end
		if f5_local12 ~= nil and f5_local12 == 90 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "body", 217 )
		end
		if f5_local12 ~= nil and f5_local12 == 97 then
			Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "body", 224 )
		end
		local f5_local13 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head" )
		local f5_local14 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body" )
		local f5_local15 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypeSuper" )
		local f5_local16 = Engine.GetPlayerDataEx( f5_arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetype" )
		if f5_local13 ~= nil and f5_local14 ~= nil then
			FrontEndScene.SetCharacterModelsByIndex( FrontEndScene.ClientCharacters.Self, f5_local14, f5_local13 )
		end
		if f5_local15 ~= nil and f5_local16 ~= nil then
			FrontEndScene.SetWeaponForSuper( FrontEndScene.ClientCharacters.Self, f5_local15, f5_local16 )
			FrontEndScene.PlayIdleForSuper( FrontEndScene.ClientCharacters.Self, f5_local15, f5_local16 )
		end
		Engine.SetPlayerDataEx( f5_arg0, CoD.StatsGroup.Ranked, "postShipFlags", 3, true )
		Engine.ExecNow( "uploadstats", f5_arg0 )
	end
end

local f0_local5 = function ( f6_arg0 )
	f0_local4( f6_arg0 )
end

local f0_local6 = function ( f7_arg0, f7_arg1 )
	Engine.SetDvarString( "ui_version_show", "0" )
	Engine.SetDvarBool( "ui_onlineRequired", false )
	FrontEndScene.CurrentMissionTeam = MissionDirector.InvalidTeamID
	FrontEndScene.SetScene( "mp_main" )
	local f7_local0 = f7_arg1.controller
	if not f7_local0 then
		f7_local0 = Engine.GetFirstActiveController()
	end
	CheckTURequirement( f7_arg0, f7_local0 )
	local f7_local1 = DataSources.alwaysLoaded.playerData.MP.rankedloadouts.squadMembers
	local f7_local2 = Cac.GetRigFrontEndProfileAnim( f7_local1.archetype:GetValue( f7_local0 ) )
	FrontEndScene.SetCharacterModelsByIndex( FrontEndScene.ClientCharacters.Profile, f7_local1.body:GetValue( f7_local0 ), f7_local1.head:GetValue( f7_local0 ) )
	ClientCharacter.SetCharacterWeapons( FrontEndScene.ClientCharacters.Profile, nil )
	ClientCharacter.PlayCharacterAnim( FrontEndScene.ClientCharacters.Profile, f7_local2 )
	ClientCharacter.SetCharacterVisible( FrontEndScene.ClientCharacters.Profile, true )
	ClientWeapon.SetWeaponIsViewModel( 0, true )
	ClientWeapon.SetWeaponVisible( 0, false )
	f0_local2( f7_arg0, f7_arg1 )
	if Engine.GetDvarBool( "enable_unlock_sync" ) then
		local f7_local3 = {}
		for f7_local7, f7_local8 in pairs( AAR.UnlockTypes ) do
			table.insert( f7_local3, f7_local8 )
		end
		Rewards.SyncUnlocks( f7_local0, f7_local3, #f7_local3 )
	end
	if not Lobby.IsInPrivateParty() then
		Engine.ExecNow( "xstartprivateparty", f7_arg1.controller )
	end
	f7_arg0:registerEventHandler( "not_below_blocking_fence", function ( element, event )
		Engine.SetDvarBool( "cg_mlg_gamebattles_match", false )
		if not MLG.IsGameBattleMatch() then
			Lobby.WakePrivateParty( f7_local0 )
		end
	end )
	Lobby.SetPartyUIRoot( PartyUIRoot.MAIN_MENU )
	f7_arg0.nextLocation = CRM.locations.MP_MOTD
	CRM.OpenNextCRMWindow( f7_arg0 )
	f7_arg0:addEventHandler( "gain_focus", CRM.OpenNextCRMWindow )
	f7_arg0:addEventHandler( "restore_focus", CRM.OpenNextCRMWindow )
end

local f0_local7 = function ( f9_arg0, f9_arg1, f9_arg2 )
	Engine.SetDvarBool( "onlinegame", true )
	if MLG.GetGameBattleMatchId( f9_arg1 ) == GAMEBATTLES.INVALID_MATCH_ID then
		Engine.SetDvarBool( "xblive_privatematch", false )
	end
end

local f0_local8 = function ( f10_arg0, f10_arg1, f10_arg2 )
	if GAMEBATTLES.ScheduleRefreshRequest then
		GAMEBATTLES.ScheduleRefreshRequest = false
		OpenGameBattlesLobby( f10_arg1 )
	else
		local f10_local0 = GAMEBATTLES.GetCurrentMatch( f10_arg1 )
		if f10_local0 ~= nil then
			if GAMEBATTLES.MatchForfeitWinningTeamIndex ~= 0 then
				GAMEBATTLES.ShowMatchForfeitPopup( f10_arg1, f10_local0, GAMEBATTLES.MatchForfeitWinningTeamIndex )
				GAMEBATTLES.MatchForfeitWinningTeamIndex = 0
			else
				local f10_local1, f10_local2 = GAMEBATTLES.GetMatchProgressInfo( f10_local0 )
				if f10_local2 then
					local f10_local3, f10_local4, f10_local5 = GAMEBATTLES.GetMatchVictoryInfo( f10_local0 )
					if not f10_local5 then
						LUI.FlowManager.RequestPopupMenu( f10_arg0, "MLGGamebattlesMatchResultsPopup", false, f10_arg1, false, {
							controllerIndex = f10_arg1,
							gbMatch = f10_local0,
							alliesWins = f10_local3,
							axisWins = f10_local4
						} )
					end
				else
					local f10_local3 = MLG.GetGameBattleDSAcquisitionState( f10_arg1 )
					if f10_local3 == GAMEBATTLES.MLG_DS_ACQUISITION_STATE.COULD_NOT_ACQUIRE or f10_local3 == GAMEBATTLES.MLG_DS_ACQUISITION_STATE.ACQUISITION_ERROR then
						MLG.ResetGameBattleDSAcquisitionState()
						LUI.FlowManager.RequestPopupMenu( f10_arg0, "MLGGamebattlesMatchCancelledPopup", false, f10_arg1, false, {
							matchID = f10_local0.matchId
						} )
					end
				end
			end
			MLG.ResetGameBattleMatchId( f10_arg1 )
		end
	end
	assert( f10_arg0.bindButton )
	f10_arg0.isSignInMenu = true
	f10_arg0.bindButton:addEventHandler( "button_secondary", f0_local1 )
	f10_arg0:addEventHandler( "menu_create", f0_local6 )
	if not CONDITIONS.IsGameBattlesAllowed() then
		local f10_local0, f10_local1, f10_local2, f10_local3 = f10_arg0.ButtonDescription:getLocalRect()
		local f10_local4 = f10_arg0.MPMainMenuButtons.buttonSpacing
		f10_arg0.ButtonDescription:SetTop( f10_local1 - f10_local4 )
		f10_arg0.ButtonDescription:SetBottom( f10_local3 - f10_local4 )
	end
	if false == CODTV.IsCODTVEnabled() then
		local f10_local0, f10_local1, f10_local2, f10_local3 = f10_arg0.ButtonDescription:getLocalRect()
		local f10_local4 = f10_arg0.MPMainMenuButtons.buttonSpacing
		f10_arg0.ButtonDescription:SetTop( f10_local1 - f10_local4 )
		f10_arg0.ButtonDescription:SetBottom( f10_local3 - f10_local4 )
	end
	if Engine.GetDvarFloat( "r_filmGrainAtten" ) == 1 then
		Engine.SetDvarFloat( "r_filmGrainAtten", 0.25 )
		Engine.ExecNow( "profile_setFilmGrain " .. tostring( 0.25 ), f10_arg1 )
	end
	local f10_local0 = Lobby.GetNATType()
	if f10_local0 then
		if f10_local0 == "NETWORK_STRICT" and false == Engine.GetDvarBool( "strict_nat_warning_viewed" ) then
			LUI.FlowManager.RequestPopupMenu( f10_arg0, "strict_nat_warning", true, f10_arg1, false, data )
			Engine.SetDvarBool( "strict_nat_warning_viewed", true )
		end
		f10_arg0:processEvent( {
			name = "add_button_helper_text",
			button_ref = "nat",
			helper_text = Engine.Localize( "NETWORK_YOURNATTYPE", f10_local0 ),
			side = "right",
			clickable = false
		} )
	end
	Engine.SetDvarInt( "lui_mc_numGamesFinishedInLobby", 0 )
	MissionDirector.PlayTeamMusic()
	Engine.StopMenuVideo()
	local f10_local1 = LUI.FlowManager.GetScopedData( f10_arg0 )
	if CONDITIONS.IsTrialLicense() and not f10_local1.trialPopupShown then
		f10_local1.trialPopupShown = true
		LUI.FlowManager.RequestPopupMenu( f10_arg0, "TrialFullWindow", true, f10_arg1, false )
	end
	f10_arg0:addElement( Lobby.GetMPMapMaterialStreamer() )
	f0_local5( f10_arg1 )
end

function MPMainMenu( menu, controller )
	local self = LUI.UIHorizontalNavigator.new()
	self.id = "MPMainMenu"
	local f11_local1 = controller and controller.controllerIndex
	if not f11_local1 and not Engine.InFrontend() then
		f11_local1 = self:getRootController()
	end
	assert( f11_local1 )
	f0_local7( self, f11_local1, controller )
	self:playSound( "menu_open" )
	local f11_local2 = self
	local HelperBar = nil
	
	HelperBar = MenuBuilder.BuildRegisteredType( "ButtonHelperBar", {
		controllerIndex = f11_local1
	} )
	HelperBar.id = "HelperBar"
	HelperBar:SetAnchorsAndPosition( 0, 0, 1, 0, 0, 0, _1080p * -85, 0 )
	self:addElement( HelperBar )
	self.HelperBar = HelperBar
	
	local ButtonDescription = nil
	
	ButtonDescription = MenuBuilder.BuildRegisteredType( "ButtonDescriptionText", {
		controllerIndex = f11_local1
	} )
	ButtonDescription.id = "ButtonDescription"
	ButtonDescription.Description:SetRight( _1080p * 415, 0 )
	ButtonDescription:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 645, _1080p * 554, _1080p * 654 )
	self:addElement( ButtonDescription )
	self.ButtonDescription = ButtonDescription
	
	local SocialFeed = nil
	
	SocialFeed = MenuBuilder.BuildRegisteredType( "SocialFeed", {
		controllerIndex = f11_local1
	} )
	SocialFeed.id = "SocialFeed"
	SocialFeed:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 1920, _1080p * 965, _1080p * 995 )
	self:addElement( SocialFeed )
	self.SocialFeed = SocialFeed
	
	local MPMainMenuButtons = nil
	
	MPMainMenuButtons = MenuBuilder.BuildRegisteredType( "MPMainMenuButtons", {
		controllerIndex = f11_local1
	} )
	MPMainMenuButtons.id = "MPMainMenuButtons"
	MPMainMenuButtons:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 360, _1080p * 564 )
	self:addElement( MPMainMenuButtons )
	self.MPMainMenuButtons = MPMainMenuButtons
		
	local LobbyMembers = nil
	
	LobbyMembers = MenuBuilder.BuildRegisteredType( "LobbyMembers", {
		controllerIndex = f11_local1
	} )
	LobbyMembers.id = "LobbyMembers"
	LobbyMembers:SetAnchorsAndPosition( 1, 0, 0, 1, _1080p * -700, 0, _1080p * 273, _1080p * 917 )
	self:addElement( LobbyMembers )
	self.LobbyMembers = LobbyMembers
	
	local CODLogo = nil
	
	CODLogo = LUI.UIImage.new()
	CODLogo.id = "CODLogo"
	CODLogo:setImage( RegisterMaterial( "cod_logo" ), 0 )
	CODLogo:SetAnchorsAndPosition( 0, 0, 0, 0, _1080p * 108, _1080p * -1272, _1080p * 120, _1080p * -825 )
	self:addElement( CODLogo )
	self.CODLogo = CODLogo
			
	self.addButtonHelperFunction = function ( f12_arg0, f12_arg1 )
		f12_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_SELECT" ),
			button_ref = "button_primary",
			side = "left",
			clickable = true
		} )
		f12_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "MENU_BACK" ),
			button_ref = "button_secondary",
			side = "left",
			priority = 1,
			clickable = true
		} )
		f12_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_OPTIONS_CAPS" ),
			button_ref = "button_start",
			side = "left",
			priority = 4,
			clickable = true
		} )
	end
	
	self:addEventHandler( "menu_create", self.addButtonHelperFunction )
	
	local bindButton = LUI.UIBindButton.new()
	bindButton.id = "selfBindButton"
	self:addElement( bindButton )
	self.bindButton = bindButton
	
	self.bindButton:addEventHandler( "button_start", function ( f13_arg0, f13_arg1 )
		ACTIONS.OpenMenu( "OptionsMenu", true, f13_arg1.controller or f11_local1 )
	end )
	f0_local8( self, f11_local1, controller )
	return self
end

MenuBuilder.m_types["MPMainMenu"] = MPMainMenu
LUI.FlowManager.RegisterStackPushBehaviour( "MPMainMenu", f0_local0 )
-- LockTable( _M )

--- Menu Buttons ---
buttonSpacing = 40
buttonPadding = 10
f0_local2Buttons = function ( f1_arg0, f1_arg1, f1_arg2 )
	if 0 < f1_arg2 then
		local f1_local0, f1_local1, f1_local2, f1_local3 = f1_arg0:getLocalRect()
		local f1_local4 = f1_local3 - f1_local1
		f1_arg0:SetTop( f1_local1 - (f1_local4 + f1_arg1) * f1_arg2 )
		f1_arg0:SetBottom( f1_local3 - (f1_local4 + f1_arg1) * f1_arg2 )
	end
end

local f0_local3Buttons = function ( f2_arg0, f2_arg1, f2_arg2 )
	assert( f2_arg0.ConquestButton )
	local f2_local0 = not CONDITIONS.IsTrialLicense( f2_arg0 )
	if f2_local0 then
		assert( f2_arg0.CustomGameButton )
	end
	f2_arg0.buttonSpacing = _1080p * buttonSpacing
	local f2_local1 = function ()
		return Lobby.IsInPrivateParty() and not Lobby.IsPrivatePartyHost()
	end
	
	local f2_local2 = function ()
		local f4_local0 = f2_local1()
		f2_arg0.ConquestButton:SetButtonDisabled( f4_local0 )
	end
	
	local f2_local3 = LUI.DataSourceInGlobalModel.new( "frontEnd.lobby.areWeGameHost" )
	local f2_local4 = DataSources.frontEnd.lobby.memberCount
	f2_arg0:SubscribeToModel( f2_local3:GetModel( f2_arg1 ), f2_local2 )
	f2_arg0:SubscribeToModel( f2_local4:GetModel( f2_arg1 ), f2_local2 )
	f2_arg0.ConquestButton:addEventHandler( "button_action", function ( f5_arg0, f5_arg1 )
		Engine.SetDvarBool( "cg_mlg_gamebattles_match", false )
		local f5_local0 = function ()
			LUI.FlowManager.RequestAddMenu( "SystemLinkMenu", false, f5_arg1.controller, false, {}, true )
		end
		
        f5_local0()
	end )
	if f2_local0 then
		f2_arg0.CustomGameButton:addEventHandler( "button_action", function ( f8_arg0, f8_arg1 )
			OpenPrivateMatchLobby( f8_arg1 )
		end )
	end


	local f2_local5 = _1080p * buttonPadding
	local f2_local6 = 1

	if f2_arg0.CustomGameButton then
		f0_local2Buttons( f2_arg0.CustomGameButton, f2_local5, f2_local6 )
	else
		f2_local6 = f2_local6 + 1
	end

	f2_local6 = f2_local6 + 1

end

function MPMainMenuButtons( menu, controller )
	local self = LUI.UIVerticalNavigator.new()
	self:SetAnchorsAndPosition( 0, 1, 0, 1, 0, 500 * _1080p, 0, 190 * _1080p )
	self.id = "MPMainMenuButtons"
	local f11_local1 = controller and controller.controllerIndex
	if not f11_local1 and not Engine.InFrontend() then
		f11_local1 = self:getRootController()
	end
	assert( f11_local1 )
	local f11_local2 = self
	local ConquestButton = nil
	
	ConquestButton = MenuBuilder.BuildRegisteredType( "MenuButton", {
		controllerIndex = f11_local1
	} )
	ConquestButton.id = "ConquestButton"
	ConquestButton.buttonDescription = Engine.Localize( "Browse for Custom Servers" )
	ConquestButton.Text:setText( ToUpperCase( Engine.Localize( "Server Browser" ) ), 0 )
	ConquestButton:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, 0, _1080p * 30 )
	self:addElement( ConquestButton )
	self.ConquestButton = ConquestButton
	
	local f11_local5 = nil
	if not CONDITIONS.IsTrialLicense( self ) then
		f11_local5 = MenuBuilder.BuildRegisteredType( "MenuButton", {
			controllerIndex = f11_local1
		} )
		f11_local5.id = "CustomGameButton"

		if not CONDITIONS.IsTrialLicense( self ) then
			f11_local5.buttonDescription = Engine.Localize( "LUA_MENU_CUSTOM_GAME_DESC" )
		end
		f11_local5.Text:setText( Engine.Localize( "LUA_MENU_CUSTOM_GAME_CAPS" ), 0 )
		f11_local5:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110 )
		self:addElement( f11_local5 )
		self.CustomGameButton = f11_local5
	end

	
	local CRMMain = nil
	
	CRMMain = MenuBuilder.BuildRegisteredType( "CRMMain", {
		controllerIndex = f11_local1
	} )
	CRMMain.id = "CRMMain"
	CRMMain:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 6, _1080p * 317, _1080p * 385, _1080p * 579 )
	self:addElement( CRMMain )
	self.CRMMain = CRMMain
	
	f0_local3Buttons( self, f11_local1, controller )
	return self
end

MenuBuilder.m_types["MPMainMenuButtons"] = MPMainMenuButtons