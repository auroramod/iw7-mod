if Engine.InFrontend() then
	return
end

-- Multiplayer
if CONDITIONS.IsMultiplayer then
	function CONDITIONS.IsTeamChoiceAllowed(self)
		return IsCurrentGameTypeteamBased()
	end

	function CONDITIONS.IsCODCastingAllowed(self)
		return true
	end

	function CONDITIONS.IsTeamOrCodcasterChoiceAllowed(self)
		return CONDITIONS.IsTeamChoiceAllowed(self) or CONDITIONS.IsCODCastingAllowed(self)
	end

	function Lobby.IsTeamAssignmentEnabled()
		return true
	end
end

-- ZOMBIES
if CONDITIONS.IsThirdGameMode then
end

-- Singleplayer
if CONDITIONS.IsSingleplayer then
end
