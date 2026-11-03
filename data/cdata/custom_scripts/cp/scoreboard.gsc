// The zombies scoreboard only has rows for four players. With more slots,
// send every player's stats to each client in the ui_cp_scoreboard dvar.
// ui_scripts/ZombiesPlayers reads it back.
//
// Format: "client cash score kills headshots revives downs;" per player.
// It is only sent when it changes, at most once a second.

main()
{
    if (getmaxclients() <= 4)
    {
        return;
    }

    replacefunc(scripts\cp\cp_persistence::eog_player_update_stat, ::eog_player_update_stat);
}

init()
{
    if (getmaxclients() <= 4)
    {
        return;
    }

    level thread update_scoreboard();
}

// Stock reads the running total back from the four-row EoG table, so players
// in slot 4 and up never added up (and slot 4 overwrote row 0). Keep their
// totals on the player instead.
eog_player_update_stat(name, value, absolute)
{
    slot = self getentitynumber();

    if (!isdefined(self.eog_stats))
    {
        self.eog_stats = [];
    }

    total = value;
    if (!isdefined(absolute) || !absolute)
    {
        previous = self.eog_stats[name];
        if (slot < 4)
        {
            previous = self getrankedplayerdata("cp", "EoGPlayer", slot, name);
        }

        if (!isdefined(previous))
        {
            previous = 0;
        }

        total = int(previous) + int(value);
    }

    scripts\cp\cp_persistence::try_update_lb_playerdata(name, total, 1);

    if (slot < 4)
    {
        foreach (player in level.players)
        {
            player setplayerdata("cp", "EoGPlayer", slot, name, total);
        }
    }
    else
    {
        self.eog_stats[name] = total;
    }
}

update_scoreboard()
{
    level endon("game_ended");

    for (;;)
    {
        wait 1;

        text = "";
        foreach (player in level.players)
        {
            text += stats(player) + ";";
        }

        foreach (player in level.players)
        {
            if (!isbot(player) && (!isdefined(player.cp_scoreboard) || player.cp_scoreboard != text))
            {
                player.cp_scoreboard = text;
                player setclientdvar("ui_cp_scoreboard", text);
            }
        }
    }
}

stats(player)
{
    values = [
        player getentitynumber(),
        player getrankedplayerdata("cp", "alienSession", "currency"),
        player getrankedplayerdata("cp", "alienSession", "score"),
        player getrankedplayerdata("cp", "alienSession", "kills"),
        headshots(player),
        player getrankedplayerdata("cp", "alienSession", "revives"),
        player getrankedplayerdata("cp", "alienSession", "downed")
    ];

    text = "";
    foreach (value in values)
    {
        if (text != "")
        {
            text += " ";
        }
        text += int(value);
    }

    return text;
}

headshots(player)
{
    total = 0;

    if (isdefined(player.headshots))
    {
        foreach (count in player.headshots)
        {
            total += count;
        }
    }

    return total;
}
