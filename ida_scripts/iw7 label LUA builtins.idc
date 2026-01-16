/*

    label all Infinite Warfare LUA engine functions
    by: mjkzy & quaK
    created: 1-15-2026

*/

#include <idc.idc>

static LabelFunction(name, ea)
{
    auto final_name;

    if (ea == BADADDR || ea == -1 || ea == 0)
    {
        Message("%s has invalid address\n", name);
        return;
    }

    final_name = form("LUI_CoD_LuaCall_%s", name);

    if (get_func_attr(ea, FUNCATTR_START) == BADADDR)
        add_func(ea);

    if (!MakeNameEx(ea, final_name, SN_FORCE))
        Message("failed to label %s at %a\n", final_name, ea);
}

static LabelFunctions(table_ea)
{
    auto cur, name_ptr, fn_ea, name;

    cur = table_ea;

    while (1)
    {
        name_ptr = get_qword(cur);
        if (name_ptr == 0 || name_ptr == BADADDR)
            break;

        fn_ea = get_qword(cur + 8);

        name = GetString(name_ptr, -1, ASCSTR_C);
        if (name == 0)
            name = form("sub_%a", fn_ea);

        LabelFunction(name, fn_ea);

        cur = cur + 16;
    }
}

static main(void)
{
    // Engine
    LabelFunctions(0x1414B42F0);
    // Friends
    LabelFunctions(0x1414B3B70);
    // Lobby
    LabelFunctions(0x1414AE730);
    // LobbyMember
    LabelFunctions(0x1414AEEE0);
    // PlayerData
    LabelFunctions(0x141487090);
    // Playlist
    LabelFunctions(0x1414B0960);
    // MLG
    LabelFunctions(0x1414AFDF0);
    // Groups
    LabelFunctions(0x141445860);
    // Game
    LabelFunctions(0x1414AD9B0);
    // On Screen Keyboard
    LabelFunctions(0x141445670);
    // Customization
    LabelFunctions(0x1414AD958);
    // Fences
    LabelFunctions(0x141489970);
    // CharacterScene
    LabelFunctions(0x1414ABF20);
    // ClientCharacter
    LabelFunctions(0x141486BE0);
    // ClientWeapon
    LabelFunctions(0x1414872E0);
    // Streaming
    LabelFunctions(0x141487160);
    // MatchRules
    LabelFunctions(0x1414B3E90);
    // DCache
    LabelFunctions(0x1414B40D0);
    // ShaderUpload
    LabelFunctions(0x1414B4130);
    // LoadoutDrafting
    LabelFunctions(0x1414B3FF0);
    
    // PrettyNumber
    LabelFunctions(0x1414B0C50);
    // UpSell
    LabelFunctions(0x1414980E0);
    // CoDAccount
    LabelFunctions(0x141497E60);
    // Rewards
    LabelFunctions(0x141497230);
    // Redeemables
    LabelFunctions(0x141497080);
    // Loot
    LabelFunctions(0x1414948E0);
    // Commerce
    LabelFunctions(0x141493EC0);
    // Game 2
    LabelFunctions(0x141489E30);

    Message("Labeling of LUI functions complete!\n");
}
