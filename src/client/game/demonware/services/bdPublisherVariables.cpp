#include <std_include.hpp>
#include "../dw_include.hpp"

#include <utils/io.hpp>
#include <utils/nt.hpp>

namespace demonware
{
	bdPublisherVariables::bdPublisherVariables() : service(95, "bdPublisherVariables")
	{
		this->register_task(1, &bdPublisherVariables::retrievePublisherVariables);
	}

	void bdPublisherVariables::retrievePublisherVariables(service_server* server, byte_buffer* buffer) const
	{
		std::string context, nameSpace;
		buffer->read_string(&context);
		buffer->read_string(&nameSpace);

		static std::string data;

		if (!utils::io::read_file(std::filesystem::current_path().string() + "/"s + "iw7-mod"s + "publisher_variables.json", &data))
		{
			data = utils::nt::load_resource(DW_PUBVARS);
		}
		
		auto result = std::make_unique<bdPublisherVariablesInfo>();
		result->majorVersion = 1;
		result->minorVersion = 0;
		result->nameSpace = nameSpace;
		result->variables = !data.empty() ? data : R"({})";

		auto reply = server->create_reply(this->task_id());
		reply.add(result);
		reply.send();
	}
}
