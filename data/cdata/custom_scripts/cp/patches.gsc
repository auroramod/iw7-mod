main()
{
    replacefunc(scripts\cp\agents\gametype_zombie::get_closest_player_near_interaction_point, ::get_closest_player_near_interaction_point_stub);

    // wait for players to join before determining price of doors
    determine_door_price_func = getfunction("scripts/cp/maps/cp_zmb/cp_zmb", "post_nondeterministic_func");
    if (isdefined(determine_door_price_func))
    {
        replacefunc(determine_door_price_func, ::determine_door_price_stub);
    }
}

get_closest_player_near_interaction_point_stub( var_0 )
{
    var_1 = scripts\engine\utility::get_array_of_closest( var_0.origin, level.players )[0];
    var_2 = scripts\engine\utility::getclosest( var_0.origin, level.current_interaction_structs );

    if( !isDefined( var_1 ) || !isDefined( var_2 ) )
    {
        return undefined;
    }

    if ( !scripts\cp\agents\gametype_zombie::is_player_near_interaction_point( var_1, var_2 ) )
        var_1 = undefined;

    return var_1;
}

determine_door_price_stub()
{
    wait 10;
    team_door_switches = scripts\engine\utility::getstructarray( "team_door_switch", "script_noteworthy" );

    // the dedicated server will hit this when map restarts on a blank server
    // default to 1 player, because 1 player joining is enough to start
    player_count = (level.players.size ? level.players.size : 1);

    foreach ( door in team_door_switches )
    {
        door_ents = getentarray( door.target, "targetname" );

        foreach ( door_ent in door_ents )
        {
            if ( !isdefined( door_ent.script_noteworthy ) )
                continue;
            else if ( door_ent.script_noteworthy == "progress" && !isdefined( door_ent.adjusted ) )
            {
                door_ent.adjusted = 1;
                distance_to_move = undefined;

                switch ( player_count )
                {
                    case 1:
                        distance_to_move = 12;
                        level.moon_donations = 2;
                        level.kepler_donations = 2;
                        level.triton_donations = 2;
                        break;
                    case 2:
                        distance_to_move = 8;
                        level.moon_donations = 1;
                        level.kepler_donations = 1;
                        level.triton_donations = 1;
                        break;
                    case 3:
                        distance_to_move = 4;
                        level.moon_donations = 0;
                        level.kepler_donations = 0;
                        level.triton_donations = 0;
                        break;
                    case 4:
                        level.moon_donations = -1;
                        level.kepler_donations = -1;
                        level.triton_donations = -1;
                        break;
                    default:
                        break;
                }

                if ( !isdefined( distance_to_move ) )
                    continue;

                door_ent movez( distance_to_move, 0.1 );
                door_ent waittill( "movedone" );
            }
        }
    }

    level.team_door_adjusted_for = player_count;
    scripts\engine\utility::flag_set( "team_doors_initialized" );
}
