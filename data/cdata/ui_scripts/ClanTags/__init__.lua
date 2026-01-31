if Engine.InFrontend() then
    function PostLoadFunc(f1_arg0, f1_arg1)
        assert(f1_arg0.ClanTagButton)
        local f1_local0 = f1_arg0.ClanTagButton
        local f1_local1 = f1_local0
        f1_local0 = f1_local0.SetButtonDisabled
        local f1_local2 = Engine.IsXB3()
        if f1_local2 then
            f1_local2 = not Engine.IsUserSignedInToLive(f1_arg1)
        end
        f1_local0(f1_local1, f1_local2)
        f1_arg0.ClanTagButton:addEventHandler("button_action", function(f2_arg0, f2_arg1)
            local f2_local0 = f2_arg1.controller or f1_arg1
            Engine.AntiCheatBanCheck(f2_local0, CoD.AntiCheat.Ban.FEATURE_BAN_GLOBAL_CLAN_TAG)
            OSK.OpenScreenKeyboard(f2_local0, Engine.Localize("@LUA_MENU_CUSTOM_CLAN_TAG"),
                Engine.GetCustomClanTag(f2_local0) or "", 4, true, true, true, function(f3_arg0, f3_arg1, f3_arg2)
                    if f3_arg1 then
                        Engine.SetAndEnableCustomClanTag(f3_arg0, f3_arg1)
                        Engine.ExecNow("uploadstats", f3_arg0)
                    end
                end)
        end)
        assert(f1_arg0.patchEmblem)
        f1_arg0.patchEmblem:addEventHandler("button_action", function(f4_arg0, f4_arg1)
            if not Engine.IsUserAGuest(f1_arg1) then
                ACTIONS.OpenMenu("EmblemSelectMenu", true, f1_arg1)
            elseif CONDITIONS.IsXboxOne() then
                LUI.FlowManager.RequestPopupMenu(f1_arg0, "mp_no_guest_popup", false, f1_arg1, false)
            end
        end)
        assert(f1_arg0.CallingCardButton)
        f1_arg0.CallingCardButton:addEventHandler("button_action", function(f5_arg0, f5_arg1)
            if not Engine.IsUserAGuest(f1_arg1) then
                ACTIONS.OpenMenu("CallingCardCategoryMenu", true, f1_arg1)
            elseif CONDITIONS.IsXboxOne() then
                LUI.FlowManager.RequestPopupMenu(f1_arg0, "mp_no_guest_popup", false, f1_arg1, false)
            end
        end)
        if f1_arg0.HeadquartersAnnouncerButton then
            f1_arg0.HeadquartersAnnouncerButton:addEventHandler("button_action", function(f6_arg0, f6_arg1)
                LUI.FlowManager.RequestPopupMenu(f1_arg0, "AnnouncerVoiceSelectPopup", false, f1_arg1, false, {
                    controllerIndex = f1_arg1
                })
            end)
        end
        if CONDITIONS.UseCACBreadCrumbs() and CONDITIONS.IsPublicMatch() then
            local f1_local0 = Rewards.IsTypeNew(f1_arg1, "callingCard")
            if not f1_local0 then
                f1_local0 = CallingCardUtils.HasAnyNewLootCards(f1_arg1)
            end
            if f1_local0 then
                assert(f1_arg0.CallingCardButton)
                local f1_local1 = MenuBuilder.BuildRegisteredType("NewItemNotification", {
                    controllerIndex = f1_arg1
                })
                f1_local1.id = "newWidget"
                if CONDITIONS.IsArabic(f1_arg0) then
                    f1_local1:SetAnchorsAndPosition(1, 0, 0, 0, _1080p * -490, _1080p * -426, _1080p * -16, _1080p * 16)
                else
                    f1_local1:SetAnchorsAndPosition(0, 0, 0, 1, _1080p * 385, _1080p * -51, _1080p * -17, _1080p * 47)
                end
                f1_arg0.CallingCardButton:addElement(f1_local1)
            end
        end
    end
    function HeadquartersCustomizationButtons(menu, controller)
        local self = LUI.UIVerticalList.new()
        self:SetAnchorsAndPosition(0, 1, 0, 1, 0, 500 * _1080p, 0, 319 * _1080p)
        self.id = "HeadquartersCustomizationButtons"
        self._animationSets = {}
        self._sequences = {}
        local f7_local1 = controller and controller.controllerIndex
        if not f7_local1 and not Engine.InFrontend() then
            f7_local1 = self:getRootController()
        end
        assert(f7_local1)
        local f7_local2 = self
        self:SetSpacing(10 * _1080p)
        local Spacer = nil
        Spacer = LUI.UIImage.new()
        Spacer.id = "Spacer"
        Spacer:SetAlpha(0, 0)
        Spacer:SetAnchorsAndPosition(0, 1, 0, 1, 0, 0, 0, _1080p * 9)
        self:addElement(Spacer)
        self.Spacer = Spacer
        local CallingCardButton = nil
        CallingCardButton = MenuBuilder.BuildRegisteredType("GenericButton", {
            controllerIndex = f7_local1
        })
        CallingCardButton.id = "CallingCardButton"
        CallingCardButton.buttonDescription = Engine.Localize("MENU_DESC_PLAYERCARD_BARRACKS")
        CallingCardButton.Text:setText(ToUpperCase(Engine.Localize("LUA_MENU_MP_CHANGE_PLAYERCARD")), 0)
        CallingCardButton:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 19, _1080p * 49)
        self:addElement(CallingCardButton)
        self.CallingCardButton = CallingCardButton
        local patchEmblem = nil
        patchEmblem = MenuBuilder.BuildRegisteredType("GenericButton", {
            controllerIndex = f7_local1
        })
        patchEmblem.id = "patchEmblem"
        patchEmblem.buttonDescription = Engine.Localize("MENU_DESC_EMBLEM")
        patchEmblem.Text:setText(ToUpperCase(Engine.Localize("MENU_EMBLEM_CAPS")), 0)
        patchEmblem:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 59, _1080p * 89)
        self:addElement(patchEmblem)
        self.patchEmblem = patchEmblem
        local f7_local6 = nil
        f7_local6 = MenuBuilder.BuildRegisteredType("GenericButton", {
            controllerIndex = f7_local1
        })
        f7_local6.id = "ClanTagButton"
        f7_local6.buttonDescription = Engine.Localize("LUA_MENU_CUSTOMIZATION_FIELD_DESC_CLAN_TAG")
        f7_local6.Text:setText(ToUpperCase(Engine.Localize("MENU_CLAN_TAG_CAPS")), 0)
        f7_local6:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 99, _1080p * 129)
        self:addElement(f7_local6)
        self.ClanTagButton = f7_local6
        local f7_local7 = nil
        if Engine.IsAliensMode() then
            f7_local7 = MenuBuilder.BuildRegisteredType("GenericButton", {
                controllerIndex = f7_local1
            })
            f7_local7.id = "LobbyMusicButton"
            if Engine.IsAliensMode() then
            else
            end
            if Engine.IsAliensMode() then
                f7_local7.buttonDescription = Engine.Localize("LUA_MENU_ZM_LOBBY_MUSIC_DESC")
            end
            f7_local7.Text:setText(ToUpperCase(Engine.Localize("MENU_LOBBY_MUSIC")), 0)
            f7_local7:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 139, _1080p * 169)
            self:addElement(f7_local7)
            self.LobbyMusicButton = f7_local7
        end
        local f7_local8 = nil
        if CONDITIONS.ShouldShowAnnouncersMenu(self) then
            f7_local8 = MenuBuilder.BuildRegisteredType("HeadquartersAnnouncerButton", {
                controllerIndex = f7_local1
            })
            f7_local8.id = "HeadquartersAnnouncerButton"
            f7_local8:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 500, _1080p * 179, _1080p * 209)
            self:addElement(f7_local8)
            self.HeadquartersAnnouncerButton = f7_local8
        end
        local ButtonDescription = nil
        ButtonDescription = MenuBuilder.BuildRegisteredType("ButtonDescriptionText", {
            controllerIndex = f7_local1
        })
        ButtonDescription.id = "ButtonDescription"
        ButtonDescription.Description:SetRight(_1080p * 415, 0)
        ButtonDescription:SetAnchorsAndPosition(0, 1, 0, 1, 0, _1080p * 504, _1080p * 219, _1080p * 319)
        self:addElement(ButtonDescription)
        self.ButtonDescription = ButtonDescription
        self._animationSets.DefaultAnimationSet = function()
            self._sequences.DefaultSequence = function()
            end
            ButtonDescription:RegisterAnimationSequence("CPinit", {{function()
                return self.ButtonDescription:SetRGBFromTable(SWATCHES.genericButton.textDisabled, 0)
            end}})
            self._sequences.CPinit = function()
                ButtonDescription:AnimateSequence("CPinit")
            end
        end
        self._animationSets.DefaultAnimationSet()
        if Engine.IsAliensMode() then
            f7_local7:addEventHandler("button_action", function(f12_arg0, f12_arg1)
                ACTIONS.OpenMenu("CPLobbyMusicMenu", true, f12_arg1.controller or f7_local1)
            end)
        end
        PostLoadFunc(self, f7_local1, controller)
        return self
    end
    MenuBuilder.registerType("HeadquartersCustomizationButtons", HeadquartersCustomizationButtons)
