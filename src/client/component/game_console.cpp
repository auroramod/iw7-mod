#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game_console.hpp"
#include "command.hpp"
#include "console.hpp"
#include "scheduler.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>
#include <utils/concurrency.hpp>

#include "version.hpp"

#define console_font game::R_RegisterFont("fonts/fira_mono_regular.ttf", 18)
#define material_white game::Material_RegisterHandle("white")

namespace game_console
{
	namespace
	{
		struct console_globals
		{
			float x{};
			float y{};
			float left_x{};
			float font_height{};
			bool may_auto_complete{};
			char auto_complete_choice[64]{};
			int info_line_count{};
		};

		using output_queue = std::deque<std::string>;

		struct ingame_console
		{
			char buffer[256]{};
			int cursor{};
			int font_height{};
			int visible_line_count{};
			int visible_pixel_width{};
			float screen_min[2]{}; //left & top
			float screen_max[2]{}; //right & bottom
			console_globals globals{};
			bool output_visible{};
			int display_line_offset{};
			int line_count{};
			utils::concurrency::container<output_queue, std::recursive_mutex> output{};
		};

		ingame_console con{};

		std::int32_t history_index = -1;
		std::deque<std::string> history{};

		std::string fixed_input{};
		std::vector<std::string> matches{};

		float color_white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float color_title[4] = { 0.9f, 0.9f, 0.0f, 1.0f };

		void clear()
		{
			strncpy_s(con.buffer, "", sizeof(con.buffer));
			con.cursor = 0;

			fixed_input = "";
			matches.clear();
		}

		void print_internal(const std::string& data)
		{
			con.output.access([&](output_queue& output)
			{
				if (con.visible_line_count > 0
					&& con.display_line_offset == (output.size() - con.visible_line_count))
				{
					con.display_line_offset++;
				}
				output.push_back(data);
				if (output.size() > 512)
				{
					output.pop_front();
				}
			});
		}

		void toggle_console()
		{
			clear();

			con.output_visible = false;
			*game::keyCatchers ^= 1;
		}

		void toggle_console_output()
		{
			con.output_visible = con.output_visible == 0;
		}

		void check_resize()
		{
			con.screen_min[0] = 6.0f;
			con.screen_min[1] = 6.0f;
			con.screen_max[0] = game::ScrPlace_GetViewPlacement()->realViewportSize[0] - 6.0f;
			con.screen_max[1] = game::ScrPlace_GetViewPlacement()->realViewportSize[1] - 6.0f;

			if (console_font)
			{
				con.font_height = console_font->pixelHeight;
				con.visible_line_count = static_cast<int>((con.screen_max[1] - con.screen_min[1] - (con.font_height * 2)
					) -
					24.0f) / con.font_height;
				con.visible_pixel_width = static_cast<int>(((con.screen_max[0] - con.screen_min[0]) - 10.0f) - 18.0f);
			}
			else
			{
				con.font_height = 0;
				con.visible_line_count = 0;
				con.visible_pixel_width = 0;
			}
		}

		void draw_box(const float x, const float y, const float w, const float h, float* color)
		{
			game::vec4_t dark_color;

			dark_color[0] = color[0] * 0.5f;
			dark_color[1] = color[1] * 0.5f;
			dark_color[2] = color[2] * 0.5f;
			dark_color[3] = color[3];

			game::R_AddCmdDrawStretchPic(x, y, w, h, 0.0f, 0.0f, 0.0f, 0.0f, color, material_white, 0);
			game::R_AddCmdDrawStretchPic(x, y, 2.0f, h, 0.0f, 0.0f, 0.0f, 0.0f, dark_color, material_white, 0);
			game::R_AddCmdDrawStretchPic((x + w) - 2.0f, y, 2.0f, h, 0.0f, 0.0f, 0.0f, 0.0f, dark_color,
				material_white, 0);
			game::R_AddCmdDrawStretchPic(x, y, w, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, dark_color, material_white, 0);
			game::R_AddCmdDrawStretchPic(x, (y + h) - 2.0f, w, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, dark_color,
				material_white, 0);
		}

