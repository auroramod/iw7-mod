gsc_tool = {
    source = path.join(dependencies.basePath, "gsc-tool/src")
}

function gsc_tool.import()
    links {"xsk-gsc-iw7", "xsk-gsc-utils"}
    gsc_tool.includes()
end

function gsc_tool.includes()
    includedirs {
        path.join(gsc_tool.source, "utils"), 
        path.join(gsc_tool.source, "iw7"),
        path.join(dependencies.basePath, "extra/gsc-tool") -- https://github.com/GEEKiDoS/open-teknomw3/blob/master/deps/extra/gsc-tool
    }
end

-- https://github.com/xensik/gsc-tool/blob/dev/premake5.lua#L95
function gsc_tool.project()
    project "xsk-gsc-utils"
        kind "StaticLib"
        language "C++"

        pchheader "stdafx.hpp"
        pchsource(path.join(gsc_tool.source, "utils/stdafx.cpp"))

        files {
            path.join(gsc_tool.source, "utils/**.h"), 
            path.join(gsc_tool.source, "utils/**.hpp"),
            path.join(gsc_tool.source, "utils/**.cpp")
        }

        includedirs {
            path.join(gsc_tool.source, "utils"), 
            gsc_tool.source
        }

        zlib.includes()

    project "xsk-gsc-iw7"
        kind "StaticLib"
        language "C++"

        pchheader "stdafx.hpp"
        pchsource(path.join(gsc_tool.source, "iw7/stdafx.cpp"))

        files {
            path.join(gsc_tool.source, "iw7/**.h"), 
            path.join(gsc_tool.source, "iw7/**.hpp"),
            path.join(gsc_tool.source, "iw7/**.cpp"), 
            path.join(dependencies.basePath, "extra/gsc-tool/interface.cpp")
        }

        includedirs {
            path.join(gsc_tool.source, "iw7"), 
            gsc_tool.source, 
            path.join(dependencies.basePath, "extra/gsc-tool")
        }

    -- https://github.com/xensik/gsc-tool/blob/dev/premake5.lua#L25
	-- adding these build options fixes a bunch of parser stuff
    filter "action:vs*"
        buildoptions "/bigobj"
        buildoptions "/Zc:__cplusplus"
    filter {}
end

table.insert(dependencies, gsc_tool)
