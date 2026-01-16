local SetBlurHeight = function ( element, height )
	assert( height )
	local height_limited = math.min( height, 16 )
	element.ListBlur:SetAnchorsAndPosition( 0, 0, 0, 1, 0, 0, 0, _1080p * 40 + height_limited * _1080p * 35 + height_limited * _1080p * 5, 0 )
end

local PostLoadFunc = function ( element, controllerIndex, controller )
	element.SetBlurHeight = SetBlurHeight
end

function ServerBrowser( menu, controller )
	local self = LUI.UIElement.new()
	self:SetAnchorsAndPosition( 0, 1, 0, 1, 0, 1790 * _1080p, 0, 675 * _1080p )
	self.id = "ServerBrowser"
	local controllerIndex = controller and controller.controllerIndex
	if not controllerIndex and not Engine.InFrontend() then
		controllerIndex = self:getRootController()
	end
	assert( controllerIndex )

	local ListBlur = nil
	
	ListBlur = LUI.UIBlur.new()
	ListBlur.id = "ListBlur"
	ListBlur:SetBlurStrength( 2.75, 0 )
	ListBlur:SetAnchorsAndPosition( 0, 0, 0, 1, 0, 0, 0, _1080p * 40 )
	self:addElement( ListBlur )
	self.ListBlur = ListBlur
	
	local ServerList = nil
	
	ServerList = LUI.UIDataSourceGrid.new( nil, {
		maxVisibleColumns = 1,
		maxVisibleRows = 16,
		controllerIndex = controllerIndex,
		buildChild = function ()
			return MenuBuilder.BuildRegisteredType( "SystemLinkJoinMenuRow", {
				controllerIndex = controllerIndex
			} )
		end,
		wrapX = true,
		wrapY = true,
		spacingX = _1080p * 5,
		spacingY = _1080p * 5,
		columnWidth = _1080p * 1790,
		rowHeight = _1080p * 35,
		scrollingThresholdX = 1,
		scrollingThresholdY = 1,
		adjustSizeToContent = false,
		horizontalAlignment = LUI.Alignment.Left,
		verticalAlignment = LUI.Alignment.Top,
		springCoefficient = 400,
		maxVelocity = 5000
	} )
	ServerList.id = "ServerList"
	ServerList:setUseStencil( true )
	ServerList:SetAnchorsAndPosition( 0, 0, 0, 1, 0, 0, _1080p * 40, _1080p * 675 )
	self:addElement( ServerList )
	self.ServerList = ServerList
	
	local VerticalMinimalScrollbar = nil
	
	VerticalMinimalScrollbar = MenuBuilder.BuildRegisteredType( "VerticalMinimalScrollbar", {
		controllerIndex = controllerIndex
	} )
	VerticalMinimalScrollbar.id = "VerticalMinimalScrollbar"
	VerticalMinimalScrollbar:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * -16, 0, _1080p * 40, _1080p * 675 )
	self:addElement( VerticalMinimalScrollbar )
	self.VerticalMinimalScrollbar = VerticalMinimalScrollbar
	
	local ArrowUp = nil
	
	ArrowUp = MenuBuilder.BuildRegisteredType( "ArrowUp", {
		controllerIndex = controllerIndex
	} )
	ArrowUp.id = "ArrowUp"
	ArrowUp:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 878, _1080p * 898, _1080p * 676, _1080p * 716 )
	self:addElement( ArrowUp )
	self.ArrowUp = ArrowUp
	
	local ArrowDown = nil
	
	ArrowDown = MenuBuilder.BuildRegisteredType( "ArrowDown", {
		controllerIndex = controllerIndex
	} )
	ArrowDown.id = "ArrowDown"
	ArrowDown:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 713, _1080p * 733, _1080p * 675, _1080p * 715 )
	self:addElement( ArrowDown )
	self.ArrowDown = ArrowDown
	
	local ListCount = nil
	
	ListCount = LUI.UIText.new()
	ListCount.id = "ListCount"
	ListCount:setText( "1/15", 0 )
	ListCount:SetFontSize( 24 * _1080p )
	ListCount:SetFont( FONTS.GetFont( FONTS.MainMedium.File ) )
	ListCount:SetAlignment( LUI.Alignment.Center )
	ListCount:SetAnchorsAndPosition( 0, 1, 0, 1, _1080p * 733, _1080p * 878, _1080p * 683, _1080p * 707 )
	self:addElement( ListCount )
	self.ListCount = ListCount
	
	local TitleRow = nil
	
	TitleRow = MenuBuilder.BuildRegisteredType( "SystemLinkJoinMenuTitleRow", {
		controllerIndex = controllerIndex
	} )
	TitleRow.id = "TitleRow"
	TitleRow:SetAnchorsAndPosition( 0, 0, 0, 1, 0, 0, 0, _1080p * 40 )
	self:addElement( TitleRow )
	self.TitleRow = TitleRow
	
	local scrollbar = LUI.UIScrollbar.new
	local scrollbardata = {
		direction = LUI.DIRECTION.vertical,
		eventListener = VerticalMinimalScrollbar,
		startCap = VerticalMinimalScrollbar.startCap,
		endCap = VerticalMinimalScrollbar.endCap
	}
	local slider = VerticalMinimalScrollbar.sliderArea
	if slider then
		slider = VerticalMinimalScrollbar.sliderArea.slider
	end
	scrollbardata.slider = slider
	slider = VerticalMinimalScrollbar.sliderArea
	if slider then
		slider = VerticalMinimalScrollbar.sliderArea.fixedSizeSlider
	end
	scrollbardata.fixedSizeSlider = slider
	ServerList:AddScrollbar( scrollbar( scrollbardata ) )
	ServerList:AddArrow( ArrowUp )
	ServerList:AddArrow( ArrowDown )
	ServerList:AddItemNumbers( ListCount )
	PostLoadFunc( self, controllerIndex, controller )
	return self
end

MenuBuilder.m_types["ServerBrowser"] = ServerBrowser
