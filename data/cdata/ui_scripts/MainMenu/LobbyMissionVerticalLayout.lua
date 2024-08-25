local f0_local0 = module
local f0_local1, f0_local2 = ...
f0_local0( f0_local1, package.seeall )
f0_local0 = function ( f1_arg0, f1_arg1, f1_arg2 )
	assert( f1_arg0.CAC )
	assert( f1_arg0.Barracks )
	if CONDITIONS.IsMatchSimToolEnabled( f1_arg0 ) then
		assert( f1_arg0.StartSim )
		f1_arg0.StartSim:addEventHandler( "button_action", function ( f2_arg0, f2_arg1 )
			MatchSimulator.ShowGameOverScreen()
		end )
	end
	f1_arg0.CAC:addEventHandler( "gain_focus", function ( f3_arg0, f3_arg1 )
		local f3_local0 = LUI.FlowManager.GetScopedData( f1_arg0 )
		f3_local0.mapGridIndex = 2
	end )
	f1_arg0.Barracks:addEventHandler( "gain_focus", function ( f4_arg0, f4_arg1 )
		local f4_local0 = LUI.FlowManager.GetScopedData( f1_arg0 )
		f4_local0.mapGridIndex = 0
	end )
    f1_arg0.StartButton:addEventHandler( "gain_focus", function ( f4_arg0, f4_arg1 )
		local f4_local0 = LUI.FlowManager.GetScopedData( f1_arg0 )
		f4_local0.mapGridIndex = 0
	end )
    f1_arg0.ChangeMap:addEventHandler( "gain_focus", function ( f4_arg0, f4_arg1 )
		local f4_local0 = LUI.FlowManager.GetScopedData( f1_arg0 )
		f4_local0.mapGridIndex = 0
	end )
	local f1_local0 = f1_arg0.MapVoteButtons.VoteOptions:GetGridDataSource()
	f1_local0.GetDefaultFocusIndex = function ()
		local f5_local0 = LUI.FlowManager.GetScopedData( f1_arg0 )
		return f5_local0.mapGridIndex
	end
	
end

