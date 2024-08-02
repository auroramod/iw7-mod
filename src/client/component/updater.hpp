#pragma once

#define CLIENT_DATA_FOLDER "cdata"

namespace updater
{
	enum git_branch
	{
		branch_main = 0,
		branch_develop = 1,
		branch_default = branch_main,
		branch_count
	};
}
