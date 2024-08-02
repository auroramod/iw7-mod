init()
{
    level thread onPlayerConnect();
}

onPlayerConnect()
{
    for( ;; )
    {
        level waittill( "connected", player );
        player thread onPlayerSpawned();
    }
}

watchWeaponInspection()
{
    self endon( "death" );
    self endon( "disconnect" );
    self endon( "faux_spawn" );

    if ( isai( self ) || isbot( self ) )
        return;

    self notifyonplayercommand( "weapon_inspection", "+actionslot 8" );

    for (;;)
    {
        self waittill( "weapon_inspection" );

        self startweaponinspection();
        wait 0.05;

        while ( self isinspectingweapon() )
            wait 0.05;
    }
}

onPlayerSpawned()
{
    self endon( "disconnect" );

    for (;;)
    {
        scripts\engine\utility::waittill_any( "spawned_player", "faux_spawn" );
        self thread watchWeaponInspection();
    }
}