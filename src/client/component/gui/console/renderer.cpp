#include <std_include.hpp>
#include "loader/component_loader.hpp"

#include "component/gui/gui.hpp"
#include "component/game_console.hpp"
#include "component/console/autocomplete.hpp"
#include "component/scheduler.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/string.hpp>

namespace console_renderer
{
	namespace
	{
		constexpr auto max_visible_suggestions = 10;
		constexpr auto hint_padding = 6.0f;
		constexpr auto hint_gap = 3.0f;
		constexpr auto window_opacity = 0.9f;

		constexpr ImVec4 color_white{1.0f, 1.0f, 1.0f, 1.0f};
		constexpr ImVec4 color_highlight{1.0f, 1.0f, 1.0f, 0.12f};
		constexpr ImVec4 color_hover{1.0f, 1.0f, 1.0f, 0.06f};

		struct console_state
		{
			std::string input;
			std::string pending_input;
			bool has_pending_input{};
			bool cursor_to_end{};
			int cursor_pos{};
			bool input_active{};

			bool was_open{};
			bool focus_input{};
			bool scroll_to_bottom{};
			int history_index = -1;

			bool query_valid{};
			std::string queried_input;
			std::uint32_t queried_generation{};
			autocomplete::result result;
			int last_selected = -1;
			int list_first{};
		};

		console_state state{};

		ImVec4 to_color(const game::dvar_t* dvar)
		{
			const auto* v = dvar->current.vector;
			return {v[0], v[1], v[2], v[3]};
		}

		ImVec4 get_color_code(const char code)
		{
			switch (code)
			{
			case '0': return {0.35f, 0.35f, 0.35f, 1.0f};
			case '1': return ImColor(200, 15, 30, 255);
			case '2': return ImColor(50, 200, 50, 255);
			case '3': return ImColor(200, 155, 0, 255);
			case '4': return ImColor(60, 150, 220, 255);
			case '5': return {0.3f, 0.9f, 1.0f, 1.0f};
			case '6': return {1.0f, 0.45f, 1.0f, 1.0f};
			case '8': return {1.0f, 0.6f, 0.2f, 1.0f};
			case '9': return {0.6f, 0.6f, 0.6f, 1.0f};
			default: return {1.0f, 1.0f, 1.0f, 1.0f};
			}
		}

		bool is_color_code(const std::string& text, const std::size_t pos)
		{
			return text[pos] == '^' && pos + 1 < text.size() && std::isdigit(static_cast<unsigned char>(text[pos + 1]));
		}

		std::string strip_color_codes(const std::string& text)
		{
			std::string result;
			for (std::size_t i = 0; i < text.size(); i++)
			{
				if (is_color_code(text, i))
				{
					i++;
					continue;
				}

				result.push_back(text[i]);
			}

			return result;
		}

		void render_line(const std::string& line)
		{
			auto color = get_color_code('7');
			auto drawn = false;
			std::size_t start = 0;

			const auto flush = [&](const std::size_t end)
			{
				if (end <= start)
				{
					return;
				}

				if (drawn)
				{
					ImGui::SameLine(0.0f, 0.0f);
				}

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(line.data() + start, line.data() + end);
				ImGui::PopStyleColor();
				drawn = true;
			};

			for (std::size_t i = 0; i < line.size(); i++)
			{
				if (is_color_code(line, i))
				{
					flush(i);
					color = get_color_code(line[i + 1]);
					start = i + 2;
					i++;
				}
			}

			flush(line.size());

			if (!drawn)
			{
				ImGui::TextUnformatted("");
			}
		}

		void set_input(const std::string& text)
		{
			if (state.input_active)
			{
				state.pending_input = text;
				state.has_pending_input = true;
			}
			else
			{
				state.input = text;
				state.cursor_to_end = true;
			}

			state.focus_input = true;
		}

		void replace_text(ImGuiInputTextCallbackData* data, const std::string& text)
		{
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, text.data());
			data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen;
		}

