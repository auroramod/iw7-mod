#include scripts\mp\bots\bots_loadout;

main()
{
    replaceFunc(scripts\mp\bots\bots_loadout::bot_loadout_choose_from_camotable, ::bot_loadout_choose_from_camotable);
    replaceFunc(scripts\mp\bots\bots_loadout::init_bot_camotable, ::init_bot_camotable);
    replaceFunc(scripts\mp\bots\bots_loadout::init_class_table, ::init_class_table);
    replaceFunc(scripts\mp\class::loadout_updateclasscallback, ::loadout_updateclasscallback);
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

// lets bots use weapon & scorestreak variants
loadout_updateclasscallback( var_0 )
{
    if ( !isdefined( self.classcallback ) )
        scripts\engine\utility::error( "self.classCallback function reference required for class 'callback'" );

    var_1 = self [[ self.classcallback ]]();

    if ( !isdefined( var_1 ) )
        scripts\engine\utility::error( "array required from self.classCallback for class 'callback'" );

    if ( isdefined( var_1["loadoutArchetype"] ) )
        var_0.loadoutarchetype = var_1["loadoutArchetype"];

    if ( isdefined( var_1["loadoutPrimary"] ) )
        var_0.loadoutprimary = var_1["loadoutPrimary"];

    for ( var_2 = 0; var_2 < 6; var_2++ )
    {
        var_3 = scripts\mp\class::getattachmentloadoutstring( var_2, "primary" );

        if ( isdefined( var_1[var_3] ) )
            var_0.loadoutprimaryattachments[var_2] = var_1[var_3];
    }

    if ( isdefined( var_1["loadoutPrimaryCamo"] ) )
        var_0.loadoutprimarycamo = var_1["loadoutPrimaryCamo"];

    if ( isdefined( var_1["loadoutPrimaryReticle"] ) )
        var_0.loadoutprimaryreticle = var_1["loadoutPrimaryReticle"];

    if ( isdefined( var_1["loadoutSecondary"] ) )
        var_0.loadoutsecondary = var_1["loadoutSecondary"];

    for ( var_2 = 0; var_2 < 5; var_2++ )
    {
        var_3 = scripts\mp\class::getattachmentloadoutstring( var_2, "secondary" );

        if ( isdefined( var_1[var_3] ) )
            var_0.loadoutsecondaryattachments[var_2] = var_1[var_3];
    }

    if ( isdefined( var_1["loadoutSecondaryCamo"] ) )
        var_0.loadoutsecondarycamo = var_1["loadoutSecondaryCamo"];

    if ( isdefined( var_1["loadoutSecondaryReticle"] ) )
        var_0.loadoutsecondaryreticle = var_1["loadoutSecondaryReticle"];

    if ( isdefined( var_1["loadoutPowerPrimary"] ) )
        var_0._id_AE7B = var_1["loadoutPowerPrimary"];

    if ( isdefined( var_1["loadoutPowerPrimaryPassives"] ) )
        var_0._id_AE7C = var_1["loadoutPowerPrimaryPassives"];

    if ( isdefined( var_1["loadoutExtraPowerPrimary"] ) )
        var_0.loadoutextrapowerprimary = var_1["loadoutExtraPowerPrimary"];

    if ( isdefined( var_1["loadoutPowerSecondary"] ) )
        var_0._id_AE7D = var_1["loadoutPowerSecondary"];

    if ( isdefined( var_1["loadoutPowerSecondaryPassives"] ) )
        var_0._id_AE7E = var_1["loadoutPowerSecondaryPassives"];

    if ( isdefined( var_1["loadoutExtraPowerSecondary"] ) )
        var_0.loadoutextrapowersecondary = var_1["loadoutExtraPowerSecondary"]; // stock reads "loadoutPowerExtraSecondary" here

    if ( isdefined( var_1["loadoutSuper"] ) )
        var_0.loadoutsuper = var_1["loadoutSuper"];

    if ( isdefined( var_1["loadoutGesture"] ) )
        var_0.loadoutgesture = var_1["loadoutGesture"];

    var_0.loadoutstreaksfilled = isdefined( var_1["loadoutStreak1"] ) || isdefined( var_1["loadoutStreak2"] ) || isdefined( var_1["loadoutStreak3"] );

    if ( isdefined( var_1["loadoutStreakType"] ) )
        var_0.loadoutstreaktype = var_1["loadoutStreakType"];

    if ( isdefined( var_1["loadoutStreak1"] ) )
        var_0.loadoutkillstreak1 = var_1["loadoutStreak1"];

    if ( isdefined( var_1["loadoutStreak2"] ) )
        var_0.loadoutkillstreak2 = var_1["loadoutStreak2"];

    if ( isdefined( var_1["loadoutStreak3"] ) )
        var_0.loadoutkillstreak3 = var_1["loadoutStreak3"];

    if ( isdefined( var_1["loadoutKillstreak1Passives"] ) )
        var_0._id_AE6F = var_1["loadoutKillstreak1Passives"];

    if ( isdefined( var_1["loadoutKillstreak2Passives"] ) )
        var_0._id_AE71 = var_1["loadoutKillstreak2Passives"];

    if ( isdefined( var_1["loadoutKillstreak3Passives"] ) )
        var_0._id_AE73 = var_1["loadoutKillstreak3Passives"];

    if ( isbot( self ) )
        bot_loadout_apply_variants( var_0 );
}

bot_loadout_apply_variants( loadout )
{
    loadout.loadoutprimaryvariantid = bot_get_weapon_variant( loadout.loadoutprimary );
    loadout._id_AEA5 = bot_get_weapon_variant( loadout.loadoutsecondary ); // loadoutsecondaryvariantid

    // killstreak "variant id" is the loot item id, passives come from the killstreak loot table
    loadout.loadoutkillstreak1variantid = bot_get_killstreak_variant( loadout.loadoutkillstreak1 );
    loadout.loadoutkillstreak2variantid = bot_get_killstreak_variant( loadout.loadoutkillstreak2 );
    loadout.loadoutkillstreak3variantid = bot_get_killstreak_variant( loadout.loadoutkillstreak3 );
    loadout._id_AE6F = scripts\mp\killstreak_loot::getpassiveperk( loadout.loadoutkillstreak1variantid );
    loadout._id_AE71 = scripts\mp\killstreak_loot::getpassiveperk( loadout.loadoutkillstreak2variantid );
    loadout._id_AE73 = scripts\mp\killstreak_loot::getpassiveperk( loadout.loadoutkillstreak3variantid );
}

// variant ids per weapon root, only ones that are real loot items
init_bot_weapon_variants()
{
    loot_table = "mp/loot/iw7_weapon_loot_master.csv";
    level.bot_weapon_variants = [];

    for ( row = 0;; row++ )
    {
        loot_id = tablelookupbyrow( loot_table, row, 0 );

        if ( !isdefined( loot_id ) || loot_id == "" )
            break;

        ref = tablelookupbyrow( loot_table, row, 1 );
        root = tablelookupbyrow( loot_table, row, 2 );
        weapon_table = "mp/loot/weapon/" + root + ".csv";
        variant_id = tablelookup( weapon_table, 1, ref, 0 );

        if ( !isdefined( variant_id ) || variant_id == "" || tablelookup( weapon_table, 1, ref, 3 ) == "" )
            continue;

        if ( !isdefined( level.bot_weapon_variants[root] ) )
            level.bot_weapon_variants[root] = [];

        level.bot_weapon_variants[root][level.bot_weapon_variants[root].size] = int( variant_id );
    }
}

bot_get_weapon_variant( weapon )
{
    if ( !isdefined( weapon ) || weapon == "none" )
        return -1;

    if ( !isdefined( level.bot_weapon_variants ) )
        init_bot_weapon_variants();

    root = scripts\mp\utility::getweaponrootname( weapon );

    if ( !isdefined( self.bot_weapon_variant ) )
        self.bot_weapon_variant = [];

    if ( !isdefined( self.bot_weapon_variant[root] ) )
    {
        variant_id = -1;

        if ( isdefined( level.bot_weapon_variants[root] ) && randomint( 100 ) < getdvarint( "bot_weapon_variant_chance", 75 ) )
            variant_id = scripts\engine\utility::random( level.bot_weapon_variants[root] );

        self.bot_weapon_variant[root] = variant_id;
    }

    return self.bot_weapon_variant[root];
}

bot_get_killstreak_variant( streak )
{
    if ( !isdefined( streak ) || streak == "none" || streak == "" )
        return -1;

    if ( !isdefined( self.bot_killstreak_variant ) )
        self.bot_killstreak_variant = [];

    if ( !isdefined( self.bot_killstreak_variant[streak] ) )
    {
        loot_id = undefined;

        if ( randomint( 100 ) < getdvarint( "bot_killstreak_variant_chance", 75 ) )
            loot_id = scripts\mp\killstreak_loot::getrandomvariantfrombaseref( streak );

        if ( !isdefined( loot_id ) )
            loot_id = -1;

        self.bot_killstreak_variant[streak] = loot_id;
    }

    return self.bot_killstreak_variant[streak];
}
