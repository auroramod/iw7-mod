#include scripts\mp\bots\bots;
#include scripts\mp\bots\bots_util;

main()
{
    init_dvars();
    init_level_variables();

    replacefunc(scripts\mp\bots\bots::init, ::init_stub);
    replacefunc(scripts\mp\bots\bots::bot_get_host_team, ::get_human_player_team);
    replacefunc(scripts\mp\bots\bots::bot_connect_monitor, ::bot_connect_monitor_stub);
    replacefunc(scripts\mp\bots\bots_util::bot_get_client_limit, ::bot_get_client_limit_stub);
    replacefunc(scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause, ::_wait);
    replacefunc(scripts\mp\gamelogic::waitforplayers, ::waitforplayers_stub);
}

init_dvars()
{
    // setdvar("bots_enabled", 1);
    // setdvar("bot_difficulty", 0);
    // setdvar("party_maxplayers", 18); // controls how many bots are allowed to spawn
    setdvar("scr_game_graceperiod", 15);
    setdvar("scr_game_playerwaittime", 5);
    setdvar("scr_game_matchstarttime", 5);
    level.ready_to_start = 0;
}

init_level_variables()
{
    if (!getdvarint("xblive_privatematch"))
    {
        allowedBotDifficulty(getdvarint( "bot_difficulty", 0 )); 
    }
}

allowedBotDifficulty(difficulty)
{
    level.bot_difficulty_defaults = [];
    switch (difficulty)
    {
        case 4:
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "veteran";
            break;
        case 3:
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "hardened";
            break;
        case 2:
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "regular";
            break;
        case 1:
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "recruit";
            break;
        case 0:
        default:
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "hardened";
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "regular";
            level.bot_difficulty_defaults[level.bot_difficulty_defaults.size] = "recruit";
            break;
    }
}

_wait(time)
{
    wait(time);
}

waitforplayers_stub( maxTime )
{
	startTime = gettime();
	endTime = startTime + maxTime * 1000 - 200;

	if ( maxTime > 5 )
		minTime = gettime() + getDvarInt( "min_wait_for_players" ) * 1000;
	else
		minTime = 0;

	numToWaitFor = ( level._id_4533/3 ); // connectingPlayers variable

	for ( ;; )
	{
		if ( isDefined( game["roundsPlayed"] ) && game["roundsPlayed"] )
			break;

		totalSpawnedPlayers = level.maxPlayerCount;

		curTime = gettime();

		if( level.ready_to_start )
			break;

		wait 0.05; 
	}
}

is_human()
{
    return (isplayer(self) && !isai(self));
}

gethostplayer()
{
    var_0 = getentarray( "player", "classname" );

    for ( var_1 = 0; var_1 < var_0.size; var_1++ )
    {
        if ( var_0[var_1] is_human() )
            return var_0[var_1];
    }
}

get_human_player_loop()
{
    player = undefined;
    while (!isdefined(player))
    {
        wait(0.25);
        player = gethostplayer();
    }
    return player;
}

get_human_player_team()
{
    player = get_human_player_loop();
    return player bot_get_player_team();
}

wait_for_human_player()
{
    player = get_human_player_loop();
    
    while ( isdefined( player ) && !player.hasspawned && !isdefined( player.selectedclass ) )
    {
        level.pausing_bot_connect_monitor = 1;
        wait(0.25);
    }

    // if the player leaves wait for new player
    if (!isdefined( player ))
        wait_for_human_player();
    
    level.pausing_bot_connect_monitor = 0;
    level.ready_to_start = 1;

    return 1;
}

init_stub()
{
    thread monitor_smoke_grenades();
    thread bot_triggers();
    initbotlevelvariables();

    if (!getdvarint( "bots_enabled", 0 )) 
        return;

    refresh_existing_bots();
    wait_for_human_player();

    setmatchdata( "hasBots", 1 );

    level thread scripts\mp\bots\bots::bot_connect_monitor();
}

bot_get_client_limit_stub()
{
    var_0 = getdvarint( "party_maxplayers", 0 );

    maxclients = level.maxclients;

    if ( var_0 != level.maxclients && var_0 >= 1 )
    {
        if ( var_0 > 18 )
            maxclients = 18;
        else
            maxclients = var_0;
    }

    setdvar( "sv_maxclients", maxclients );
    return maxclients;
}

