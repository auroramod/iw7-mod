function ModDownloadCancel(arg0, arg1)
    download.abort()
    LUI.FlowManager.RequestLeaveMenu(arg0)
end

function ModDownloadPopup(arg0, arg1)
    local popup = MenuBuilder.BuildRegisteredType("FenceDialogPopup", {
        message = "Downloading files...",
        controllerIndex = arg1.controllerIndex,
        onCancel = ModDownloadCancel
    })
    popup.id = "ModDownloadPopup"

    local file = ""

    popup:registerEventHandler("mod_download_set_file", function(element, event)
        file = event.request.name
        popup.Message:setText(string.format("Downloading %s...", file))
    end)

    popup:registerEventHandler("mod_download_progress", function(element, event)
        popup.Message:setText(string.format("Downloading %s (%i%%)...", file, math.floor(event.fraction * 100)))
    end)

    popup:registerEventHandler("mod_download_done", function()
        LUI.FlowManager.RequestLeaveMenu(popup)
    end)

    return popup
end

MenuBuilder.registerType("ModDownloadPopup", ModDownloadPopup)

local function reg_func()
    Engine.GetLuiRoot():registerEventHandler("mod_download_start", function(element, event)
        LUI.FlowManager.RequestPopupMenu(element, "ModDownloadPopup", true, event.controller, false)
    end)
end

scheduler.once(reg_func)
