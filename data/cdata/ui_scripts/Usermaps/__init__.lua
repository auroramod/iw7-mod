if not Engine.InFrontend() then
    return
end

local function is_usermap(name)
    return type(name) == "string" and io.fileexists("usermaps/" .. name .. "/" .. name .. ".ff")
end

local GetPlayerDataEx_og = Engine.GetPlayerDataEx
Engine.GetPlayerDataEx = function(...)
    local leaderboard = false
    for _, arg in ipairs({...}) do
        if arg == "leaderboarddata" then
            leaderboard = true
        elseif leaderboard and is_usermap(arg) then
            return 0
        end
    end

    return GetPlayerDataEx_og(...)
end

local grouped_rows = {}
local header_count = 0

local function count_usermaps()
    if not io.directoryexists("usermaps") then
        return 0
    end

    local count = 0
    local aliens = Engine.IsAliensMode()
    for _, folder in ipairs(io.listfiles("usermaps/")) do
        local name = folder:match("([^/\]+)$")
        if is_usermap(name) and (name:sub(1, 3) == "cp_") == aliens then
            count = count + 1
        end
    end

    return count
end

local function make_header(controller, text)
    header_count = header_count + 1
    local path = "frontEnd.usermaps.header." .. header_count
    local is_owned = LUI.DataSourceInGlobalModel.new(path .. ".isOwned")
    is_owned:SetValue(controller, true)

    return {
        name = LUI.DataSourceInGlobalModel.new(path .. ".name", text),
        desc = LUI.DataSourceInGlobalModel.new(path .. ".desc", ""),
        image = LUI.DataSourceInGlobalModel.new(path .. ".image", ""),
        isOwned = is_owned,
        ref = "",
        isHeader = true,
        headerText = text
    }
end

local function group_maps(source, controller, stock_title, custom_title)
    local stock, custom = {}, {}
    for i = 0, source:GetCountValue(controller) - 1 do
        local entry = source:GetDataSourceAtIndex(i)
        if is_usermap(entry.ref) then
            local image = entry.image and entry.image:GetValue(controller)
            if image and image ~= "" then
                RegisterMaterial(image)
            end
            table.insert(custom, entry)
        else
            table.insert(stock, entry)
        end
    end

    if #custom == 0 then
        grouped_rows = {}
        return source
    end

    local rows = { make_header(controller, stock_title) }
    for _, entry in ipairs(stock) do
        table.insert(rows, entry)
    end
    table.insert(rows, make_header(controller, custom_title))
    for _, entry in ipairs(custom) do
        table.insert(rows, entry)
    end
    grouped_rows = rows

    local grouped = LUI.DataSourceFromList.new(#rows)
    grouped.MakeDataSourceAtIndex = function(_, index)
        return rows[index + 1]
    end
    grouped.GetDefaultFocusIndex = function()
        local mapname = Engine.GetDvarString("ui_mapname")
        for index, entry in ipairs(rows) do
            if not entry.isHeader and entry.ref == mapname then
                return index - 1
            end
        end
        return 1
    end

    return grouped
end

local in_map_menu = false

local UIDataSourceGridNew = LUI.UIDataSourceGrid.new
LUI.UIDataSourceGrid.new = function(definition, options, ...)
    local usermap_count = in_map_menu and options and count_usermaps() or 0
    if usermap_count > 0 then
        if Engine.IsAliensMode() then
            options.maxVisibleRows = math.min(options.maxVisibleRows + 2 + usermap_count, 18)
        else
            options.maxVisibleRows = options.maxVisibleRows + 2
        end
        options.isPositionFocusable = function(_, y)
            local entry = grouped_rows[y + 1]
            return entry == nil or not entry.isHeader
        end
    end

    return UIDataSourceGridNew(definition, options, ...)
end

local function wrap_map_menu(stock_title, custom_title)
    return function(build)
        return function(menu, controller)
            in_map_menu = true
            local ok, self = pcall(build, menu, controller)
            in_map_menu = false
            if not ok then
                error(self)
            end

            local index = controller and controller.controllerIndex or self:getRootController()
            local source = self:GetDataSource()
            if source then
                self:SetDataSource(group_maps(source, index, stock_title, custom_title), index)
                if #grouped_rows > 0 and self.CPMapsTalisman then
                    self.CPMapsTalisman:SetAnchorsAndPosition(0, 1, 0, 1, _1080p * 88, _1080p * 130,
                        _1080p * 235, _1080p * 437)
                end
            end
            return self
        end
    end
end

local function set_header(self, text)
    self.Button:SetAlpha(text and 0 or 1, 0)
    if self.DownloadIcon and text then
        self.DownloadIcon:SetAlpha(0, 0)
    end

    if text then
        if not self.UsermapsHeader then
            local header = LUI.UIText.new()
            header.id = "UsermapsHeader"
            header:SetFont(FONTS.GetFont(FONTS.MainCondensed.File))
            header:SetAlignment(LUI.Alignment.Left)
            header:SetRGBFromInt(0xC8C8C8, 0)
            header:SetAnchorsAndPosition(0, 0, 0, 0, 0, 0, _1080p * 4, _1080p * -2)
            self:addElement(header)
            self.UsermapsHeader = header
        end
        self.UsermapsHeader:setText(text, 0)
        self.UsermapsHeader:SetAlpha(1, 0)
    elseif self.UsermapsHeader then
        self.UsermapsHeader:SetAlpha(0, 0)
    end
end

local wrappers = {
    CPMaps = wrap_map_menu("INFINITE WARFARE ZOMBIES", "CUSTOM ZOMBIES"),
    Maps = wrap_map_menu("INFINITE WARFARE MAPS", "CUSTOM MAPS"),
    MapButton = function(build)
        return function(menu, controller)
            local self = build(menu, controller)
            self:SubscribeToModelThroughElement(self, "name", function()
                local source = self:GetDataSource()
                set_header(self, source and source.isHeader and source.headerText or nil)
            end)
            local button_over = self.Button.m_eventHandlers.button_over
            self.Button:registerEventHandler("button_over", function(element, event)
                local source = self:GetDataSource()
                if source and source.isHeader then
                    return
                end
                return button_over(element, event)
            end)
            return self
        end
    end
}

local types = MenuBuilder.m_types
for name, wrap in pairs(wrappers) do
    if types[name] then
        types[name] = wrap(types[name])
    end
end

local registerType = MenuBuilder.registerType
MenuBuilder.registerType = function(name, build)
    if wrappers[name] then
        build = wrappers[name](build)
    end
    return registerType(name, build)
end
