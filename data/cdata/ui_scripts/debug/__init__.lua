LUI.UIElement.debugDraw = function ( f84_arg0, f84_arg1 )
	if not f84_arg1 then
		f84_arg1 = 1
	end
	f84_arg0:addElement( LUI.UIImage.new( {
		topAnchor = true,
		bottomAnchor = true,
		leftAnchor = true,
		rightAnchor = true,
		material = RegisterMaterial( "white" ),
		red = 0,
		blue = 0,
		green = 1,
		alpha = 0.2
	} ) )
	f84_arg0:addElement( LUI.UIImage.new( {
		topAnchor = true,
		bottomAnchor = false,
		leftAnchor = true,
		rightAnchor = true,
		material = RegisterMaterial( "white" ),
		red = 1,
		blue = 0,
		green = 0,
		alpha = 1,
		bottom = f84_arg1
	} ) )
	f84_arg0:addElement( LUI.UIImage.new( {
		topAnchor = true,
		bottomAnchor = true,
		leftAnchor = false,
		rightAnchor = true,
		material = RegisterMaterial( "white" ),
		red = 1,
		blue = 0,
		green = 0,
		alpha = 1,
		left = -1 * f84_arg1
	} ) )
	f84_arg0:addElement( LUI.UIImage.new( {
		topAnchor = false,
		bottomAnchor = true,
		leftAnchor = true,
		rightAnchor = true,
		material = RegisterMaterial( "white" ),
		red = 1,
		blue = 0,
		green = 0,
		alpha = 1,
		top = -1 * f84_arg1
	} ) )
	f84_arg0:addElement( LUI.UIImage.new( {
		topAnchor = true,
		bottomAnchor = true,
		leftAnchor = true,
		rightAnchor = false,
		material = RegisterMaterial( "white" ),
		red = 1,
		blue = 0,
		green = 0,
		alpha = 1,
		right = f84_arg1
	} ) )
end
