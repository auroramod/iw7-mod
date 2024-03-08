#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "filesystem.hpp"

#include "component/dvars.hpp"
#include "component/console/console.hpp"

#include "game/game.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/io.hpp>
#include <utils/properties.hpp>

namespace filesystem
{
	namespace
	{
		utils::hook::detour fs_startup_hook;
		utils::hook::detour fs_build_os_path_hook;

		bool initialized = false;

		std::deque<std::filesystem::path>& get_search_paths_internal()
		{
			static std::deque<std::filesystem::path> search_paths{};
			return search_paths;
		}

		void fs_display_path()
		{
			console::info("Current language: %s\n", game::SEH_GetLanguageName(*reinterpret_cast<int*>(0x74C6420_b)));
			console::info("Current search paths:\n");

			if (game::fs_searchpaths.get())
			{
				for (auto i = game::fs_searchpaths.get()->next; i; i = i->next)
				{
					console::info("%s/%s\n", i->dir->path, i->dir->gamedir);
				}
			}

			for (auto path : filesystem::get_search_paths())
			{
				console::info("%s\n", path.data());
			}
		}

		void fs_startup_stub(const char* name)
		{
			console::info("----- FS_Startup -----\n");

			initialized = true;

			filesystem::register_path(utils::properties::get_appdata_path() / "cdata"); // CLIENT_DATA_FOLDER
			filesystem::register_path(L".");
			filesystem::register_path(L"iw7-mod");
			filesystem::register_path(L"devraw_shared");
			filesystem::register_path(L"devraw");
			filesystem::register_path(L"raw_shared");
			filesystem::register_path(L"raw");
			filesystem::register_path(L"main_shared");
			filesystem::register_path(L"main");

			fs_startup_hook.invoke<void>(name);

			fs_display_path();
			console::info("----------------------\n");
		}

		std::vector<std::filesystem::path> get_paths(const std::filesystem::path& path)
		{
			std::vector<std::filesystem::path> paths{};
			paths.push_back(path);
			return paths;
		}

		bool can_insert_path(const std::filesystem::path& path)
		{
			for (const auto& path_ : get_search_paths_internal())
			{
				if (path_ == path)
				{
					return false;
				}
			}

			return true;
		}

		const char* sys_default_install_path_stub()
		{
			static auto current_path = std::filesystem::current_path().string();
			return current_path.data();
		}

		void fs_build_os_path_stub(const char* base, const char* game, const char* qpath, char* ospath)
		{
			if (!_stricmp(game, "players2"))
			{
				const auto path = "iw7-mod/"s + game;
				fs_build_os_path_hook.invoke<void>(base, path.data(), qpath, ospath);
				return;
			}

			fs_build_os_path_hook.invoke<void>(base, game, qpath, ospath);
		}
	}

	std::string read_file(const std::string& path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				return utils::io::read_file(path_.generic_string());
			}
		}

		return {};
	}

	bool read_file(const std::string& path, std::string* data, std::string* real_path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::read_file(path_.generic_string(), data))
			{
				if (real_path != nullptr)
				{
					*real_path = path_.generic_string();
				}

				return true;
			}
		}

		return false;
	}

	bool find_file(const std::string& path, std::string* real_path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				*real_path = path_.generic_string();
				return true;
			}
		}

		return false;
	}

	bool exists(const std::string& path)
	{
		for (const auto& search_path : get_search_paths_internal())
		{
			const auto path_ = search_path / path;
			if (utils::io::file_exists(path_.generic_string()))
			{
				return true;
			}
		}

		return false;
	}

	void register_path(const std::filesystem::path& path)
	{
		if (!initialized)
		{
			return;
		}

		const auto paths = get_paths(path);
		for (const auto& path_ : paths)
		{
			if (can_insert_path(path_))
			{
				console::debug("[FS] Registering path '%s'\n", path_.generic_string().data());
				get_search_paths_internal().push_front(path_);
			}
		}
	}

	void unregister_path(const std::filesystem::path& path)
	{
		if (!initialized)
		{
			return;
		}

		const auto paths = get_paths(path);
		for (const auto& path_ : paths)
		{
			auto& search_paths = get_search_paths_internal();
			for (auto i = search_paths.begin(); i != search_paths.end();)
			{
				if (*i == path_)
				{
					console::debug("[FS] Unregistering path '%s'\n", path_.generic_string().data());
					i = search_paths.erase(i);
				}
				else
				{
					++i;
				}
			}
		}
	}

	std::vector<std::string> get_search_paths()
	{
		std::vector<std::string> paths{};

		for (const auto& path : get_search_paths_internal())
		{
			paths.push_back(path.generic_string());
		}

		return paths;
	}

	std::vector<std::string> get_search_paths_rev()
	{
		std::vector<std::string> paths{};
		const auto& search_paths = get_search_paths_internal();

		for (auto i = search_paths.rbegin(); i != search_paths.rend(); ++i)
		{
			paths.push_back(i->generic_string());
		}

		return paths;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			fs_startup_hook.create(0xCDD800_b, fs_startup_stub);
			fs_build_os_path_hook.create(0xCDBBF0_b, fs_build_os_path_stub);

			utils::hook::jump(0xCFE5E0_b, sys_default_install_path_stub);

			// fs_game flags
			utils::hook::set<uint32_t>(0xCDD415_b, 0);
		}
	};
}

REGISTER_COMPONENT(filesystem::component)
