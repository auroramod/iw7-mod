#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "auth.hpp"

#include "game/game.hpp"
#include "steam/steam.hpp"

#include "command.hpp"
#include "console/console.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>
#include <utils/smbios.hpp>
#include <utils/info_string.hpp>
#include <utils/cryptography.hpp>
#include <utils/properties.hpp>
#include <utils/io.hpp>

namespace auth
{
	namespace
	{
		std::string get_hdd_serial()
		{
			DWORD serial{};
			if (!GetVolumeInformationA("C:\\", nullptr, 0, &serial, nullptr, nullptr, nullptr, 0))
			{
				return {};
			}

			return utils::string::va("%08X", serial);
		}

		std::string get_hw_profile_guid()
		{
			auto hw_profile_path = (utils::properties::get_appdata_path() / "iw7-guid.dat").generic_string();
			if (utils::io::file_exists(hw_profile_path))
			{
				utils::io::remove_file(hw_profile_path);
			}

			HW_PROFILE_INFO info;
			if (!GetCurrentHwProfileA(&info))
			{
				return {};
			}

			auto hw_profile_info = std::string{ info.szHwProfileGuid, sizeof(info.szHwProfileGuid) };
			return hw_profile_info;
		}

		std::string get_protected_data()
		{
			std::string input = "X-Labs-IW7MOD-Auth";

			DATA_BLOB data_in{}, data_out{};
			data_in.pbData = reinterpret_cast<uint8_t*>(input.data());
			data_in.cbData = static_cast<DWORD>(input.size());
			if (CryptProtectData(&data_in, nullptr, nullptr, nullptr, nullptr, CRYPTPROTECT_LOCAL_MACHINE, &data_out) != TRUE)
			{
				return {};
			}

			const auto size = std::min<DWORD>(data_out.cbData, 52);
			std::string result{ reinterpret_cast<char*>(data_out.pbData), size };
			LocalFree(data_out.pbData);

			return result;
		}

		std::string get_key_entropy()
		{
			std::string entropy{};
			entropy.append(utils::smbios::get_uuid());
			entropy.append(get_hw_profile_guid());
			entropy.append(get_protected_data());
			entropy.append(get_hdd_serial());

			if (entropy.empty())
			{
				entropy.resize(32);
				utils::cryptography::random::get_data(entropy.data(), entropy.size());
			}

			return entropy;
		}

		bool load_key(utils::cryptography::ecc::key& key)
		{
			std::string data{};

			auto key_path = (utils::properties::get_appdata_path() / "iw7-private.key").generic_string();
			if (!utils::io::read_file(key_path, &data))
			{
				return false;
			}

			key.deserialize(data);
			if (!key.is_valid())
			{
				console::error("Loaded key is invalid!\n");
				return false;
			}

			return true;
		}

		utils::cryptography::ecc::key generate_key()
		{
			auto key = utils::cryptography::ecc::generate_key(512, get_key_entropy());
			if (!key.is_valid())
			{
				throw std::runtime_error("Failed to generate cryptographic key!");
			}

			auto key_path = (utils::properties::get_appdata_path() / "iw7-private.key").generic_string();
			if (!utils::io::write_file(key_path, key.serialize()))
			{
				console::error("Failed to write cryptographic key!\n");
			}

			console::info("Generated cryptographic key: %llX\n", key.get_hash());
			return key;
		}

		utils::cryptography::ecc::key load_or_generate_key()
		{
			utils::cryptography::ecc::key key{};
			if (load_key(key))
			{
				console::info("Loaded cryptographic key: %llX\n", key.get_hash());
				return key;
			}

			return generate_key();
		}

		utils::cryptography::ecc::key get_key_internal()
		{
			auto key = load_or_generate_key();

			auto key_path = (utils::properties::get_appdata_path() / "iw7-public.key").generic_string();
			if (!utils::io::write_file(key_path, key.get_public_key()))
			{
				console::error("Failed to write public key!\n");
			}

			return key;
		}

		utils::cryptography::ecc::key& get_key()
		{
			static auto key = get_key_internal();
			return key;
		}
	}

	uint64_t get_guid()
	{
		if (game::environment::is_dedi())
		{
			return 0x110000100000000 | (::utils::cryptography::random::get_integer() & ~0x80000000);
		}

		return get_key().get_hash();
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			// Patch steam id bit check
			std::vector<std::pair<size_t, size_t>> patches{};
			const auto p = [&patches](const size_t a, const size_t b)
			{
				patches.emplace_back(a, b);
			};

			p(0x1405CC014, 0x1405CC057);
			p(0x1405CC0D4, 0x1405CC12B);
			p(0x1405CC1D4, 0x1405CC227);
			p(0x1405CC2C4, 0x1405CC317);
			p(0x1405CC3B4, 0x1405CC407);
			p(0x1405CE0C0, 0x1405CE131); // party
			p(0x140D2F495, 0x140D2F4F7);
			p(0x140D304FD, 0x140D3053D);
			p(0x140DA8650, 0x140DA86AF);
			p(0x140DA891E, 0x140DA898A); // steamlobbycreatefail
			p(0x140DA8B1A, 0x140DA8B86); // steamlobbyjoinfail
			p(0x140DBCF55, 0x140DBCF95);

			p(0x1405CE425, 0x1405CE466); // party
			p(0x140DA8516, 0x140DA853C);
			p(0x140DA8C89, 0x140DA8CCF);

			p(0x1405CCD2B, 0x1405CCDD7);
			p(0x1405CCF25, 0x1405CCF71);
			p(0x140D30126, 0x140D30172);
			p(0x140DA8D76, 0x140DA8DE2);

			for (const auto& patch : patches)
			{
				utils::hook::jump(patch.first, patch.second);
			}

			command::add("guid", []() -> void
			{
				console::info("Your guid: %llX\n", steam::SteamUser()->GetSteamID().bits);
			});
		}
	};
}

REGISTER_COMPONENT(auth::component)
