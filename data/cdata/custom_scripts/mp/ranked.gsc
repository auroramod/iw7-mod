main()
{
    if (!getdvarint("xblive_privatematch"))
    {
        level.onlinegame = 1;
        level.rankedmatch = 1;

        setdvar("systemlink", 0);
        setdvar("onlinegame", 1);

        replacefunc(scripts\mp\utility::rankingenabled, ::rankingenabled);
    }

    replacefunc(scripts\mp\menus::addtoteam, ::addtoteam_stub);
    replacefunc(scripts\mp\menus::watchforteamchange, ::watchforteamchange_stub);
	
	// Bypass check for sessionteam
	replacefunc(scripts\mp\playerlogic::connect_validateplayerteam, ::connect_validateplayerteam_stub);
}

rankingenabled()
{
    return !(!isplayer( self ) || isai( self ));
}

addtoteam_stub( team, firstConnect, changeTeamsWithoutRespawning )
{
    if ( isdefined( self.team ) )
    {
        scripts\mp\playerlogic::removefromteamcount();

        if ( isdefined( changeTeamsWithoutRespawning ) && changeTeamsWithoutRespawning )
            scripts\mp\playerlogic::decrementalivecount( self.team );
    }

    if ( isdefined( self.pers["team"] ) && self.pers["team"] != "" && self.pers["team"] != "spectator" )
        self.pers["last_team"] = self.pers["team"];

    self.pers["team"] = team;
    self.team = team;

    // bypass session team is readonly in ranked matches if "teambased" is set on the playlist
    if ( level.teambased )
        self.sessionteam = team;
    else if ( team == "spectator" )
        self.sessionteam = "spectator";
    else
        self.sessionteam = "none";

    if ( game["state"] != "postgame" )
    {
        scripts\mp\playerlogic::addtoteamcount();

        if ( isdefined( changeTeamsWithoutRespawning ) && changeTeamsWithoutRespawning )
            scripts\mp\playerlogic::incrementalivecount( self.team );
    }

    if ( isgamebattlematch() )
        setmatchdata( "players", self.clientid, "team", team );

    scripts\mp\utility::updateobjectivetext();

    if ( isdefined( firstConnect ) && firstConnect )
        waittillframeend;

    scripts\mp\utility::updatemainmenu();

    if ( team == "spectator" )
    {
        self notify( "joined_spectators" );
        level notify( "joined_team", self );
    }
    else
    {
        self notify( "joined_team" );
        level notify( "joined_team", self );
    }
}

watchforteamchange_stub()
{
    self endon( "disconnect" );
    level endon( "game_ended" );
	
	//------------------
	// 0 = axis
	// 1 = allies
	// 2 = auto
	// 3 = spectate
	//------------------

    for (;;)
    {
        self waittill( "luinotifyserver", channel, teamSelected );

        if ( channel != "team_select" )
            continue;

        var_2 = 0;

        if ( teamSelected >= 3 )
            var_2 = 1;

        if ( var_2 )
        {
            self setclientomnvar( "ui_spectator_selected", 1 );
            self setclientomnvar( "ui_loadout_selected", -1 );
            self.spectating_actively = 1;
        }
        else
        {
            self setclientomnvar( "ui_spectator_selected", -1 );
            self.spectating_actively = 0;
        }

        var_3 = self ismlgspectator();
        var_4 = !var_3 && isdefined( self.team ) && self.team == "spectator";
        var_5 = var_3 && teamSelected == 3 || var_4 && teamSelected == 4;

        if ( teamSelected == 4 )
        {
            teamSelected = 3;
            self setmlgspectator( 1 );
        }
        else
            self setmlgspectator( 0 );

        self setclientomnvar( "ui_team_selected", teamSelected );

        if ( teamSelected == 0 )
            teamSelected = "axis";
        else if ( teamSelected == 1 )
            teamSelected = "allies";
        else if ( teamSelected == 2 )
            teamSelected = "random";
        else
            teamSelected = "spectator";

        if ( !var_5 && isdefined( self.pers["team"] ) && teamSelected == self.pers["team"] )
            continue;

        self setclientomnvar( "ui_loadout_selected", -1 );
        thread scripts\mp\menus::logteamselection( teamSelected );

        if ( teamSelected == "axis" )
        {
            thread scripts\mp\menus::setteam( "axis" );
            continue;
        }

        if ( teamSelected == "allies" )
        {
            thread scripts\mp\menus::setteam( "allies" );
            continue;
        }

        if ( teamSelected == "random" )
        {
            thread scripts\mp\menus::autoassign();
            continue;
        }

        if ( teamSelected == "spectator" )
            thread scripts\mp\menus::setspectator( var_5 );
    }
}

connect_validateplayerteam_stub()
{
    if ( !isdefined( self ) )
        return;
}