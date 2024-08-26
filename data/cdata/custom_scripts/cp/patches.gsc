main()
{
    replacefunc(scripts\cp\agents\gametype_zombie::get_closest_player_near_interaction_point, ::get_closest_player_near_interaction_point_stub);
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