end

Engine.TruncateToGlyphCount = function(str, len)
    return str
end

Game.GetPlayerClanTag = function(arg0)
    return game:getplayerclantag(arg0)
end

--- Player Nameplates
local f0_local0 = function(f1_arg0, f1_arg1, f1_arg2)
    if f1_arg1 < Engine.GetDvarInt("ui_maxclients") then
        local f1_local0 = DataSources.inGame.MP.players.nameplate:GetDataSourceAtIndex(f1_arg1, f1_arg2)
        f1_arg0:SetDataSource(f1_local0, f1_arg2)
        f1_arg0:SetAlpha(0)
        f1_arg0.validClientEntityNum = true
        local f1_local1 = f1_local0.name:GetValue(f1_arg2)
        if f1_local1 then
            local clanTag = Game.GetPlayerClanTag(f1_arg1)
            if clanTag and clanTag ~= "" then
                f1_local1 = "[" .. clanTag .. "] " .. f1_local1
            end
            f1_arg0.Name:setText(f1_local1)
        end
        f1_arg0:ResetAnchor()
    else
        f1_arg0.validClientEntityNum = false
        f1_arg0:SetAlpha(0)
    end
end

f0_local1 = function(f2_arg0, f2_arg1, f2_arg2)
    local f2_local0 = MenuBuilder.BuildRegisteredType("PlayerName")
    f2_local0.id = "playerName"
    f2_local0:SetDataSource(DataSources.inGame.MP.players.nameplate:GetDataSourceAtIndex(
        Game.GetClientNumFromEntityNum(f2_arg0), f2_arg1), f2_arg1)
    f2_local0.Name:SetRGBFromTable({
        r = f2_arg2.r * 0.5,
        g = f2_arg2.g * 0.5,
        b = f2_arg2.b * 0.5
    }, 0)
    return f2_local0
