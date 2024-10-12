#include scripts\mp\bots\bots;
#include scripts\mp\bots\bots_util;

main()
{
    initDvars();
    initLevelVariables();
    replacefunc(scripts\mp\bots\bots::init, ::init_stub);
    replacefunc(scripts\mp\bots\bots::bot_get_host_team, ::get_human_player_team);
    replacefunc(scripts\mp\bots\bots::bot_connect_monitor, ::bot_connect_monitor);
    replacefunc(scripts\mp\bots\bots_util::bot_get_client_limit, ::bot_get_client_limit);
    replacefunc(scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause, ::_wait);
}

initDvars()
{
    // setdvar("bots_enabled", 1);
    // setdvar("bot_difficulty", 0);
    // setdvar("party_maxplayers", 18); // controls how many bots are allowed to spawn
}

initLevelVariables()
{
    if(!getdvarint("xblive_privatematch"))
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

human()
{
    if ( !isplayer( self ) || isai( self ) )
        return 0;
    return 1;
}

gethostplayer()
{
    var_0 = getentarray( "player", "classname" );

    for ( var_1 = 0; var_1 < var_0.size; var_1++ )
    {
        if ( var_0[var_1] human() )
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

    level.pausing_bot_connect_monitor = 0;
    
    return 1;
}

init_stub()
{
    thread monitor_smoke_grenades();
    thread bot_triggers();
    initbotlevelvariables();

    if(!getdvarint( "bots_enabled", 0 )) return;

    refresh_existing_bots();
    wait_for_human_player();

    setmatchdata("hasBots", 1);
    level thread bot_connect_monitor();
}

bot_get_client_limit()
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

bot_connect_monitor( var_0, var_1 )
{
    level endon( "game_ended" );
    self notify( "bot_connect_monitor" );
    self endon( "bot_connect_monitor" );
    level.pausing_bot_connect_monitor = 0;
    childthread monitor_pause_spawning();
    scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( 0.5 );
    var_2 = 1.5;

    if ( !isdefined( level.bot_cm_spawned_bots ) )
        level.bot_cm_spawned_bots = 0;

    if ( !isdefined( level.bot_cm_waited_players_time ) )
        level.bot_cm_waited_players_time = 0;

    if ( !isdefined( level.bot_cm_human_picked ) )
        level.bot_cm_human_picked = 0;

    for (;;)
    {
        if ( level.pausing_bot_connect_monitor )
        {
            scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( var_2 );
            continue;
        }

        var_3 = isdefined( level.bots_ignore_team_balance ) || !level.teambased;
        var_4 = botgetteamlimit( 0 );
        var_5 = botgetteamlimit( 1 );

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

        var_8 = "allies";
        var_9 = "axis";
        var_10 = bot_client_counts();
        var_11 = cat_array_get( var_10, "humans" );

        if ( var_11 > 1 )
        {
            var_12 = bot_get_host_team();

            if (isdefined( var_12 ) && var_12 != "spectator" )
            {
                var_8 = var_12;
                var_9 = scripts\mp\utility::getotherteam( var_12 );
            }
            else
            {
                var_13 = cat_array_get( var_10, "humans_allies" );
                var_14 = cat_array_get( var_10, "humans_axis" );

                if ( var_14 > var_13 )
                {
                    var_8 = "axis";
                    var_9 = "allies";
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
                    var_8 = var_16;
                    var_9 = scripts\mp\utility::getotherteam( var_16 );
                }
            }
        }

        var_17 = scripts\mp\bots\bots_util::bot_get_team_limit();
        var_18 = scripts\mp\bots\bots_util::bot_get_team_limit();

        if ( var_17 + var_18 < scripts\mp\bots\bots_util::bot_get_client_limit() )
        {
            if ( var_17 < var_4 )
                var_17++;
            else if ( var_18 < var_5 )
                var_18++;
        }

        var_19 = cat_array_get( var_10, "humans_" + var_8 );
        var_20 = cat_array_get( var_10, "humans_" + var_9 );
        var_21 = var_19 + var_20;
        var_22 = cat_array_get( var_10, "spectator" );
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

        var_27 = cat_array_get( var_10, "bots_" + var_8 );
        var_28 = cat_array_get( var_10, "bots_" + var_9 );
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
            var_32 = !var_3 && var_5 != var_4 && !level.bot_cm_spawned_bots && ( level.bot_cm_waited_players_time < 10 || !scripts\mp\utility::gameflag( "prematch_done" ) );

            if ( var_31 || var_32 )
            {
                level.bot_cm_waited_players_time = level.bot_cm_waited_players_time + var_2;
                scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( var_2 );
                continue;
            }
        }

        var_33 = int( min( var_17 - var_19 - var_23, var_4 ) );
        var_34 = int( min( var_18 - var_20 - var_24, var_5 ) );
        var_35 = 1;
        var_36 = var_33 + var_34 + var_11;
        var_37 = var_4 + var_5 + var_11;

        for ( var_38 = [ -1, -1 ]; var_36 < scripts\mp\bots\bots_util::bot_get_client_limit() && var_36 < var_37; var_35 = !var_35 )
        {
            if ( var_35 && var_33 < var_4 && bot_can_join_team( var_8 ) )
                var_33++;
            else if ( !var_35 && var_34 < var_5 && bot_can_join_team( var_9 ) )
                var_34++;

            var_36 = var_33 + var_34 + var_11;

            if ( var_38[var_35] == var_36 )
                break;

            var_38[var_35] = var_36;
        }

        if ( var_4 == var_5 && !var_3 && var_23 == 1 && var_24 == 0 && var_34 > 0 )
        {
            if ( !isdefined( level.bot_prematchdonetime ) && scripts\mp\utility::gameflag( "prematch_done" ) )
                level.bot_prematchdonetime = gettime();

            if ( var_30 && ( !isdefined( level.bot_prematchdonetime ) || gettime() - level.bot_prematchdonetime < 10000 ) )
                var_34--;
        }

        var_39 = var_33 - var_27;
        var_40 = var_34 - var_28;
        var_41 = 1;

        if ( var_3 )
        {
            var_42 = var_17 + var_18;
            var_43 = var_4 + var_5;
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
                move_bots_from_team_to_team( var_49, var_9, var_8, var_6 );
            else if ( var_40 > 0 )
                move_bots_from_team_to_team( var_49, var_8, var_9, var_7 );

            var_41 = 0;
        }

        if ( var_41 )
        {
            if ( var_40 < 0 )
                drop_bots( var_40 * -1, var_9 );

            if ( var_39 < 0 )
                drop_bots( var_39 * -1, var_8 );

            if ( var_40 > 0 )
                level thread spawn_bots( var_40, var_9, undefined, undefined, "spawned_enemies", var_7 );

            if ( var_39 > 0 )
                level thread spawn_bots( var_39, var_8, undefined, undefined, "spawned_allies", var_6 );

            if ( var_40 > 0 && var_39 > 0 )
                level scripts\engine\utility::waittill_multiple( "spawned_enemies", "spawned_allies" );
            else if ( var_40 > 0 )
                level waittill( "spawned_enemies" );
            else if ( var_39 > 0 )
                level waittill( "spawned_allies" );
        }

        if ( var_7 != var_6 )
        {
            bots_update_difficulty( var_9, var_7 );
            bots_update_difficulty( var_8, var_6 );
        }

        scripts\mp\hostmigration::waitlongdurationwithhostmigrationpause( var_2 );
    }
}