		void draw_input_box(const int lines, float* color)
		{
			draw_box(
				con.globals.x - 6.0f,
				con.globals.y - 6.0f,
				(con.screen_max[0] - con.screen_min[0]) - ((con.globals.x - 6.0f) - con.screen_min[0]),
				(lines * con.globals.font_height) + 12.0f,
				color);
		}

		void draw_input_text_and_over(const char* str, float* color)
		{
			game::R_AddCmdDrawText(str, 0x7FFFFFFF, console_font, con.globals.x,
				con.globals.y + con.globals.font_height, 1.0f,
				1.0f, 0.0f, color, 0);
			con.globals.x = game::R_TextWidth(str, 0, console_font) + con.globals.x + 6.0f;
		}

		float draw_hint_box(const int lines, float* color, [[maybe_unused]] float offset_x = 0.0f,
			[[maybe_unused]] float offset_y = 0.0f)
		{
			const auto _h = lines * con.globals.font_height + 12.0f;
			const auto _y = con.globals.y - 3.0f + con.globals.font_height + 12.0f + offset_y;
			const auto _w = (con.screen_max[0] - con.screen_min[0]) - ((con.globals.x - 6.0f) - con.screen_min[0]);

			draw_box(con.globals.x - 6.0f, _y, _w, _h, color);
			return _h;
		}

		void draw_hint_text(const int line, const char* text, float* color, const float offset_x = 0.0f, const float offset_y = 0.0f)
		{
			const auto _y = con.globals.font_height + con.globals.y + (con.globals.font_height * (line + 1)) + 15.0f + offset_y;

			game::R_AddCmdDrawText(text, 0x7FFFFFFF, console_font, con.globals.x + offset_x, _y, 1.0f, 1.0f, 0.0f, color, 0);
		}

		void find_matches(std::string input, std::vector<std::string>& suggestions, const bool exact)
		{
			input = utils::string::to_lower(input);

			for (int i = 0; i < *game::dvarCount; i++)
			{
				if (game::dvarPool[i])
				{
					std::string name = dvars::dvar_get_name(game::dvarPool[i]);

					if (!name.empty())
					{
						std::string lower_name = utils::string::to_lower(name);

						if (utils::string::match_compare(input, lower_name, exact))
						{
							suggestions.push_back(name);
						}

						if (exact && suggestions.size() > 1)
						{
							return;
						}
					}
				}
			}

			if (suggestions.size() == 0 && game::Dvar_FindVar(input.data()))
			{
				suggestions.push_back(input);
			}

			game::cmd_function_s* cmd = (*game::cmd_functions);
			while (cmd)
			{
				if (cmd->name)
				{
					std::string name = utils::string::to_lower(cmd->name);

					if (utils::string::match_compare(input, name, exact))
					{
						suggestions.push_back(cmd->name);
					}

					if (exact && suggestions.size() > 1)
					{
						return;
					}
				}

				cmd = cmd->next;
			}
		}

