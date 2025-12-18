if not Engine.InFrontend() then
	return
end

-- GENERAL
Engine.SetDvarBool("daily_login_bonus_enabled", false)

function CONDITIONS.IsStoreAllowed(self)
	return true
end

function CODTV.IsCODTVEnabled()
	return false
end

function CONDITIONS.IsGameBattlesAllowed()
	return false
end

-- MP
if CONDITIONS.InFrontendPublicMP then
	function CONDITIONS.IsServerBrowserAllowed(self)
		return true
	end
end

-- MP PRIVATE
if CONDITIONS.InFrontend and Engine.IsCoreMode() and (IsPrivateMatch() or IsSystemLink()) then
end
