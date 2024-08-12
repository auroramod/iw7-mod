if CONDITIONS.InFrontend then
	require("InFrontend")
end

if CONDITIONS.InGame then
	require("InGame")
end

function CheckTURequirement(f10_arg0, f10_arg1)
	return false
end
