#pragma once

namespace syscon
{
	void init();

	void set_title(const std::string& title);

	void Sys_Print(const char* msg);
}