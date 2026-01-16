-- get rid of cod points button(s)
MenuBuilder.m_types["PurchaseCODPointsButton"] = function(menu, controller)
	local self = LUI.UIButton.new()
	self.id = "PurchaseCODPointsButton"
	return self
end
