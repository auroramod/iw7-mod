if Engine.GetDvarInt("bot_init") ~= 1 then
	Engine.Exec("set bot_init 1")
	Engine.Exec("set bot_allies 1")
	Engine.Exec("set bot_enemies 1")
	Engine.Exec("set bot_free 1")
	Engine.Exec("set bot_difficulty_allies 1")
	Engine.Exec("set bot_difficulty_enemies 1")
	Engine.Exec("set bot_difficulty_free 1")
end

function GetMaxBotLimit()
	return 17
end

if not Engine.InFrontend() then
	return
end

Lobby.ShouldDisplayMap = function(f37_arg0, f37_arg1)
	if not (Engine.IsAliensMode() == Lobby.GetMapSupportsAliensByIdx(f37_arg0)) then
		return false
	else
		local ID = Lobby.GetMapPackForMapIndex(f37_arg0)
		if ID == Lobby.DLC1_MAP_PACK_INDEX and Engine.GetDvarBool("killswitch_dlc1_maps") then
			return false
		elseif ID == Lobby.DLC2_MAP_PACK_INDEX and not Engine.GetDvarBool("dlc2_maps_enabled") then
			return false
		elseif ID == Lobby.DLC3_MAP_PACK_INDEX and not Engine.GetDvarBool("dlc3_maps_enabled") then
			return false
		elseif ID == Lobby.DLC4_MAP_PACK_INDEX and not Engine.GetDvarBool("dlc4_maps_enabled") then
			return false
		else
			local map = Lobby.GetMapLoadNameByIndex(f37_arg0)
			if Engine.GetDvarBool("lui_checkIfLevelInFileSystem") and not Engine.IsLevelInFileSystem(map) then
				return false
			elseif not Engine.IsMapPackOwned(ID) then
				return false
			elseif
				map == "mp_dome_dusk"
				or map == "mp_permafrost2"
				or map == "mp_carnage2"
				or map == "mp_renaissance2"
			then
				return true
			elseif map == "mp_skyway" then
				return true
			elseif map == "mp_turista2" and not Engine.GetDvarBool("mp_turista2_enabled") then
				return false
			else
				return true
			end
		end
	end
end

require("LobbyMissionButtons")
require("GameSetupOptions")
require("GameSetupButtonsBots")
