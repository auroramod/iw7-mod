#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "command.hpp"
#include "console.hpp"
#include "filesystem.hpp"
#include "localized_strings.hpp"

#include "dvars.hpp"
#include "game/game.hpp"

#include <utils/io.hpp>
#include <utils/flags.hpp>
#include <utils/hook.hpp>

#define CLIENT_DATA_FOLDER "cdata"
#define LANGUAGE_FILE "players2/default/language"

namespace filesystem
{
	namespace
	{

		std::filesystem::path get_appdata_path()
		{
			PWSTR path;
			if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path)))
			{
				throw std::runtime_error("Failed to read APPDATA path!");
			}

			auto _ = gsl::finally([&path]
			{
				CoTaskMemFree(path);
			});

			static auto appdata = std::filesystem::path(path) / "iw7-mod";
			return appdata;
		}

		utils::hook::detour fs_startup_hook;

		bool initialized = false;

		std::deque<std::filesystem::path>& get_search_paths_internal()
		{
			static std::deque<std::filesystem::path> search_paths{};
			return search_paths;
		}

		bool is_fallback_lang()
		{
			static const auto* loc_language = game::Dvar_FindVar("loc_language");
			if (loc_language) 
			{
				const auto id = loc_language->current.integer;
				return id == 5 || id == 6 || id == 8 || id == 9 || id == 10 || id == 11 || id == 13 || id == 15;
			}
			return false;
		}

		void fs_startup_stub(const char* name)
		{
			console::info("[FS] Startup\n");

			initialized = true;

			filesystem::register_path(get_appdata_path() / CLIENT_DATA_FOLDER);
			filesystem::register_path(L".");
			filesystem::register_path(L"devraw");
			filesystem::register_path(L"devraw_shared");
			filesystem::register_path(L"raw_shared");
			filesystem::register_path(L"raw");
			filesystem::register_path(L"iw7-mod");
			filesystem::register_path(L"main");

			console::info("[FS] Finished registering paths\n");
			fs_startup_hook.invoke<void>(name);
		}

		std::vector<std::filesystem::path> get_paths(const std::filesystem::path& path)
		{
			std::vector<std::filesystem::path> paths{};

			const auto code = game::SEH_GetCurrentLanguageName();

			if (!::utils::io::file_exists(LANGUAGE_FILE) or ::utils::io::file_size(LANGUAGE_FILE) == 0)
			{
				::utils::io::write_file(LANGUAGE_FILE, code);
			}

			paths.push_back(path);
			if (is_fallback_lang())
			{
				paths.push_back(path / "fallback");
			}
			paths.push_back(path / code);
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

			utils::hook::jump(0xCFE5E0_b, sys_default_install_path_stub);

			// fs_game flags
			//utils::hook::set<uint32_t>(0xCDD3EC_b, 0);

			dvars::override::register_string("fs_basegame", "iw7-mod", 2048);
		}
	};
}

REGISTER_COMPONENT(filesystem::component)
