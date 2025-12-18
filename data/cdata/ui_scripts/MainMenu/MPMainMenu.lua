LUI.FlowManager.RegisterFenceGroup("MPMainMenu", {
    "mp",
    "mpInstall",
    "onlineServices",
    "onlineData",
    "patch",
    "exchange",
    "playlists",
    "dailyReward"
})

LUI.FlowManager.RequestSetStack("MPMainMenu", {
    { name = "MainLockoutMenu" },
    { name = "MainMenu" }
})

local HandleSplitscreenRefresh = function ()
    if Engine.SplitscreenPlayerCount() > 0 then
        local activeController = Engine.GetFirstActiveController()
        if activeController and Engine.IsUserSignedIn(activeController) then
            Engine.PLMRefreshData()
        end
    end
end

local HandleBackButton = function(f2_arg0, f2_arg1)
    local f2_local0 = Engine.HasActiveLocalClient( f2_arg1.controller )
	if f2_local0 then
		f2_local0 = not Engine.IsActiveLocalClientPrimary( f2_arg1.controller )
	end
	if f2_local0 then
		local f2_local1 = LUI.FlowManager.GetScopedData( f2_arg0 )
		if f2_local1 and f2_local1.focusedPage and f2_local1.focusedPage > 2 then
			f2_arg0:dispatchEventToRoot( {
				name = "lobby_slide_left",
				immediate = true
			} )
		end
	elseif Lobby.IsNotAloneInPrivateParty() then
		if Engine.IsXB3() then
			LUI.FlowManager.RequestPopupMenu( f2_arg0, "RateLimitedLeaveCommonMPMainMenuPopup", true, f2_arg1.controller, false, {
				menu = f2_arg0,
				rateLimitIntervalMS = 5000,
				rateLimitAttempts = 3
			} )
		else
			LUI.FlowManager.RequestPopupMenu( f2_arg0, "LeaveCommonMPMainMenuPopup", true, f2_arg1.controller, false, {
				menu = f2_arg0
			} )
		end
	else
		Lobby.LeaveCommonMPMainMenu( f2_arg0, f2_arg1.controller )
	end
	return true
end

local InitializeMainMenu = function(arg0, arg1)
    local activeController = Engine.GetFirstActiveController()
    if Engine.GetDvarBool("xblive_competitionmatch") then
        Engine.SetDvarBool("xblive_competitionmatch", false)
        Engine.Exec("set remove_mlg_rules 1")
    end
    Engine.ExecNow(MPConfig.default_xboxlive, activeController)
end

local UpdatePlayerData = function(arg0)
    local postShipFlag = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.Ranked, "postShipFlags", 3)
    if postShipFlag == nil or postShipFlag == false then
        local archetypePreferences = {
            { "head", 1, "head_160", "head_287" },
            { "head", 3, "head_167", "head_294" },
            { "body", 1, "body_90", "body_217" },
            { "body", 3, "body_97", "body_224" }
        }
        
        for _, pref in ipairs(archetypePreferences) do
            local currentValue = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", pref[2], pref[1])
            if currentValue ~= nil and currentValue == pref[3] then
                Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypePreferences", pref[2], pref[1], pref[4])
            end
        end
        
        local headData = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head")
        local bodyData = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body")
        
        if headData ~= nil and (headData == 160 or headData == 167) then
            Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head", headData == 160 and 287 or 294)
        end
        if bodyData ~= nil and (bodyData == 90 or bodyData == 97) then
            Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body", bodyData == 90 and 217 or 224)
        end

        -- Same operation for private loadouts
        for _, pref in ipairs(archetypePreferences) do
            local currentValue = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", pref[2], pref[1])
            if currentValue ~= nil and currentValue == pref[3] then
                Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "archetypePreferences", pref[2], pref[1], pref[4])
            end
        end

        local privateHeadData = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "head")
        local privateBodyData = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "body")
        
        if privateHeadData ~= nil and (privateHeadData == 160 or privateHeadData == 167) then
            Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "head", privateHeadData == 160 and 287 or 294)
        end
        if privateBodyData ~= nil and (privateBodyData == 90 or privateBodyData == 97) then
            Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.PrivateLoadouts, "squadMembers", "body", privateBodyData == 90 and 217 or 224)
        end

        local characterHead = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "head")
        local characterBody = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "body")
        local characterSuper = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetypeSuper")
        local characterArchetype = Engine.GetPlayerDataEx(arg0, CoD.StatsGroup.RankedLoadouts, "squadMembers", "archetype")
        
        if characterHead ~= nil and characterBody ~= nil then
            FrontEndScene.SetCharacterModelsByIndex(FrontEndScene.ClientCharacters.Self, characterBody, characterHead)
        end
        
        if characterSuper ~= nil and characterArchetype ~= nil then
            FrontEndScene.SetWeaponForSuper(FrontEndScene.ClientCharacters.Self, characterSuper, characterArchetype)
            FrontEndScene.PlayIdleForSuper(FrontEndScene.ClientCharacters.Self, characterSuper, characterArchetype)
        end
        
        Engine.SetPlayerDataEx(arg0, CoD.StatsGroup.Ranked, "postShipFlags", 3, true)
        Engine.ExecNow("uploadstats", arg0)
    end
