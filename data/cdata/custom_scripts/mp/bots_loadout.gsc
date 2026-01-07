#include scripts\mp\bots\bots_loadout;

main()
{
    replaceFunc(scripts\mp\bots\bots_loadout::bot_loadout_choose_from_camotable, ::bot_loadout_choose_from_camotable);
    replaceFunc(scripts\mp\bots\bots_loadout::init_bot_camotable, ::init_bot_camotable);
    replaceFunc(scripts\mp\bots\bots_loadout::init_class_table, ::init_class_table);
}

getcamotablecolumnindex(param_00)
{
	switch(param_00)
	{
		case "index":
			return 0;

		case "ref":
			return 1;

		case "type":
			return 2;

		case "target_material":
			return 3;

		case "tint":
			return 4;

		case "atlas_dims":
			return 5;

		case "name":
			return 6;

		case "image":
			return 7;

		case "weapon_index":
			return 8;

		case "bot_valid":
			return 9;

		case "description":
			return 10;

		case "category":
			return 11;

		default:
			return undefined;
	}
}

init_bot_camotable()
{
    var_0 = "mp/camotable.csv";
    var_1 = 1;
    var_2 = 8;
    var_3 = 9;
    level._id_2D1E = [];
    var_4 = 0;

    difficulty = ["recruit", "regular", "hardened", "veteran"];

    for (;;)
    {
        var_5 = tablelookupbyrow( var_0, var_4, getcamotablecolumnindex( "ref" ) );

        if ( !isdefined( var_5 ) || var_5 == "" )
            break;

        var_6 = tablelookupbyrow( var_0, var_4, getcamotablecolumnindex( "bot_valid" ) );

        if ( isdefined( var_6 ) && int( var_6 ) )
        {

            var_7 = tablelookupbyrow( var_0, var_4, getcamotablecolumnindex( "category" ) );

            if ( isDefined( var_7 ) )
            {
                foreach ( var_9 in difficulty )
                {
                    if ( !isdefined( level._id_2D1E[var_9] ) )
                        level._id_2D1E[var_9] = [];

                    var_10 = level._id_2D1E[var_9].size;
                    level._id_2D1E[var_9][var_10] = var_5;
                }
            }
        }

        var_4++;
    }
}

bot_loadout_choose_from_camotable( var_0, var_1, var_2, var_3, var_4 )
{
    var_5 = "none";

    if ( !isdefined( level._id_2D1E ) )
        return var_5;

    if ( !isdefined( level._id_2D1E[var_4] ) )
        return var_5;

    var_6 = randomint( 100 ) > randomint(30); // default is 50 chance to not pick camo

    if ( var_6 )
        var_5 = bot_loadout_choose_from_set( level._id_2D1E[var_4], var_0, var_1, var_2 );

    return var_5;
}

init_class_table()
{
    var_0 = "mp/botClassTable.csv";
    level.botloadoutsets = [];
    var_1 = bot_loadout_fields();
    var_2 = 0;

    for (;;)
    {
        var_2++;
        var_3 = tablelookup( var_0, 0, "botArchetype", var_2 );
        var_4 = tablelookup( var_0, 0, "botPersonalities", var_2 );
        var_5 = tablelookup( var_0, 0, "botDifficulties", var_2 );

        if ( !isdefined( var_3 ) || var_3 == "" )
            break;

        if ( !isdefined( var_4 ) || var_4 == "" )
            break;

        if ( !isdefined( var_5 ) || var_5 == "" )
            break;

        var_6 = [];

        foreach ( var_8 in var_1 )
        {
            if (var_8 == "loadoutPrimaryCamo" )
                var_6[var_8] = "camotable"; // this falls back to none originally
            else
                var_6[var_8] = tablelookup( var_0, 0, var_8, var_2 );
        }

        var_10 = strtok( var_3, "|" );
        var_11 = strtok( var_4, "| " );
        var_12 = strtok( var_5, "| " );

        foreach ( var_14 in var_10 )
        {
            var_14 = "archetype_" + var_14;
            var_6["loadoutArchetype"] = var_14;

            foreach ( var_16 in var_11 )
            {
                foreach ( var_18 in var_12 )
                {
                    var_19 = bot_loadout_set( var_14, var_16, var_18, 1 );
                    var_20 = spawnstruct();
                    var_20.loadoutvalues = var_6;
                    var_19.loadouts[var_19.loadouts.size] = var_20;
                }
            }
        }
    }
}
