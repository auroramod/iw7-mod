#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "scheduler.hpp"
#include "console/console.hpp"
#include "updater.hpp"

#include "version.hpp"

#include <utils/http.hpp>
#include <utils/io.hpp>
#include <utils/nt.hpp>
#include <utils/concurrency.hpp>
#include <utils/string.hpp>
#include <utils/properties.hpp>
#include <utils/cryptography.hpp>
#include <utils/flags.hpp>

#define FILES_PATH "files.json"
#define FILES_PATH_DEV "files-dev.json"

#define DATA_PATH "data/"
#define DATA_PATH_DEV "data-dev/"

namespace updater
{
	namespace
	{
		std::vector<std::string> dedi_ignore =
		{
			"ui_scripts/*",
			"zone/iw7mod_ui_mp.ff",
		};

		bool is_dedi_ignore_file(const std::string& name)
		{
			if (!game::environment::is_dedi())
			{
				return false;
			}

			const auto cdata_path = CLIENT_DATA_FOLDER + "/"s;

			for (auto& ignore_file : dedi_ignore)
			{
				if (name == cdata_path + ignore_file)
				{
					return true;
				}

				if (ignore_file.ends_with('*'))
				{
					std::string ignore_prefix = ignore_file.substr(0, ignore_file.size() - 1); // Remove the last character
					if (name.starts_with(cdata_path + ignore_prefix))
					{
						return true;
					}
				}
			}

			return false;
		}
	}

	namespace
	{
		constexpr auto override_cache = true;

		struct file_data
		{
			std::string name;
			std::string data;
		};

		struct file_data_previous
		{
			std::string name;
			std::string data;
			bool exists;
		};

		struct file_info
		{
			std::string name;
			std::string hash;
		};

		std::unordered_map<std::string, git_branch> git_branches =
		{
			{"develop", branch_develop},
			{"main", branch_main},
		};

		std::string get_branch_name(const git_branch branch)
		{
			for (const auto& [name, b] : git_branches)
			{
				if (branch == b)
				{
					return name;
				}
			}

			throw std::runtime_error("invalid branch");
		}

		git_branch load_current_branch()
		{
			const auto branch_str = GIT_BRANCH;
			const auto iter = git_branches.find(branch_str);
			if (iter != git_branches.end())
			{
				return iter->second;
			}

			return branch_default;
		}

		git_branch get_current_branch()
		{
			static const auto branch = load_current_branch();
			return branch;
		}

		std::string select(const std::string& main, const std::string& develop)
		{
			switch (updater::get_current_branch())
			{
			case branch_develop:
				return develop;
			case branch_main:
				return main;
			}

			return main;
		}

		std::string load_binary_name()
		{
			utils::nt::library self;
			return self.get_name();
		}

		std::string get_binary_name()
		{
			static const auto name = load_binary_name();
			return name;
		}

		std::optional<std::string> get_server_file(const std::string& endpoint)
		{
			static std::vector<std::string> server_urls =
			{
				{"https://iw7-mod.auroramod.dev/"},
			};

			const auto try_url = [&](const std::string& base_url)
			{
				const auto url = base_url + endpoint;
				console::debug("[HTTP] GET file \"%s\"\n", url.data());
				const auto result = utils::http::get_data(url);
				return result;
			};

			for (const auto& url : server_urls)
			{
				const auto result = try_url(url);
				if (result.has_value() && result->response_code == 200)
				{
					return result->buffer;
				}
			}

			return {};
		}

		bool check_file(const file_info& info)
		{
			std::string data;

			if (get_binary_name() == info.name)
			{
				if (!utils::io::read_file(info.name, &data))
				{
					return false;
				}
			}
			else
			{
				const auto appdata_folder = utils::properties::get_appdata_path();
				const auto path = (appdata_folder / info.name).generic_string();
				if (!utils::io::read_file(path, &data))
				{
					return false;
				}
			}

			if (utils::cryptography::sha1::compute(data, true) != info.hash)
			{
				return false;
			}

			return true;
		}

		bool is_binary_name(const std::string& name)
		{
			return get_binary_name() == name;
		}

		bool write_binary(const file_data& file_data)
		{
			return (!utils::io::file_exists(file_data.name) ||
				utils::io::move_file(file_data.name, file_data.name + ".old")) &&
				utils::io::write_file(file_data.name, file_data.data);
		}

		bool write_file(const file_data& file_data)
		{
			const auto is_binary = is_binary_name(file_data.name);

			if (is_binary)
			{
				return write_binary(file_data);
			}
			else
			{
				const auto appdata_folder = utils::properties::get_appdata_path();
				const auto path = (appdata_folder / file_data.name).generic_string();
				return utils::io::write_file(path, file_data.data);
			}
		}

		void delete_old_file()
		{
			utils::io::remove_file(get_binary_name() + ".old");
		}

		std::string get_time_str()
		{
			return utils::string::va("%i", override_cache ? std::uint32_t(time(nullptr)) : 0);
		}

		std::string format_url(const std::string& file)
		{
			const auto url = std::format("{}?{}", file, get_time_str());
			return url;
		}

		std::optional<std::string> download_file_list()
		{
			const auto file = format_url(select(FILES_PATH, FILES_PATH_DEV));
			return updater::get_server_file(file);
		}

