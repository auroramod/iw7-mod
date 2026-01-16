#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "fastfiles.hpp"

#include <utils/hook.hpp>

#define CUSTOM_IMAGEFILE_INDEX 431

namespace imagefile
{
	namespace
	{
		void pakfile_open_stub(unsigned int fileIndex, __int16 isLocalized, char* buffer, unsigned int bufferSize)
		{
			if (fileIndex == CUSTOM_IMAGEFILE_INDEX)
			{
				const auto name = fastfiles::get_current_fastfile();
				snprintf(buffer, bufferSize, "%s.pak", name.data());
				return;
			}

			if (isLocalized)
			{
				snprintf(buffer, bufferSize, "%s_imagefile%d.pak", game::SEH_GetCurrentLanguageCode(), fileIndex);
			}
			else
			{
				snprintf(buffer, bufferSize, "imagefile%d.pak", fileIndex);
			}
		}

		utils::hook::detour image_file_decrypt_value_hook;
		bool image_file_decrypt_value_stub(char* value, int size, char* buffer)
		{
			auto is_all_zero = true;
			for (auto i = 0; i < size; i++)
			{
				if (value[i] != 0)
				{
					is_all_zero = false;
				}
			}

			if (is_all_zero)
			{
				memset(buffer, 0, size);
				return true;
			}

			return image_file_decrypt_value_hook.invoke<bool>(value, size, buffer);
		}
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			utils::hook::jump(0x140CFD990, pakfile_open_stub);
			image_file_decrypt_value_hook.create(0x1409E6BE0, image_file_decrypt_value_stub);
			utils::hook::set(0x1409E6390, 0xC301B0); // DBX_AuthLoad_ValidateSignature
		}
	};
}

REGISTER_COMPONENT(imagefile::component)