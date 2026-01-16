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
	static int safeStoi(const std::string& str, const int defaultValue = 0)
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
		catch (const std::invalid_argument&)
		{
			// Conversion failed due to non-numeric string
			return defaultValue;
		}
		catch (const std::out_of_range&) 
		{
			// Number out of integer range
			return defaultValue;
		}
	}

	static std::string DecodeEntityBlock(const std::string& encoded_block)
	{
		std::string result = "{\n";
		std::istringstream block_stream(encoded_block);
		std::string line;

		// Skip the opening brace
		std::getline(block_stream, line);

		// Process each line
		while (std::getline(block_stream, line))
		{
			// Trim whitespace
			line.erase(0, line.find_first_not_of(" \t\n\r"));

			// Skip closing brace - we'll add it later
			if (line.find("}") == 0)
				continue;

			// Find the key ID (number at start of line)
			size_t space_pos = line.find(" ");
			if (space_pos != std::string::npos)
			{
				std::string key_id_str = line.substr(0, space_pos);

				// Try to convert to a number
				try 
				{
					const int key_id = std::stoi(key_id_str);

					if (auto key_string = gsc::gsc_ctx->token_name(key_id); !key_string.empty())
					{
						// Replace the ID with the string
						result += "    \"" + std::string(key_string) + "\" ";

						// Add the rest of the line
						result += line.substr(space_pos + 1) + "\n";
					}
					else
					{
						// If lookup fails, keep the original line
						result += "    " + line + "\n";
					}
				}
				catch (...) {
					// If not a valid number, keep the original line
					result += "    " + line + "\n";
				}
			}
			else
			{
				// Line doesn't have a space, keep it as is
				result += "    " + line + "\n";
			}
		}

		result += "}";
		return result;
	}

	static void DumpScriptFile(const char* filename, game::ScriptFile* scriptfile)
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

	static void DumpLuaFile(const char* filename, game::LuaFile* luaFile)
	{
		std::string buffer;
		if (luaFile->len > 0)
		{
			buffer.append(luaFile->buffer, luaFile->len);
		}

		utils::io::write_file(filename, buffer);
		console::info("Dumped %s (%d bytes)\n", filename, buffer.length());
	}

	static void DumpStringTableFile(std::string filename, game::StringTable* stringTable)
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

	static void DumpMapEnts(const char* filename, game::MapEnts* mapEnts)
	{
		std::string buffer;
		const char* map_ents_ptr = mapEnts->entityString;
		std::string map_ents(map_ents_ptr);

		size_t pos = 0;

		while (pos < map_ents.length())
		{
			size_t block_start = map_ents.find('{', pos);
			if (block_start == std::string::npos) break;

			size_t block_end = map_ents.find('}', block_start);
			if (block_end == std::string::npos) break;

			std::string block_text = map_ents.substr(block_start, block_end - block_start + 1);
			pos = block_end + 1;

			// decode the block to get the proper key names
			std::string decoded_block = DecodeEntityBlock(block_text);
			buffer.append(decoded_block);
			buffer.append("\n");
		}

		utils::io::write_file(filename, buffer);
		console::info("Dumped %s (%d bytes)\n", filename, buffer.length());
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

			command::add("entityDump", []()
			{
				game::DB_EnumXAssets(game::XAssetType::ASSET_TYPE_MAP_ENTS, [&](game::XAssetHeader header)
				{
					if (const auto asset = header.mapEnts; asset != nullptr)
					{
						std::string name = asset->name;
						name = utils::string::replace(name, "maps/mp/", "");
						name = utils::string::replace(name, "maps/cp/", "");
						name = utils::string::replace(name, "maps/", "");
						name = utils::string::replace(name, ".d3dbsp", "");
						DumpMapEnts(utils::string::va("iw7-mod/dump/mapents/%s.txt", name.c_str()), asset);
					}
				});
			});
		}
	};
}

REGISTER_COMPONENT(dump::component)
#endif