if Engine.GetDvarInt("bot_init") ~= 1 then
    Engine.Exec("set bot_init 1")
    Engine.Exec("set bot_allies 0")
    Engine.Exec("set bot_enemies 0")
    Engine.Exec("set bot_free 0")
    Engine.Exec("set bot_difficulty_allies 0")
    Engine.Exec("set bot_difficulty_enemies 0")
    Engine.Exec("set bot_difficulty_free 0")
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
        local map = Lobby.GetMapLoadNameByIndex(f37_arg0)
        if Engine.GetDvarBool("lui_checkIfLevelInFileSystem") and not io.zoneexists(map) then
            return false
        elseif not Engine.IsMapPackOwned(ID) then
            return false
        else
            return true
        end
    end
end

require("LobbyMissionButtons")
require("GameSetupOptions")
require("GameSetupButtonsBots")

-- Fix issue with LobbyMission being rendered when error message is shown
local orig_HandleErrors = LUI.UIRoot.HandleErrors
LUI.UIRoot.HandleErrors = function(f16_arg0)
    orig_HandleErrors(f16_arg0)
    if LUI.FlowManager.IsInStack("error_popmenu") then
        LUI.FlowManager.RequestLeaveMenuByName("LobbyMission", true)
    end
end
