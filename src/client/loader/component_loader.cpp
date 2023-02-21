#include <std_include.hpp>
#include "component_loader.hpp"

void component_loader::register_component(std::unique_ptr<component_interface>&& component_, [[maybe_unused]] const std::string& name)
{
#ifdef DEV_BUILD
	printf("registering component: %s\n", name.data());
#endif

	get_components().push_back(std::move(component_));
}

bool component_loader::post_start()
{
	static auto handled = false;
	if (handled) return true;
	handled = true;

	try
	{
		for (const auto& component_ : get_components())
		{
			component_->post_start();
		}
	}
	catch (premature_shutdown_trigger&)
	{
		return false;
	}

	return true;
}

bool component_loader::post_load()
{
	static auto handled = false;
	if (handled) return true;
	handled = true;

	try
	{
		for (const auto& component_ : get_components())
		{
			component_->post_load();
		}
	}
	catch (premature_shutdown_trigger&)
	{
		return false;
	}

	return true;
}

void component_loader::post_unpack()
{
	static auto handled = false;
	if (handled) return;
	handled = true;

	for (const auto& component_ : get_components())
	{
		try
		{
			component_->post_unpack();
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "ERROR", MB_ICONERROR);
			quick_exit(EXIT_FAILURE);
		}
	}
}

void component_loader::pre_destroy()
{
	static auto handled = false;
	if (handled) return;
	handled = true;

	for (const auto& component_ : get_components())
	{
		try
		{
			component_->pre_destroy();
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "ERROR", MB_ICONERROR);
			quick_exit(EXIT_FAILURE);
		}
	}
}

void component_loader::sort()
{
	auto& components = get_components();

	std::ranges::stable_sort(components, [](const std::unique_ptr<component_interface>& a,
		const std::unique_ptr<component_interface>& b)
	{
		return a->priority() > b->priority();
	});
}

void* component_loader::load_import(const std::string& library, const std::string& function)
{
	void* function_ptr = nullptr;

	for (const auto& component_ : get_components())
	{
		auto* const component_function_ptr = component_->load_import(library, function);
		if (component_function_ptr)
		{
			function_ptr = component_function_ptr;
		}
	}

	return function_ptr;
}

void component_loader::trigger_premature_shutdown()
{
	throw premature_shutdown_trigger();
}

std::vector<std::unique_ptr<component_interface>>& component_loader::get_components()
{
	using component_vector = std::vector<std::unique_ptr<component_interface>>;
	using component_vector_container = std::unique_ptr<component_vector, std::function<void(component_vector*)>>;

	static component_vector_container components(new component_vector, [](component_vector* component_vector)
	{
		pre_destroy();
		delete component_vector;
	});

	return *components;
}