		int input_callback(ImGuiInputTextCallbackData* data)
		{
			switch (data->EventFlag)
			{
			case ImGuiInputTextFlags_CallbackCharFilter:
				return data->EventChar == '`' || data->EventChar == '~';

			case ImGuiInputTextFlags_CallbackCompletion:
				replace_text(data, autocomplete::complete(std::string(data->Buf, data->BufTextLen), ImGui::GetIO().KeyShift));
				break;

			case ImGuiInputTextFlags_CallbackHistory:
			{
				const auto& history = autocomplete::get_history();
				if (history.empty())
				{
					break;
				}

				if (data->EventKey == ImGuiKey_UpArrow && state.history_index + 1 < static_cast<int>(history.size()))
				{
					state.history_index++;
				}
				else if (data->EventKey == ImGuiKey_DownArrow && state.history_index >= 0)
				{
					state.history_index--;
				}

				replace_text(data, state.history_index >= 0 ? history[state.history_index] : "");
				break;
			}

			case ImGuiInputTextFlags_CallbackAlways:
				if (state.has_pending_input)
				{
					replace_text(data, state.pending_input);
					state.has_pending_input = false;
				}
				else if (state.cursor_to_end)
				{
					data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen;
				}

				state.cursor_to_end = false;
				state.cursor_pos = data->CursorPos;
				break;

			default:
				break;
			}

			return 0;
		}

		void update_query()
		{
			const auto generation = autocomplete::get_generation();
			if (state.query_valid && state.queried_input == state.input && state.queried_generation == generation)
			{
				return;
			}

			if (state.queried_input != state.input)
			{
				state.list_first = 0;
			}

			state.result = state.input.empty() ? autocomplete::result{} : autocomplete::query(state.input);
			state.queried_input = state.input;
			state.queried_generation = generation;
			state.query_valid = true;
		}

		void on_suggestion_clicked(const autocomplete::match& match)
		{
			const auto& result = state.result;
			auto text = result.head + match.name;

			if (result.arg_index == 0)
			{
				text.push_back(' ');

				if (match.type == autocomplete::match_type::dvar)
				{
					text.append(match.description.find(' ') != std::string::npos ? "\"" + match.description + "\"" : match.description);
				}
			}

			set_input(text);
		}

		struct hint_painter
		{
			ImDrawList* draw_list{};
			ImVec2 origin{};
			float width{};
			float line_height{};
			float y{};

			float value_x() const
			{
				return std::floor(width / 4.0f);
			}

			float description_x() const
			{
				return std::floor(width / 2.5f);
			}

			float box(const int lines) const
			{
				const auto height = static_cast<float>(lines) * line_height + hint_padding * 2.0f;
				if (draw_list)
				{
					const auto color = to_color(dvars::con_inputHintBoxColor);
					const auto border = ImGui::GetColorU32(ImVec4(color.x * 0.5f, color.y * 0.5f, color.z * 0.5f, color.w));
					const auto min = ImVec2(origin.x, origin.y + y);
					const auto max = ImVec2(origin.x + width, origin.y + y + height);

					draw_list->AddRectFilled(min, max, ImGui::GetColorU32(color));
					draw_list->AddRectFilled(min, ImVec2(min.x + 2.0f, max.y), border);
					draw_list->AddRectFilled(ImVec2(max.x - 2.0f, min.y), max, border);
					draw_list->AddRectFilled(min, ImVec2(max.x, min.y + 2.0f), border);
					draw_list->AddRectFilled(ImVec2(min.x, max.y - 2.0f), max, border);
				}

				return height;
			}

			ImVec2 line_pos(const int line, const float x = 0.0f) const
			{
				return {origin.x + hint_padding + x, origin.y + y + hint_padding + static_cast<float>(line) * line_height};
			}

			void text(const int line, const float x, const float max_x, const ImVec4& color, const std::string& value) const
			{
				if (!draw_list || value.empty())
				{
					return;
				}

				const auto pos = line_pos(line, x);
				const ImVec4 clip{pos.x, pos.y, origin.x + std::min(max_x, width - hint_padding), pos.y + line_height};
				draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, ImGui::GetColorU32(color),
					value.data(), value.data() + value.size(), 0.0f, &clip);
			}

