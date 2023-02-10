#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "filesystem.hpp"

#include "game/game.hpp"

#include "dvars.hpp"

#include <utils/hook.hpp>
#include <utils/string.hpp>

namespace game::filesystem
{
	file::file(std::string name)
		: name_(std::move(name))
	{
		char* buffer{};
		const auto size = game::FS_ReadFile(this->name_.data(), &buffer);

		if (size >= 0 && buffer)
		{
			this->valid_ = true;
			this->buffer_.append(buffer, size);
			game::FS_FreeFile(buffer);
		}
	}

	bool file::exists() const
	{
		return this->valid_;
	}

	const std::string& file::get_buffer() const
	{
		return this->buffer_;
	}

	const std::string& file::get_name() const
	{
		return this->name_;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			dvars::override::register_string("fs_basegame", "iw7-mod", 2048);
		}
	};
}

REGISTER_COMPONENT(game::filesystem::component)