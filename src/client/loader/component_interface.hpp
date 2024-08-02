#pragma once

enum class component_priority
{
	min = 0,
	dvars,
	steam_proxy,
	arxan,
	updater,
};

class component_interface
{
public:
	virtual ~component_interface() = default;

	virtual void post_start()
	{
	}

	virtual void post_load()
	{
	}

	virtual void pre_destroy()
	{
	}

	virtual void post_unpack()
	{
	}

	virtual void* load_import([[maybe_unused]] const std::string& library, [[maybe_unused]] const std::string& function)
	{
		return nullptr;
	}

	virtual component_priority priority()
	{
		return component_priority::min;
	}
};
