local function PostLoadFunc( f1_arg0, f1_arg1, f1_arg2 )
	assert( f1_arg0.GenericButton )
	
	f1_arg0.GenericButton:addEventHandler( "button_action", function ( f3_arg0, f3_arg1 )
		local f3_local0 = f1_arg0:GetDataSource()
		f3_local0.buttonOnClickFunction( f3_arg0, f3_arg1 )
	end )
	f1_arg0.GenericButton:addEventHandler( "button_over", function ( f4_arg0, f4_arg1 )
		local f4_local0 = f1_arg0:GetDataSource()
		f4_local0.buttonOnHoverFunction( f4_arg0, f4_arg1 )
		f4_local0 = f1_arg0:GetDataSource()
		f4_local0 = f4_local0.levelName
	end )
	f1_arg0.GenericButton:addEventHandler( "button_up", function ( f5_arg0, f5_arg1 )
		local f5_local0 = f1_arg0:GetDataSource()
		f5_local0.buttonOnHoverFunction( f5_arg0, f5_arg1 )
		f5_local0 = f1_arg0:GetDataSource()
		f5_local0 = f5_local0.levelName
	end )
	f1_arg0:registerEventHandler( "grid_anim", function ( element, event )
		element:SetAlpha( event.value )
	end )
	assert( f1_arg0.MainMissionIcon )
	assert( f1_arg0.SAIcon )
	assert( f1_arg0.JAIcon )
	f1_arg0:SubscribeToDataSourceThroughElement( f1_arg0, nil, function ()
		local f7_local0 = f1_arg0:GetDataSource()
		f7_local0 = f7_local0.levelName
	end )
end

