main()
{
    level.usermap_posters = [];

    foreach ( name in strtok( getdvar( "ui_usermap_posters" ), " " ) )
    {
        level.usermap_posters[name] = "zmb_poster_" + name;
        precachemodel( level.usermap_posters[name] );
    }
}

init()
{
    level thread watch_players();
}

watch_players()
{
    for (;;)
    {
        player = level.playerviewowner;

        if ( isdefined( player ) && !isdefined( player.usermap_poster_watcher ) )
        {
            player.usermap_poster_watcher = 1;
            player thread usermap_poster_watcher();
        }

        wait 0.5;
    }
}

usermap_poster_watcher()
{
    self endon( "disconnect" );
    poster = getent( "map_select_poster", "targetname" );

    for (;;)
    {
        self waittill( "luinotifyserver", name, value );

        if ( isdefined( level.usermap_posters[name] ) )
            poster setmodel( level.usermap_posters[name] );
    }
}