		void draw_input()
		{
			con.globals.font_height = static_cast<float>(console_font->pixelHeight);
			con.globals.x = con.screen_min[0] + 6.0f;
			con.globals.y = con.screen_min[1] + 6.0f;
			con.globals.left_x = con.screen_min[0] + 6.0f;

			draw_input_box(1, dvars::con_inputBoxColor->current.vector);
			draw_input_text_and_over("IW7-Mod: " VERSION ">", color_title);

			con.globals.left_x = con.globals.x;
			con.globals.auto_complete_choice[0] = 0;

			game::R_AddCmdDrawTextWithCursor(con.buffer, 0x7FFFFFFF, console_font, 18, con.globals.x,
				con.globals.y + con.globals.font_height, 1.0f, 1.0f, 0, color_white, 0,
				con.cursor, '|');

			// check if using a prefixed '/' or not
			const auto input = con.buffer[1] && (con.buffer[0] == '/' || con.buffer[0] == '\\')
				? std::string(con.buffer).substr(1)
				: std::string(con.buffer);

			if (!input.length())
			{
				return;
			}

			if (input != fixed_input)
			{
				matches.clear();

				if (input.find(" ") != std::string::npos)
				{
					find_matches(input.substr(0, input.find(" ")), matches, true);
				}
				else
				{
					find_matches(input, matches, false);
				}

				fixed_input = input;
			}

			con.globals.may_auto_complete = false;
			if (matches.size() > 24)
			{
				draw_hint_box(1, dvars::con_inputHintBoxColor->current.vector);
				draw_hint_text(0, utils::string::va("%i matches (too many to show here)", matches.size()),
					dvars::con_inputDvarMatchColor->current.vector);
			}
			else if (matches.size() == 1)
			{
				auto* const dvar = game::Dvar_FindVar(matches[0].data());
				const auto line_count = dvar ? 3 : 1;

				const auto height = draw_hint_box(line_count, dvars::con_inputHintBoxColor->current.vector);
				draw_hint_text(0, matches[0].data(), dvar
					? dvars::con_inputDvarMatchColor->current.vector
					: dvars::con_inputCmdMatchColor->current.vector);

				if (dvar)
				{
					const auto offset_value = floor((con.screen_max[0] - con.globals.x) / 4.f);

					draw_hint_text(0, game::Dvar_ValueToString(dvar, dvar->current),
						dvars::con_inputDvarValueColor->current.vector, offset_value);
					draw_hint_text(1, "  default", dvars::con_inputDvarInactiveValueColor->current.vector);
					draw_hint_text(1, game::Dvar_ValueToString(dvar, dvar->reset),
						dvars::con_inputDvarInactiveValueColor->current.vector, offset_value);
					draw_hint_text(2, dvars::dvar_get_description(dvar).data(),
						color_white, 0);

					const auto offset_y = height + 3.f;
					const auto line_count_ = dvar->type == game::dvar_type::enumeration
						? dvar->domain.enumeration.stringCount + 1
						: 1;

					draw_hint_box(line_count_, dvars::con_inputHintBoxColor->current.vector, 0, offset_y);
					draw_hint_text(0, dvars::dvar_get_domain(dvar->type, dvar->domain).data(),
						dvars::con_inputCmdMatchColor->current.vector, 0, offset_y);
				}

				strncpy_s(con.globals.auto_complete_choice, matches[0].data(), 64);
				con.globals.may_auto_complete = true;
			}
			else if (matches.size() > 1)
			{
				draw_hint_box(static_cast<int>(matches.size()), dvars::con_inputHintBoxColor->current.vector);

				const auto offset_value = floor((con.screen_max[0] - con.globals.x) / 4.f);
				const auto offset_description = floor((con.screen_max[0] - con.globals.x) / 2.5f);

				for (size_t i = 0; i < matches.size(); i++)
				{
					auto* const dvar = game::Dvar_FindVar(matches[i].data());

					draw_hint_text(static_cast<int>(i), matches[i].data(),
						dvar
						? dvars::con_inputDvarMatchColor->current.vector
						: dvars::con_inputCmdMatchColor->current.vector);

					if (dvar)
					{
						draw_hint_text(static_cast<int>(i), game::Dvar_ValueToString(dvar, dvar->current),
							dvars::con_inputDvarValueColor->current.vector, offset_value);

						draw_hint_text(static_cast<int>(i), dvars::dvar_get_description(dvar).data(),
							dvars::con_inputDvarValueColor->current.vector, offset_description);
					}
				}

				strncpy_s(con.globals.auto_complete_choice, matches[0].data(), 64);
				con.globals.may_auto_complete = true;
			}
		}

