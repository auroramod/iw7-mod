-- Zombies HUD for more than four players.
--
-- The stock CP data sources only build portrait and souvenir sources for
-- clients 0-3. A player in a higher slot gets nil from them, the HUD fails to
-- build and the game retries every frame until LUI runs out of memory.
--
-- The zombies scoreboard data also only has room for four players, so the
-- server sends every player's stats in the ui_cp_scoreboard dvar
-- (custom_scripts/cp/scoreboard.gsc) and the HUD reads them from there.

if type(InitCommonCPDataSources) ~= "function" then
	return
end

local init_common_cp_data_sources = InitCommonCPDataSources

InitCommonCPDataSources = function(...)
	init_common_cp_data_sources(...)

	local cp = DataSources.inGame.CP
	local images, big_images, coins = {}, {}, {}

	local function image(client)
		images[client] = images[client] or LUI.AggregateDataSource.new(cp.playerPhoto,
			{ cp.playerCharacter, cp.playerStatus }, "image." .. client, function(controller)
				return ZombiesUtils.GetZombiesPlayerPhoto(cp.playerCharacter:GetValue(controller),
					cp.playerStatus:GetValue(controller), client)
			end)
		return images[client]
	end

	local function big_image(client)
		big_images[client] = big_images[client] or LUI.AggregateDataSource.new(cp.playerPhoto,
			{ cp.playerCharacter, cp.playerStatus }, "bigImage." .. client, function(controller)
				return ZombiesUtils.GetZombiesBigPhoto(cp.playerCharacter:GetValue(controller), client)
			end)
		return big_images[client]
	end

	local function coin(client)
		coins[client] = coins[client] or cp.zombies.playersCoins:Filter(tostring(client), function(value)
			local piece = ZombiesUtils.GetZombiesPlayerCoin(value, client)
			if piece == 0 then
				return "specialty_ks_null"
			end
			return Engine.TableLookup(CSV.zombieSouvenirPieceInfo.File, CSV.zombieSouvenirPieceInfo.Cols.Idx,
				tostring(piece), CSV.zombieSouvenirPieceInfo.Cols.Icon)
		end)
		return coins[client]
	end

	local function fill(source, client, with_big_image)
		source.image = source.image or image(client)
		source.coinImage = source.coinImage or coin(client)
		if with_big_image then
			source.bigImage = source.bigImage or big_image(client)
		end
		return source
	end

	local make_player = cp.players.MakeDataSourceAtIndex
	cp.players.MakeDataSourceAtIndex = function(list, index, controller)
		return fill(make_player(list, index, controller), index, true)
	end

	local make_other = cp.otherPlayers.MakeDataSourceAtIndex
	cp.otherPlayers.MakeDataSourceAtIndex = function(list, index, controller)
		local client = index
		if DataSources.inGame.currentPlayerClientNum:GetValue(controller) <= client then
			client = client + 1
		end
		return fill(make_other(list, index, controller), client, false)
	end
end

local function players()
	local clients = {}
	for index = 0, Game.GetNumPlayersOnTeam(Teams.allies) - 1 do
		clients[#clients + 1] = Game.GetTeamPlayerAtIndex(Teams.allies, index)
	end
	table.sort(clients)
	return clients
end

local function has_high_slot(clients)
	return #clients > 0 and clients[#clients] > 3
end

-- "client cash score kills headshots revives downs;" per player
local function stats()
	local result = {}
	for entry in string.gmatch(Engine.GetDvarString("ui_cp_scoreboard") or "", "[^;]+") do
		local values = {}
		for value in string.gmatch(entry, "%d+") do
			values[#values + 1] = tonumber(value)
		end
		local client = table.remove(values, 1)
		if client then
			result[client] = values
		end
	end
	return result
end

local function voice(client)
	for row = 0, Game.GetNumPlayersOnTeam(Teams.allies) - 1 do
		local ok, data = pcall(Game.GetScoreboardRowData, row, Teams.allies)
		if ok and data and data.clientNum == client then
			return data.isMicOn, data.isMuted
		end
	end
	return false, false
end

local get_player_count = Game.GetCPNumScoreboardPlayers
Game.GetCPNumScoreboardPlayers = function(...)
	local clients = players()
	if not has_high_slot(clients) then
		return get_player_count(...)
	end
	return #clients
end

local get_row_data = Game.GetCPScoreboardRowData
Game.GetCPScoreboardRowData = function(...)
	local clients = players()
	if not has_high_slot(clients) then
		return get_row_data(...)
	end

	local row, self, exclude_self = ...
	if exclude_self then
		for index, client in ipairs(clients) do
			if client == self then
				table.remove(clients, index)
				break
			end
		end
	end

	local client = clients[row + 1]
	if not client then
		return nil
	end

	local values = stats()[client] or {}
	local mic_on, muted = voice(client)
	return {
		clientNum = client,
		name = Game.GetPlayerScoreInfo(client).name,
		currency = values[1] or 0,
		score = values[2] or 0,
		kills = values[3] or 0,
		headShots = values[4] or 0,
		revives = values[5] or 0,
		downs = values[6] or 0,
		isMicOn = mic_on,
		isMuted = muted,
	}
end

-- Show every player instead of the stock three teammates and four scoreboard
-- rows. Scoreboard rows are packed a little tighter so eight fit above the
-- sticker pack, and the mute hint is hidden when it would cover a row.
local function show_rows(type_name, rows, row_height)
	local build = MenuBuilder.m_types[type_name]
	if type(build) ~= "function" then
		return
	end

	MenuBuilder.m_types[type_name] = function(...)
		local new_grid = LUI.UIGrid.new
		LUI.UIGrid.new = function(element, options)
			options.maxVisibleRows = rows
			options.numRows = rows
			if row_height then
				options.rowHeight = row_height
				options.spacingY = 0
			end
			return new_grid(element, options)
		end
		local ok, result = pcall(build, ...)
		LUI.UIGrid.new = new_grid
		if not ok then
			error(result, 0)
		end

		if result.ButtonHelperBar then
			result:addEventHandler("refreshScores", function(element)
				element.ButtonHelperBar:SetAlpha(element:GetNumPlayers() > 4 and 0 or 1)
			end)
		end
		return result
	end
end

show_rows("PlayerCash", 7)
show_rows("ZomScoreboard", 8, _1080p * 96)
show_rows("ZomScoreboardDLC1", 8, _1080p * 96)