function ModSelectButton( menu, controller )
	local self = LUI.UIButton.new()
	self:SetAnchorsAndPosition( 0, 1, 0, 1, 0, 500 * _1080p, 0, 30 * _1080p )
	self.id = "ModSelectButton"
	self._animationSets = {}
	self._sequences = {}
	local f8_local1 = controller and controller.controllerIndex
	if not f8_local1 and not Engine.InFrontend() then
		f8_local1 = self:getRootController()
	end
	assert( f8_local1 )
	local f8_local2 = self
	local GenericButton = nil
	
	GenericButton = MenuBuilder.BuildRegisteredType( "GenericButton", {
		controllerIndex = f8_local1
	} )
	GenericButton.id = "GenericButton"
	GenericButton:SetAlpha( 0, 0 )
	GenericButton:SetAnchorsAndPosition( 0, 1, 0, 0, 0, _1080p * 500, 0, 0 )
	GenericButton:SubscribeToModelThroughElement( self, "buttonLabel", function ()
		local f9_local0 = self:GetDataSource()
		f9_local0 = f9_local0.buttonLabel:GetValue( f8_local1 )
		if f9_local0 ~= nil then
			GenericButton.Text:setText( LocalizeString( ToUpperCase( f9_local0 ) ), 0 )
		end
	end )
	self:addElement( GenericButton )
	self.GenericButton = GenericButton
	
	local GenericListButtonBackground = nil
	
	GenericListButtonBackground = MenuBuilder.BuildRegisteredType( "GenericListArrowButtonBackground", {
		controllerIndex = f8_local1
	} )
	GenericListButtonBackground.id = "GenericListButtonBackground"
	GenericListButtonBackground:SetAnchorsAndPosition( 0, 0, 0, 0, 0, 0, 0, 0 )
	self:addElement( GenericListButtonBackground )
	self.GenericListButtonBackground = GenericListButtonBackground
	
	local Text = nil
	
	Text = LUI.UIStyledText.new()
	Text.id = "Text"
	Text:SetRGBFromInt( 14277081, 0 )
	Text:SetFontSize( 22 * _1080p )
	Text:SetFont( FONTS.GetFont( FONTS.MainMedium.File ) )
	Text:SetAlignment( LUI.Alignment.Left )
	Text:SetStartupDelay( 2000 )
	Text:SetLineHoldTime( 400 )
	Text:SetAnimMoveTime( 300 )
	Text:SetEndDelay( 1500 )
	Text:SetCrossfadeTime( 750 )
	Text:SetAutoScrollStyle( LUI.UIStyledText.AutoScrollStyle.ScrollH )
	Text:SetMaxVisibleLines( 1 )
	Text:SetOutlineRGBFromInt( 0, 0 )
	Text:SetAnchorsAndPosition( 0, 0, 0.5, 0.5, _1080p * 44, _1080p * -41, _1080p * -11, _1080p * 11 )
	Text:SubscribeToModelThroughElement( self, "buttonLabel", function ()
		local f10_local0 = self:GetDataSource()
		f10_local0 = f10_local0.buttonLabel:GetValue( f8_local1 )
		if f10_local0 ~= nil then
			Text:setText( LocalizeString( ToUpperCase( f10_local0 ) ), 0 )
		end
	end )
	self:addElement( Text )
	self.Text = Text
	
	local Lock = nil
	
	Lock = LUI.UIImage.new()
	Lock.id = "Lock"
	Lock:SetRGBFromTable( SWATCHES.genericButton.textDisabled, 0 )
	Lock:SetAlpha( 0, 0 )
	Lock:setImage( RegisterMaterial( "icon_slot_locked" ), 0 )
	Lock:SetAnchorsAndPosition( 1, 0, 0.5, 0.5, _1080p * -32, _1080p * -6, _1080p * -12, _1080p * 14 )
	self:addElement( Lock )
	self.Lock = Lock
	
	self._animationSets.DefaultAnimationSet = function ()
		self._sequences.DefaultSequence = function ()
			
		end
		
		Text:RegisterAnimationSequence( "ButtonOver", {
			{
				function ()
					return self.Text:SetRGBFromInt( 0, 0 )
				end
			},
			{
				function ()
					return self.Text:SetAlpha( 1, 0 )
				end
			}
		} )
		Lock:RegisterAnimationSequence( "ButtonOver", {
			{
				function ()
					return self.Lock:SetAlpha( 0, 0 )
				end
			},
			{
				function ()
					return self.Lock:SetRGBFromTable( SWATCHES.genericButton.textDisabled, 0 )
				end
			}
		} )
		self._sequences.ButtonOver = function ()
			Text:AnimateSequence( "ButtonOver" )
			Lock:AnimateSequence( "ButtonOver" )
		end
		
		Text:RegisterAnimationSequence( "ButtonUp", {
			{
				function ()
					return self.Text:SetRGBFromInt( 14277081, 0 )
				end
			}
		} )
		Lock:RegisterAnimationSequence( "ButtonUp", {
			{
				function ()
					return self.Lock:SetAlpha( 0, 0 )
				end
			}
		} )
		self._sequences.ButtonUp = function ()
			Text:AnimateSequence( "ButtonUp" )
			Lock:AnimateSequence( "ButtonUp" )
		end
		
		Text:RegisterAnimationSequence( "ButtonOverDisabled", {
			{
				function ()
					return self.Text:SetRGBFromInt( 0, 0 )
				end
			},
			{
				function ()
					return self.Text:SetAlpha( 1, 0 )
				end
			}
		} )
		Lock:RegisterAnimationSequence( "ButtonOverDisabled", {
			{
				function ()
					return self.Lock:SetAlpha( 1, 0 )
				end
			},
			{
				function ()
					return self.Lock:SetRGBFromInt( 0, 0 )
				end
			}
		} )
		self._sequences.ButtonOverDisabled = function ()
			Text:AnimateSequence( "ButtonOverDisabled" )
			Lock:AnimateSequence( "ButtonOverDisabled" )
		end
		
		Text:RegisterAnimationSequence( "ButtonUpDisabled", {
			{
				function ()
					return self.Text:SetRGBFromInt( 14277081, 0 )
				end
			}
		} )
		Lock:RegisterAnimationSequence( "ButtonUpDisabled", {
			{
				function ()
					return self.Lock:SetAlpha( 1, 0 )
				end
			},
			{
				function ()
					return self.Lock:SetRGBFromInt( 12566463, 0 )
				end
			}
		} )
		self._sequences.ButtonUpDisabled = function ()
			Text:AnimateSequence( "ButtonUpDisabled" )
			Lock:AnimateSequence( "ButtonUpDisabled" )
		end
		
	end
	
	self._animationSets.DefaultAnimationSet()
	PostLoadFunc( self, f8_local1, controller )
	return self
end

MenuBuilder.registerType( "ModSelectButton", ModSelectButton )