		void draw_output_scrollbar(const float x, float y, const float width, const float height, output_queue& output)
		{
			const auto _x = (x + width) - 10.0f;
			draw_box(_x, y, 10.0f, height, dvars::con_outputBarColor->current.vector);

			auto _height = height;
			if (output.size() > con.visible_line_count)
			{
				const auto percentage = static_cast<float>(con.visible_line_count) / output.size();
				_height *= percentage;

				const auto remainingSpace = height - _height;
				const auto percentageAbove = static_cast<float>(con.display_line_offset) / (output.size() - con.
					visible_line_count);

				y = y + (remainingSpace * percentageAbove);
			}

			draw_box(_x, y, 10.0f, _height, dvars::con_outputSliderColor->current.vector);
		}

		void draw_output_text(const float x, float y, output_queue& output)
		{
			const auto offset = output.size() >= con.visible_line_count
				? 0.0f
				: (con.font_height * (con.visible_line_count - output.size()));

			for (auto i = 0; i < con.visible_line_count; i++)
			{
				y = console_font->pixelHeight + y;

				const auto index = i + con.display_line_offset;
				if (index >= output.size())
				{
					break;
				}

				game::R_AddCmdDrawText(output.at(index).data(), 0x7FFF, console_font, x, y + offset, 1.0f, 1.0f,
					0.0f, color_white, 0);
			}
		}

		void draw_output_window()
		{
			con.output.access([](output_queue& output)
			{
				draw_box(con.screen_min[0], con.screen_min[1] + 32.0f, con.screen_max[0] - con.screen_min[0],
					(con.screen_max[1] - con.screen_min[1]) - 32.0f, dvars::con_outputWindowColor->current.vector);

				const auto x = con.screen_min[0] + 6.0f;
				const auto y = (con.screen_min[1] + 32.0f) + 6.0f;
				const auto width = (con.screen_max[0] - con.screen_min[0]) - 12.0f;
				const auto height = ((con.screen_max[1] - con.screen_min[1]) - 32.0f) - 12.0f;

				game::R_AddCmdDrawText("IW7-Mod ", 0x7FFFFFFF, console_font, x,
					((height - 16.0f) + y) + console_font->pixelHeight, 1.0f, 1.0f, 0.0f, color_title, 0);

				draw_output_scrollbar(x, y, width, height, output);
				draw_output_text(x, y, output);
			});
		}

		void draw_console()
		{
			check_resize();

			if (*game::keyCatchers & 1)
			{
				if (!(*game::keyCatchers & 1))
				{
					con.output_visible = false;
				}

				if (con.output_visible)
				{
					draw_output_window();
				}

				draw_input();
			}
		}
	}

	void print_internal(const char* fmt, ...)
	{
		char va_buffer[0x200] = { 0 };

		va_list ap;
		va_start(ap, fmt);
		vsprintf_s(va_buffer, fmt, ap);
		va_end(ap);

		const auto formatted = std::string(va_buffer);
		const auto lines = utils::string::split(formatted, '\n');

		for (const auto& line : lines)
		{
			print_internal(line);
		}
	}

	void print(const int type, const std::string& data)
	{
		try
		{
			if (game::environment::is_dedi())
			{
				return;
			}
		}
		catch (std::exception&)
		{
			return;
		}

		const auto lines = utils::string::split(data, '\n');
		for (const auto& line : lines)
		{
			print_internal(type == console::con_type_info ? line : "^"s.append(std::to_string(type)).append(line));
		}
	}

