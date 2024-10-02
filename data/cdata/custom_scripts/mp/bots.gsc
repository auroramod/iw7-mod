#include scripts\mp\bots\bots;
#include scripts\mp\bots\bots_util;

main()
{
    initdvars();
    replacefunc(scripts\mp\bots\bots::init, ::init_stub);
    replacefunc(scripts\mp\bots\bots_util::bot_get_client_limit, ::bot_get_client_limit);
}

initdvars()
{
}

gethostplayer()
{
    var_0 = getentarray( "player", "classname" );

    for ( var_1 = 0; var_1 < var_0.size; var_1++ )
    {
        if ( var_0[var_1] ishost() )
            return var_0[var_1];
    }
}

get_host()
{
    host = gethostplayer();
    
    while (!isdefined(host))
    {
        wait(0.25);
        host = gethostplayer();
    }
    
    return host;
}

wait_for_host()
{
    host = get_host();
    
    while ( isdefined( host ) && !host.hasspawned && !isdefined( host.selectedclass ) )
    {
        wait(0.25);
    }
    
    return 1;
}

init_stub()
{
    thread monitor_smoke_grenades();
    thread bot_triggers();
    initbotlevelvariables();

    refresh_existing_bots();
    wait_for_host();

    var_0 = botautoconnectenabled();
    setmatchdata( "hasBots", 1 );
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
