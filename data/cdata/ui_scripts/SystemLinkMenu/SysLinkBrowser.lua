serverBrowserOffsets = {
	10,
	240,
	360,
	450
}

local f0_local4 = function ( f5_arg0, f5_arg1 )
	Engine.ExecNow( "xstopprivateparty", f5_arg1.controller )
	Engine.SetDvarBool( "systemlink", false )
	Engine.SetDvarBool( "systemlink_host", false )
	Engine.StopGameMode()
	LUI.FlowManager.RequestLeaveMenu( f5_arg0, true )
end

local f0_local6 = nil
f0_local6 = function ( menu, controller, index )
	local f9_local0 = menu:Wait( 250 )
	f9_local0.onComplete = function ()
		return f0_local6( menu, controller )
	end
	
	Lobby.UpdateServerDisplayList( controller )
	Lobby.CreateServersDataModels( controller )
end

local f0_local7 = function ( f11_arg0, f11_arg1, f11_arg2 )

	assert( f11_arg0.ServerBrowser )
	assert( f11_arg0.ServerBrowser.ServerList )
	f11_arg0:setPriority( -1 )
	f11_arg0.isSignInMenu = true
	local f11_local0 = Engine.IsAliensMode()
	if f11_local0 then
		Engine.SetFrontEndSceneSection( "zm_main", 1 )
	end

	if not f11_local0 then
		f11_arg0.MenuTitle.MenuBreadcrumbs:setText( ToUpperCase( Engine.Localize( "LUA_MENU_BREADCRUMB_2_ITEMS", "IW7-Mod", "MULTIPLAYER" ) ), 0 )
	end

	f11_arg0.addButtonHelperFunction = function ( f12_arg0, f12_arg1 )
		f11_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "MENU_SB_TOOLTIP_BTN_REFRESH" ),
			button_ref = "button_alt1",
			side = "left",
			priority = 2,
			clickable = true
		} )
	end
	
	f11_arg0:addEventHandler( "menu_create", f11_arg0.addButtonHelperFunction )
	
	assert( Engine.IsUserSignedIn( f11_arg1 ) )
	if f11_local0 then
		Engine.ExecNow( CPConfig.default_systemlink, f11_arg1 )
	else
		Engine.ExecNow( MPConfig.default_systemlink, f11_arg1 )
	end
	if not Lobby.IsInPrivateParty() then
		Engine.ExecNow( "xstartprivateparty", f11_arg1 )
	end

	Engine.Exec( MPConfig.default_dvars )

	f11_arg0:addEventHandler( "menu_create", function ( f13_arg0, f13_arg1 )

		assert( f11_arg0.bindButton )

        if CONDITIONS.IsThirdGameMode( f11_arg0 ) then
            ACTIONS.AnimateSequence( f11_arg0, "DisplayServerBrowser" )
        end

		f11_arg0.bindButton:addEventHandler( "button_secondary", function ( f14_arg0, f14_arg1 )
			local f14_local0 = LUI.FlowManager.GetScopedData( f11_arg0 )
			if f14_local0.serverBrowserFocused then
				f14_local0.serverBrowserFocused = false
				ACTIONS.LoseFocus( f11_arg0, "ServerBrowser", f11_arg1 )
				ACTIONS.GainFocus( f11_arg0.SystemLinkMenuButtons, "JoinGameButton", f11_arg1 )
			else
				f0_local4( f14_arg0, f14_arg1 )
			end
		end )

		f11_arg0.bindButton:addEventHandler( "button_alt1", function ( f15_arg0, f15_arg1 )
			Lobby.RefreshServerList( f11_arg1 )
			ACTIONS.PlaySoundSetSound( f11_arg0, "selectAlt", false )
		end )
	end )
    f11_arg0.SystemLinkMenuButtons:addEventHandler( "gain_focus", function ( f16_arg0, f16_arg1 )
		local f16_local0 = LUI.FlowManager.GetScopedData( f11_arg0 )
		if f16_local0.serverBrowserFocused then
			f16_local0.serverBrowserFocused = false
			ACTIONS.LoseFocus( f11_arg0, "ServerBrowser", f11_arg1 )
			if CONDITIONS.IsThirdGameMode( f11_arg0 ) then
				ACTIONS.AnimateSequence( f11_arg0, "DisplayServerBrowser" )
				f11_arg0:RemoveButtonHelperText( "button_alt1", "left" )
			end
		end
	end )

	
	Engine.PLMRefreshData()
	if Engine.IsCoreMode() then
		FrontEndScene.CurrentMissionTeam = MissionDirector.InvalidTeamID
		FrontEndScene.SetScene( "private_lobby_menu" )
		ClientCharacter.SetCharacterVisible( FrontEndScene.ClientCharacters.Self, true )
		if Fences.OfflineDataFetched( f11_arg1 ) then
			local f11_local1 = CoD.GetPrivateLoadoutDataSource()
			local f11_local2 = f11_local1.squadMembers.head:GetValue( f11_arg1 )
			local f11_local3 = f11_local1.squadMembers.body:GetValue( f11_arg1 )
			local f11_local4 = f11_local1.squadMembers.archetype:GetValue( f11_arg1 )
			local f11_local5 = f11_local1.squadMembers.archetypeSuper:GetValue( f11_arg1 )
			FrontEndScene.SetCharacterModelsByIndex( FrontEndScene.ClientCharacters.Self, f11_local3, f11_local2 )
			FrontEndScene.SetWeaponForSuper( FrontEndScene.ClientCharacters.Self, f11_local5, f11_local4 )
			FrontEndScene.PlayIdleForSuper( FrontEndScene.ClientCharacters.Self, f11_local5, f11_local4 )
			ClientWeapon.SetWeaponIsViewModel( 0, true )
			ClientWeapon.SetWeaponVisible( 0, false )
			ClientCharacter.SetCharacterVisible( 0, true )
		end
	end
	Lobby.SetPartyUIRoot( PartyUIRoot.SYSTEM_LINK )
	local f11_local1 = LUI.FlowManager.GetScopedData( f11_arg0 )
	if f11_local1.optionsMenuOpened then
		f11_local1.optionsMenuOpened = nil
		if f11_local1.serverBrowserFocused then
			f11_local1.serverBrowserFocused = false
			f11_arg0:clearSavedState()
			if f11_local0 then
				LUI.FlowManager.ClearSavedMenuState( "CPSystemLinkMenu" )
			else
				LUI.FlowManager.ClearSavedMenuState( "MPSystemLinkMenu" )
			end
		end
	else
		f11_local1.serverBrowserFocused = false
	end
	Lobby.BuildServerList( f11_arg1 )
	Lobby.RefreshServerList( f11_arg1 )

	assert( f11_arg0.ServerBrowser )
	assert( f11_arg0.ServerBrowser.ServerList )
	local f11_local2 = LUI.DataSourceInGlobalModel.new( f0_local0 .. ".serverCount" )
	f11_arg0:SubscribeToModel( f11_local2:GetModel( f11_arg1 ), function ()
		local f17_local0 = f11_local2:GetValue( f11_arg1 )
		if f17_local0 ~= nil then
			if f17_local0 == 0 then
				local f17_local1 = LUI.FlowManager.GetScopedData( f11_arg0 )
				if f17_local1.serverBrowserFocused then
					f17_local1.serverBrowserFocused = false
					ACTIONS.LoseFocus( f11_arg0, "ServerBrowser", f11_arg1 )
				end
			end
			f11_arg0.ServerBrowser:SetBlurHeight( f17_local0 )
		end
	end )
	local f11_local3 = LUI.DataSourceFromList.new( f11_local2 )
	f11_local3.MakeDataSourceAtIndex = function ( f18_arg0, f18_arg1, f18_arg2 )
		local f18_local0 = f18_arg1 % 2 and COLORS.white or COLORS.black
		local f18_local1 = LUI.DataSourceInGlobalModel.new( f0_local0 .. "." .. f18_arg1 .. ".status" )
		return {
			index = f18_arg1,
			color = f18_local0,
			host = LUI.DataSourceInGlobalModel.new( f0_local0 .. "." .. f18_arg1 .. ".host" ),
			map = LUI.DataSourceInGlobalModel.new( f0_local0 .. "." .. f18_arg1 .. ".map" ),
			players = LUI.DataSourceInGlobalModel.new( f0_local0 .. "." .. f18_arg1 .. ".players" ),
			type = LUI.DataSourceInGlobalModel.new( f0_local0 .. "." .. f18_arg1 .. ".type" ),
			status = f18_local1:Filter( "status", function ( f19_arg0, f19_arg1 )
				local f19_local0
				if f19_arg0 == 1 then
					f19_local0 = Engine.Localize( "MENU_LOBBY" )
					if not f19_local0 then
					
					else
						return f19_local0
					end
				end
				f19_local0 = Engine.Localize( "MENU_IN_GAME_CAPS" )
			end
			 )
		}
	end
	
	f11_arg0.ServerBrowser.ServerList:SetGridDataSource( f11_local3 )
	assert( f11_arg0.bindButton )
	f11_arg0.bindButton:addEventHandler( "button_start", function ( f20_arg0, f20_arg1 )
		local f20_local0 = LUI.FlowManager.GetScopedData( f11_arg0 )
		f20_local0.optionsMenuOpened = true
		ACTIONS.OpenMenu( "OptionsMenu", true, f20_arg1.controller )
		ACTIONS.PlaySoundSetSound( f11_arg0, "selectAlt", false )
	end )
	
	local f11_local4 = f11_arg0:Wait( 250 )
	f11_local4.onComplete = function ()
		return f0_local6( f11_arg0, f11_arg1 )
	end
	
	Engine.StopMenuVideo()