function LobbyMissionVerticalLayout( menu, controller )
	local self = LUI.UIVerticalList.new()
	self:SetAnchorsAndPosition( 0, 1, 0, 1, 0, 600 * _1080p, 0, 565 * _1080p )
	self.id = "LobbyMissionVerticalLayout"
	self._animationSets = {}
	self._sequences = {}
	local f6_local1 = controller and controller.controllerIndex
	if not f6_local1 and not Engine.InFrontend() then
		f6_local1 = self:getRootController()
	end
	assert( f6_local1 )
	self.soundSet = "bink"
	local f6_local2 = self
	self:SetSpacing( 10 * _1080p )
	local CAC = nil

    local StartButton = nil

    StartButton = MenuBuilder.BuildRegisteredType( "GenericButton", {
		controllerIndex = f6_local1
	} )
	StartButton.id = "StartButton"
	StartButton.buttonDescription = Engine.Localize( "LUA_MENU_DESC_START_MATCH" )
	StartButton.Text:setText( ToUpperCase(Engine.Localize( "MENU_START_MATCH" ) ), 0 )
	StartButton:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150 )
	self:addElement( StartButton )
	self.StartButton = StartButton
	
	CAC = MenuBuilder.BuildRegisteredType( "CreateAClassButton", {
		controllerIndex = f6_local1
	} )
	CAC.id = "CAC"
	CAC:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, 0, _1080p * 30 )
	self:addElement( CAC )
	self.CAC = CAC

    local ChangeMap = nil

    ChangeMap = MenuBuilder.BuildRegisteredType( "GenericButton", {
		controllerIndex = f6_local1
	} )
	ChangeMap.id = "ChangeMap"
	ChangeMap.buttonDescription = Engine.Localize( "LUA_MENU_DESC_MAP" )
	ChangeMap.Text:setText( ToUpperCase(Engine.Localize( "LUA_MENU_MAP_CAPS" ) ), 0 )
	ChangeMap:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150 )
	self:addElement( ChangeMap )
	self.ChangeMap = ChangeMap
	
	local MissionTeamSelect = nil
	
	MissionTeamSelect = MenuBuilder.BuildRegisteredType( "GenericButton", {
		controllerIndex = f6_local1
	} )
	MissionTeamSelect.id = "MissionTeamSelect"
	MissionTeamSelect.buttonDescription = Engine.Localize( "LUA_MENU_DESC_BARRACKS_PLAYLIST" )
	MissionTeamSelect.Text:setText( ToUpperCase( Engine.Localize( "MENU_MISSION_TEAMS" ) ), 0 )
	MissionTeamSelect:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 40, _1080p * 70 )
	self:addElement( MissionTeamSelect )
	self.MissionTeamSelect = MissionTeamSelect
	
	local Armory = nil
	
	Armory = MenuBuilder.BuildRegisteredType( "QuartermasterButton", {
		controllerIndex = f6_local1
	} )
	Armory.id = "Armory"
	Armory:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110 )
	self:addElement( Armory )
	self.Armory = Armory
	
	local Barracks = nil
	
	Barracks = MenuBuilder.BuildRegisteredType( "GenericButton", {
		controllerIndex = f6_local1
	} )
	Barracks.id = "Barracks"
	Barracks.buttonDescription = Engine.Localize( "LUA_MENU_DESC_BARRACKS_PLAYLIST" )
	Barracks.Text:setText( Engine.Localize( "LUA_MENU_BARRACKS_CAPS" ), 0 )
	Barracks:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150 )
	self:addElement( Barracks )
	self.Barracks = Barracks

	local f6_local7 = nil
	if CONDITIONS.AreContractsEnabled( self ) then
		f6_local7 = MenuBuilder.BuildRegisteredType( "ContractsButton", {
			controllerIndex = f6_local1
		} )
		f6_local7.id = "Contracts"
		f6_local7:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 160, _1080p * 220 )
		self:addElement( f6_local7 )
		self.Contracts = f6_local7
	end
	local ButtonDescription = nil
	
	ButtonDescription = MenuBuilder.BuildRegisteredType( "ButtonDescriptionText", {
		controllerIndex = f6_local1
	} )
	ButtonDescription.id = "ButtonDescription"
	ButtonDescription.Description:SetRight( _1080p * 415, 0 )
	ButtonDescription:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 600, _1080p * 230, _1080p * 270 )
	self:addElement( ButtonDescription )
	self.ButtonDescription = ButtonDescription


	
	local Spacer = nil
	
	Spacer = LUI.UIImage.new()
	Spacer.id = "Spacer"
	Spacer:SetAlpha( 0, 0 )
	Spacer:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 511, _1080p * 280, _1080p * 310 )
	self:addElement( Spacer )
	self.Spacer = Spacer
	
	local f6_local10 = nil
	if CONDITIONS.IsMatchSimToolEnabled( self ) then
		f6_local10 = MenuBuilder.BuildRegisteredType( "GenericButton", {
			controllerIndex = f6_local1
		} )
		f6_local10.id = "StartSim"
		if CONDITIONS.IsMatchSimToolEnabled( self ) then
			
		else
			
		end
		if CONDITIONS.IsMatchSimToolEnabled( self ) then
			
		else
			
		end
		f6_local10.Text:setText( "Start Sim", 0 )
		f6_local10:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 320, _1080p * 350 )
		self:addElement( f6_local10 )
		self.StartSim = f6_local10
	end
    
	local MapVoteButtons = nil
	
	MapVoteButtons = MenuBuilder.BuildRegisteredType( "MapVoteDetails", {
		controllerIndex = f6_local1
	} )
	MapVoteButtons.id = "MapVoteButtons"
	MapVoteButtons:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 504, _1080p * 360, _1080p * 635 )
	self:addElement( MapVoteButtons )
	self.MapVoteButtons = MapVoteButtons
	
	self._animationSets.DefaultAnimationSet = function ()
		self._sequences.DefaultSequence = function ()
			
		end
		
		Armory:RegisterAnimationSequence( "ContractsKillswitched", {
			{
				function ()
					return self.Armory:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 80, _1080p * 110, 0 )
				end
			}
		} )
		Barracks:RegisterAnimationSequence( "ContractsKillswitched", {
			{
				function ()
					return self.Barracks:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 500, _1080p * 120, _1080p * 150, 0 )
				end
			}
		} )
		ButtonDescription:RegisterAnimationSequence( "ContractsKillswitched", {
			{
				function ()
					return self.ButtonDescription:SetAnchorsAndPosition( 0, 1, 0, 1, 0, _1080p * 600, _1080p * 161, _1080p * 201, 0 )
				end
			}
		} )
		self._sequences.ContractsKillswitched = function ()
			Armory:AnimateSequence( "ContractsKillswitched" )
			Barracks:AnimateSequence( "ContractsKillswitched" )
			ButtonDescription:AnimateSequence( "ContractsKillswitched" )
		end
		
	end
	
	self._animationSets.DefaultAnimationSet()
	MissionTeamSelect:addEventHandler( "button_action", function ( f13_arg0, f13_arg1 )
		ACTIONS.OpenMenu( "MissionTeamSelect", true, f13_arg1.controller or f6_local1 )
	end )
	Barracks:addEventHandler( "button_action", function ( f14_arg0, f14_arg1 )
		ACTIONS.OpenMenu( "Headquarters", true, f14_arg1.controller or f6_local1 )
	end )
    StartButton:addEventHandler( "button_action", function ( f15_arg0, f15_arg1 )
        Engine.Exec( "xblive_privatematch 1;xpartygo;xblive_privatematch 0" )
	end )
    ChangeMap:addEventHandler( "button_action", function ( f16_arg0, f16_arg1 )
       -- Engine.Exec( "xstartlobby" )
       ACTIONS.OpenMenu( "Maps", true, f16_arg1.controller or f6_local1 )
	end )
	f0_local0( self, f6_local1, controller )
	return self
end

MenuBuilder.registerType( "LobbyMissionVerticalLayout", LobbyMissionVerticalLayout )
LockTable( _M )
