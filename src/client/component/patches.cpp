#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"
#include "command.hpp"
#include "party.hpp"
#include "network.hpp"
#include "console/console.hpp"

#include "fastfiles.hpp"
#include "filesystem.hpp"
#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/info_string.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour com_register_common_dvars_hook;
		utils::hook::detour com_game_mode_supports_feature_hook;
		utils::hook::detour live_get_map_index_hook;
		utils::hook::detour content_do_we_have_content_pack_hook;

		std::string get_login_username()
		{
			return "Unknown Soldier";
		}

		void com_register_common_dvars_stub()
		{
			game::dvar_t* name_dvar;
			game::dvar_t* com_maxfps;

			name_dvar = game::Dvar_RegisterString("name", get_login_username().data(), game::DVAR_FLAG_SAVED, "Player name.");

			if (game::environment::is_dedi())
			{
				com_maxfps = game::Dvar_RegisterInt("com_maxfps", 250, 0, 250, game::DVAR_FLAG_NONE, "Cap frames per second");
			}
			else
			{
				com_maxfps = game::Dvar_RegisterInt("com_maxfps", 0, 0, 1000, game::DVAR_FLAG_SAVED, "Cap frames per second");
			}

			*reinterpret_cast<game::dvar_t**>(0x146005758) = com_maxfps;
			dvars::disable::re_register("com_maxfps");
			dvars::disable::de_register("com_maxfps");

			return com_register_common_dvars_hook.invoke<void>();
		}

		bool com_game_mode_supports_feature_stub(game::Com_GameMode_Feature feature)
		{
			if (feature == game::FEATURE_GRAVITY)
			{
				return true;
			}
			else if (feature == game::FEATURE_TIMESCALE)
			{
				return true;
			}

			return com_game_mode_supports_feature_hook.invoke<bool>(feature);
		}

		const char* live_get_local_client_name()
		{
			static const auto* name = game::Dvar_FindVar("name");
			return name != nullptr ? name->current.string : "Unknown Soldier";
		}

		std::vector<std::string> dvar_save_variables;
		void dvar_write_single_variable(const game::dvar_t* dvar, int* user_data)
		{
			if ((dvar->flags & game::DVAR_FLAG_SAVED) != 0)
			{
				const char* val = game::Dvar_DisplayableLatchedValue(dvar);
				auto h = *user_data;

				std::string dvar_name = dvars::dvar_get_name(dvar);
				if (dvar_name.empty())
				{
					game::FS_Printf(h, "setcl %d \"%s\"\n", dvar->checksum, val);
				}
				else
				{
					dvar_save_variables.push_back(dvar_name);
				}
			}
		}

		void dvar_write_variables_stub(int handle)
		{
			dvar_save_variables.clear();

			int* user_data = &handle;
			game::Dvar_ForEach(dvar_write_single_variable, user_data);

			std::sort(dvar_save_variables.begin(), dvar_save_variables.end()); // alphabetize sort
			for (size_t i = 0; i < dvar_save_variables.size(); i++)
			{
				const auto* dvar_name = dvar_save_variables.at(i).data();
				const auto* dvar = game::Dvar_FindVar(dvar_name);
				const char* val = game::Dvar_DisplayableLatchedValue(dvar);
				game::FS_Printf(handle, "seta %s \"%s\"\n", dvar_name, val);
			}
		}

		void missing_content_error_stub(int, const char*)
		{
			game::Com_Error(game::ERR_DROP, utils::string::va("MISSING FILE\n%s.ff",
				fastfiles::get_current_fastfile().data()));
		}

		const char* stored_mapname;
		int live_get_map_index_stub(const char* map)
		{
			stored_mapname = map;
			return live_get_map_index_hook.invoke<int>(map);
		}

		bool content_do_we_have_content_pack_stub(int index)
		{
			if (stored_mapname != nullptr && !fastfiles::exists(stored_mapname))
			{
				stored_mapname = nullptr;
				return false;
			}
			return content_do_we_have_content_pack_hook.invoke<bool>(index);
		}

		void cg_set_client_dvar_from_server_stub(void* client_num, void* cgame_glob, const char* dvar_checksum, const char* value)
		{
			unsigned int checksum = static_cast<unsigned int>(atoi(dvar_checksum));
			auto* dvar = game::Dvar_FindMalleableVar(checksum);

			static unsigned int cg_fov_checksum = game::Dvar_GenerateChecksum("cg_fov");
			static unsigned int cg_fovScale_checksum = game::Dvar_GenerateChecksum("cg_fovScale");

			if (checksum == cg_fov_checksum ||
				checksum == cg_fovScale_checksum)
			{
				return;
			}

			// register new dvar
			if (!dvar)
			{
				game::Dvar_SetFromStringByChecksum(checksum, value, game::DvarSetSource::DVAR_SOURCE_EXTERNAL);
			}
			// only set if dvar has no flags or has external flag
			else if (dvar->flags == game::DVAR_FLAG_NONE ||
				(dvar->flags & game::DVAR_FLAG_EXTERNAL) != 0)
			{
				game::Dvar_SetFromStringFromSource(dvar, value, game::DvarSetSource::DVAR_SOURCE_EXTERNAL);
			}

			// original code
			unsigned int index = 0;
			auto result = utils::hook::invoke<__int64>(0x140B7AC60, dvar, &index); // NetConstStrings_SV_GetNetworkDvarIndex
			if (result)
			{
				std::string index_str = std::to_string(index);
				return utils::hook::invoke<void>(0x140856D70, client_num, cgame_glob, index_str.data(), value);
			}
		}

		game::dvar_t* get_client_dvar(const char* name)
		{
			game::dvar_t* dvar = game::Dvar_FindVar(name);
			if (!dvar)
			{
				static game::dvar_t dummy{ 0 };
				dummy.checksum = game::Dvar_GenerateChecksum(name);
				return &dummy;
			}
			return dvar;
		}

		bool get_client_dvar_checksum(game::dvar_t* dvar, unsigned int* checksum)
		{
			*checksum = dvar->checksum;
			return true;
		}

		utils::hook::detour db_read_raw_file_hook;
		const char* db_read_raw_file_stub(const char* filename, char* buf, const int size)
		{
			std::string buffer{};
			if (filesystem::read_file(filename, &buffer))
			{
				snprintf(buf, size, "%s\n", buffer.data());
				return buf;
			}

			return db_read_raw_file_hook.invoke<const char*>(filename, buf, size);
		}

		void cbuf_execute_buffer_internal_stub(int local_client_num, int controller_index, char* buffer, [[maybe_unused]]void* callback)
		{
			game::Dvar_OverrideCheatProtection(0);
			game::Cbuf_ExecuteBufferInternal(local_client_num, controller_index, buffer, game::Cmd_ExecuteSingleCommand);
			game::Dvar_OverrideCheatProtection(1);
		}

		void init_network_dvars_stub(game::dvar_t* dvar)
		{
			//init_network_dvars_hook.invoke<void>(dvar);
		}

		void disconnect()
		{
			utils::hook::invoke<void>(0x140C58E20); // SV_MainMP_MatchEnd
			for (int i = 0; i < 18; i++)
			{
				party::g_memberInfo[i] = {};
				party::g_memberInfoValid[i] = false;
			}
		}

		void* update_last_seen_players_stub()
		{
			return utils::hook::assemble([](utils::hook::assembler& a)
			{
				const auto safe_continue = a.newLabel();

				// (game's code)
				a.mov(rax, ptr(rsi)); // g_entities pointer

				// Avoid crash if pointer is nullptr
				a.test(rax, rax);
				a.jz(safe_continue);

				// Jump back in (game's code)
				a.mov(dword_ptr(rax, 0x4D10), 0);

				// Continue to next iter in this loop
				a.bind(safe_continue);
				a.jmp(0x140B22287);
			});
		}

		void request_start_match(game::PartyData* party, bool/* skip_start_countdown*/)
		{
			utils::hook::invoke<void>(0x1409D8900, party, true); // PartyHost_RequestStartMatch
		}

		void dvar_set_command_stub(const char* name, const char* value, bool superuser)
		{
			// party_maxplayers is the true max clients value
			if (!strcmp(name, "sv_maxclients"))
				name = "party_maxplayers";

			utils::hook::invoke<void>(0x140CECB30, name, value, superuser);
		}

		utils::hook::detour cmd_lui_notify_server_hook;
		void cmd_lui_notify_server_stub(game::gentity_s* ent)
		{
			const auto svs_clients = *game::svs_clients;
			if (svs_clients == nullptr)
			{
				return;
			}

			command::params_sv params{};
			const auto menu_id = atoi(params.get(1));
			const auto client = &svs_clients[ent->s.number];

			if (client == nullptr)
			{
				return;
			}

			//// 161 => "end_game"
			if (menu_id == 161 && client->remoteAddress.type != game::NA_LOOPBACK)
			{
				game::SV_DropClient(client, "PLATFORM_STEAM_KICK_CHEAT", true);
				return;
			}

			cmd_lui_notify_server_hook.invoke<void>(ent);
		}

		constexpr auto high_byte(std::uint64_t l)
		{
			return static_cast<std::uint8_t>((l >> 24) & 0xFF);
		}

		// Stop Server Crash from CL_NetChan_Transmit
		utils::hook::detour msg_readlong_hook;
		__int64 msg_readlong_stub(game::msg_t* msg)
		{
			void* retAddr = (void*)_ReturnAddress();

			if (retAddr == (void*)0x140C59438)
			{
				__int64 reliable_acknowledge = msg_readlong_hook.invoke<__int64>(msg);

				if (high_byte(static_cast<std::uint64_t>(reliable_acknowledge)) > 0x7F)
				{
					return 0;
				}
				return reliable_acknowledge;
			}

			return msg_readlong_hook.invoke<__int64>(msg);
		}
		
		void op_wait_entry_stub(utils::hook::assembler& a)
		{
			const auto handle_float = a.newLabel();
			const auto handle_int = a.newLabel();
			const auto finish_wait = a.newLabel();
			const auto script_error = a.newLabel();

			a.mov(eax, dword_ptr(rbx, 8)); // Type? Float = 5 | Int = 6
			a.cmp(eax, 5);
			a.je(handle_float);
			a.cmp(eax, 6);
			a.je(handle_int);

			a.jmp(0x140C0EA73); // Default error path

			a.bind(handle_float);
			a.movss(xmm1, dword_ptr(rbx)); // Load the float scalar-wise from rbx
 
			// x20 scaling - avoid relying on xmm7/xmm8 - could change?
			const auto l_20 = a.newLabel();
			const auto l_05 = a.newLabel();
			a.mulss(xmm1, ptr(l_20));
			a.addss(xmm1, ptr(l_05));
			a.cvttss2si(edi, xmm1);	// Round to an int
			a.jmp(finish_wait);

			a.bind(l_20);
			a.embedFloat(20.0f);
			a.bind(l_05);
			a.embedFloat(0.5f);

			// Check for negative result (LABEL_258 check)
			a.test(edi, edi);
			a.js(script_error);
			a.jmp(finish_wait);

			a.bind(handle_int);
			a.mov(eax, dword_ptr(rbx));	// Load int
			a.imul(eax, eax, 20); // ticks = secs * 20
			a.mov(edi, eax);

			a.test(edi, edi); // sign-check for integers
			a.js(script_error);

			// Ensure edi (v174) is at least 1 if original float wasn't 0.0
			a.bind(finish_wait);
			const auto not_zero = a.newLabel();
			a.test(edi, edi);
			a.jnz(not_zero);
			a.mov(edi, 1);
			a.bind(not_zero);

			// Save the result
			a.mov(dword_ptr(rsp, 0x440 - 0x3FC), edi);
			a.jmp(0x140C0EA92);

			a.bind(script_error);
			a.jmp(0x140C0EB63);
		}

		utils::hook::detour Party_FillInOurMemberInfo_Hook;
		void Party_FillInOurMemberInfo_Stub(game::PartyData* party, int localControllerIndex, game::ClientAuthoritativeMemberInfo* memberInfo)
		{
			Party_FillInOurMemberInfo_Hook.invoke(party, localControllerIndex, memberInfo);
			strncpy_s(memberInfo->clanAbbrev, game::GamerProfile_GetClanName(localControllerIndex), 4);
			memberInfo->clanAbbrev[4] = 0;
		}

		utils::hook::detour ClientUserinfoChanged_Hook;
		void ClientUserinfoChanged_Stub(signed int clientNum)
		{
			auto client = game::g_entities[clientNum].client;
			if (!client) return;

			char s[1024];
			char destination[1024];

			game::SV_GetUserinfo(clientNum, s, 1024);
			if (!game::Info_Validate(s))
			{
				strcpy_s(s, "\\name\\badinfo");
			}

			auto isLocalClient = game::SV_IsLocalClient((unsigned int)clientNum);
			bool isConnected = (client->sess.connected == 2);
			*(DWORD*)&client->sess.__pad0[0x114] = isLocalClient;

			if (isConnected && game::dword_143CA1588)
			{
				char* nameVal = game::Info_ValueForKey(s, "name");
				game::Com_CleanName(nameVal, client->sess.name, 32);
			}
			else
			{
				strncpy_s(destination, client->sess.cs.name, 0x3FFu);
				destination[1023] = 0;
				char* nameVal = game::Info_ValueForKey(s, "name");
				game::Com_CleanName(nameVal, client->sess.cs.name, 32);

				strncpy_s(client->sess.name, client->sess.cs.name, 31);
				client->sess.name[31] = 0;

				auto clanVal = game::Info_ValueForKey(s, "clanAbbrev");

				if (clanVal && *clanVal)
				{
					strncpy_s(client->sess.cs.clanAbbrev, clanVal, 7);
					client->sess.cs.clanAbbrev[7] = 0;
				}
				else 
				{
					memset(client->sess.cs.clanAbbrev, 0, sizeof(client->sess.cs.clanAbbrev));
				}

				*(DWORD*)&client->__pad1[76] = atol(game::Info_ValueForKey(s, "mlg_spectator")) == 1;
				*(DWORD*)&client->__pad1[68] = atol(game::Info_ValueForKey(s, "pph"));
				*(DWORD*)&client->__pad1[72] = atol(game::Info_ValueForKey(s, "pps"));
			}

			uintptr_t managerAddr = *(uintptr_t*)game::qword_143F25A88;
			if (managerAddr) 
			{
				uintptr_t* vtable = *(uintptr_t**)managerAddr;
				auto GetClientObj = (uintptr_t(__fastcall*)(uintptr_t, int))vtable[4];
				auto Finalize = (uintptr_t(__fastcall*)(uintptr_t, int))vtable[6];

				uintptr_t clientObj = GetClientObj(managerAddr, clientNum);
				if (clientObj)
				{
					// Copy the finalized name to the UI object
					strncpy_s((char*)(clientObj + 4), 32, client->sess.cs.name, 31);
					strncpy_s((char*)(clientObj + 52), 8, client->sess.cs.clanAbbrev, 7);
				}

				uintptr_t result = Finalize(managerAddr, clientNum);
				if (result)
				{
					*(int*)(result + 12) = client->sess.cs.team;
				}
			}
		}

		utils::hook::detour PlayerCmd_GetClanTag_Hook;
		void PlayerCmd_GetClanTag_Stub(game::scr_entref_t entref)
		{
			auto entnum = entref.entnum;
			if (entref.classnum)
			{
				game::Scr_ErrorInternal();
				return;
			}

			auto entity = &game::g_entities[entnum];
			if (entity->client == nullptr)
			{
				game::Scr_ErrorInternal();
				return;
			}

			auto clanAbbrev = entity->client->sess.cs.clanAbbrev;
			game::Scr_AddString(clanAbbrev);
		}

		utils::hook::detour LUI_PushPlayerName_Hook;
		bool LUI_PushPlayerName_Stub(int localClientNum, int clientNum, int playerNameSize, char* outPlayerName)
		{
			if (clientNum < 18)
			{
				auto lobbyMember = &party::g_clientMemberInfo[clientNum];
				auto lobbyMemberValid = &party::g_clientMemberInfoValid[clientNum];

				if (!lobbyMember || !lobbyMemberValid) return LUI_PushPlayerName_Hook.invoke<bool>(localClientNum, clientNum, playerNameSize, outPlayerName);

				auto lobbyMemberClanAbbrev = lobbyMember->clanTag.c_str();

				if (lobbyMemberClanAbbrev && *lobbyMemberClanAbbrev)
				{
					strcpy_s(outPlayerName, 43, utils::string::va("[%s]%s", lobbyMemberClanAbbrev, lobbyMember->name.c_str()));
					return true;
				}
			}

			return LUI_PushPlayerName_Hook.invoke<bool>(localClientNum, clientNum, playerNameSize, outPlayerName);
		}

		utils::hook::detour playertag_hook;
		bool playertag_stub(int localClientNum, int clientNum, char* gamerTag, int gamerTagLength, char* clanAbbrev, int clanTagLength)
		{
			auto result = playertag_hook.invoke<bool>(localClientNum, clientNum, gamerTag, gamerTagLength, clanAbbrev, clanTagLength);
			
			if (clientNum < 18)
			{
				auto lobbyMember = &party::g_clientMemberInfo[clientNum];
				auto lobbyMemberValid = &party::g_clientMemberInfoValid[clientNum];
				if (!lobbyMember || !lobbyMemberValid) return result;

				auto lobbyMemberClanAbbrev = lobbyMember->clanTag.c_str();
				if (lobbyMemberClanAbbrev && *lobbyMemberClanAbbrev)
				{
					strcpy_s(clanAbbrev, clanTagLength, utils::string::va("[%s]", lobbyMemberClanAbbrev));
				}
			}

			return result;
		}

		void send_member_info(const game::netadr_s& target)
		{
			game::ClientAuthoritativeMemberInfo memberInfo;
			memset(&memberInfo, 0, sizeof(memberInfo));
			auto party = game::Lobby_GetPartyData();
			game::Party_FillInOurMemberInfo(party, 0, &memberInfo);

			utils::info_string info{};
			char xuidStr[32]{};
			auto xuid = game::Live_GetXuid(0);
			game::XUIDToString(&xuid, xuidStr);
			info.set("xuid", xuidStr);
			info.set("gamertag", memberInfo.gamertag);
			info.set("clanAbbrev", memberInfo.clanAbbrev);

			network::send(target, "clientInfo", info.build(), '\n');
		}

		utils::hook::detour cl_parse_gamestate_hook;
		void cl_parse_gamestate_stub(int localClientNum, game::msg_t* msg, __int64 a3)
		{
			for (auto i = 0; i < 18; i++)
			{
				party::g_clientMemberInfo[i] = {};
				party::g_clientMemberInfoValid[i] = false;
			}
			cl_parse_gamestate_hook.invoke(localClientNum, msg, a3);
			auto* server_connection_state = party::get_server_connection_state();
			if (!game::Com_FrontEnd_IsInFrontEnd() && server_connection_state->hostDefined)
				send_member_info(server_connection_state->host);
		}

		int get_client_num_from_ptr(game::client_t* client)
		{
			for (unsigned int i = 0; i < *game::svs_numclients; i++)
			{
				if (game::svs_clients[i] == client) return i;
			}
			return -1;
		}

		utils::hook::detour sv_drop_client_hook;
		void sv_drop_client_stub(game::client_t* client, const char* reason, bool tellThem)
		{
			sv_drop_client_hook.invoke(client, reason, tellThem);
			if (game::Com_FrontEnd_IsInFrontEnd()) return;
			auto clientNum = get_client_num_from_ptr(client);
			if (clientNum >= 0 && clientNum < 18)
			{
				party::g_clientMemberInfo[clientNum] = {};
				party::g_clientMemberInfoValid[clientNum] = false;
			}
		}

		utils::hook::detour g_sayto_hook;
		void g_sayto_stub(game::gentity_s* ent, game::gentity_s* other, int mode, int color,
			const char* teamString, const char* cleanname, const char* message)
		{
			char finalName[64]{};
			auto clanAbbrev = ent->client->sess.cs.clanAbbrev;

			if (clanAbbrev && *clanAbbrev)
			{
				strcpy_s(finalName, utils::string::va("[%s]%s", clanAbbrev, cleanname));
			}
			else
			{
				strcpy_s(finalName, cleanname);
			}

			g_sayto_hook.invoke(ent, other, mode, color, teamString, finalName, message);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::nop(0x1409CB794, 6); // Stop clantag from being unset
			Party_FillInOurMemberInfo_Hook.create(0x1409CB720, Party_FillInOurMemberInfo_Stub); // Set clantag in memberinfo
			ClientUserinfoChanged_Hook.create(0x140B008A0, ClientUserinfoChanged_Stub); // Add clanAbbrev to clientState
			PlayerCmd_GetClanTag_Hook.create(0x140B0C9D0, PlayerCmd_GetClanTag_Stub); // Return clantag to gsc functions
			LUI_PushPlayerName_Hook.create(0x140504E70, LUI_PushPlayerName_Stub); // UI Elements
			playertag_hook.create(0x1409BDB20, playertag_stub); // Player Tags in-game

			if (!game::environment::is_dedi())
			{
				cl_parse_gamestate_hook.create(0x1409B6EE0, cl_parse_gamestate_stub);
			}

			sv_drop_client_hook.create(game::SV_DropClient, sv_drop_client_stub);
			g_sayto_hook.create(game::G_SayTo, g_sayto_stub);

			utils::hook::jump(0x140C0E9F5, utils::hook::assemble(op_wait_entry_stub), true);

			msg_readlong_hook.create(0x140BB37D0, msg_readlong_stub);

			cmd_lui_notify_server_hook.create(0x140B10B00, cmd_lui_notify_server_stub);

			// register custom dvars
			com_register_common_dvars_hook.create(0x140BADF30, com_register_common_dvars_stub);

			// patch some features
			com_game_mode_supports_feature_hook.create(game::Com_GameMode_SupportsFeature, com_game_mode_supports_feature_stub);

			// get client name from dvar
			utils::hook::jump(0x140D32770, live_get_local_client_name);

			// write better config
			utils::hook::jump(0x140BB2A90, dvar_write_variables_stub);

			// show missing fastfiles
			utils::hook::call(0x1403BBD4B, missing_content_error_stub);

			// show missing map
			stored_mapname = nullptr;
			live_get_map_index_hook.create(0x140CE72C0, live_get_map_index_stub);
			content_do_we_have_content_pack_hook.create(0x140CE8550, content_do_we_have_content_pack_stub);

			// make setclientdvar behave like older games
			utils::hook::call(0x14084A136, cg_set_client_dvar_from_server_stub);
			utils::hook::call(0x140B0A9BB, get_client_dvar_checksum); // setclientdvar
			utils::hook::call(0x140B0ACD7, get_client_dvar_checksum); // setclientdvars
			utils::hook::call(0x140B0A984, get_client_dvar); // setclientdvar
			utils::hook::call(0x140B0AC9F, get_client_dvar); // setclientdvars
			utils::hook::set<uint8_t>(0x140B0A9AC, 0xEB); // setclientdvar
			utils::hook::set<uint8_t>(0x140B0ACC8, 0xEB); // setclientdvars

			// Allow loading of rawfiles from disk
			db_read_raw_file_hook.create(game::DB_ReadRawFile, db_read_raw_file_stub);

			// Add cheat override to exec
			utils::hook::call(0x140B7CF11, cbuf_execute_buffer_internal_stub);

			// don't register every replicated dvar as a network dvar
			utils::hook::jump(0x140B7A920, init_network_dvars_stub);

			// some [data validation] anti tamper thing that kills performance
			dvars::override::register_int("dvl", 0, 0, 0, game::DVAR_FLAG_NONE);
			dvars::override::register_int("data_validation_allow_drop", 0, 0, 0, game::DVAR_FLAG_NONE);
			utils::hook::set<uint8_t>(0x1405C9FA0, 0xC3); // ValidateData
			utils::hook::set<uint8_t>(0x1405C9300, 0xC3); // ValidateMetaData
			utils::hook::set<uint8_t>(0x1405C9D70, 0xC3); // UpdateValidationDataInternal
			utils::hook::set<uint8_t>(0x1405C9590, 0xC3); // RegisterValidationData
			utils::hook::set<uint8_t>(0x1405C9960, 0xC3); // ShutdownValidationData
			utils::hook::set<uint8_t>(0x1405C8EC0, 0xC3); // FreeValidationData
			utils::hook::set<uint8_t>(0x1405C90C0, 0xC3);

			// killswitches
			dvars::override::register_bool("mission_team_contracts_enabled", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_store", false, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_quartermaster", false, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_cod_points", false, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_custom_emblems", false, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_matchID", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_mp_leaderboards", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_cp_leaderboards", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_streak_variants", false, game::DVAR_FLAG_READ);
			dvars::override::register_bool("killswitch_blood_anvil", false, game::DVAR_FLAG_READ);

			// announcer packs
			if (!game::environment::is_dedi())
			{
				dvars::override::register_bool("killswitch_announcers", false, game::DVAR_FLAG_READ);
				dvars::override::register_int("igs_announcer", 0x1F, 0, 0x7FFFFFFF, game::DVAR_FLAG_READ); // show all announcer packs
			}

			// disable cod account
			dvars::override::register_bool("enable_cod_account", false, game::DVAR_FLAG_READ);

			// enable boss battles
			dvars::override::register_bool("online_zombie_boss_battle", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_zmb", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_rave", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_disco", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_town", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_final", true, game::DVAR_FLAG_READ);
			dvars::override::register_bool("online_zombie_boss_dc", true, game::DVAR_FLAG_READ);

			// uncheat protect gamepad-related dvars
			dvars::override::register_float("gpad_button_deadzone", 0.13f, 0, 1, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("gpad_stick_deadzone_min", 0.2f, 0, 1, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("gpad_stick_deadzone_max", 0.01f, 0, 1, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("gpad_stick_pressed", 0.4f, 0, 1, game::DVAR_FLAG_SAVED);
			dvars::override::register_float("gpad_stick_pressed_hysteresis", 0.1f, 0, 1, game::DVAR_FLAG_SAVED);

			// disable host migration
			utils::hook::jump(0x140C5A200, disconnect);

			// precache is always allowed
			utils::hook::set(0x1406D5280, 0xC301B0); // NetConstStrings_IsPrecacheAllowed

			// allow localized string to create config strings post init
			utils::hook::nop(0x1405EE287, 2);

			utils::hook::nop(0x140E6A2FB, 2); // don't wait for occlusion query to succeed (forever loop)
			utils::hook::nop(0x140E6A30C, 2); // ^

			// Patch crash caused by the server trying to kick players for 'invalid password'
			//utils::hook::nop(0x140B2215B, 18);
			//utils::hook::jump(0x140B2215B, update_last_seen_players_stub(), true);

			// Start match without the timer
			utils::hook::jump(0x1409AA7F5, request_start_match);

			// register bot difficulty script dvars
			game::Dvar_RegisterInt("bot_difficulty_allies", 0, 0, 4, game::DVAR_FLAG_NONE, "Bot difficulty for friendly bots. 0: Mixed, 1: Recruit, 2: Regular, 3: Hardened, 4: Veteran");
			game::Dvar_RegisterInt("bot_difficulty_enemies", 0, 0, 4, game::DVAR_FLAG_NONE, "Bot difficulty for enemy bots. 0: Mixed, 1: Recruit, 2: Regular, 3: Hardened, 4: Veteran");
		
			// re-direct some dvars to others for backwards compatibility on configurations
			utils::hook::call(0x140BB241C, dvar_set_command_stub);

			// disable the cipher code as its not working
			game::Dvar_RegisterBool("online_qrm5tr_cipher_enabled", false, game::DVAR_FLAG_READ, "Is the cipher available in the Quartermaster");

			// allow servers to check for new packages more often
			dvars::override::register_int("sv_network_fps", 1000, 20, 1000, game::DVAR_FLAG_SAVED);

			// Massively increate timeouts
			dvars::override::register_int("cl_timeout", 90, 90, 1800, game::DVAR_FLAG_NONE); // Seems unused
			dvars::override::register_int("sv_timeout", 90, 90, 1800, game::DVAR_FLAG_NONE); // 30 - 0 - 1800
			dvars::override::register_int("cl_connectTimeout", 120, 120, 1800, game::DVAR_FLAG_NONE); // Seems unused
			dvars::override::register_int("sv_connectTimeout", 120, 120, 1800, game::DVAR_FLAG_NONE); // 60 - 0 - 1800
		}
	};
}

REGISTER_COMPONENT(patches::component)