bot_connect_monitor_stub( num_ally_bots, num_enemy_bots )
{
    level endon( "game_ended" );
    
    self notify( "bot_connect_monitor" );
	self endon( "bot_connect_monitor" );

    level.pausing_bot_connect_monitor = false;
	childthread monitor_pause_spawning();

    scripts\mp\hostmigration::waitLongDurationWithHostMigrationPause( 0.5 );
	bot_connect_monitor_update_time = 1.5;

    if ( !IsDefined( level.bot_cm_spawned_bots ) )
		level.bot_cm_spawned_bots = false;

	if ( !IsDefined( level.bot_cm_waited_players_time ) )
		level.bot_cm_waited_players_time = 0;
	
	if ( !IsDefined( level.bot_cm_human_picked ) )
		level.bot_cm_human_picked = false;

    for(;;)
    {
        if ( level.pausing_bot_connect_monitor )
        {
            scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( bot_connect_monitor_update_time );
            continue;
        }

        ignore_team_balance = IsDefined(level.bots_ignore_team_balance) || !level.teamBased;
        max_ally_bots_absolute = botgetteamlimit( 0 );
        max_enemy_bots_absolute = botgetteamlimit( 1 );

        if ( level.rankedmatch )
        {
            var_6 = "default";
            var_7 = "default";
        }
        else
        {
            var_6 = botgetteamdifficulty( 0 );
            var_7 = botgetteamdifficulty( 1 );
        }

        team_ally = "allies";	// team_ally is the team that the human player is on
		team_enemy = "axis";	// team_enemy is the enemy team (opposed to the team the human player is on)
		clientCounts = bot_client_counts();
        numHumans = cat_array_get( clientCounts, "humans" );

        if ( numHumans > 1 )
        {
            hostPlayerTeam = bot_get_host_team();

            if (isdefined( hostPlayerTeam ) && hostPlayerTeam != "spectator" )
            {
                team_ally = hostPlayerTeam;
                team_enemy = scripts\mp\utility::getotherteam( hostPlayerTeam );
            }
            else
            {
                cur_num_allies_player = cat_array_get( clientCounts, "humans_allies" );
                cur_num_axis_players = cat_array_get( clientCounts, "humans_axis" );

                if ( cur_num_axis_players > cur_num_allies_player )
                {
                    team_ally = "axis";
                    team_enemy = "allies";
                }
            }
        }
        else
        {
            var_15 = get_human_player();

            if ( isdefined( var_15 ) )
            {
                var_16 = var_15 bot_get_player_team();

                if ( isdefined( var_16 ) && var_16 != "spectator" )
                {
                    team_ally = var_16;
                    team_enemy = scripts\mp\utility::getotherteam( var_16 );
                }
            }
        }

        var_17 = scripts\mp\bots\bots_util::bot_get_team_limit();
        var_18 = scripts\mp\bots\bots_util::bot_get_team_limit();

        if ( var_17 + var_18 < scripts\mp\bots\bots_util::bot_get_client_limit() )
        {
            if ( var_17 < max_ally_bots_absolute )
                var_17++;
            else if ( var_18 < max_enemy_bots_absolute )
                var_18++;
        }

        var_19 = cat_array_get( clientCounts, "humans_" + team_ally );
        var_20 = cat_array_get( clientCounts, "humans_" + team_enemy );
        var_21 = var_19 + var_20;
        var_22 = cat_array_get( clientCounts, "spectator" );
        var_23 = 0;

        for ( var_24 = 0; var_22 > 0; var_22-- )
        {
            var_25 = var_19 + var_23 + 1 <= var_17;
            var_26 = var_20 + var_24 + 1 <= var_18;

            if ( var_25 && !var_26 )
            {
                var_23++;
                continue;
            }

            if ( !var_25 && var_26 )
            {
                var_24++;
                continue;
            }

            if ( var_25 && var_26 )
            {
                if ( var_22 % 2 == 1 )
                {
                    var_23++;
                    continue;
                }

                var_24++;
            }
        }

        var_27 = cat_array_get( clientCounts, "bots_" + team_ally );
        var_28 = cat_array_get( clientCounts, "bots_" + team_enemy );
        var_29 = var_27 + var_28;

        if ( var_29 > 0 )
            level.bot_cm_spawned_bots = 1;

        var_30 = 0;

        if ( !level.bot_cm_human_picked )
        {
            var_30 = !bot_get_human_picked_team();

            if ( !var_30 )
                level.bot_cm_human_picked = 1;
        }

        if ( var_30 )
        {
            var_31 = !getdvarint( "systemlink" ) && !getdvarint( "onlinegame" );
            var_32 = !ignore_team_balance && max_enemy_bots_absolute != max_ally_bots_absolute && !level.bot_cm_spawned_bots && ( level.bot_cm_waited_players_time < 10 || !scripts\mp\utility::gameflag( "prematch_done" ) );

            if ( var_31 || var_32 )
            {
                level.bot_cm_waited_players_time = level.bot_cm_waited_players_time + bot_connect_monitor_update_time;
                scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( bot_connect_monitor_update_time );
                continue;
            }
        }

        var_33 = int( min( var_17 - var_19 - var_23, max_ally_bots_absolute ) );
        var_34 = int( min( var_18 - var_20 - var_24, max_enemy_bots_absolute ) );
        var_35 = 1;
        var_36 = var_33 + var_34 + numHumans;
        var_37 = max_ally_bots_absolute + max_enemy_bots_absolute + numHumans;

        for ( var_38 = [ -1, -1 ]; var_36 < scripts\mp\bots\bots_util::bot_get_client_limit() && var_36 < var_37; var_35 = !var_35 )
        {
            if ( var_35 && var_33 < max_ally_bots_absolute && bot_can_join_team( team_ally ) )
                var_33++;
            else if ( !var_35 && var_34 < max_enemy_bots_absolute && bot_can_join_team( team_enemy ) )
                var_34++;

            var_36 = var_33 + var_34 + numHumans;

            if ( var_38[var_35] == var_36 )
                break;

            var_38[var_35] = var_36;
        }

        if ( max_ally_bots_absolute == max_enemy_bots_absolute && !ignore_team_balance && var_23 == 1 && var_24 == 0 && var_34 > 0 )
        {
            if ( !isdefined( level.bot_prematchdonetime ) && scripts\mp\utility::gameflag( "prematch_done" ) )
                level.bot_prematchdonetime = gettime();

            if ( var_30 && ( !isdefined( level.bot_prematchdonetime ) || gettime() - level.bot_prematchdonetime < 10000 ) )
                var_34--;
        }

        var_39 = var_33 - var_27;
        var_40 = var_34 - var_28;
        var_41 = 1;

        if ( ignore_team_balance )
        {
            var_42 = var_17 + var_18;
            var_43 = max_ally_bots_absolute + max_enemy_bots_absolute;
            var_44 = var_19 + var_20;
            var_45 = var_27 + var_28;
            var_46 = int( min( var_42 - var_44, var_43 ) );
            var_47 = var_46 - var_45;

            if ( var_47 == 0 )
                var_41 = 0;
            else if ( var_47 > 0 )
            {
                var_39 = int( var_47 / 2 ) + var_47 % 2;
                var_40 = int( var_47 / 2 );
            }
            else if ( var_47 < 0 )
            {
                var_48 = var_47 * -1;
                var_39 = -1 * int( min( var_48, var_27 ) );
                var_40 = -1 * ( var_48 + var_39 );
            }
        }
        else if ( ( var_39 * var_40 < 0 && !isdefined( level.bots_disable_team_switching ) ) )
        {
            var_49 = int( min( abs( var_39 ), abs( var_40 ) ) );

            if ( var_39 > 0 )
                move_bots_from_team_to_team( var_49, team_enemy, team_ally, var_6 );
            else if ( var_40 > 0 )
                move_bots_from_team_to_team( var_49, team_ally, team_enemy, var_7 );

            var_41 = 0;
        }

        if ( var_41 )
        {
            if ( var_40 < 0 )
                drop_bots( var_40 * -1, team_enemy );

            if ( var_39 < 0 )
                drop_bots( var_39 * -1, team_ally );

            if ( var_40 > 0 )
                level thread spawn_bots( var_40, team_enemy, undefined, undefined, "spawned_enemies", var_7 );

            if ( var_39 > 0 )
                level thread spawn_bots( var_39, team_ally, undefined, undefined, "spawned_allies", var_6 );

            if ( var_40 > 0 && var_39 > 0 )
                level scripts\engine\utility::waittill_multiple( "spawned_enemies", "spawned_allies" );
            else if ( var_40 > 0 )
                level waittill( "spawned_enemies" );
            else if ( var_39 > 0 )
                level waittill( "spawned_allies" );
        }

        if ( var_7 != var_6 )
        {
            bots_update_difficulty( team_enemy, var_7 );
            bots_update_difficulty( team_ally, var_6 );
        }

        scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( bot_connect_monitor_update_time );
    }
}
