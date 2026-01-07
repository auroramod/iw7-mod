#include <std_include.hpp>

#ifdef DEBUG
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include "command.hpp"
#include "console/console.hpp"
#include "gsc/script_loading.hpp"

#include <utils/io.hpp>
#include "utils/zlib.hpp"
#include <utils/string.hpp>

namespace dump
{
	int safeStoi(const std::string& str, const int defaultValue = 0) 
	{
		try 
		{
			// Check if string is empty first
			if (str.empty()) 
			{
				return defaultValue;
			}

			// Attempt conversion, handling potential exceptions
			size_t pos = 0;
			long long result = std::stoll(str, &pos);

			// Check if entire string was converted
			if (pos != str.length())
			{
				return defaultValue;
			}

			// Check if result is within int range
			if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min()) 
			{
				return defaultValue;
			}

			return static_cast<int>(result);
		}
		catch (const std::invalid_argument& e)
		{
			// Conversion failed due to non-numeric string
			return defaultValue;
		}
		catch (const std::out_of_range& e) 
		{
			// Number out of integer range
			return defaultValue;
		}
	}

	void DumpScriptFile(const char* filename, game::ScriptFile* scriptfile)
	{
		std::string buffer;
		buffer.append(scriptfile->name, std::strlen(scriptfile->name) + 1);
		buffer.append(reinterpret_cast<char*>(&scriptfile->compressedLen), 4);
		buffer.append(reinterpret_cast<char*>(&scriptfile->len), 4);
		buffer.append(reinterpret_cast<char*>(&scriptfile->bytecodeLen), 4);
		buffer.append(scriptfile->buffer, scriptfile->compressedLen);
		buffer.append(scriptfile->bytecode, scriptfile->bytecodeLen);

		// gscbin buffer -> buffer
		auto script = std::vector<std::uint8_t>{};
		auto stack = std::vector<std::uint8_t>{};

		xsk::gsc::asset asset;
		const std::vector<uint8_t> buffer_u8(buffer.begin(), buffer.end());
		asset.deserialize(buffer_u8);

		if (!asset.len || asset.len == 0)
		{
			utils::io::write_file(filename, "", false);
			return;
		}

		script = std::move(asset.bytecode);
		stack = utils::zlib::decompress(asset.buffer, asset.len);

		try
		{
			const auto outasm = gsc::gsc_ctx->disassembler().disassemble(script, stack);
			const auto outast = gsc::gsc_ctx->decompiler().decompile(*outasm);
			auto outsrc = gsc::gsc_ctx->source().dump(*outast);

			const std::string rawBuffer(outsrc.begin(), outsrc.end());
			utils::io::write_file(filename, rawBuffer, false);
			console::info("Dumped %s (%d bytes)\n", filename, rawBuffer.length());
		}
		catch (...)
		{
			console::info("Skipped %s", filename);
		}
	}

	void DumpLuaFile(const char* filename, game::LuaFile* luaFile)
	{
		std::string buffer;
		if (luaFile->len > 0)
		{
			buffer.append(luaFile->buffer, luaFile->len);
		}

		utils::io::write_file(filename, buffer);
		console::info("Dumped %s (%d bytes)\n", filename, buffer.length());
	}

	void DumpStringTableFile(std::string filename, game::StringTable* stringTable)
	{
		std::string buffer;
		console::info("Dumping string table %s with %d rows and %d columns\n", stringTable->name, stringTable->rowCount, stringTable->columnCount);

		for (auto row = 0; row < stringTable->rowCount; row++)
		{
			for (auto column = 0; column < stringTable->columnCount; column++)
			{
				const auto index = (row * stringTable->columnCount) + column;
				const auto string_value = stringTable->values[index].string;
				const auto last_char = (column == stringTable->columnCount - 1) ? "\n" : ",";

				if (string_value == nullptr)
				{
					buffer.append(last_char);
				}
				else
				{
					std::string str = string_value;
					auto added_quotes = false;
					if (str.contains(','))
					{
						added_quotes = true;
						str.insert(str.begin(), '"');
						str.insert(str.end(), '"');
					}

					if (str.contains('\"') && !added_quotes)
					{
						str = std::regex_replace(str, std::regex("\""), "\\\"");

						str.insert(str.begin(), '"');
						str.insert(str.end(), '"');
					}

					str = std::regex_replace(str, std::regex("\n"), "\\n");
					buffer.append(utils::string::va("%s%s", str.data(), last_char));
				}
			}
		}

		auto finalFilename = utils::string::va("iw7-mod/dump/stringtables/%s", stringTable->name);
		utils::io::write_file(finalFilename, buffer);
		console::info("Dumped %s (%d bytes)\n", finalFilename, buffer.length());
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			command::add("gscDump", []()
			{
				game::DB_EnumXAssets(game::XAssetType::ASSET_TYPE_SCRIPTFILE, [](const game::XAssetHeader header)
				{
					if (const auto asset = header.scriptfile; asset != nullptr)
					{
						std::string filename = utils::string::to_lower(asset->name);
						if (const auto filename_token = safeStoi(filename); filename_token != 0)
						{
							if (const auto mapped_filename_token = gsc::gsc_ctx->token_name(filename_token); !mapped_filename_token.empty() && !utils::string::starts_with(mapped_filename_token, "_id_"))
							{
								filename = utils::string::to_lower(mapped_filename_token);
							}
						}

						DumpScriptFile(utils::string::va("iw7-mod/dump/gsc/%s.gsc", filename.c_str()), asset);
					}
				});
			});

			command::add("luaDump", []()
			{
				game::DB_EnumXAssets(game::XAssetType::ASSET_TYPE_LUA_FILE, [&](const game::XAssetHeader header)
				{
					if (const auto asset = header.luaFile; asset != nullptr)
					{
						const std::string filename = utils::string::to_lower(asset->name);
						DumpLuaFile(utils::string::va("iw7-mod/dump/lua/%s", filename.c_str()), asset);
					}
				});
			});

			command::add("stringTableDump", []()
			{
				game::DB_EnumXAssets(game::XAssetType::ASSET_TYPE_STRINGTABLE, [&](game::XAssetHeader header)
				{
					if (const auto asset = header.stringTable; asset != nullptr)
					{
						std::string filename = utils::string::to_lower(asset->name);
						DumpStringTableFile(filename, asset);
					}
				});
			});

			command::add("commandDump", []()
			{
				game::cmd_function_s* cmd = (*game::cmd_functions);
				int i = 0;
				while (cmd)
				{
					if (cmd->name)
					{
						const auto line = std::format("{}\r\n", cmd->name);
						utils::io::write_file("iw7-mod/dump/commandlist.txt", line, i != 0);
						i++;
					}
					cmd = cmd->next;
				}
				console::info("dumped %d commands", i);
			});
		}
	};
}

REGISTER_COMPONENT(dump::component)
#endif