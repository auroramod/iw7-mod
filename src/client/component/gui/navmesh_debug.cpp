#include <std_include.hpp>

#ifdef _DEBUG
#include "loader/component_loader.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include "component/scheduler.hpp"

#include <utils/string.hpp>

namespace navmesh_debug
{
	namespace
	{
		game::dvar_t* cg_drawNavMesh;
		game::dvar_t* cg_drawNavMeshDistance;

		constexpr auto nav_graph_offset = 0x30;
		constexpr auto nav_graph_area_bytes = 0x8;
		constexpr auto nav_graph_header_size = 0x150;
		constexpr auto nav_area_size = 72;
		constexpr auto nav_area_pos = 32;
		constexpr auto nav_area_radius = 44;
		constexpr auto nav_area_flags = 56;
		constexpr auto nav_edge_size = 32;
		constexpr auto nav_edge_pos = 8;
		constexpr auto nav_edge_flags = 20;
		constexpr auto nav_edge_portal = 0x8000;

		float boundary_color[4] = { 1.0f, 0.25f, 0.2f, 1.0f };
		float shared_color[4] = { 0.2f, 0.9f, 0.3f, 0.45f };
		float portal_color[4] = { 1.0f, 0.85f, 0.1f, 1.0f };
		float link_color[4] = { 0.2f, 0.85f, 1.0f, 1.0f };
		float spawner_color[4] = { 1.0f, 0.55f, 0.1f, 1.0f };
		float node_color[4] = { 0.2f, 0.85f, 1.0f, 1.0f };
		float goal_color[4] = { 1.0f, 0.3f, 1.0f, 1.0f };

		struct ent_label
		{
			game::vec3_t origin;
			std::string name;
			float* color;
		};

		const char* cached_ents = nullptr;
		std::vector<ent_label> labels;

		std::string key_name(const std::string& key)
		{
			static const std::unordered_map<std::string, std::string> canonical =
			{
				{"72", "animscript"},
				{"157", "classname"},
				{"543", "origin"},
				{"638", "script_noteworthy"},
				{"820", "target"},
				{"822", "targetname"},
			};

			const auto itr = canonical.find(key);
			return itr == canonical.end() ? key : itr->second;
		}

		std::string get_value(const std::unordered_map<std::string, std::string>& ent, const std::string& key)
		{
			const auto itr = ent.find(key);
			return itr == ent.end() ? std::string{} : itr->second;
		}

		void add_label(const std::unordered_map<std::string, std::string>& ent)
		{
			const auto classname = get_value(ent, "classname");
			const auto targetname = get_value(ent, "targetname");
			const auto origin = get_value(ent, "origin");

			if (origin.empty())
			{
				return;
			}

			ent_label label{};
			if (sscanf_s(origin.data(), "%f %f %f", &label.origin[0], &label.origin[1], &label.origin[2]) != 3)
			{
				return;
			}

			if (classname == "node_negotiation_begin")
			{
				label.name = utils::string::va("%s -> %s", get_value(ent, "animscript").data(), get_value(ent, "target").data());
				label.color = node_color;
			}
			else if (classname == "node_negotiation_end")
			{
				label.name = utils::string::va("end %s", targetname.data());
				label.color = node_color;
			}
			else if (classname == "script_struct" && targetname.find("spawner") != std::string::npos)
			{
				label.name = targetname;
				label.color = spawner_color;
			}
			else if (classname == "script_struct" && (targetname == "window_entrance" || targetname == "exterior_goal"))
			{
				label.name = targetname;
				label.color = goal_color;
			}
			else
			{
				return;
			}

			labels.emplace_back(std::move(label));
		}

		void parse_ents(const char* entity_string)
		{
			labels.clear();

			std::unordered_map<std::string, std::string> ent{};
			std::istringstream stream(entity_string);
			std::string line{};

			while (std::getline(stream, line))
			{
				if (!line.empty() && line.back() == '\r')
				{
					line.pop_back();
				}

				if (line == "{")
				{
					ent.clear();
					continue;
				}

				if (line == "}")
				{
					add_label(ent);
					continue;
				}

				const auto value_end = line.find_last_of('"');
				const auto value_start = value_end == std::string::npos ? std::string::npos : line.find_last_of('"', value_end - 1);
				if (value_start == std::string::npos || value_start == 0)
				{
					continue;
				}

				auto key = line.substr(0, value_start);
				while (!key.empty() && (key.back() == ' ' || key.back() == '\t'))
				{
					key.pop_back();
				}

				if (key.size() >= 2 && key.front() == '"' && key.back() == '"')
				{
					key = key.substr(1, key.size() - 2);
				}

				ent[key_name(key)] = line.substr(value_start + 1, value_end - value_start - 1);
			}
		}

