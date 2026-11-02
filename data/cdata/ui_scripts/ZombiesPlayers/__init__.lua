-- Zombies HUD data for client numbers above 3.
-- The stock CP data sources only build portrait and souvenir sources for
-- clients 0-3. A player in a higher slot gets nil from them, the HUD fails to
-- build and the game retries every frame until LUI runs out of memory.

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
