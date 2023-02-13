#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/logfile.hpp"
#include "component/scripting.hpp"
#include "component/scheduler.hpp"
#include "component/gsc/script_extension.hpp"

#include "game/dvars.hpp"

#include <utils/hook.hpp>
#include <utils/io.hpp>

namespace logfile
{
	bool hook_enabled = true;

	namespace
	{
		struct gsc_hook_t
		{
			bool is_lua_hook{};
			const char* target_pos{};
			sol::protected_function lua_function;
		};

		std::unordered_map<const char*, gsc_hook_t> vm_execute_hooks;
		utils::hook::detour scr_player_killed_hook;
		utils::hook::detour scr_player_damage_hook;

		utils::hook::detour client_command_hook;

		utils::hook::detour g_log_printf_hook;

		std::vector<sol::protected_function> player_killed_callbacks;
		std::vector<sol::protected_function> player_damage_callbacks;

		std::vector<scripting::function> say_callbacks;

		game::dvar_t* logfile;
		game::dvar_t* g_log;

		utils::hook::detour vm_execute_hook;
		char empty_function[2] = {0x32, 0x34}; // CHECK_CLEAR_PARAMS, END
		const char* target_function = nullptr;

		sol::lua_value convert_entity(lua_State* state, const game::gentity_s* ent)
		{
			if (!ent)
			{
				return {};
			}

			const scripting::entity player{game::Scr_GetEntityId(ent->s.number, 0)};
			return scripting::lua::convert(state, player);
		}

		std::string get_weapon_name(unsigned int weapon, bool isAlternate)
		{
			char output[1024] = {0};
			game::BG_GetWeaponNameComplete(weapon, isAlternate, output, 1024);
			return output;
		}

		sol::lua_value convert_vector(lua_State* state, const float* vec)
		{
			if (!vec)
			{
				return {};
			}

			const auto vec_ = scripting::vector(vec);
			return scripting::lua::convert(state, vec_);
		}

		std::string convert_mod(const int meansOfDeath)
		{
			const auto value = reinterpret_cast<game::scr_string_t**>(0x196AA40_b)[meansOfDeath];
			const auto string = game::SL_ConvertToString(*value);
			return string;
		}

		void scr_player_killed_stub(game::gentity_s* self, const game::gentity_s* inflictor, 
			game::gentity_s* attacker, int damage, const int meansOfDeath, const unsigned int weapon, 
			const bool isAlternate, const float* vDir, const unsigned int hitLoc, int psTimeOffset, int deathAnimDuration)
		{
			{
				const std::string hitloc = reinterpret_cast<const char**>(0x196AAF0_b)[hitLoc];
				const auto mod_ = convert_mod(meansOfDeath);

				const auto weapon_ = get_weapon_name(weapon, isAlternate);

				for (const auto& callback : player_killed_callbacks)
				{
					const auto state = callback.lua_state();

					const auto self_ = convert_entity(state, self);
					const auto inflictor_ = convert_entity(state, inflictor);
					const auto attacker_ = convert_entity(state, attacker);

					const auto dir = convert_vector(state, vDir);

					const auto result = callback(self_, inflictor_, attacker_, damage,
						mod_, weapon_, dir, hitloc, psTimeOffset, deathAnimDuration);

					scripting::lua::handle_error(result);

					if (result.valid() && result.get_type() == sol::type::number)
					{
						damage = result.get<int>();
					}
				}

				if (damage == 0)
				{
					return;
				}
			}

			scr_player_killed_hook.invoke<void>(self, inflictor, attacker, damage, meansOfDeath, 
				weapon, isAlternate, vDir, hitLoc, psTimeOffset, deathAnimDuration);
		}

		void scr_player_damage_stub(game::gentity_s* self, const game::gentity_s* inflictor, 
			game::gentity_s* attacker, int damage, int dflags, const int meansOfDeath, 
			const unsigned int weapon, const bool isAlternate, const float* vPoint, 
			const float* vDir, const unsigned int hitLoc, const int timeOffset)
		{
			{
				const std::string hitloc = reinterpret_cast<const char**>(0x196AAF0_b)[hitLoc];
				const auto mod_ = convert_mod(meansOfDeath);

				const auto weapon_ = get_weapon_name(weapon, isAlternate);

				for (const auto& callback : player_damage_callbacks)
				{
					const auto state = callback.lua_state();

					const auto self_ = convert_entity(state, self);
					const auto inflictor_ = convert_entity(state, inflictor);
					const auto attacker_ = convert_entity(state, attacker);

					const auto point = convert_vector(state, vPoint);
					const auto dir = convert_vector(state, vDir);

					const auto result = callback(self_, inflictor_, attacker_, 
						damage, dflags, mod_, weapon_, point, dir, hitloc);

					scripting::lua::handle_error(result);

					if (result.valid() && result.get_type() == sol::type::number)
					{
						damage = result.get<int>();
					}
				}

				if (damage == 0)
				{
					return;
				}
			}

			scr_player_damage_hook.invoke<void>(self, inflictor, attacker, damage, dflags, 
				meansOfDeath, weapon, isAlternate, vPoint, vDir, hitLoc, timeOffset);
		}

		unsigned int local_id_to_entity(unsigned int local_id)
		{
			const auto variable = game::scr_VarGlob->objectVariableValue[local_id];
			return variable.u.f.next;
		}