		float distance_sqr(const float* a, const float* b)
		{
			const auto x = a[0] - b[0];
			const auto y = a[1] - b[1];
			const auto z = a[2] - b[2];
			return x * x + y * y + z * z;
		}

		void draw_line(const float* start, const float* end, float* color, float width)
		{
			const auto placement = game::ScrPlace_GetActivePlacement();

			game::vec2_t a{}, b{};
			if (!game::CG_WorldPosToScreenPosReal(0, placement, start, a) || !game::CG_WorldPosToScreenPosReal(0, placement, end, b))
			{
				return;
			}

			const auto dx = b[0] - a[0];
			const auto dy = b[1] - a[1];
			const auto length = std::sqrt(dx * dx + dy * dy);
			if (length < 0.5f)
			{
				return;
			}

			const auto nx = -dy / length * width * 0.5f;
			const auto ny = dx / length * width * 0.5f;

			const float verts[4][2] =
			{
				{ a[0] - nx, a[1] - ny },
				{ b[0] - nx, b[1] - ny },
				{ b[0] + nx, b[1] + ny },
				{ a[0] + nx, a[1] + ny },
			};

			static const auto material = game::Material_RegisterHandle("white");
			game::R_AddCmdDrawQuadPic(verts, 0.0f, 0.0f, 1.0f, 1.0f, color, material, 0);
		}

		void draw_text(const std::string& text, const float* origin, float* color, int line)
		{
			game::vec2_t screen{};
			if (!game::CG_WorldPosToScreenPosReal(0, game::ScrPlace_GetActivePlacement(), origin, screen))
			{
				return;
			}

			const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", 18);
			if (!font)
			{
				return;
			}

			game::R_AddCmdDrawText(text.data(), 0x7FFFFFFF, font, screen[0], screen[1] + 20.0f * static_cast<float>(line), 1.0f, 1.0f, 0.0f, color, 6);
		}

		void draw_label(const std::string& name, const float* origin, float* color)
		{
			const float top[3] = { origin[0], origin[1], origin[2] + 24.0f };
			draw_line(origin, top, color, 2.0f);
			draw_text(name, top, color, 0);
			draw_text(utils::string::va("%.1f %.1f %.1f", origin[0], origin[1], origin[2]), top, color, 1);
		}

		void draw_graph(const char* buffer, const float* view, float max_sqr)
		{
			const auto* graph = buffer + nav_graph_offset;
			const auto area_bytes = *reinterpret_cast<const std::uint32_t*>(graph + nav_graph_area_bytes);
			const auto* areas_start = graph + nav_graph_header_size;
			const auto* areas_end = areas_start + area_bytes;

			std::vector<std::pair<float, const char*>> nearby{};

			for (const auto* area = areas_start; area < areas_end;)
			{
				const auto num_edges = *reinterpret_cast<const std::uint32_t*>(area + nav_area_flags) & 0x7F;
				if (num_edges == 0)
				{
					break;
				}

				const auto* center = reinterpret_cast<const float*>(area + nav_area_pos);
				const auto radius = *reinterpret_cast<const float*>(area + nav_area_radius);
				const auto distance = distance_sqr(center, view);
				if (distance <= max_sqr + radius * radius)
				{
					nearby.emplace_back(distance, area);
				}

				area += nav_area_size + num_edges * nav_edge_size;
			}

			std::sort(nearby.begin(), nearby.end());

			for (const auto& entry : nearby)
			{
				const auto* area = entry.second;
				const auto num_edges = *reinterpret_cast<const std::uint32_t*>(area + nav_area_flags) & 0x7F;
				const auto* edges = area + nav_area_size;

				const auto self = static_cast<std::uint64_t>(area - graph);
				const auto self_address = reinterpret_cast<std::uint64_t>(area);

				for (auto i = 0u; i < num_edges; i++)
				{
					const auto* edge = edges + i * nav_edge_size;
					const auto* edge_next = edges + ((i + 1) % num_edges) * nav_edge_size;
					const auto adjacent = *reinterpret_cast<const std::uint64_t*>(edge);
					const auto flags = *reinterpret_cast<const std::uint32_t*>(edge + nav_edge_flags);

					if (adjacent && adjacent < (adjacent > area_bytes + nav_graph_header_size ? self_address : self))
					{
						continue;
					}

					const auto* a = reinterpret_cast<const float*>(edge + nav_edge_pos);
					const auto* b = reinterpret_cast<const float*>(edge_next + nav_edge_pos);
					const float start[3] = { a[0], a[1], a[2] + 2.0f };
					const float end[3] = { b[0], b[1], b[2] + 2.0f };

					if (!adjacent)
					{
						draw_line(start, end, boundary_color, 2.0f);
					}
					else if (flags & nav_edge_portal)
					{
						draw_line(start, end, portal_color, 1.5f);
					}
					else
					{
						draw_line(start, end, shared_color, 1.0f);
					}
				}
			}
		}

