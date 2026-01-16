local CPMainMenu_original = MenuBuilder.m_types["CPMainMenu"]
function CPMainMenuStub(menu, controller)
	ret = CPMainMenu_original(menu, controller)

	-- play music immediately
	Engine.PlayMusic(CoD.Music.MainCPMusic)

	return ret
end

MenuBuilder.m_types["CPMainMenu"] = CPMainMenuStub
