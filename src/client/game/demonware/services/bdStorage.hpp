#pragma once

namespace demonware
{
	class bdStorage final : public service
	{
	public:
		bdStorage();

		static std::string get_user_file_path(const std::string& name);

	private:
		using callback = std::function<std::string()>;
		using resource_variant = std::variant<std::string, callback>;
		std::vector<std::pair<std::regex, resource_variant>> publisher_resources_;

		void map_publisher_resource(const std::string& expression, INT id);
		void map_publisher_resource_variant(const std::string& expression, resource_variant resource);
		bool load_publisher_resource(const std::string& name, std::string& buffer);

		void listAllPublisherFiles(service_server* server, byte_buffer* buffer);
		void getPublisherFile(service_server* server, byte_buffer* buffer);
		void uploadAndValidateFiles(service_server* server, byte_buffer* buffer) const;
		void uploadFiles(service_server* server, byte_buffer* buffer) const;
		void getFiles(service_server* server, byte_buffer* buffer) const;
		void getFile(service_server* server, byte_buffer* buffer) const;
	};
}
