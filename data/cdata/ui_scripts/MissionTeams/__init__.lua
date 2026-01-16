if not Engine.InFrontend() then
    orig_FinalizeMission = LUI.PostGameManager.FinalizeMission

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

    MissionDirector.HandlePostGame = function(f25_arg0)
        local f25_local0 = MissionDirector.GetActiveMissionTeam(f25_arg0)
        local f25_local1 = MissionDirector.GetActiveMissionSlot(f25_arg0, f25_local0)
        GiveRewards(f25_arg0, f25_local0)
        Engine.SetPlayerDataEx(f25_arg0, CoD.StatsGroup.Ranked, "activeMissionComplete", -1)
        MissionDirector.GenerateMission(f25_arg0, f25_local0, f25_local1)
        Engine.ExecNow("uploadstats", f25_arg0)
    end

    GiveRewards = function(f22_arg0, f22_arg1)
        local rewardsXP = MissionDirector.GetPostMatchMissionXP(f22_arg0, f22_arg1)
        local activeMissionComplete = Engine.GetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "activeMissionComplete")
        local roundXP = MissionDirector.GetRoundXP(f22_arg0, activeMissionComplete, f22_arg1, DataSources.alwaysLoaded
            .playerData.MP.common.round.gameMode:GetValue(f22_arg0))
        Engine.SetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "missionTeams", f22_arg1, "missionXP", rewardsXP)
        local MissionTeamLevel = MissionDirector.GetMissionTeamLevel(f22_arg0, f22_arg1)
        local MaxTeamLevel = MissionDirector.GetMaxTeamLevel(f22_arg0, f22_arg1)
        if MissionTeamLevel < MaxTeamLevel then
            local MissionXPToNextLevel = MissionDirector.GetMissionXPToNextLevel(f22_arg0, f22_arg1)
            while MissionXPToNextLevel <= rewardsXP and MissionTeamLevel < MaxTeamLevel do
                local numMissionSlots = MissionDirector.GetNumMissionSlots(f22_arg0, f22_arg1)
                Engine.SetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "missionTeams", f22_arg1, "level",
                    MissionTeamLevel + 1)
                rewardsXP = rewardsXP - MissionXPToNextLevel
                if MaxTeamLevel <= MissionTeamLevel + 1 then
                    rewardsXP = 0
                end
                Engine.SetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "missionTeams", f22_arg1, "missionXP", rewardsXP)
                local numMissionSlots = MissionDirector.GetNumMissionSlots(f22_arg0, f22_arg1)
                if numMissionSlots < numMissionSlots then
                    for f22_local8 = numMissionSlots, numMissionSlots - 1, 1 do
                        MissionDirector.GenerateMission(f22_arg0, f22_arg1, f22_local8)
                    end
                end
                MissionXPToNextLevel = MissionDirector.GetMissionXPToNextLevel(f22_arg0, f22_arg1)
                MissionTeamLevel = MissionTeamLevel + 1
            end
        end
        -- printf("mission_team_level %d current_player_level %d xp_gained %d mission_team %d",
        --     MissionDirector.GetMissionTeamLevel(f22_arg0, f22_arg1), Engine.GetRankForXP(
        --         Engine.GetPlayerDataEx(f22_arg0, CoD.StatsGroup.Ranked, "progression", "playerLevel", "xp")), rewardsXP,
        --     f22_arg1)
    end
end