	bool console_char_event(const int local_client_num, const int key)
	{
		if (key == game::keyNum_t::K_GRAVE ||
			key == game::keyNum_t::K_TILDE ||
			key == '|' ||
			key == '\\')
		{
			return false;
		}

		if (key > 127)
		{
			return true;
		}

		if (*game::keyCatchers & 1)
		{
			if (key == game::keyNum_t::K_TAB) // tab (auto complete) 
			{
				if (con.globals.may_auto_complete)
				{
					const auto first_char = con.buffer[0];

					clear();

					if (first_char == '\\' || first_char == '/')
					{
						con.buffer[0] = first_char;
						con.buffer[1] = '\0';
					}

					strncat_s(con.buffer, con.globals.auto_complete_choice, 64);
					con.cursor = static_cast<int>(std::string(con.buffer).length());

					if (con.cursor != 254)
					{
						con.buffer[con.cursor++] = ' ';
						con.buffer[con.cursor] = '\0';
					}
				}
			}

			if (key == 'v' - 'a' + 1) // paste
			{
				const auto clipboard = utils::string::get_clipboard_data();
				if (clipboard.empty())
				{
					return false;
				}

				for (size_t i = 0; i < clipboard.length(); i++)
				{
					console_char_event(local_client_num, clipboard[i]);
				}

				return false;
			}

			if (key == 'c' - 'a' + 1) // clear
			{
				clear();
				con.line_count = 0;
				con.display_line_offset = 0;
				con.output.access([](output_queue& output)
					{
						output.clear();
					});
				history_index = -1;
				history.clear();

				return false;
			}

			if (key == 'h' - 'a' + 1) // backspace
			{
				if (con.cursor > 0)
				{
					memmove(con.buffer + con.cursor - 1, con.buffer + con.cursor,
						strlen(con.buffer) + 1 - con.cursor);
					con.cursor--;
				}

				return false;
			}

			if (key < 32)
			{
				return false;
			}

			if (con.cursor == 256 - 1)
			{
				return false;
			}

			memmove(con.buffer + con.cursor + 1, con.buffer + con.cursor, strlen(con.buffer) + 1 - con.cursor);
			con.buffer[con.cursor] = static_cast<char>(key);
			con.cursor++;

			if (con.cursor == strlen(con.buffer) + 1)
			{
				con.buffer[con.cursor] = 0;
			}
		}

		return true;
	}

	bool console_key_event(const int local_client_num, const int key, const int down)
	{
		if (key == game::keyNum_t::K_GRAVE || key == game::keyNum_t::K_TILDE)
		{
			if (!down)
			{
				return false;
			}

			const auto shift_down = game::playerKeys[local_client_num].keys[game::keyNum_t::K_SHIFT].down;
			if (shift_down)
			{
				if (!(*game::keyCatchers & 1))
				{
					toggle_console();
				}

				toggle_console_output();
				return false;
			}

			toggle_console();

			return false;
		}

		if (*game::keyCatchers & 1)
		{
			if (down)
			{
				if (key == game::keyNum_t::K_UPARROW)
				{
					if (++history_index >= history.size())
					{
						history_index = static_cast<int>(history.size()) - 1;
					}

					clear();

					if (history_index != -1)
					{
						strncpy_s(con.buffer, history.at(history_index).c_str(), 0x100);
						con.cursor = static_cast<int>(strlen(con.buffer));
					}
				}
				else if (key == game::keyNum_t::K_DOWNARROW)
				{
					if (--history_index < -1)
					{
						history_index = -1;
					}

					clear();

					if (history_index != -1)
					{
						strncpy_s(con.buffer, history.at(history_index).c_str(), 0x100);
						con.cursor = static_cast<int>(strlen(con.buffer));
					}
				}

				if (key == game::keyNum_t::K_RIGHTARROW)
				{
					if (con.cursor < strlen(con.buffer))
					{
						con.cursor++;
					}

					return false;
				}

				if (key == game::keyNum_t::K_LEFTARROW)
				{
					if (con.cursor > 0)
					{
						con.cursor--;
					}

					return false;
				}

				//scroll through output
				if (key == game::keyNum_t::K_MWHEELUP || key == game::keyNum_t::K_PGUP)
				{
					con.output.access([](output_queue& output)
						{
							if (output.size() > con.visible_line_count && con.display_line_offset > 0)
							{
								con.display_line_offset--;
							}
						});
				}
				else if (key == game::keyNum_t::K_MWHEELDOWN || key == game::keyNum_t::K_PGDN)
				{
					con.output.access([](output_queue& output)
						{
							if (output.size() > con.visible_line_count
								&& con.display_line_offset < (output.size() - con.visible_line_count))
							{
								con.display_line_offset++;
							}
						});
				}

				if (key == game::keyNum_t::K_ENTER)
				{
					game::Cbuf_AddText(0, utils::string::va("%s \n", fixed_input.data()));

					if (history_index != -1)
					{
						const auto itr = history.begin() + history_index;

						if (*itr == con.buffer)
						{
							history.erase(history.begin() + history_index);
						}
					}

					history.push_front(con.buffer);

					console::info("]%s\n", con.buffer);

					if (history.size() > 10)
					{
						history.erase(history.begin() + 10);
					}

					history_index = -1;

					clear();
				}
			}
		}

		return true;
	}

