-- get rid of cod points button(s)
MenuBuilder.m_types["PurchaseCODPointsButton"] = function(menu, controller)
	local self = LUI.UIButton.new()
	self.id = "PurchaseCODPointsButton"
	return self
end

-- get rid of friends list as its not working
MenuBuilder.m_types["online_friends_widget"] = function(menu, controller)
	local self = LUI.UIElement.new( {
		topAnchor = false,
		bottomAnchor = true,
		leftAnchor = false,
		rightAnchor = true,
		right = -130,
		bottom = -28,
		width = 40,
		height = 20,
		alpha = 1
	} )
	self.id = "online_friends_widget"
	return self
end
