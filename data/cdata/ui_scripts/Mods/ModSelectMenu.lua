local f0_local0 = "frontEnd.ModSelect"
f0_local1 = function ()
	WipeGlobalModelsAtPath( f0_local0 )
end

local function getmodname(path)
    local name = path
    local desc = Engine.Localize("LUA_MENU_MOD_DESC_DEFAULT", name)

    return name, desc
end

local function set_mod( modname )
	Engine.SetDvarString( "fs_game", modname )
	Engine.Exec( "vid_restart" )
end

local unload_mod = function( arg0, arg1 )
	set_mod( "" )
end

local f0_local4 = function ( f4_arg0, f4_arg1 )
	LUI.FlowManager.RequestLeaveMenu( f4_arg0 )
end

local f0_local8 = function ( f8_arg0, f8_arg1 )
	local f8_local0 = LUI.FlowManager.GetScopedData( f8_arg0 )
	if not f8_local0.currentLabel then
		f8_local0.currentLabel = ""
	end
	if not f8_local0.currentDesc then
		f8_local0.currentDesc = ""
	end
	f8_arg0:processEvent( {
		name = "menu_refresh"
	} )
	local f8_local2 = f8_arg0:GetCurrentMenu()
	assert( f8_local2.ModInfoTitle )
	f8_local2.ModInfoTitle:setText( f8_local0.currentLabel )
    assert( f8_local2.ModInfoText )
	f8_local2.ModInfoText:setText( f8_local0.currentDesc )
end

local f0_local9 = function ( f9_arg0, f9_arg1, f9_arg2 )
	set_mod( f9_arg2 )
end

local f0_local10 = function ( f11_arg0, f11_arg1, f11_arg2 )
	local f11_local0 = LUI.FlowManager.GetScopedData( f11_arg0 )
    f11_local0.currentLabel = f11_arg2.buttonLabel
	f11_local0.currentName = f11_arg2.modName
    f11_local0.currentDesc = f11_arg2.objectiveText
    f0_local8( f11_arg0, f11_arg1 )
	Engine.PlaySound( CoD.SFX.SPMinimap )
end