end

local LoadMainMenu = function(controller)
    UpdatePlayerData(controller)
end

-- Function to set up the main menu for multiplayer
local SetupMainMenu = function(menu, controller)
    -- Disable UI version display and online requirement
    Engine.SetDvarString("ui_version_show", "0")
    Engine.SetDvarBool("ui_onlineRequired", false)

    -- Set up front end scene
    FrontEndScene.CurrentMissionTeam = MissionDirector.InvalidTeamID
    FrontEndScene.SetScene("mp_main")

    -- Get controller index
    local controllerIndex = controller and controller.controller or Engine.GetFirstActiveController()

    -- Check title update requirement
    CheckTURequirement(menu, controllerIndex)

    -- Set up player character based on loadout data
    local loadoutData = DataSources.alwaysLoaded.playerData.MP.rankedloadouts.squadMembers
    local rigAnim = Cac.GetRigFrontEndProfileAnim(loadoutData.archetype:GetValue(controllerIndex))
    FrontEndScene.SetCharacterModelsByIndex(
        FrontEndScene.ClientCharacters.Profile,
        loadoutData.body:GetValue(controllerIndex),
        loadoutData.head:GetValue(controllerIndex)
    )
    ClientCharacter.SetCharacterWeapons(FrontEndScene.ClientCharacters.Profile, nil)
    ClientCharacter.PlayCharacterAnim(FrontEndScene.ClientCharacters.Profile, rigAnim)
    ClientCharacter.SetCharacterVisible(FrontEndScene.ClientCharacters.Profile, true)

    -- Set up weapon visibility
    ClientWeapon.SetWeaponIsViewModel(0, true)
    ClientWeapon.SetWeaponVisible(0, false)

    -- Initialize main menu
    InitializeMainMenu(menu, controller)

    -- Sync unlocks if enabled
    if Engine.GetDvarBool("enable_unlock_sync") then
        local unlockTypes = {}
        for _, unlockType in pairs(AAR.UnlockTypes) do
            table.insert(unlockTypes, unlockType)
        end
        Rewards.SyncUnlocks(controllerIndex, unlockTypes, #unlockTypes)
    end

    -- Start private party if not already in one
    if not Lobby.IsInPrivateParty() then
        Engine.ExecNow("xstartprivateparty", controllerIndex)
    end

    -- Handle events related to party and CRM (Customer Relationship Management)
    menu:registerEventHandler("not_below_blocking_fence", function()
        Engine.SetDvarBool("cg_mlg_gamebattles_match", false)
        if not MLG.IsGameBattleMatch() then
            Lobby.WakePrivateParty(controllerIndex)
        end
    end)

    -- Set up party UI and CRM
    Lobby.SetPartyUIRoot(PartyUIRoot.MAIN_MENU)
    menu.nextLocation = CRM.locations.MP_MOTD
    CRM.OpenNextCRMWindow(menu)
    menu:addEventHandler("gain_focus", CRM.OpenNextCRMWindow)
    menu:addEventHandler("restore_focus", CRM.OpenNextCRMWindow)
end

-- Post-load function to handle game battles and other settings
local PostLoadFunc = function(menu, controllerIndex, controller)
    if GAMEBATTLES.ScheduleRefreshRequest then
        GAMEBATTLES.ScheduleRefreshRequest = false
        OpenGameBattlesLobby(controllerIndex)
    else
        local currentMatch = GAMEBATTLES.GetCurrentMatch(controllerIndex)
        if currentMatch then
            if GAMEBATTLES.MatchForfeitWinningTeamIndex ~= 0 then
                GAMEBATTLES.ShowMatchForfeitPopup(controllerIndex, currentMatch, GAMEBATTLES.MatchForfeitWinningTeamIndex)
                GAMEBATTLES.MatchForfeitWinningTeamIndex = 0
            else
                local victoryInfo, progressInfo = GAMEBATTLES.GetMatchProgressInfo(currentMatch)
                if progressInfo then
                    local alliesWins, axisWins, matchVictory = GAMEBATTLES.GetMatchVictoryInfo(currentMatch)
                    if not matchVictory then
                        LUI.FlowManager.RequestPopupMenu(menu, "MLGGamebattlesMatchResultsPopup", false, controllerIndex, false, {
                            controllerIndex = controllerIndex,
                            gbMatch = currentMatch,
                            alliesWins = alliesWins,
                            axisWins = axisWins
                        })
                    end
                else
                    local dsAcquisitionState = MLG.GetGameBattleDSAcquisitionState(controllerIndex)
                    if dsAcquisitionState == GAMEBATTLES.MLG_DS_ACQUISITION_STATE.COULD_NOT_ACQUIRE or dsAcquisitionState == GAMEBATTLES.MLG_DS_ACQUISITION_STATE.ACQUISITION_ERROR then
                        MLG.ResetGameBattleDSAcquisitionState()
                        LUI.FlowManager.RequestPopupMenu(menu, "MLGGamebattlesMatchCancelledPopup", false, controllerIndex, false, {
                            matchID = currentMatch.matchId
                        })
                    end
                end
            end
            MLG.ResetGameBattleMatchId(controllerIndex)
        end
    end

    -- Additional setup and event handling
    assert(menu.bindButton)
    menu.isSignInMenu = true
    menu.bindButton:addEventHandler("button_secondary", HandleBackButton)
    menu:addEventHandler("menu_create", SetupMainMenu)

    -- Adjust UI elements based on various conditions
    local adjustButtonDescription = function()
        local _, top, _, bottom = menu.ButtonDescription:getLocalRect()
        local buttonSpacing = menu.MPMainMenuButtons.buttonSpacing
        menu.ButtonDescription:SetTop(top - buttonSpacing)
        menu.ButtonDescription:SetBottom(bottom - buttonSpacing)
    end

    if not CONDITIONS.IsGameBattlesAllowed() then
        adjustButtonDescription()
    end

    if not CODTV.IsCODTVEnabled() then
        adjustButtonDescription()
    end

    if not CONDITIONS.IsStoreAllowed(menu) then
        adjustButtonDescription()
    end

    if Engine.GetDvarFloat("r_filmGrainAtten") == 1 then
        Engine.SetDvarFloat("r_filmGrainAtten", 0.25)
        Engine.ExecNow("profile_setFilmGrain " .. tostring(0.25), controllerIndex)
    end

    -- Handle NAT type warning
    local natType = Lobby.GetNATType()
    if natType then
        if natType == "NETWORK_STRICT" and not Engine.GetDvarBool("strict_nat_warning_viewed") then
            LUI.FlowManager.RequestPopupMenu(menu, "strict_nat_warning", true, controllerIndex, false, data)
            Engine.SetDvarBool("strict_nat_warning_viewed", true)
        end
        menu:processEvent({
            name = "add_button_helper_text",
            button_ref = "nat",
            helper_text = Engine.Localize("NETWORK_YOURNATTYPE", natType),
            side = "right",
            clickable = false
        })
    end

    Engine.SetDvarInt("lui_mc_numGamesFinishedInLobby", 0)
    MissionDirector.PlayTeamMusic()
    Engine.StopMenuVideo()

    -- Handle trial license popup
    local scopedData = LUI.FlowManager.GetScopedData(menu)
    if CONDITIONS.IsTrialLicense() and not scopedData.trialPopupShown then
        scopedData.trialPopupShown = true
        LUI.FlowManager.RequestPopupMenu(menu, "TrialFullWindow", true, controllerIndex, false)
    end

    menu:addElement(Lobby.GetMPMapMaterialStreamer())
    LoadMainMenu(controllerIndex)
end

function MPMainMenu(menu, controller)
    local self = LUI.UIHorizontalNavigator.new()
    self.id = "MPMainMenu"

    -- Determine controller index
    local controllerIndex = controller and controller.controllerIndex
    if not controllerIndex and not Engine.InFrontend() then
        controllerIndex = self:getRootController()
    end
    assert(controllerIndex)

    -- Initialize the main menu
    InitializeMainMenu(self, controllerIndex, controller)
    self:playSound("menu_open")

    -- Helper bar
    local HelperBar = MenuBuilder.BuildRegisteredType("ButtonHelperBar", {
        controllerIndex = controllerIndex
    })
    HelperBar.id = "HelperBar"
    HelperBar:SetAnchorsAndPosition(0, 0, 1, 0, 0, 0, _1080p * -85, 0)
    self:addElement(HelperBar)
    self.HelperBar = HelperBar

    -- Button description
    local ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
        controllerIndex = controllerIndex
    })
    ButtonDescription.id = "ButtonDescription"
    ButtonDescription.Description:SetRight(_1080p * 415, 0)
    ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 645, _1080p * 594, _1080p * 694)
    self:addElement(ButtonDescription)
    self.ButtonDescription = ButtonDescription

    -- Social feed
    local SocialFeed = MenuBuilder.BuildRegisteredType("SocialFeed", {
        controllerIndex = controllerIndex
    })
    SocialFeed.id = "SocialFeed"
    SocialFeed:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 1920, _1080p * 965, _1080p * 995)
    self:addElement(SocialFeed)
    self.SocialFeed = SocialFeed

    -- Main menu buttons
    local MPMainMenuButtons = MenuBuilder.BuildRegisteredType("MPMainMenuButtons", {
        controllerIndex = controllerIndex
    })
    MPMainMenuButtons.id = "MPMainMenuButtons"
    MPMainMenuButtons:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 630, _1080p * 360, _1080p * 564)
    self:addElement(MPMainMenuButtons)
    self.MPMainMenuButtons = MPMainMenuButtons

    -- Friends widget
    local FriendsElement = MenuBuilder.BuildRegisteredType("online_friends_widget", {
        controllerIndex = controllerIndex
    })
    FriendsElement.id = "FriendsElement"
    FriendsElement:SetFont(FONTS.GetFont(FONTS.Dev.File))
    FriendsElement:SetAlignment(LUI.Alignment.Left)
    FriendsElement:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 1420, _1080p * 1920, _1080p * 1026, _1080p * 1071)
    self:addElement(FriendsElement)
    self.FriendsElement = FriendsElement

    -- Lobby members list
    local LobbyMembers = MenuBuilder.BuildRegisteredType("LobbyMembers", {
        controllerIndex = controllerIndex
    })
    LobbyMembers.id = "LobbyMembers"
    LobbyMembers:SetAnchorsAndPosition(1, 0, 0, 1, _1080p * -700, 0, _1080p * 273, _1080p * 917)
    self:addElement(LobbyMembers)
    self.LobbyMembers = LobbyMembers

    -- COD logo
    local CODLogo = LUI.UIImage.new()
    CODLogo.id = "CODLogo"
    CODLogo:setImage(RegisterMaterial("cod_logo"), 0)
    CODLogo:SetAnchorsAndPosition(0, 0, 0, 0, _1080p * 108, _1080p * -1272, _1080p * 120, _1080p * -825)
    self:addElement(CODLogo)
    self.CODLogo = CODLogo

    -- Menu title
    local MenuTitle = LUI.UIStyledText.new()
    MenuTitle.id = "MenuTitle"
    MenuTitle:setText(ToUpperCase(Engine.Localize("MENU_MULTIPLAYER")), 0)
    MenuTitle:SetFontSize(50 * _1080p)
    MenuTitle:SetFont(FONTS.GetFont(FONTS.MainMedium.File))
    MenuTitle:SetAlignment(LUI.Alignment.Left)
    MenuTitle:SetStartupDelay(1250)
    MenuTitle:SetLineHoldTime(400)
    MenuTitle:SetAnimMoveTime(300)
    MenuTitle:SetEndDelay(1000)
    MenuTitle:SetCrossfadeTime(500)
    MenuTitle:SetAutoScrollStyle(LUI.UIStyledText.AutoScrollStyle.ScrollH)
    MenuTitle:SetMaxVisibleLines(1)
    MenuTitle:SetDecodeLetterLength(25)
    MenuTitle:SetDecodeMaxRandChars(3)
    MenuTitle:SetDecodeUpdatesPerLetter(4)
    MenuTitle:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 130, _1080p * 660, _1080p * 296.5, _1080p * 346.5)
    self:addElement(MenuTitle)
    self.MenuTitle = MenuTitle

    -- Double XP notifications
    local DoubleXPNotifications = MenuBuilder.BuildRegisteredType("DoubleXPNotifications", {
        controllerIndex = controllerIndex
    })
    DoubleXPNotifications.id = "DoubleXPNotifications"
    DoubleXPNotifications:SetScale(-0.5, 0)
    DoubleXPNotifications:SetAnchorsAndPosition(0.5, 0.5, 0, 1, _1080p * -370, _1080p * -114, _1080p * 104, _1080p * 232)
    self:addElement(DoubleXPNotifications)
    self.DoubleXPNotifications = DoubleXPNotifications

    -- Button helper function
    self.addButtonHelperFunction = function(arg0, arg1)
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("LUA_MENU_SELECT"),
            button_ref = "button_primary",
            side = "left",
            clickable = true
        })
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("MENU_BACK"),
            button_ref = "button_secondary",
            side = "left",
            priority = 1,
            clickable = true
        })
        arg0:AddButtonHelperText({
            helper_text = Engine.Localize("LUA_MENU_OPTIONS_CAPS"),
            button_ref = "button_start",
            side = "left",
            priority = 4,
            clickable = true
        })
    end

    self:addEventHandler("menu_create", self.addButtonHelperFunction)

    -- Bind button handler
    local bindButton = LUI.UIBindButton.new()
    bindButton.id = "selfBindButton"
    self:addElement(bindButton)
    self.bindButton = bindButton

    self.bindButton:addEventHandler("button_start", function(arg0, arg1)
        ACTIONS.OpenMenu("OptionsMenu", true, arg1.controller or controllerIndex)
    end)

    PostLoadFunc(self, controllerIndex, controller)
    return self
end

MenuBuilder.m_types["MPMainMenu"] = MPMainMenu
LUI.FlowManager.RegisterStackPushBehaviour( "MPMainMenu", PushFunc )