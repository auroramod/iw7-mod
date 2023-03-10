#pragma once

namespace terminal
{
	void init();

	int dispatch_message(const int type, const std::string& message);
}