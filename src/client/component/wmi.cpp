#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "game/game.hpp"

#include "console/console.hpp"

#include <utils/hook.hpp>

// Speeds up startup by disabling WMI

namespace wmi
{
	namespace
	{
		HRESULT WINAPI co_initialize_ex_stub(LPVOID pvReserved, DWORD dwCoInit)
		{
			if ((uint64_t)_ReturnAddress() == 0x1412B36F2)
			{
				return E_FAIL;
			}
			
			return CoInitializeEx(pvReserved, dwCoInit);
		}
	}

	class component final : public component_interface
	{
	public:
		void* load_import(const std::string& library, const std::string& function) override
		{
			if (function == "CoInitializeEx")
			{
				return co_initialize_ex_stub;
			}

			return nullptr;
		}

		void post_unpack() override
		{
			// disable WMI and remove Hardware query(uses WMI)
			utils::hook::set<uint8_t>(0x140110260, 0xC3); // WMI
			utils::hook::set<uint8_t>(0x14002D808, 0xC3); // Hardware query
		}
	};
}

REGISTER_COMPONENT(wmi::component)