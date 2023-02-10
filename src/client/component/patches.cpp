#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "fastfiles.hpp"
#include "filesystem.hpp"
#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace patches
{
	namespace
	{
		utils::hook::detour com_register_common_dvars_hook;
		utils::hook::detour com_game_mode_supports_feature_hook;
		utils::hook::detour cg_set_client_dvar_from_server_hook;
		utils::hook::detour live_get_map_index_hook;
		utils::hook::detour content_do_we_have_content_pack_hook;

		std::string get_login_username()
		{
			char username[UNLEN + 1];
			DWORD username_len = UNLEN + 1;
			if (!GetUserNameA(username, &username_len))
			{
				return "Unknown Soldier";
			}

			return std::string{ username, username_len - 1 };
		}

		void com_register_common_dvars_stub()
		{
			game::dvar_t* name_dvar;
			game::dvar_t* com_maxfps;
			game::dvar_t* cg_fov;
			game::dvar_t* cg_fovScale;

			name_dvar = game::Dvar_RegisterString("name", get_login_username().data(), game::DVAR_FLAG_SAVED, "Player name.");
			com_maxfps = game::Dvar_RegisterInt("com_maxfps", 0, 0, 1000, game::DVAR_FLAG_SAVED, "Cap frames per second");
			cg_fov = game::Dvar_RegisterFloat("cg_fov", 65.0f, 1.0f, 160.f, game::DVAR_FLAG_SAVED,
				"The field of view angle in degrees");
			cg_fovScale = game::Dvar_RegisterFloat("cg_fovScale", 1.0f, 0.1f, 2.0f, game::DVAR_FLAG_SAVED,
				"Scale applied to the field of view");

			*reinterpret_cast<game::dvar_t**>(0x6005758_b) = com_maxfps;
			dvars::disable::re_register("com_maxfps");
			dvars::disable::de_register("com_maxfps");

			*reinterpret_cast<game::dvar_t**>(0x1FA6DA0_b) = cg_fov;
			dvars::disable::re_register("cg_fov");
			dvars::disable::de_register("cg_fov");

			*reinterpret_cast<game::dvar_t**>(0x1FA6DB0_b) = cg_fovScale;
			dvars::disable::re_register("cg_fovScale");
			dvars::disable::de_register("cg_fovScale");

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
			return game::Dvar_FindVar("name")->current.string;
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
			auto result = utils::hook::invoke<__int64>(0xB7AC60_b, dvar, &index); // NetConstStrings_SV_GetNetworkDvarIndex
			if (result)
			{
				std::string index_str = std::to_string(index);
				return cg_set_client_dvar_from_server_hook.invoke<void>(client_num, cgame_glob, index_str.data(), value);
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

		char* db_read_raw_file_stub(const char* filename, char* buf, const int size)
		{
			const auto file = game::filesystem::file(filename);
			if (file.exists())
			{
				snprintf(buf, size, "%s\n", file.get_buffer().data());
				return buf;
			}

			return game::DB_ReadRawFile(filename, buf, size);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// register custom dvars
			com_register_common_dvars_hook.create(0xBADF30_b, com_register_common_dvars_stub);

			// patch some features
			com_game_mode_supports_feature_hook.create(0x5AFDE0_b, com_game_mode_supports_feature_stub);

			// get client name from dvar
			utils::hook::jump(0xD32770_b, live_get_local_client_name);

			// write better config
			utils::hook::jump(0xBB2A90_b, dvar_write_variables_stub);

			// show missing fastfiles
			utils::hook::call(0x3BBD4B_b, missing_content_error_stub);

			// show missing map
			stored_mapname = nullptr;
			live_get_map_index_hook.create(0xCE72C0_b, live_get_map_index_stub);
			content_do_we_have_content_pack_hook.create(0xCE8550_b, content_do_we_have_content_pack_stub);

			// make setclientdvar behave like older games
			cg_set_client_dvar_from_server_hook.create(0x856D70_b, cg_set_client_dvar_from_server_stub);
			utils::hook::call(0xB0A9BB_b, get_client_dvar_checksum); // setclientdvar
			utils::hook::call(0xB0ACD7_b, get_client_dvar_checksum); // setclientdvars
			utils::hook::call(0xB0A984_b, get_client_dvar); // setclientdvar
			utils::hook::call(0xB0AC9F_b, get_client_dvar); // setclientdvars
			utils::hook::set<uint8_t>(0xB0A9AC_b, 0xEB); // setclientdvar
			utils::hook::set<uint8_t>(0xB0ACC8_b, 0xEB); // setclientdvars

			// Allow executing custom cfg files with the "exec" command
			utils::hook::call(0xB7CEF9_b, db_read_raw_file_stub);

			// don't reset our fov
			utils::hook::set<uint8_t>(0x8A6160_b, 0xC3);

			// some [data validation] anti tamper thing that kills performance
			dvars::override::register_int("dvl", 0, 0, 0, game::DVAR_FLAG_READ);
		}
	};
}

REGISTER_COMPONENT(patches::component)