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

if Loot and Loot.IsOwned then
	local IsOwned = Loot.IsOwned
	Loot.IsOwned = function(controller, lootId, ...)
		local count = IsOwned(controller, lootId, ...)
		-- 30000 (double xp token) crashes, same as stats.cpp item_quantity_stub
		if lootId ~= 30000 and Engine.GetDvarBool("cg_unlockall_loot") then
			return math.max(count, 1)
		end
		return count
	end
end