			void text_wrapped(const int line, const ImVec4& color, const std::string& value) const
			{
				if (!draw_list || value.empty())
				{
					return;
				}

				draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), line_pos(line), ImGui::GetColorU32(color),
					value.data(), value.data() + value.size(), text_width());
			}

			float text_width() const
			{
				return width - hint_padding * 2.0f;
			}

			int wrapped_lines(const std::string& value) const
			{
				if (value.empty())
				{
					return 0;
				}

				const auto size = ImGui::CalcTextSize(value.data(), value.data() + value.size(), false, text_width());
				return std::max(1, static_cast<int>(std::round(size.y / line_height)));
			}
		};

		int count_lines(const std::string& text)
		{
			return text.empty() ? 0 : static_cast<int>(std::count(text.begin(), text.end(), '\n')) + 1;
		}

		std::string trim_trailing_newlines(std::string text)
		{
			while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
			{
				text.pop_back();
			}

			return text;
		}

		ImVec4 get_match_color(const autocomplete::match& match)
		{
			switch (match.type)
			{
			case autocomplete::match_type::dvar:
				return to_color(dvars::con_inputDvarMatchColor);
			case autocomplete::match_type::command:
				return to_color(dvars::con_inputCmdMatchColor);
			default:
				return color_white;
			}
		}

		void draw_dvar_details(hint_painter& painter, const std::string& name, game::dvar_t* dvar)
		{
			const auto description = dvars::dvar_get_description(dvar);
			const auto domain = trim_trailing_newlines(dvars::dvar_get_domain(dvar->type, dvar->domain));
			const auto value_x = painter.value_x();
			const auto details_height = painter.box(2 + painter.wrapped_lines(description));

			painter.text(0, 0.0f, value_x - hint_padding, to_color(dvars::con_inputDvarMatchColor), name);
			painter.text(0, value_x, FLT_MAX, to_color(dvars::con_inputDvarValueColor), game::Dvar_ValueToString(dvar, dvar->current));
			painter.text(1, 0.0f, value_x - hint_padding, to_color(dvars::con_inputDvarInactiveValueColor), "  default");
			painter.text(1, value_x, FLT_MAX, to_color(dvars::con_inputDvarInactiveValueColor), game::Dvar_ValueToString(dvar, dvar->reset));
			painter.text_wrapped(2, color_white, description);

			painter.y += details_height + hint_gap;

			if (domain.empty())
			{
				return;
			}

			const auto domain_height = painter.box(count_lines(domain));

			auto line = 0;
			std::size_t start = 0;
			while (true)
			{
				const auto end = domain.find('\n', start);
				painter.text(line++, 0.0f, FLT_MAX, to_color(dvars::con_inputCmdMatchColor),
					domain.substr(start, end == std::string::npos ? std::string::npos : end - start));

				if (end == std::string::npos)
				{
					break;
				}

				start = end + 1;
			}

			painter.y += domain_height + hint_gap;
		}

		void draw_command_name(hint_painter& painter, const char* name)
		{
			const auto height = painter.box(1);
			painter.text(0, 0.0f, FLT_MAX, to_color(dvars::con_inputCmdMatchColor), name);
			painter.y += height + hint_gap;
		}

		void update_list_scroll(const autocomplete::result& result)
		{
			const auto count = static_cast<int>(result.matches.size());
			const auto visible = std::min(count, max_visible_suggestions);

			if (result.selected >= 0 && result.selected != state.last_selected)
			{
				if (result.selected < state.list_first)
				{
					state.list_first = result.selected;
				}
				else if (result.selected >= state.list_first + visible)
				{
					state.list_first = result.selected - visible + 1;
				}
			}

			state.last_selected = result.selected;
			state.list_first = std::clamp(state.list_first, 0, std::max(0, count - visible));
		}

		void draw_match_list(hint_painter& painter, const autocomplete::result& result)
		{
			const auto& matches = result.matches;
			const auto count = static_cast<int>(matches.size());
			const auto visible = std::min(count, max_visible_suggestions);
			const auto hidden = count - visible;

			const auto height = painter.box(visible + (hidden > 0 ? 1 : 0));
			if (!painter.draw_list)
			{
				painter.y += height + hint_gap;
				return;
			}

			const auto value_x = painter.value_x();
			const auto description_x = painter.description_x();

			for (auto line = 0; line < visible; line++)
			{
				const auto index = state.list_first + line;
				const auto& match = matches[index];
				const auto selected = index == result.selected;

				const auto row_min = ImVec2(painter.origin.x + 4.0f, painter.line_pos(line).y);
				const auto row_size = ImVec2(painter.width - 8.0f, painter.line_height);

				ImGui::SetCursorScreenPos(row_min);
				ImGui::PushID(index);
				const auto clicked = ImGui::InvisibleButton("##match", row_size);
				const auto hovered = ImGui::IsItemHovered();
				ImGui::PopID();

				if (selected || hovered)
				{
					painter.draw_list->AddRectFilled(ImVec2(row_min.x, row_min.y - 1.0f),
						ImVec2(row_min.x + row_size.x, row_min.y + row_size.y + 1.0f),
						ImGui::GetColorU32(selected ? color_highlight : color_hover));
				}

				if (clicked)
				{
					on_suggestion_clicked(match);
				}

				const auto name_color = selected ? to_color(dvars::con_inputSelectedColor) : get_match_color(match);

				if (match.type == autocomplete::match_type::dvar)
				{
					painter.text(line, 0.0f, value_x - hint_padding, name_color, match.name);
					painter.text(line, value_x, description_x - hint_padding, to_color(dvars::con_inputDvarValueColor), match.description);

					if (const auto* dvar = game::Dvar_FindVar(match.name.data()))
					{
						painter.text(line, description_x, FLT_MAX, to_color(dvars::con_inputDvarValueColor),
							dvars::dvar_get_description(dvar));
					}
				}
				else
				{
					painter.text(line, 0.0f, match.description.empty() ? FLT_MAX : value_x - hint_padding, name_color, match.name);
					painter.text(line, value_x, FLT_MAX, to_color(dvars::con_inputDvarInactiveValueColor), match.description);
				}
			}

			if (hidden > 0)
			{
				painter.text(visible, 0.0f, FLT_MAX, to_color(dvars::con_inputDvarInactiveValueColor),
					utils::string::va("... %d more (tab to cycle)", hidden));
			}

			painter.y += height + hint_gap;
		}

		struct hint_plan
		{
			game::dvar_t* dvar{};
			std::string dvar_name;
			const char* command_name{};
			bool show_list{};
		};

		hint_plan get_hint_plan(const autocomplete::result& result)
		{
			hint_plan plan{};

			if (result.arg_index == 0)
			{
				if (result.matches.size() == 1 && result.matches[0].type == autocomplete::match_type::dvar)
				{
					if (auto* const dvar = game::Dvar_FindVar(result.matches[0].name.data()))
					{
						plan.dvar = dvar;
						plan.dvar_name = result.matches[0].name;
						return plan;
					}
				}

				plan.show_list = !result.matches.empty();
				return plan;
			}

			if (auto* const dvar = game::Dvar_FindVar(result.command.data()); dvar && result.arg_index == 1)
			{
				const auto name = dvars::dvar_get_name(dvar);
				plan.dvar = dvar;
				plan.dvar_name = name.empty() ? result.command : name;
				plan.show_list = result.show_list && result.matches.size() > 2 && dvar->type != game::DvarType::enumeration;
				return plan;
			}

			if (!result.show_list)
			{
				return plan;
			}

			if (result.matches.empty())
			{
				for (auto* cmd = *game::cmd_functions; cmd; cmd = cmd->next)
				{
					if (cmd->name && !_stricmp(cmd->name, result.command.data()))
					{
						plan.command_name = cmd->name;
						break;
					}
				}

				return plan;
			}

			plan.show_list = true;
			return plan;
		}

		void draw_hints(hint_painter& painter, const hint_plan& plan, const autocomplete::result& result)
		{
			if (plan.dvar)
			{
				draw_dvar_details(painter, plan.dvar_name, plan.dvar);
			}
			else if (plan.command_name)
			{
				draw_command_name(painter, plan.command_name);
			}

			if (plan.show_list)
			{
				draw_match_list(painter, result);
			}
		}

		void render_suggestions(const ImVec2& input_min, const ImVec2& input_size)
		{
			const auto& result = state.result;
			const auto plan = get_hint_plan(result);

			if (!plan.dvar && !plan.command_name && !plan.show_list)
			{
				state.last_selected = -1;
				return;
			}

			if (plan.show_list)
			{
				update_list_scroll(result);
			}

			hint_painter painter{};
			painter.width = input_size.x;
			painter.line_height = ImGui::GetTextLineHeight();
			draw_hints(painter, plan, result);

			const auto height = painter.y - hint_gap;

			auto pos = ImVec2(input_min.x, input_min.y + input_size.y + hint_gap);
			if (pos.y + height > ImGui::GetIO().DisplaySize.y)
			{
				pos.y = input_min.y - height - hint_gap;
			}

			ImGui::SetNextWindowPos(pos);
			ImGui::SetNextWindowSize(ImVec2(input_size.x, height));

			constexpr auto flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			if (ImGui::Begin("##console_suggestions", nullptr, flags))
			{
				ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

				if (ImGui::IsWindowFocused() && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					state.focus_input = true;
				}

				const auto& io = ImGui::GetIO();
				if (plan.show_list && ImGui::IsWindowHovered() && io.MouseWheel != 0.0f && !io.KeyCtrl)
				{
					state.list_first -= static_cast<int>(io.MouseWheel);
					update_list_scroll(result);
				}

				painter.draw_list = ImGui::GetWindowDrawList();
				painter.origin = ImGui::GetWindowPos();
				painter.y = 0.0f;
				draw_hints(painter, plan, result);
			}

			ImGui::End();
			ImGui::PopStyleVar(2);
		}

		void render_log()
		{
			const auto footer_height = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("##log", ImVec2(0.0f, -footer_height), false, ImGuiWindowFlags_HorizontalScrollbar);

			game_console::access_output([](const game_console::output_queue& output)
			{
				ImGuiListClipper clipper;
				clipper.Begin(static_cast<int>(output.size()));

				while (clipper.Step())
				{
					for (auto i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{
						render_line(output[i]);
					}
				}
			});

			if (state.scroll_to_bottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			{
				ImGui::SetScrollHereY(1.0f);
			}

			state.scroll_to_bottom = false;

			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Copy all"))
				{
					std::string text;
					game_console::access_output([&](const game_console::output_queue& output)
					{
						for (const auto& line : output)
						{
							text.append(strip_color_codes(line)).push_back('\n');
						}
					});

					utils::string::set_clipboard_data(text);
				}

				if (ImGui::MenuItem("Clear"))
				{
					game_console::clear_output();
				}

				ImGui::EndPopup();
			}

			ImGui::EndChild();
		}

		void render_input()
		{
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() &&
				!ImGui::IsMouseDown(ImGuiMouseButton_Left) && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
			{
				state.focus_input = true;
			}

			if (state.focus_input)
			{
				ImGui::SetKeyboardFocusHere();
				state.focus_input = false;
			}

			constexpr auto flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
				ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways |
				ImGuiInputTextFlags_CallbackCharFilter;

			ImGui::PushStyleColor(ImGuiCol_Text, color_white);
			ImGui::PushItemWidth(-1.0f);
			const auto submitted = ImGui::InputText("##input", &state.input, flags, input_callback);
			ImGui::PopItemWidth();
			ImGui::PopStyleColor();

			const auto input_min = ImGui::GetItemRectMin();
			const auto input_size = ImGui::GetItemRectSize();
			state.input_active = ImGui::IsItemActive();

			if (submitted)
			{
				scheduler::once([input = state.input]
				{
					game_console::execute(input);
				}, scheduler::pipeline::main);

				state.input.clear();
				state.history_index = -1;
				state.scroll_to_bottom = true;
				state.focus_input = true;
			}

			update_query();

			const auto cursor_at_end = !state.input_active || state.cursor_pos == static_cast<int>(state.input.size());
			if (cursor_at_end)
			{
				const auto ghost = autocomplete::get_ghost_text(state.result);
				const auto& style = ImGui::GetStyle();
				const auto typed_width = ImGui::CalcTextSize(state.input.data()).x;

				if (!ghost.empty() && typed_width + ImGui::CalcTextSize(ghost.data()).x < input_size.x - style.FramePadding.x * 2.0f)
				{
					ImGui::GetWindowDrawList()->AddText(
						ImVec2(input_min.x + style.FramePadding.x + typed_width, input_min.y + style.FramePadding.y),
						ImGui::GetColorU32(to_color(dvars::con_inputGhostColor)), ghost.data());
				}
			}

			render_suggestions(input_min, input_size);
		}

		void render_window()
		{
			if (!game_console::is_new_console_enabled() || !game_console::is_console_open())
			{
				if (state.was_open)
				{
					state.input.clear();
					state.has_pending_input = false;
					state.cursor_to_end = false;
					state.query_valid = false;
					state.result = {};
					state.last_selected = -1;
					state.list_first = 0;
				}

				state.was_open = false;
				return;
			}

			if (!state.was_open)
			{
				state.was_open = true;
				state.focus_input = true;
				state.scroll_to_bottom = true;
				state.history_index = -1;
				ImGui::SetNextWindowFocus();
			}
			else if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
			{
				ImGui::SetNextWindowFocus();
			}

			auto* font = gui::get_console_font();
			if (font)
			{
				ImGui::PushFont(font);
			}

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, window_opacity));
			ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.1f, 0.1f, window_opacity));
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.15f, 0.15f, 0.15f, window_opacity));
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.05f, 0.05f, 0.05f, 0.9f));

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2.0f);

			ImGui::SetNextWindowSize(ImVec2(600.0f, 400.0f), ImGuiCond_FirstUseEver);

			auto keep_open = true;
			if (ImGui::Begin("CONSOLE###console", &keep_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar))
			{
				render_log();
				render_input();
			}

			ImGui::End();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(4);

			if (font)
			{
				ImGui::PopFont();
			}

			if (!keep_open)
			{
				scheduler::once([]
				{
					if (game_console::is_console_open())
					{
						game_console::toggle_console();
					}
				}, scheduler::pipeline::main);
			}
		}
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

			gui::on_frame(render_window, true, false);
		}
	};
}

REGISTER_COMPONENT(console_renderer::component)
