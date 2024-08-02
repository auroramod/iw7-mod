#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "system_check.hpp"

#include "game/game.hpp"

#include <utils/cryptography.hpp>
#include <utils/io.hpp>
#include <utils/nt.hpp>

namespace system_check
{
	namespace
	{
		std::string read_zone(const std::string& name)
		{
			std::string data{};
			if (utils::io::read_file(name, &data))
			{
				return data;
			}

			if (utils::io::read_file("zone/" + name, &data))
			{
				return data;
			}

			return {};
		}

		std::string hash_zone(const std::string& name)
		{
			const auto data = read_zone(name);
			return utils::cryptography::sha256::compute(data, true);
		}

		bool verify_hashes(const std::unordered_map<std::string, std::string>& zone_hashes)
		{
			for (const auto& zone_hash : zone_hashes)
			{
				const auto hash = hash_zone(zone_hash.first);
				if (hash != zone_hash.second)
				{
					return false;
				}
			}

			return true;
		}

		bool is_system_valid()
		{
			static std::unordered_map<std::string, std::string> mp_zone_hashes =
			{
				{"patch_common_mp.ff", "E263215ADF7B93A28769E6A9909D7710F5351B4F7586FF69EE0BFA9D62A247C9"},
			};

			static std::unordered_map<std::string, std::string> sp_zone_hashes =
			{
				{"patch_common.ff", "A427FDFBFCF16B0BA662459383C48AE15C884B9B44C61C12B98E062FBC4C6DB3"},
			};


			return verify_hashes(mp_zone_hashes) && (game::environment::is_dedi() || verify_hashes(sp_zone_hashes));
		}

		void verify_binary_version()
		{
			const auto value = *reinterpret_cast<DWORD*>(0x140001337);
			if (!utils::nt::is_wine() && value != 0xB43C9275)
			{
				throw std::runtime_error("Unsupported Call of Duty: Infinite Warfare version");
			}
		}
	}

	bool is_valid()
	{
		static auto valid = is_system_valid();
		return valid;
	}

	class component final : public component_interface
	{
	public:
		void post_load() override
		{
			verify_binary_version();

			if (!is_valid())
			{
				MessageBoxA(nullptr, "Your game files are outdated or unsupported.\n"
					"Please get the latest officially supported Call of Duty: Infinite Warfare files, or you will get random crashes and issues.",
					"Invalid game files!", MB_ICONINFORMATION);
			}
		}
	};
}

REGISTER_COMPONENT(system_check::component)