end

MenuBuilder.m_types["SystemLinkMenu"] = function ( menu, controller )
	local self = LUI.UIElement.new()
	self.id = "SystemLinkMenu"
	self._animationSets = {}
	self._sequences = {}
	local f22_local1 = controller and controller.controllerIndex
	if not f22_local1 and not Engine.InFrontend() then
		f22_local1 = self:getRootController()
	end
	assert( f22_local1 )
	self.soundSet = "cpMainMenuDefault"
	self:playSound( "menu_open" )
	local f22_local2 = self
	local f22_local3 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local3 = MenuBuilder.BuildRegisteredType( "CPMenuOuterMask", {
			controllerIndex = f22_local1
		} )
		f22_local3.id = "OuterMaskTop"
		f22_local3:SetZRotation( -168, 0 )
		f22_local3:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320, _1080p * 100 )
		self:addElement( f22_local3 )
		self.OuterMaskTop = f22_local3
	end
	local f22_local4 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local4 = MenuBuilder.BuildRegisteredType( "CPMenuOuterMask", {
			controllerIndex = f22_local1
		} )
		f22_local4.id = "OuterMaskBottom"
		f22_local4:SetZRotation( -168, 0 )
		f22_local4:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205, _1080p * 215 )
		self:addElement( f22_local4 )
		self.OuterMaskBottom = f22_local4
	end
	local f22_local5 = nil
	if CONDITIONS.IsCoreMultiplayer( self ) then
		f22_local5 = MenuBuilder.BuildRegisteredType( "MenuTitle", {
			controllerIndex = f22_local1
		} )
		f22_local5.id = "MenuTitle"
		f22_local5.MenuTitle:setText( ToUpperCase( Engine.Localize( "SERVER BROWSER" ) ), 0 )
		f22_local5.MenuBreadcrumbs:setText( ToUpperCase( Engine.Localize( "EXE_LOCAL_PLAY" ) ), 0 )
		f22_local5.Icon:SetTop( _1080p * -28.5, 0 )
		f22_local5.Icon:SetBottom( _1080p * 61.5, 0 )
		f22_local5:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134 )
		self:addElement( f22_local5 )
		self.MenuTitle = f22_local5
	end
	local ButtonHelperBar = nil
	
	ButtonHelperBar = MenuBuilder.BuildRegisteredType( "ButtonHelperBar", {
		controllerIndex = f22_local1
	} )
	ButtonHelperBar.id = "ButtonHelperBar"
	ButtonHelperBar:SetAnchorsAndPosition( 0, 0, 1, 0, 0, 0, _1080p * -85, 0 )
	self:addElement( ButtonHelperBar )
	self.ButtonHelperBar = ButtonHelperBar
	
	local SocialFeed = nil
	
	SocialFeed = MenuBuilder.BuildRegisteredType( "SocialFeed", {
		controllerIndex = f22_local1
	} )
	SocialFeed.id = "SocialFeed"
	SocialFeed:SetAnchorsAndPosition( 0, 0, 1, 0, 0, 0, _1080p * -115, _1080p * -85 )
	self:addElement( SocialFeed )
	self.SocialFeed = SocialFeed
	
	local SystemLinkMenuButtons = nil
	
	SystemLinkMenuButtons = MenuBuilder.BuildRegisteredType( "SystemLinkMenuButtons", {
		controllerIndex = f22_local1
	} )
	SystemLinkMenuButtons.id = "SystemLinkMenuButtons"
	SystemLinkMenuButtons:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 438 )
	self:addElement( SystemLinkMenuButtons )
	self.SystemLinkMenuButtons = SystemLinkMenuButtons
	
	local f22_local9 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local9 = MenuBuilder.BuildRegisteredType( "LobbyMembers", {
			controllerIndex = f22_local1
		} )
		f22_local9.id = "LobbyMembers"
		f22_local9:SetAlpha( 0, 0 )
		f22_local9:SetAnchorsAndPosition( 1, 0, 0, 1, _1080p * -1790, _1080p * -1090, _1080p * 791.5, _1080p * 1435.5 )
		self:addElement( f22_local9 )
		self.LobbyMembers = f22_local9
	end
	local ServerBrowser = nil
	
	ServerBrowser = MenuBuilder.BuildRegisteredType( "ServerBrowser", {
		controllerIndex = f22_local1
	} )
	ServerBrowser.id = "ServerBrowser"
	ServerBrowser:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216, _1080p * 885 )
	self:addElement( ServerBrowser )
	self.ServerBrowser = ServerBrowser
	
	local f22_local11 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local11 = LUI.UIImage.new()
		f22_local11.id = "BloodSplat"
		f22_local11:SetAlpha( 0.5, 0 )
		f22_local11:setImage( RegisterMaterial( "cp_menu_zombies_blood_splats_large" ), 0 )
		f22_local11:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 48, _1080p * 678, _1080p * -71, _1080p * 441 )
		self:addElement( f22_local11 )
		self.BloodSplat = f22_local11
	end
	local f22_local12 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local12 = LUI.UIImage.new()
		f22_local12.id = "ZombiesLogo"
		f22_local12:setImage( RegisterMaterial( "cp_menu_zombies_logo" ), 0 )
		f22_local12:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 60, _1080p * 740, _1080p * 34, _1080p * 434 )
		self:addElement( f22_local12 )
		self.ZombiesLogo = f22_local12
	end
	local f22_local13 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local13 = LUI.UIStyledText.new()
		f22_local13.id = "ZomLanPartyShadow"
		f22_local13:SetRGBFromInt( 0, 0 )
		f22_local13:setText( Engine.Localize( "Server Browser" ), 0 )
		f22_local13:SetFontSize( 36 * _1080p )
		f22_local13:SetFont( FONTS.GetFont( FONTS.ZmClean.File ) )
		f22_local13:SetAlignment( LUI.Alignment.Center )
		f22_local13:SetOptOutRightToLeftAlignmentFlip( true )
		f22_local13:SetShadowRGBFromInt( 0, 0 )
		f22_local13:SetOutlineRGBFromInt( 0, 0 )
		f22_local13:SetDecodeLetterLength( 25 )
		f22_local13:SetDecodeMaxRandChars( 3 )
		f22_local13:SetDecodeUpdatesPerLetter( 4 )
		f22_local13:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 272, _1080p * 597.5, _1080p * 302, _1080p * 338 )
		self:addElement( f22_local13 )
		self.ZomLanPartyShadow = f22_local13
	end
	local f22_local14 = nil
	if CONDITIONS.IsThirdGameMode( self ) then
		f22_local14 = LUI.UIStyledText.new()
		f22_local14.id = "ZomLanParty"
		f22_local14:SetRGBFromInt( 14042667, 0 )
		f22_local14:setText( Engine.Localize( "Server Browser" ), 0 )
		f22_local14:SetFontSize( 36 * _1080p )
		f22_local14:SetFont( FONTS.GetFont( FONTS.ZmClean.File ) )
		f22_local14:SetAlignment( LUI.Alignment.Center )
		f22_local14:SetOptOutRightToLeftAlignmentFlip( true )
		f22_local14:SetShadowRGBFromInt( 0, 0 )
		f22_local14:SetOutlineRGBFromInt( 0, 0 )
		f22_local14:SetDecodeLetterLength( 25 )
		f22_local14:SetDecodeMaxRandChars( 3 )
		f22_local14:SetDecodeUpdatesPerLetter( 4 )
		f22_local14:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 272, _1080p * 597.5, _1080p * 302, _1080p * 338 )
		self:addElement( f22_local14 )
		self.ZomLanParty = f22_local14
	end
	local MPLobbyMembersPlayercards = nil
	
	MPLobbyMembersPlayercards = MenuBuilder.BuildRegisteredType( "MPLobbyMembersPlayercards", {
		controllerIndex = f22_local1
	} )
	MPLobbyMembersPlayercards.id = "MPLobbyMembersPlayercards"
	MPLobbyMembersPlayercards:SetAnchorsAndPosition( 0, 1, 1, 0, _1080p * 130, _1080p * 630, _1080p * -255, _1080p * -189 )
	self:addElement( MPLobbyMembersPlayercards )
	self.MPLobbyMembersPlayercards = MPLobbyMembersPlayercards
	
	local ButtonDescriptionText = nil
	
	ButtonDescriptionText = MenuBuilder.BuildRegisteredType( "ButtonDescriptionText", {
		controllerIndex = f22_local1
	} )
	ButtonDescriptionText.id = "ButtonDescriptionText"
	ButtonDescriptionText:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 550, _1080p * 334, _1080p * 366 )
	self:addElement( ButtonDescriptionText )
	self.ButtonDescriptionText = ButtonDescriptionText
	
	self._animationSets.DefaultAnimationSet = function ()
		ServerBrowser:RegisterAnimationSequence( "DefaultSequence", {
			{
				function ()
					return self.ServerBrowser:SetAlpha( 1, 0 )
				end
			},
			{
				function ()
					return self.ServerBrowser:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216, _1080p * 885, 0 )
				end
			}
		} )
		self._sequences.DefaultSequence = function ()
			ServerBrowser:AnimateSequence( "DefaultSequence" )
		end
		
		SystemLinkMenuButtons:RegisterAnimationSequence( "IntroCore", {
			{
				function ()
					return self.SystemLinkMenuButtons:SetAlpha( 1, 0 )
				end
			},
			{
				function ()
					return self.SystemLinkMenuButtons:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 438, 0 )
				end
			}
		} )
		ServerBrowser:RegisterAnimationSequence( "IntroCore", {
			{
				function ()
					return self.ServerBrowser:SetAlpha( 1, 0 )
				end
			},
			{
				function ()
					return self.ServerBrowser:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 659, _1080p * 1790, _1080p * 216, _1080p * 885, 0 )
				end
			}
		} )
		self._sequences.IntroCore = function ()
			SystemLinkMenuButtons:AnimateSequence( "IntroCore" )
			ServerBrowser:AnimateSequence( "IntroCore" )
		end
		
	end
	
	self._animationSets.ThirdGameModeAnimationSet = function ()
		self._sequences.DefaultSequence = function ()
			
		end
		
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local3:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.OuterMaskTop:SetAlpha( 0, 0 )
					end,
					function ()
						return self.OuterMaskTop:SetAlpha( 0.6, 500, LUI.EASING.outBack )
					end
				},
				{
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -635, _1080p * -215, 0 )
					end,
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -635, _1080p * -215, 500, LUI.EASING.outBack )
					end,
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320, _1080p * 100, 500, LUI.EASING.outQuadratic )
					end,
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320, _1080p * 100, 139 )
					end,
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -280, _1080p * 140, 9 )
					end,
					function ()
						return self.OuterMaskTop:SetAnchorsAndPosition( 0, 0, 0, 1, _1080p * -190, _1080p * 76, _1080p * -320, _1080p * 100, 200, LUI.EASING.outQuadratic )
					end
				}
			} )
		end
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local4:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.OuterMaskBottom:SetAlpha( 0, 0 )
					end,
					function ()
						return self.OuterMaskBottom:SetAlpha( 0.6, 500, LUI.EASING.outBack )
					end
				},
				{
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * 225, _1080p * 645, 0 )
					end,
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * 225, _1080p * 645, 500, LUI.EASING.outBack )
					end,
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205, _1080p * 215, 500, LUI.EASING.outQuadratic )
					end,
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205, _1080p * 215, 139 )
					end,
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -245, _1080p * 175, 9 )
					end,
					function ()
						return self.OuterMaskBottom:SetAnchorsAndPosition( 0, 0, 1, 0, _1080p * -83, _1080p * 183, _1080p * -205, _1080p * 215, 200, LUI.EASING.outQuadratic )
					end
				}
			} )
		end
		SystemLinkMenuButtons:RegisterAnimationSequence( "Intro", {
			{
				function ()
					return self.SystemLinkMenuButtons:SetAlpha( 0, 0 )
				end,
				function ()
					return self.SystemLinkMenuButtons:SetAlpha( 0, 1220 )
				end,
				function ()
					return self.SystemLinkMenuButtons:SetAlpha( 1, 490 )
				end
			},
			{
				function ()
					return self.SystemLinkMenuButtons:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 406, _1080p * 628, 0 )
				end
			}
		} )
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local9:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.LobbyMembers:SetAlpha( 0, 0 )
					end,
					function ()
						return self.LobbyMembers:SetAlpha( 0, 2000 )
					end
				}
			} )
		end
		ServerBrowser:RegisterAnimationSequence( "Intro", {
			{
				function ()
					return self.ServerBrowser:SetAlpha( 0, 0 )
				end
			},
			{
				function ()
					return self.ServerBrowser:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 789, _1080p * 1920, _1080p * 133, _1080p * 802, 0 )
				end
			}
		} )
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local11:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.BloodSplat:SetAlpha( 0, 0 )
					end,
					function ()
						return self.BloodSplat:SetAlpha( 0, 1000 )
					end,
					function ()
						return self.BloodSplat:SetAlpha( 0, 140 )
					end,
					function ()
						return self.BloodSplat:SetAlpha( 0.5, 10 )
					end,
					function ()
						return self.BloodSplat:SetAlpha( 0.5, 200, LUI.EASING.outQuadratic )
					end,
					function ()
						return self.BloodSplat:SetAlpha( 0.4, 650, LUI.EASING.outQuadratic )
					end
				},
				{
					function ()
						return self.BloodSplat:SetZRotation( -5, 1000 )
					end,
					function ()
						return self.BloodSplat:SetZRotation( -5, 150 )
					end,
					function ()
						return self.BloodSplat:SetZRotation( 0, 850, LUI.EASING.outQuadratic )
					end
				},
				{
					function ()
						return self.BloodSplat:playSound( "splat", false, 1150 )
					end
				},
				{
					function ()
						return self.BloodSplat:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 218.92, _1080p * 547.08, _1080p * 51.65, _1080p * 318.35, 1149 )
					end,
					function ()
						return self.BloodSplat:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 88, _1080p * 718, _1080p * -71, _1080p * 441, 70 )
					end,
					function ()
						return self.BloodSplat:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 80, _1080p * 710, _1080p * -71, _1080p * 441, 129, LUI.EASING.outQuadratic )
					end
				}
			} )
		end
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local12:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.ZombiesLogo:SetAlpha( 0, 0 )
					end,
					function ()
						return self.ZombiesLogo:SetAlpha( 0, 1000 )
					end,
					function ()
						return self.ZombiesLogo:SetAlpha( 1, 150 )
					end
				},
				{
					function ()
						return self.ZombiesLogo:SetZRotation( 5, 1000 )
					end,
					function ()
						return self.ZombiesLogo:SetZRotation( 5, 150 )
					end,
					function ()
						return self.ZombiesLogo:SetZRotation( 0, 850, LUI.EASING.outQuadratic )
					end
				},
				{
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * -429.8, _1080p * 1249.8, _1080p * -260, _1080p * 728, 1000 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 80, _1080p * 720, _1080p * 51.65, _1080p * 416.35, 149 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 34, _1080p * 434, 200 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 36, _1080p * 436, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 68, _1080p * 748, _1080p * 35.68, _1080p * 435.68, 50 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70.35, _1080p * 750.35, _1080p * 33.39, _1080p * 433.39, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70.28, _1080p * 750.28, _1080p * 31.51, _1080p * 431.51, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 68.22, _1080p * 748.22, _1080p * 34.03, _1080p * 434.03, 50 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70.64, _1080p * 750.64, _1080p * 34.02, _1080p * 434.02, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70.47, _1080p * 750.47, _1080p * 36.01, _1080p * 436.01, 50 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 68.33, _1080p * 748.33, _1080p * 35.4, _1080p * 435.4, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 68.93, _1080p * 748.93, _1080p * 32.9, _1080p * 432.9, 59 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 69.4, _1080p * 749.4, _1080p * 35.38, _1080p * 435.38, 40 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 69.73, _1080p * 749.73, _1080p * 36.61, _1080p * 436.61, 49 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 71.93, _1080p * 751.93, _1080p * 34.65, _1080p * 434.65, 50 )
					end,
					function ()
						return self.ZombiesLogo:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 70, _1080p * 750, _1080p * 34, _1080p * 434, 49, LUI.EASING.outQuadratic )
					end
				}
			} )
		end
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local13:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.ZomLanPartyShadow:SetAlpha( 0, 0 )
					end,
					function ()
						return self.ZomLanPartyShadow:SetAlpha( 0, 1710 )
					end,
					function ()
						return self.ZomLanPartyShadow:SetAlpha( 1, 290 )
					end
				}
			} )
		end
		if CONDITIONS.IsThirdGameMode( self ) then
			f22_local14:RegisterAnimationSequence( "Intro", {
				{
					function ()
						return self.ZomLanParty:SetAlpha( 0, 0 )
					end,
					function ()
						return self.ZomLanParty:SetAlpha( 0, 1220 )
					end,
					function ()
						return self.ZomLanParty:SetAlpha( 1, 490 )
					end
				},
				{
					function ()
						return self.ZomLanParty:SetRGBFromInt( 14042667, 0 )
					end
				}
			} )
		end
		MPLobbyMembersPlayercards:RegisterAnimationSequence( "Intro", {
			{
				function ()
					return self.MPLobbyMembersPlayercards:SetAlpha( 0, 0 )
				end,
				function ()
					return self.MPLobbyMembersPlayercards:SetAlpha( 0, 700 )
				end,
				function ()
					return self.MPLobbyMembersPlayercards:SetAlpha( 0, 1010 )
				end,
				function ()
					return self.MPLobbyMembersPlayercards:SetAlpha( 1, 290 )
				end
			}
		} )
		ButtonDescriptionText:RegisterAnimationSequence( "Intro", {
			{
				function ()
					return self.ButtonDescriptionText:SetAlpha( 0, 0 )
				end,
				function ()
					return self.ButtonDescriptionText:SetAlpha( 0, 1220 )
				end,
				function ()
					return self.ButtonDescriptionText:SetAlpha( 1, 490 )
				end
			},
			{
				function ()
					return self.ButtonDescriptionText:SetRGBFromTable( SWATCHES.text.primaryText, 0 )
				end
			},
			{
				function ()
					return self.ButtonDescriptionText:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 132, _1080p * 552, _1080p * 524, _1080p * 556, 0 )
				end
			}
		} )
		self._sequences.Intro = function ()
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local3:AnimateSequence( "Intro" )
			end
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local4:AnimateSequence( "Intro" )
			end
			SystemLinkMenuButtons:AnimateSequence( "Intro" )
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local9:AnimateSequence( "Intro" )
			end
			ServerBrowser:AnimateSequence( "Intro" )
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local11:AnimateSequence( "Intro" )
			end
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local12:AnimateSequence( "Intro" )
			end
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local13:AnimateSequence( "Intro" )
			end
			if CONDITIONS.IsThirdGameMode( self ) then
				f22_local14:AnimateSequence( "Intro" )
			end
			MPLobbyMembersPlayercards:AnimateSequence( "Intro" )
			ButtonDescriptionText:AnimateSequence( "Intro" )
		end
		
		ServerBrowser:RegisterAnimationSequence( "DisplayServerBrowser", {
			{
				function ()
					return self.ServerBrowser:SetAlpha( 0, 0 )
				end,
				function ()
					return self.ServerBrowser:SetAlpha( 1, 80 )
				end
			},
			{
				function ()
					return self.ServerBrowser:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 727, _1080p * 1858, _1080p * 216, _1080p * 885, 0 )
				end
			}
		} )
		self._sequences.DisplayServerBrowser = function ()
			ServerBrowser:AnimateSequence( "DisplayServerBrowser" )
		end
		
		ServerBrowser:RegisterAnimationSequence( "HideServerBrowser", {
			{
				function ()
					return self.ServerBrowser:SetAlpha( 1, 0 )
				end,
				function ()
					return self.ServerBrowser:SetAlpha( 0, 80 )
				end
			}
		} )
		self._sequences.HideServerBrowser = function ()
			ServerBrowser:AnimateSequence( "HideServerBrowser" )
		end
		
	end
	
	self._animationSets.DefaultAnimationSet()
	self.addButtonHelperFunction = function ( f117_arg0, f117_arg1 )
		f117_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_SELECT" ),
			button_ref = "button_primary",
			side = "left",
			clickable = true
		} )
		f117_arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "LUA_MENU_BACK" ),
			button_ref = "button_secondary",
			side = "left",
			priority = 1,
			clickable = true
		} )
		f117_arg0:AddButtonHelperText( {
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
	
	f0_local7( self, f22_local1, controller )
	ACTIONS.AnimateSequence( self, "DefaultSequence" )
	if CONDITIONS.IsThirdGameMode( self ) then
		ACTIONS.SetAnimationSet( self, "ThirdGameModeAnimationSet" )
		ACTIONS.AnimateSequence( self, "Intro" )
	end
	if CONDITIONS.IsCoreMultiplayer( self ) then
		ACTIONS.SetAnimationSet( self, "DefaultAnimationSet" )
		ACTIONS.AnimateSequence( self, "IntroCore" )
	end

	return self
end

LUI.FlowManager.RegisterStackPushBehaviour( "SystemLinkMenu", PushFunc )
LUI.FlowManager.RegisterStackPopBehaviour( "SystemLinkMenu", f0_local3 )