local f0_local12 = function ( f13_arg0, f13_arg1 )
	local f13_local0 = {}
    local mods = io.listfiles("mods/")
	for i = 1, #mods do
		local name, desc = getmodname(mods[i])
		f13_local0[#f13_local0 + 1] = {
            buttonLabel = ToUpperCase(name),
            modName = name,
            objectiveText = desc,
        }
	end
	local f13_local1 = LUI.DataSourceFromList.new( #f13_local0 )
	f13_local1.MakeDataSourceAtIndex = function ( f14_arg0, f14_arg1, f14_arg2 )
		return {
			buttonLabel = LUI.DataSourceInGlobalModel.new( f0_local0 .. ".mods." .. f14_arg1, f13_local0[f14_arg1 + 1].buttonLabel ),
			buttonOnClickFunction = function ( f15_arg0, f15_arg1 )
				f0_local9( f15_arg0, f15_arg1, f13_local0[f14_arg1 + 1].modName )
			end
			,
			buttonOnHoverFunction = function ( f16_arg0, f16_arg1 )
				f0_local10( f16_arg0, f16_arg1, f13_local0[f14_arg1 + 1] )
			end
			,
			modName = f13_local0[f14_arg1 + 1].modName
		}
	end

	assert( f13_arg0.ModSelectionList )
	f13_arg0.ModSelectionList:SetGridDataSource( f13_local1, f13_arg1 )
end

local function PostLoadFunc( f17_arg0, f17_arg1, f17_arg2 )
	assert( f17_arg0.bindButton )
	f17_arg0.bindButton:addEventHandler( "button_secondary", f0_local4 )

	local fs_game = Engine.GetDvarString( "fs_game" )
	if fs_game ~= "" then
		f17_arg0.LoadedModName:setText( "^3Loaded mod^7: " .. fs_game )
		f17_arg0.bindButton:addEventHandler( "button_alt2", unload_mod )
	else
		f17_arg0.LoadedModName:setText( "" )
	end

	--f17_arg0:addEventHandler( "menu_create", f0_local3 )
	f0_local12( f17_arg0, f17_arg1 )
	f17_arg0:addEventHandler( "gain_focus", function ( f18_arg0, f18_arg1 )
		local f18_local0 = f18_arg0.ModSelectionList
		local f18_local1 = f18_local0:GetContentOffset( LUI.DIRECTION.vertical )
		f18_local0:SetFocusedPosition( {
			x = 0,
			y = f18_local1
		}, true )
		local f18_local2 = f18_local0:GetElementAtPosition( 0, f18_local1 )
		if f18_local2 then
			f18_local2:processEvent( {
				name = "gain_focus",
				controllerIndex = f17_arg1
			} )
		end
	end )
end

function ModSelectMenu( menu, controller )
    local self = LUI.UIElement.new()
	self.id = "ModSelectMenu"

    local f20_local1 = controller and controller.controllerIndex
	if not f20_local1 and not Engine.InFrontend() then
		f20_local1 = self:getRootController()
	end
	assert( f20_local1 )

    self:playSound( "menu_open" )

	if Engine.IsSingleplayer() then
		local Background = nil
	
		Background = LUI.UIImage.new()
		Background.id = "Background"
		Background:setImage( RegisterMaterial( "sp_frontend_bink_background" ), 0 )
		self:addElement( Background )
		self.Background = Background
		
		local Bink = nil
		
		Bink = LUI.UIImage.new()
		Bink.id = "Bink"
		Bink:setImage( RegisterMaterial( "cinematic" ), 0 )
		self:addElement( Bink )
		self.Bink = Bink
	end

    local ButtonHelperBar = nil
	
	ButtonHelperBar = MenuBuilder.BuildRegisteredType( "ButtonHelperBar", {
		controllerIndex = f61_local1
	} )
	ButtonHelperBar.id = "ButtonHelperBar"
	ButtonHelperBar:SetAnchorsAndPosition( 0, 0, 1, 0, 0, 0, _1080p * -85, 0 )
	self:addElement( ButtonHelperBar )
	self.ButtonHelperBar = ButtonHelperBar

    MenuTitle = MenuBuilder.BuildRegisteredType( "MenuTitle", {
        controllerIndex = f61_local1
    } )
    MenuTitle.id = "MenuTitle"
    MenuTitle.MenuTitle:setText( ToUpperCase( Engine.Localize( "LUA_MENU_MODS" ) ), 0 )
	--MenuTitle.MenuTitle:setText( ToUpperCase( "Mods" ), 0 )
    MenuTitle.MenuBreadcrumbs:setText( ToUpperCase( "" ), 0 )
    MenuTitle.Icon:SetTop( _1080p * -28.5, 0 )
    MenuTitle.Icon:SetBottom( _1080p * 61.5, 0 )
    MenuTitle:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 96, _1080p * 1056, _1080p * 54, _1080p * 134 )
    self:addElement( MenuTitle )
    self.MenuTitle = MenuTitle

    local ModInfoTitle = nil
	
	ModInfoTitle = LUI.UIStyledText.new()
	ModInfoTitle.id = "ModInfoTitle"
	ModInfoTitle:setText( "", 0 )
	ModInfoTitle:SetFontSize( 30 * _1080p )
	ModInfoTitle:SetFont( FONTS.GetFont( FONTS.MainMedium.File ) )
	ModInfoTitle:SetAlignment( LUI.Alignment.Left )
	ModInfoTitle:SetStartupDelay( 2000 )
	ModInfoTitle:SetLineHoldTime( 400 )
	ModInfoTitle:SetAnimMoveTime( 300 )
	ModInfoTitle:SetEndDelay( 1500 )
	ModInfoTitle:SetCrossfadeTime( 750 )
	ModInfoTitle:SetAutoScrollStyle( LUI.UIStyledText.AutoScrollStyle.ScrollH )
	ModInfoTitle:SetMaxVisibleLines( 1 )
	ModInfoTitle:SetDecodeLetterLength( 15 )
	ModInfoTitle:SetDecodeMaxRandChars( 6 )
	ModInfoTitle:SetDecodeUpdatesPerLetter( 4 )
	ModInfoTitle:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 1254, _1080p * 1824, _1080p * 216, _1080p * 246 )
	self:addElement( ModInfoTitle )
	self.ModInfoTitle = ModInfoTitle
	
	local ModInfoText = nil
	
	ModInfoText = LUI.UIStyledText.new()
	ModInfoText.id = "ModInfoText"
	ModInfoText:setText( "", 0 )
	ModInfoText:SetFontSize( 20 * _1080p )
	ModInfoText:SetFont( FONTS.GetFont( FONTS.MainCondensed.File ) )
	ModInfoText:SetAlignment( LUI.Alignment.Left )
	ModInfoText:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 1254, _1080p * 1824, _1080p * 248, _1080p * 268 )
	self:addElement( ModInfoText )
	self.ModInfoText = ModInfoText

    local ModSelectionList = nil
	
	ModSelectionList = LUI.UIDataSourceGrid.new( nil, {
		maxVisibleColumns = 1,
		maxVisibleRows = 17,
		controllerIndex = f20_local1,
		buildChild = function ()
			return MenuBuilder.BuildRegisteredType( "ModSelectButton", {
				controllerIndex = f20_local1
			} )
		end,
		wrapX = true,
		wrapY = true,
		spacingX = _1080p * 10,
		spacingY = _1080p * 10,
		columnWidth = _1080p * 500,
		rowHeight = _1080p * 30,
		scrollingThresholdX = 1,
		scrollingThresholdY = 1,
		adjustSizeToContent = false,
		horizontalAlignment = LUI.Alignment.Left,
		verticalAlignment = LUI.Alignment.Top,
		springCoefficient = 600,
		maxVelocity = 5000
	} )
	ModSelectionList.id = "ModSelectionList"
	ModSelectionList:setUseStencil( false )
	ModSelectionList:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 216, _1080p * 886 )
	self:addElement( ModSelectionList )
	self.ModSelectionList = ModSelectionList

    local ArrowUp = nil
	
	ArrowUp = MenuBuilder.BuildRegisteredType( "ArrowUp", {
		controllerIndex = f20_local1
	} )
	ArrowUp.id = "ArrowUp"
	ArrowUp:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 452.5, _1080p * 472.5, _1080p * 887, _1080p * 927 )
	self:addElement( ArrowUp )
	self.ArrowUp = ArrowUp
	
	local ArrowDown = nil
	
	ArrowDown = MenuBuilder.BuildRegisteredType( "ArrowDown", {
		controllerIndex = f20_local1
	} )
	ArrowDown.id = "ArrowDown"
	ArrowDown:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 287.5, _1080p * 307.5, _1080p * 886, _1080p * 926 )
	self:addElement( ArrowDown )
	self.ArrowDown = ArrowDown
	
	local ListCount = nil
	
	ListCount = LUI.UIText.new()
	ListCount.id = "ListCount"
	ListCount:setText( "1/15", 0 )
	ListCount:SetFontSize( 24 * _1080p )
	ListCount:SetFont( FONTS.GetFont( FONTS.MainMedium.File ) )
	ListCount:SetAlignment( LUI.Alignment.Center )
	ListCount:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 307.5, _1080p * 452.5, _1080p * 894, _1080p * 918 )
	self:addElement( ListCount )
	self.ListCount = ListCount

	local LoadedModName = nil
	
	LoadedModName = LUI.UIText.new()
	LoadedModName.id = "LoadedModName"
	LoadedModName:setText( "LOADED MOD NAME", 0 )
	LoadedModName:SetFontSize( 20 * _1080p )
	LoadedModName:SetFont( FONTS.GetFont( FONTS.MainBold.File ) )
	LoadedModName:SetAlignment( LUI.Alignment.Left )
	LoadedModName:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 942, _1080p * 966 )
	self:addElement( LoadedModName )
	self.LoadedModName = LoadedModName

    ModSelectionList:AddArrow( ArrowUp )
	ModSelectionList:AddArrow( ArrowDown )
	ModSelectionList:AddItemNumbers( ListCount )

    self.addButtonHelperFunction = function ( arg0, arg1 )
		arg0:AddButtonHelperText( {
			helper_text = Engine.Localize( "MENU_BACK" ),
			button_ref = "button_secondary",
			side = "left",
			clickable = true
		} )
		local fs_game = Engine.GetDvarString( "fs_game" )
		if fs_game ~= "" then
			arg0:AddButtonHelperText( {
				helper_text = Engine.Localize( "LUA_MENU_UNLOAD" ),
				button_ref = "button_alt2",
				side = "left",
				clickable = true
			} )
		end
	end
	
	self:addEventHandler( "menu_create", self.addButtonHelperFunction )

    local bindButton = LUI.UIBindButton.new()
	bindButton.id = "selfBindButton"
	self:addElement( bindButton )
	self.bindButton = bindButton

    PostLoadFunc( self, f20_local1, controller )

    return self
end

MenuBuilder.registerType( "ModSelectMenu", ModSelectMenu )
LUI.FlowManager.RegisterStackPushBehaviour( "ModSelectMenu", PushFunc )
LUI.FlowManager.RegisterStackPopBehaviour( "ModSelectMenu", f0_local1 )