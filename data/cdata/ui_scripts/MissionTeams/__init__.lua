local function setup_increment_dvars()
    Engine.SetDvarInt("reward_base_keys", 8)
    Engine.SetDvarInt("reward_victory_keys", 4)
    Engine.SetDvarInt("reward_keys_increment", 2) -- idk how this will work tbh

    Engine.SetDvarInt("reward_base_salv", 200)
    Engine.SetDvarInt("reward_victory_salv", 200)
    Engine.SetDvarInt("reward_salv_increment", 100) -- ^
end

setup_increment_dvars()

if not Engine.InFrontend() then
    local orig_FinalizeMission = LUI.PostGameManager.FinalizeMission

    LUI.PostGameManager.FinalizeMission = function(f11_arg0)
        if MissionDirector.GetFlowState() == MissionDirector.FlowState.PRE_INIT then
            MissionDirector.SetFlowState(MissionDirector.FlowState.MISSION_ACTIVE)
        end

        orig_FinalizeMission(f11_arg0)

        if MissionDirector.GetFlowState() == MissionDirector.FlowState.MISSION_OVER then
            MissionDirector.SetFlowState(MissionDirector.FlowState.PRE_INIT)
            for f12_local8 = 0, Engine.GetMaxControllerCount() - 1, 1 do
                if Engine.HasActiveLocalClient(f12_local8) then
                    Contracts.RunPostMatchFrontEndLogic(f12_local8)
                    MissionDirector.HandlePostGame(f12_local8)
                end
            end
        end
    end

    local GiveRewards = function(controller, mission_team)
        local rewardsXP = MissionDirector.GetPostMatchMissionXP(controller, mission_team)
        local activeMissionComplete = Engine.GetPlayerDataEx(controller, CoD.StatsGroup.Ranked, "activeMissionComplete")
        local roundXP = MissionDirector.GetRoundXP(controller, activeMissionComplete, mission_team,
            DataSources.alwaysLoaded.playerData.MP.common.round.gameMode:GetValue(controller))
        Engine.SetPlayerDataEx(controller, CoD.StatsGroup.Ranked, "missionTeams", mission_team, "missionXP", rewardsXP)
        local MissionTeamLevel = MissionDirector.GetMissionTeamLevel(controller, mission_team)
        local MaxTeamLevel = MissionDirector.GetMaxTeamLevel(controller, mission_team)

        if MissionTeamLevel < MaxTeamLevel then
            local MissionXPToNextLevel = MissionDirector.GetMissionXPToNextLevel(controller, mission_team)
            while MissionXPToNextLevel <= rewardsXP and MissionTeamLevel < MaxTeamLevel do
                local numMissionSlots = MissionDirector.GetNumMissionSlots(controller, mission_team)
                Engine.SetPlayerDataEx(controller, CoD.StatsGroup.Ranked, "missionTeams", mission_team, "level",
                    MissionTeamLevel + 1)
                rewardsXP = rewardsXP - MissionXPToNextLevel
                if MaxTeamLevel <= MissionTeamLevel + 1 then
                    rewardsXP = 0
                end
                Engine.SetPlayerDataEx(controller, CoD.StatsGroup.Ranked, "missionTeams", mission_team, "missionXP",
                    rewardsXP)
                local numMissionSlots = MissionDirector.GetNumMissionSlots(controller, mission_team)
                if numMissionSlots < numMissionSlots then
                    for f22_local8 = numMissionSlots, numMissionSlots - 1, 1 do
                        MissionDirector.GenerateMission(controller, mission_team, f22_local8)
                    end
                end
                MissionXPToNextLevel = MissionDirector.GetMissionXPToNextLevel(controller, mission_team)
                MissionTeamLevel = MissionTeamLevel + 1
            end
        end

        -- printf("mission_team_level %d current_player_level %d xp_gained %d mission_team %d",
        --     MissionDirector.GetMissionTeamLevel(f22_arg0, f22_arg1), Engine.GetRankForXP(
        --         Engine.GetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "progression", "playerLevel", "xp")), rewardsXP,
        --     f22_arg1)
    end

    --[[
    local juice =    function ( f11_arg0 )

        local controller_index = f11_arg0._controllerIndex
        if Engine.GetPlayerDataEx( controller_index, CoD.StatsGroup.Common, "round", "scoreboardType" ) == "none" then
            DebugPrint( "Scoreboard Type is None" )
            return 
        end

        DebugPrint( "LUI.PostGameManager.FinalizeMission" )
        local time_played = 0
        local f11_local2 = 0
        local f11_local3 = Engine.GetClientMatchData( "scoreboardPlayerCount" )
        local my_username = ""
        if Engine.GetUsernameByController then
            my_username = Engine.GetUsernameByController( controller_index )
            DebugPrint( "Searching for user " .. my_username )
        end
        if f11_local3 ~= nil and f11_local3 > 0 then
            for index = 1, f11_local3, 1 do
                DebugPrint( "Looking at " .. index )
                local scoreboard_type = Engine.GetPlayerDataEx( controller_index, CoD.StatsGroup.Common, "round", "scoreboardType" )
                DebugPrint( "Scoreboard Type " .. scoreboard_type )
                local player_index = Engine.GetClientMatchData( "scoreboards", ComputeScoreboardSlot( scoreboard_type, index - 1 ) )
                DebugPrint( "Player " .. player_index )
                if player_index >= 0 then
                    local player_username = Engine.GetClientMatchData( "players", player_index, "username" )
                    DebugPrint( "Found Player " .. player_username )
                    if player_username == my_username then
                        time_played = Engine.GetPlayerDataEx( controller_index, CoD.StatsGroup.Common, "round", "timePlayed" )
                        DebugPrint( "Timeplayed " .. time_played )
                        local f11_local11 = Engine.GetClientMatchData( "players", player_index, "team" )
                        local f11_local12 = Engine.GetClientMatchData( "axisScore" )
                        local f11_local13 = Engine.GetClientMatchData( "alliesScore" )
                        if f11_local11 == "axis" then
                            if f11_local13 <= f11_local12 then
                                DebugPrint( "Axis Win" )
                                f11_local2 = 1
                                break
                            end
                        elseif f11_local11 == "allies" and f11_local12 <= f11_local13 then
                            DebugPrint( "Allies Win" )
                            f11_local2 = 1
                            break
                        end
                    end
                end
            end
        end
        DebugPrint( "Post Game" .. time_played .. " Result " .. f11_local2 )
        for f11_local5 = 0, Engine.GetMaxControllerCount() - 1, 1 do
            if Engine.HasActiveLocalClient( f11_local5 ) then
                Contracts.RunEndOfMatchLogic( f11_local5 )
                MissionDirector.PostGame( f11_local5, time_played, f11_local2 )
            end
        end
        DebugPrint( " Setting Mission State to MISSION_OVER - " .. MissionDirector.FlowState.MISSION_OVER )
        MissionDirector.SetFlowState( MissionDirector.FlowState.MISSION_OVER )
        f11_arg0._finalizedMission = true
    end
    ]] --

    local ending_reasons = {"MP_DRAW", "MP_ROUND_DRAW", "MP_ROUND_WIN", "MP_ROUND_LOSS", "MP_VICTORY", "MP_DEFEAT",
                            "MP_HALFTIME", "MP_OVERTIME", "MP_ROUNDEND", "MP_INTERMISSION", "MP_SWITCHING_SIDES",
                            "MP_MATCH_BONUS_IS", "MP_MATCH_TIE", "SPLASHES_BLANK", "MP_FINAL_ROUND", "MP_MATCH_POINT"}
    local score_var_gamemodes = {"conf", "war", "hp", "koth", "dom", "sab"}
    local player_old_score = 0

    local round_end = function(f22_arg0, f22_arg1)
        if GameX.IsRankedMatch() then
            local player_score = 0

            local gamemode = GameX.GetGameMode()
            local player_stats = Game.GetPlayerScoreInfoAtRank(Game.GetPlayerTeam(), Game.GetPlayerScoreRanking())

            --[[
                this will do the job for when its needed, aka when round loss/win occurs
                this check may be true more than once cuz this callback happens 3 times, 
                but the player_old_score variable will stop us from adding more currency
            ]] --
            local unlocalized_string = ending_reasons[Game.GetOmnvar("ui_round_end_title")]
            local is_round_based = starts_with(unlocalized_string, "MP_ROUND") or IsGameTypeRoundBased(gamemode)

            if is_round_based or has_value(score_var_gamemodes, gamemode) then
                player_score = player_stats.score
            else
                player_score = player_stats.extrascore0
            end

            local currency_gain = math.floor((player_score - player_old_score) * 10 / 100)
            if currency_gain <= 0 then
                return
            end

            --custom_depot.get_function("add_currency")(InventoryCurrencyType.Parts, currency_gain)
            --custom_depot.get_function("add_currency")(InventoryCurrencyType.Credits, math.random(2, 3))

            player_old_score = player_score
            --custom_depot.get_function("save_depot_data")()
        end
    end

    local function debug_print(msg)
        print(msg)
    end

    local give_marketplace_rewards = function(controller)
        -- give a random X amount of keys * every minute for participation
        -- check if our team won the match for bonus
        -- check if our kd was above 2-3 for bonus

        --[[
        
                keys:
                - 8 keys per game
                - 4 additionally for a win
                - 1KD = 1, 1.5KD = 2, 2KD = 3

                salvage:
                - 200 per game
                - 200 more per win
                - 1KD = 100, 1.5KD = 200, 2KD = 300

        ]] --

        local keys_to_give = Engine.GetDvarInt("reward_base_keys")

        -- check if victory
        keys_to_give = keys_to_give + Engine.GetDvarInt("reward_victory_keys")

        -- randomize chances of giving bonus
        keys_to_give = keys_to_give + 4

        -- take KD and 
        keys_to_give = keys_to_give + Engine.GetDvarInt("reward_keys_increment")

        local time_played = Engine.GetPlayerDataEx(controller, CoD.StatsGroup.Common, "round", "timePlayed")
        local time_minutes_played = math.floor(time_played / 60)

        debug_print("-----------")
        debug_print("played for " .. time_played .. "minutes (" .. time_played .. ")")
        debug_print("-----------")
    end

    -- callback for when a game is done
    MissionDirector.HandlePostGame = function(controller)
        local mission_team = MissionDirector.GetActiveMissionTeam(controller)
        local mission_slot = MissionDirector.GetActiveMissionSlot(controller, mission_team)
        GiveRewards(controller, mission_team)
        Engine.SetPlayerDataEx(controller, CoD.StatsGroup.Ranked, "activeMissionComplete", -1)
        MissionDirector.GenerateMission(controller, mission_team, mission_slot)

        -- give rewards for participation, if they won, and good performance
        give_marketplace_rewards(controller)

        Engine.ExecNow("uploadstats", controller)
    end
end