		std::optional<std::string> download_data_file(const std::string& name)
		{
			const auto file = format_url(std::format("{}{}", select(DATA_PATH, DATA_PATH_DEV), name));
			return updater::get_server_file(file);
		}

		std::vector<file_info> get_file_list()
		{
			console::info("[Updater] Downloading file list\n");

			const auto list = download_file_list();
			if (!list.has_value())
			{
				console::error("[Updater] Failed to download file list\n");
				return {};
			}

			rapidjson::Document j;
			j.Parse(list->data());

			std::vector<file_info> parsed_list;

			const auto files = j.GetArray();
			for (const auto& file : files)
			{
				if (!file.IsArray() || file.Size() != 3 || !file[0].IsString() || !file[2].IsString())
				{
					continue;
				}

				const auto name = file[0].GetString();
				const auto sha = file[2].GetString();

				if (is_dedi_ignore_file(name))
				{
					continue;
				}

#if GIT_DIRTY == 1
				if (name == get_binary_name())
				{
					continue;
				}
#endif

				console::info("[Updater] Add file \"%s\"\n", name);

				parsed_list.emplace_back(name, sha);
			}

			return parsed_list;
		}

		std::thread create_file_thread(const file_info& file, const std::function<void(const std::optional<std::string>& result)>& cb)
		{
			return std::thread([=]
			{
				console::info("[Updater] Downloading file \"%s\"\n", file.name.data());
				const auto data = download_data_file(file.name);

				if (!data.has_value())
				{
					console::error("[Updater] File failed to download \"%s\"\n", file.name.data());
					cb({});
					return;
				}

				const auto hash = utils::cryptography::sha1::compute(data.value(), true);
				if (hash != file.hash)
				{
					console::error("[Updater] File hash mismatch \"%s\"\n", file.name.data());
					cb({});
					return;
				}

				cb(data);
			});
		}

		void delete_garbage_files(const std::vector<file_info>& update_files)
		{
			const auto appdata_folder = utils::properties::get_appdata_path();
			const auto path = (appdata_folder / CLIENT_DATA_FOLDER).generic_string();
			if (!utils::io::directory_exists(path))
			{
				return;
			}

			const auto current_files = utils::io::list_files_recursively(path);
			for (const auto& file : current_files)
			{
				bool found = false;
				for (const auto& update_file : update_files)
				{
					const auto update_file_ = (appdata_folder / update_file.name).generic_string();
					const auto path_a = std::filesystem::path(file);
					const auto path_b = std::filesystem::path(update_file_);
					const auto is_directory = utils::io::directory_exists(file);
					const auto compare = path_a.compare(path_b);

					if ((is_directory && compare == -1) || compare == 0)
					{
						found = true;
						break;
					}
				}

				const auto file_ = std::string(file.begin() + appdata_folder.generic_string().size() + 1, file.end());
				if (!found && std::filesystem::is_regular_file(file) && !is_dedi_ignore_file(file_))
				{
					console::info("[Updater] Deleting extra file %s\n", file.data());
					utils::io::remove_file(file);
				}
			}
		}
		
		void run_update()
		{
			const auto file_list = get_file_list();
			if (file_list.empty())
			{
				console::info("[Updater] Update aborted\n");
				return;
			}

			std::vector<std::thread> download_threads;

			delete_garbage_files(file_list);

			utils::concurrency::container<std::vector<file_data>> result_data;
			std::atomic_bool download_failed = false;

			for (const auto& file : file_list)
			{
				if (check_file(file) || download_failed)
				{
					continue;
				}

				const auto cb = [&result_data, &download_failed, file](const std::optional<std::string>& data)
				{
					if (!data.has_value() || download_failed)
					{
						download_failed = true;
						return;
					}

					result_data.access([=](std::vector<file_data>& list)
					{
						list.emplace_back(file.name, data.value());
					});
				};

				download_threads.emplace_back(create_file_thread(file, cb));
			}

			if (download_threads.size() == 0)
			{
				return;
			}

			for (auto& thread : download_threads)
			{
				if (thread.joinable())
				{
					thread.join();
				}
			}

			if (download_failed)
			{
				console::info("[Updater] Update aborted\n");
				return;
			}

			std::vector<file_data_previous> previous_data;

			auto is_binary_modified = false;

			result_data.access([&](std::vector<file_data>& list)
			{
				for (const auto& file : list)
				{
					if (!write_file(file))
					{
						console::error("[Updater] Failed to write file \"%s\", aborting update\n", file.name.data());
						download_failed = true;
						return;
					}
					
					if (is_binary_name(file.name))
					{
						is_binary_modified = true;
					}
				}
			});

			if (!download_failed && is_binary_modified)
			{
				if (!utils::flags::has_flag("update-only"))
				{
					console::info("[Updater] Restarting\n");
					utils::nt::relaunch_self();
				}

				utils::nt::terminate();
			}
		}
	}

	class component final : public component_interface
	{
	public:
		void post_start() override
		{
			delete_old_file();
			run_update();
		}

		component_priority priority() override
		{
			return component_priority::updater;
		}
	};
}

#if GIT_DIRTY == 0
REGISTER_COMPONENT(updater::component)
#endif