end

function PlayerNameplates(f3_arg0, f3_arg1)
    local f3_local0 = f3_arg1.controllerIndex
    local f3_local1 = {
        screenOffsetY = -25,
        entityTag = "j_head",
        tagOffsetX = 8,
        obstructionEntityTag = "j_mainroot",
        minScale = 0.7,
        onScope = LUI.UIAnchoredElement.ON_SCOPE.hide,
        onObstruction = LUI.UIAnchoredElement.ON_OBSTRUCTION.sendEvent,
        roundScreenPosition = true,
        useClientDetailWorld = true,
        displayOnObstructionUnknown = false
    }
    local f3_local2 = LUI.ShallowCopy(f3_local1)
    f3_local2.snapToScreenEdges = Engine.GetDvarBool("cg_drawFriendlyNamesAlways")
    f3_local2.alwaysShowUnderCrosshair = true
    local f3_local3 = {
        worldOffsetZ = 24,
        minScale = 0.7,
        onScope = LUI.UIAnchoredElement.ON_SCOPE.hide,
        onObstruction = LUI.UIAnchoredElement.ON_OBSTRUCTION.hide,
        roundScreenPosition = true
    }
    local f3_local4 = {
        onSpawn = function(f4_arg0, f4_arg1)
            return f0_local0(f4_arg0, f4_arg1, f3_local0)
        end,
        buildChild = function(f5_arg0)
            local f5_local0 = MenuBuilder.BuildRegisteredType("PlayerNameplate")
            f5_local0.id = "playerNameplate"
            f5_local0.isFriendly = true
            f5_local0.Name:SetRGBFromTable(SWATCHES.HUD.friendlyTeam, 0)
            f5_local0.Name:SetAlpha(0.6)
            f5_local0.Health.Fill:SetRGBFromTable(SWATCHES.HUD.friendlyTeam, 0)
            f5_local0.Health.Fill:SetAlpha(0.6)
            f5_local0.Health.Frame:SetRGBFromTable(SWATCHES.HUD.background, 0)
            f5_local0.Health.Frame:SetAlpha(0.4)
            return f5_local0
        end,
        removeDataLinksOnDespawn = false,
        spawnDistance = Engine.GetDvarFloat("cg_friendlyNameplateDistance"),
        spawnAngle = Engine.GetDvarFloat("cg_friendlyNameplateAngle"),
        isSpawnAngleRelative = true,
        targetEntities = LUI.UIElementOnEntitySpawner.FILTER.clients + LUI.UIElementOnEntitySpawner.FILTER.friendlies,
        renderMode = LUI.UIElementOnEntitySpawner.RENDER_MODES.anchored,
        renderingOptions = f3_local2,
        maxVisibleElements = 10,
        initialPoolSize = 8,
        maxPoolSize = 8
    }
    local f3_local5 = {
        onSpawn = function(f6_arg0, f6_arg1)
            return f0_local0(f6_arg0, f6_arg1, f3_local0)
        end,
        buildChild = function()
            local f7_local0 = MenuBuilder.BuildRegisteredType("PlayerNameplate")
            f7_local0.id = "playerNameplate"
            f7_local0.isFriendly = false
            f7_local0.Name:SetRGBFromTable(SWATCHES.HUD.enemyTeam, 0)
            f7_local0.Health.Fill:SetRGBFromTable(SWATCHES.HUD.enemyTeam, 0)
            f7_local0.Health.Frame:SetRGBFromTable(SWATCHES.HUD.background, 0)
            f7_local0.Health.Frame:SetAlpha(0.6)
            return f7_local0
        end,
        removeDataLinksOnDespawn = false,
        spawnDistance = Engine.GetDvarFloat("cg_enemyNameplateDistance"),
        spawnAngle = Engine.GetDvarFloat("cg_enemyNameplateAngle"),
        isSpawnAngleRelative = true,
        targetEntities = LUI.UIElementOnEntitySpawner.FILTER.clients + LUI.UIElementOnEntitySpawner.FILTER.enemies +
            LUI.UIElementOnEntitySpawner.FILTER.flyingTarget,
        renderMode = LUI.UIElementOnEntitySpawner.RENDER_MODES.anchored,
        renderingOptions = f3_local1,
        maxVisibleElements = 10,
        initialPoolSize = 5,
        maxPoolSize = 5
    }
    local f3_local6 = {
        buildChild = function(f8_arg0)
            return f0_local1(f8_arg0, f3_local0, SWATCHES.HUD.friendlyTeam)
        end,
        spawnDistance = Engine.GetDvarFloat("cg_friendlyNameplateDistance"),
        spawnAngle = Engine.GetDvarFloat("cg_friendlyNameplateAngle"),
        targetEntities = LUI.UIElementOnEntitySpawner.FILTER.dogTags + LUI.UIElementOnEntitySpawner.FILTER.friendlies,
        renderMode = LUI.UIElementOnEntitySpawner.RENDER_MODES.anchored,
        renderingOptions = f3_local3,
        maxVisibleElements = 10
    }
    local f3_local7 = {
        buildChild = function(f9_arg0)
            return f0_local1(f9_arg0, f3_local0, SWATCHES.HUD.enemyTeam)
        end,
        spawnDistance = Engine.GetDvarFloat("cg_friendlyNameplateDistance"),
        spawnAngle = Engine.GetDvarFloat("cg_friendlyNameplateAngle"),
        targetEntities = LUI.UIElementOnEntitySpawner.FILTER.dogTags + LUI.UIElementOnEntitySpawner.FILTER.enemies,
        renderMode = LUI.UIElementOnEntitySpawner.RENDER_MODES.anchored,
        renderingOptions = f3_local3,
        maxVisibleElements = 10
    }
    local self = LUI.UIElement.new({
        topAnchor = true,
        bottomAnchor = true,
        leftAnchor = true,
        rightAnchor = true,
        top = 0,
        bottom = 0,
        left = 0,
        right = 0
    })
    self.id = "PlayerNameplates"
    local f3_local9 = LUI.UIElementOnEntitySpawner.new(f3_local4)
    f3_local9.id = "entitySpawnerFriendly"
    f3_local9:SubscribeToModel(DataSources.inGame.player.spawnCount:GetModel(f3_local0), function()
        f3_local9:DespawnForAllEntities(f3_local0)
    end)
    self:addElement(f3_local9)
    local f3_local10 = LUI.UIElementOnEntitySpawner.new(f3_local6)
    f3_local10.id = "entitySpawnerDogTagsFriendly"
    self:addElement(f3_local10)
    if not GameX.IsHardcoreMode() then
        local f3_local11 = LUI.UIElementOnEntitySpawner.new(f3_local5)
        f3_local11.id = "entitySpawnerEnemy"
        local f3_local12 = function()
            local f11_local0 = DataSources.inGame.player.marksman:GetValue(f3_local0)
            local f11_local1 = DataSources.inGame.player.currentWeapon.enemyNameplateDisplayRange:GetValue(f3_local0)
            if f11_local0 == nil or f11_local1 == nil then
                return
            end
            local f11_local2 = math.min(f3_local5.spawnDistance, f11_local1)
            local f11_local3 = f3_local5.spawnAngle
            if MLG.IsMLGSpectator() then
                f11_local2 = 100000
                f11_local3 = 360
            elseif f11_local0 then
                f11_local2 = f11_local2 * Engine.GetDvarFloat("perk_marksmanNameDistanceScale")
                f11_local3 = f11_local3 * Engine.GetDvarFloat("perk_marksmanNameAngleScale")
            end
            f3_local11:SetSpawnDistance(f11_local2)
            f3_local11:SetSpawnAngle(f11_local3)
        end

        f3_local11:SubscribeToModel(DataSources.inGame.player.marksman:GetModel(f3_local0), f3_local12, true)
        f3_local11:SubscribeToModel(DataSources.inGame.player.currentWeapon.enemyNameplateDisplayRange:GetModel(
            f3_local0), f3_local12, true)
        self:addElement(f3_local11)
        self.entitySpawnerEnemy = f3_local11
    end
    local f3_local11 = LUI.UIElementOnEntitySpawner.new(f3_local7)
    f3_local11.id = "entitySpawnerDogTagsEnemy"
    self:addElement(f3_local11)
    return self
end

MenuBuilder.registerType("PlayerNameplates", PlayerNameplates)