	class component final : public component_interface
	{
	public:
		void post_unpack() override
		{
			if (game::environment::is_dedi())
			{
				return;
			}

			scheduler::loop(draw_console, scheduler::pipeline::renderer);

			// initialize our structs
			con.cursor = 0;
			con.visible_line_count = 0;
			con.output_visible = false;
			con.display_line_offset = 0;
			con.line_count = 0;
			strncpy_s(con.buffer, "", 256);

			con.globals.x = 0.0f;
			con.globals.y = 0.0f;
			con.globals.left_x = 0.0f;
			con.globals.font_height = 0.0f;
			con.globals.may_auto_complete = false;
			con.globals.info_line_count = 0;
			strncpy_s(con.globals.auto_complete_choice, "", 64);

			// add clear command
			command::add("clear", [&]()
			{
				clear();
				con.line_count = 0;
				con.display_line_offset = 0;
				con.output.access([](output_queue& output)
					{
						output.clear();
					});
				history_index = -1;
				history.clear();
			});

			// add our dvars
			dvars::con_inputBoxColor = game::Dvar_RegisterVec4("con_inputBoxColor", 0.2f, 0.2f, 0.2f, 0.9f, 0.0f, 1.0f,
				game::DVAR_FLAG_SAVED,
				"color of console input box");
			dvars::con_inputHintBoxColor = game::Dvar_RegisterVec4("con_inputHintBoxColor", 0.3f, 0.3f, 0.3f, 1.0f,
				0.0f, 1.0f,
				game::DVAR_FLAG_SAVED, "color of console input hint box");
			dvars::con_outputBarColor = game::Dvar_RegisterVec4("con_outputBarColor", 0.5f, 0.5f, 0.5f, 0.6f, 0.0f,
				1.0f, game::DVAR_FLAG_SAVED,
				"color of console output bar");
			dvars::con_outputSliderColor = game::Dvar_RegisterVec4("con_outputSliderColor", 0.3f, 0.7f, 0.3f, 1.0f,
				0.0f, 1.0f,
				game::DVAR_FLAG_SAVED, "color of console output slider");
			dvars::con_outputWindowColor = game::Dvar_RegisterVec4("con_outputWindowColor", 0.25f, 0.25f, 0.25f, 0.85f,
				0.0f,
				1.0f, game::DVAR_FLAG_SAVED, "color of console output window");
			dvars::con_inputDvarMatchColor = game::Dvar_RegisterVec4("con_inputDvarMatchColor", 1.0f, 1.0f, 0.8f, 1.0f,
				0.0f,
				1.0f, game::DVAR_FLAG_SAVED, "color of console matched dvar");
			dvars::con_inputDvarValueColor = game::Dvar_RegisterVec4("con_inputDvarValueColor", 1.0f, 1.0f, 0.8f, 1.0f,
				0.0f,
				1.0f, game::DVAR_FLAG_SAVED, "color of console matched dvar value");
			dvars::con_inputDvarInactiveValueColor = game::Dvar_RegisterVec4(
				"con_inputDvarInactiveValueColor", 0.8f, 0.8f,
				0.8f, 1.0f, 0.0f, 1.0f, game::DVAR_FLAG_SAVED,
				"color of console inactive dvar value");
			dvars::con_inputCmdMatchColor = game::Dvar_RegisterVec4("con_inputCmdMatchColor", 0.80f, 0.80f, 1.0f, 1.0f,
				0.0f,
				1.0f, game::DVAR_FLAG_SAVED, "color of console matched command");
		}
	};
}

REGISTER_COMPONENT(game_console::component)