		void draw_navmesh()
		{
			if (!cg_drawNavMesh || !cg_drawNavMesh->current.enabled)
			{
				return;
			}

			const auto mapname = game::Dvar_FindVar("mapname");
			if (!mapname)
			{
				return;
			}

			const std::string asset_name = utils::string::va("maps/%s/%s.d3dbsp", game::Com_GameMode_GetActiveGameModeStr(), mapname->current.string);

			game::vec3_t view{};
			game::CG_GetPlayerViewOrigin(0, game::SV_GetPlayerstateForClientNum(0), &view);

			const auto max_distance = static_cast<float>(cg_drawNavMeshDistance->current.integer);
			const auto max_sqr = max_distance * max_distance;

			const auto map_ents = game::DB_FindXAssetHeader(game::ASSET_TYPE_MAP_ENTS, asset_name.data(), 0).mapEnts;
			if (map_ents && map_ents->entityString)
			{
				if (cached_ents != map_ents->entityString)
				{
					cached_ents = map_ents->entityString;
					parse_ents(cached_ents);
				}

				for (const auto& label : labels)
				{
					if (distance_sqr(label.origin, view) <= max_sqr)
					{
						draw_label(label.name, label.origin, label.color);
					}
				}
			}

			const auto nav_mesh = game::DB_FindXAssetHeader(game::ASSET_TYPE_NAVMESH, asset_name.data(), 0).navMeshData;
			if (nav_mesh)
			{
				for (auto i = 0; i < nav_mesh->numLinkCreationData; i++)
				{
					const auto* link = &nav_mesh->linkCreationData[i];
					const auto* start = link->m_Start.m_Pt1;
					const auto* end = link->m_End.m_Pt1;

					if (distance_sqr(start, view) > max_sqr && distance_sqr(end, view) > max_sqr)
					{
						continue;
					}

					draw_line(start, end, link_color, 3.0f);

					const float tip[3] = { end[0], end[1], end[2] + 12.0f };
					draw_line(end, tip, link_color, 3.0f);
				}

				for (auto i = 0; i < nav_mesh->numNavResources; i++)
				{
					const auto* resource = &nav_mesh->navResources[i];
					if (resource->pGraphBuffer && resource->graphSize > nav_graph_offset + nav_graph_header_size)
					{
						draw_graph(resource->pGraphBuffer, view, max_sqr);
					}
				}
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

			cg_drawNavMesh = game::Dvar_RegisterBool("cg_drawNavMesh", false, game::DVAR_FLAG_NONE, "Draw the map navmesh, its links and the named spawn and link spots");
			cg_drawNavMeshDistance = game::Dvar_RegisterInt("cg_drawNavMeshDistance", 1000, 0, 50000, game::DVAR_FLAG_NONE, "cg_drawNavMesh draw distance from the player view");

			scheduler::loop([]
			{
				if (game::CL_IsGameClientActive(0))
				{
					draw_navmesh();
				}
			}, scheduler::renderer);
		}
	};
}

REGISTER_COMPONENT(navmesh_debug::component)
#endif