		bool execute_vm_hook(const char* pos)
		{
			if (vm_execute_hooks.find(pos) == vm_execute_hooks.end())
			{
				hook_enabled = true;
				return false;
			}

			if (!hook_enabled && pos > reinterpret_cast<char*>(vm_execute_hooks.size()))
			{
				hook_enabled = true;
				return false;
			}

			const auto& hook = vm_execute_hooks[pos];
			if (hook.is_lua_hook)
			{
				const auto& function = hook.lua_function;
				const auto state = function.lua_state();

				const scripting::entity self = local_id_to_entity(game::scr_VmPub->function_frame->fs.localId);

				std::vector<sol::lua_value> args;

				const auto top = game::scr_function_stack->top;

				for (auto* value = top; value->type != game::VAR_PRECODEPOS; --value)
				{
					args.push_back(scripting::lua::convert(state, *value));
				}

				const auto result = function(self, sol::as_args(args));
				scripting::lua::handle_error(result);
				target_function = empty_function;
			}
			else
			{
				target_function = hook.target_pos;
			}

			return true;
		}

		void vm_execute_stub(utils::hook::assembler& a)
		{
			const auto replace = a.newLabel();
			const auto end = a.newLabel();

			a.pushad64();

			a.mov(rcx, r14);
			a.call_aligned(execute_vm_hook);

			a.cmp(al, 0);
			a.jne(replace);

			a.popad64();
			a.jmp(end);

			a.bind(end);

			a.movzx(r15d, byte_ptr(r14));
			a.inc(r14);
			a.mov(dword_ptr(rbp, 0xA4), r15d);

			a.jmp(0xC0D0B2_b);

			a.bind(replace);

			a.popad64();
			a.mov(rax, qword_ptr(reinterpret_cast<int64_t>(&target_function)));
			a.mov(r14, rax);
			a.jmp(end);
		}

		void g_log_printf_stub(const char* fmt, ...)
		{
			if (!logfile->current.enabled)
			{
				return;
			}

			char va_buffer[0x400] = {0};

			va_list ap;
			va_start(ap, fmt);
			vsprintf_s(va_buffer, fmt, ap);
			va_end(ap);

			const auto file = g_log->current.string;
			const auto time = *game::level_time / 1000;

			utils::io::write_file(file, utils::string::va("%3i:%i%i %s",
				time / 60,
				time % 60 / 10,
				time % 60 % 10,
				va_buffer
			), true);
		}
	}

	void add_player_damage_callback(const sol::protected_function& callback)
	{
		player_damage_callbacks.push_back(callback);
	}

	void add_player_killed_callback(const sol::protected_function& callback)
	{
		player_killed_callbacks.push_back(callback);
	}

	void clear_callbacks()
	{
		player_damage_callbacks.clear();
		player_killed_callbacks.clear();
		vm_execute_hooks.clear();
	}

	void enable_vm_execute_hook()
	{
		hook_enabled = true;
	}

	void disable_vm_execute_hook()
	{
		hook_enabled = false;
	}

	bool client_command_stub(const int client_num)
	{
		auto self = &game::g_entities[client_num];
		char cmd[1024] = {0};

		game::SV_Cmd_ArgvBuffer(0, cmd, 1024);

		auto hidden = false;
		if (cmd == "say"s || cmd == "say_team"s)
		{
			std::string message(game::ConcatArgs(1));
			message.erase(0, 1);

			for (const auto& callback : say_callbacks)
			{
				const auto entity_id = game::Scr_GetEntityId(client_num, 0);
				const auto result = callback(entity_id, {message, cmd == "say_team"s});

				if (result.is<int>() && !hidden)
				{
					hidden = result.as<int>() == 0;
				}
			}

			scheduler::once([cmd, message, self, hidden]()
			{
				const scripting::entity level{*game::levelEntityId};
				const scripting::entity player{game::Scr_GetEntityId(self->s.number, 0)};

				notify(level, cmd, {player, message, hidden});
				notify(player, cmd, {message, hidden});

				game::G_LogPrintf("%s;%s;%i;%s;%s\n",
					cmd,
					player.call("getguid").as<const char*>(),
					player.call("getentitynumber").as<int>(),
					player.get("name").as<const char*>(),
					message.data());
			}, scheduler::pipeline::server);

			if (hidden)
			{
				return false;
			}
		}

		return true;
	}

	void set_lua_hook(const char* pos, const sol::protected_function& callback)
	{
		gsc_hook_t hook;
		hook.is_lua_hook = true;
		hook.lua_function = callback;
		vm_execute_hooks[pos] = hook;
	}

	void set_gsc_hook(const char* source, const char* target)
	{
		gsc_hook_t hook;
		hook.is_lua_hook = false;
		hook.target_pos = target;
		vm_execute_hooks[source] = hook;
	}

	void clear_hook(const char* pos)
	{
		vm_execute_hooks.erase(pos);
	}

	size_t get_hook_count()
	{
		return vm_execute_hooks.size();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0xC0D0A4_b, utils::hook::assemble(vm_execute_stub), true);

			scr_player_damage_hook.create(0xB5E9E0_b, scr_player_damage_stub);
			scr_player_killed_hook.create(0xB5EB40_b, scr_player_killed_stub);

			// Reimplement game log
			scheduler::once([]()
			{
				logfile = game::Dvar_RegisterBool("logfile", true, game::DVAR_FLAG_NONE, "Enable game logging");
				g_log = game::Dvar_RegisterString("g_log", "h1-mod\\logs\\games_mp.log", game::DVAR_FLAG_NONE, "Log file path");
			}, scheduler::pipeline::main);

			g_log_printf_hook.create(game::G_LogPrintf, g_log_printf_stub);

			gsc::function::add("onplayersay", [](const gsc::function_args& args)
			{
				const auto function = args[0].as<scripting::function>();
				say_callbacks.push_back(function);
				return scripting::script_value{};
			});

			scripting::on_shutdown([](bool, bool post_shutdown)
			{
				if (!post_shutdown)
				{
					say_callbacks.clear();
				}
			});
		}
	};
}

REGISTER_COMPONENT(logfile::component)