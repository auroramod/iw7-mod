#include <std_include.hpp>
#ifdef DEBUG
#include "loader/component_loader.hpp"

#include "../dvars.hpp"
#include "../scheduler.hpp"

#include "game/game.hpp"
#include "game/dvars.hpp"

#include <utils/string.hpp>
#include <utils/hook.hpp>

namespace debug
{
	namespace
	{
		game::dvar_t* cg_draw_material;

		float distance_2d(float* a, float* b)
		{
			return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]));
		}

		float distance_3d(float* a, float* b)
		{
			return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2]));
		}

		constexpr auto EPSILON = std::numeric_limits<float>::epsilon();

		// Calculates the cross product of two 3D vectors
		void crossProduct3D(float v1[3], float v2[3], float result[3])
		{
			result[0] = v1[1] * v2[2] - v1[2] * v2[1];
			result[1] = v1[2] * v2[0] - v1[0] * v2[2];
			result[2] = v1[0] * v2[1] - v1[1] * v2[0];
		}

		// Normalizes a 3D vector
		void normalize3D(float v[3])
		{
			float length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			v[0] /= length;
			v[1] /= length;
			v[2] /= length;
		}

		// Calculates the normal vector of a triangle defined by three 3D points
		void calculateTriangleNormal(float p[3][3], float normal[3])
		{
			float v1[3], v2[3];
			for (int i = 0; i < 3; i++) {
				v1[i] = p[1][i] - p[0][i];
				v2[i] = p[2][i] - p[0][i];
			}
			crossProduct3D(v1, v2, normal);
			normalize3D(normal);
		}

		// Calculates the dot product of two 3D vectors
		float dotProduct3D(float v1[3], float v2[3])
		{
			return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
		}

		// Calculates the distance from a point to a plane defined by a point on the plane and the plane's normal vector
		float distancePointToPlane(float planePoint[3], float normal[3], float point[3])
		{
			float dist = 0.0f;
			for (int i = 0; i < 3; i++) {
				dist += (point[i] - planePoint[i]) * normal[i];
			}
			return dist;
		}

		// Calculates the barycentric coordinates of a point in a triangle defined by three 3D points
		void calculateBarycentricCoordinates(float p[3][3], float point[3], float& alpha, float& beta, float& gamma)
		{
			float v0[3], v1[3], v2[3];
			for (int i = 0; i < 3; i++) {
				v0[i] = p[2][i] - p[0][i];
				v1[i] = p[1][i] - p[0][i];
				v2[i] = point[i] - p[0][i];
			}
			float dot00 = dotProduct3D(v0, v0);
			float dot01 = dotProduct3D(v0, v1);
			float dot02 = dotProduct3D(v0, v2);
			float dot11 = dotProduct3D(v1, v1);
			float dot12 = dotProduct3D(v1, v2);
			float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
			beta = (dot11 * dot02 - dot01 * dot12) * invDenom;
			gamma = (dot00 * dot12 - dot01 * dot02) * invDenom;
			alpha = 1.0f - beta - gamma;
		}

		bool lineTriangleIntersection(float p[3][3], float linePoint[3], float lineDir[3])
		{
			// Calculate the normal vector of the triangle
			float normal[3];
			calculateTriangleNormal(p, normal);

			// Calculate the dot product of the normal vector and the line direction vector
			float dotProduct = dotProduct3D(normal, lineDir);

			// If the dot product is close to zero, the line is parallel to the triangle and does not intersect
			if (fabs(dotProduct) < EPSILON) {
				return false;
			}

			// Calculate the distance from the line point to the plane of the triangle
			float distance = distancePointToPlane(p[0], normal, linePoint);

			// If the distance is zero or the sign of the distance is different than the sign of the dot product, the line does not intersect the triangle
			if (fabs(distance) < EPSILON || (distance > 0 && dotProduct > 0) || (distance < 0 && dotProduct < 0)) {
				return false;
			}

			// Calculate the intersection point of the line and the plane of the triangle
			float t = -distance / dotProduct;
			float intersection[3];
			intersection[0] = linePoint[0] + t * lineDir[0];
			intersection[1] = linePoint[1] + t * lineDir[1];
			intersection[2] = linePoint[2] + t * lineDir[2];

			// Calculate the barycentric coordinates of the intersection point
			float alpha, beta, gamma;
			calculateBarycentricCoordinates(p, intersection, alpha, beta, gamma);

			// If the barycentric coordinates are all greater than or equal to zero, the intersection point is inside the triangle and the line intersects the triangle
			if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) {
				return true;
			}

			// Otherwise, the line does not intersect the triangle
			return false;
		}

		void getCenterPoint(float p[3][3], float center[3]) {
			for (int i = 0; i < 3; i++) {
				center[i] = (p[0][i] + p[1][i] + p[2][i]) / 3.0f;
			}
		}

		void render_draw_material()
		{
			static const auto* sv_running = game::Dvar_FindVar("sv_running");
			if (!sv_running || !sv_running->current.enabled)
			{
				return;
			}

			if (!cg_draw_material || !cg_draw_material->current.enabled)
			{
				return;
			}

			static const auto* cg_draw2d = game::Dvar_FindVar("cg_draw2D");
			if (cg_draw2d && !cg_draw2d->current.enabled)
			{
				return;
			}

			static const auto* gfx_map = *game::g_world;
			if (gfx_map == nullptr)
			{
				return;
			}

			const auto placement = game::ScrPlace_GetViewPlacement();
			static const game::vec4_t text_color = { 1.0f, 1.0f, 1.0f, 1.0f };

			game::rectDef_s rect{};
			rect.x = 8.f;
			rect.y = 240.f;
			rect.w = 500.f;
			rect.horzAlign = 1;
			rect.vertAlign = 0;

			const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", 22);
			if (!font)
			{
				return;
			}

			const auto client = game::g_entities[0].client;

			game::vec3_t origin{};
			game::CG_GetPlayerViewOrigin(0, game::SV_GetPlayerstateForClientNum(0), &origin);

			game::vec3_t forward{};
			game::AngleVectors(client->ps.viewangles, forward, nullptr, nullptr);

			float min_distance = -1.f;
			float second_min_distance = -1.f;
			float third_min_distance = -1.f;

			game::vec3_t target_center{}, target_triangle[3]{};
			game::vec3_t secondary_target_center{}, secondary_target_triangle[3]{};
			game::vec3_t third_target_center{}, third_target_triangle[3]{};

			game::GfxSurface* target_surface = nullptr;
			game::GfxSurface* secondary_surface = nullptr;
			game::GfxSurface* third_surface = nullptr;

			for (auto i = 0u; i < gfx_map->surfaceCount; i++)
			{
				const auto surface = &gfx_map->dpvs.surfaces[i];
				const auto indices = &gfx_map->draw.indices[surface->tris.baseIndex];
				bool too_far = false;

				for (auto o = 0; o < surface->tris.triCount && !too_far; o++)
				{
					game::vec3_t triangle[3]{};

					for (auto j = 0; j < 3; j++)
					{
						const auto index = indices[o * 3 + j] + surface->tris.firstVertex;
						const auto vertex = &gfx_map->draw.transientZones[0]->vd.vertices[index];

						if (distance_2d(vertex->xyz, origin) > 1000.f)
						{
							too_far = true;
							break;
						}

						std::memcpy(&triangle[j], vertex->xyz, sizeof(float[3]));
					}

					if (!too_far && lineTriangleIntersection(triangle, origin, forward))
					{
						game::vec3_t center{};
						getCenterPoint(triangle, center);
						const auto dist = distance_3d(center, origin);

						// a messy if statement of gross shifting between mats
						if (dist < min_distance || min_distance == -1.f)
						{
							third_surface = secondary_surface;
							third_min_distance = second_min_distance;
							std::memcpy(&third_target_triangle, &secondary_target_triangle, sizeof(third_target_triangle));
							std::memcpy(&third_target_center, &secondary_target_center, sizeof(game::vec3_t));

							secondary_surface = target_surface;
							second_min_distance = min_distance;
							std::memcpy(&secondary_target_triangle, &target_triangle, sizeof(secondary_target_triangle));
							std::memcpy(&secondary_target_center, &target_center, sizeof(game::vec3_t));

							target_surface = surface;
							min_distance = dist;
							std::memcpy(&target_triangle, &triangle, sizeof(target_triangle));
							std::memcpy(&target_center, &center, sizeof(game::vec3_t));
						}
						else if (dist < second_min_distance || second_min_distance == -1.f)
						{
							third_surface = secondary_surface;
							third_min_distance = second_min_distance;
							std::memcpy(&third_target_triangle, &secondary_target_triangle, sizeof(third_target_triangle));
							std::memcpy(&third_target_center, &secondary_target_center, sizeof(game::vec3_t));

							secondary_surface = surface;
							second_min_distance = dist;
							std::memcpy(&secondary_target_triangle, &triangle, sizeof(secondary_target_triangle));
							std::memcpy(&secondary_target_center, &center, sizeof(game::vec3_t));
						}
						else if (dist < third_min_distance || third_min_distance == -1.f)
						{
							third_surface = surface;
							third_min_distance = dist;
							std::memcpy(&third_target_triangle, &triangle, sizeof(third_target_triangle));
							std::memcpy(&third_target_center, &center, sizeof(game::vec3_t));
						}
					}
				}
			}

			if (min_distance == -1.f)
			{
				return;
			}

			// Surface materials info
			const char* text = nullptr;

			auto format_material_info = [](const game::GfxSurface* surface)
			{
				if (!surface || !surface->material || !surface->material->name)
				{
					return "";
				}

				auto techniqueset_name = surface->material->techniqueSet && surface->material->techniqueSet->name ?
					utils::string::va("^3%s^7", surface->material->techniqueSet->name) : "^1null^7";
				return utils::string::va("%s (%s)\n", surface->material->name, techniqueset_name);
			};

			text = utils::string::va("%s%s%s",
				format_material_info(target_surface),
				format_material_info(secondary_surface),
				format_material_info(third_surface));

			game::UI_DrawWrappedText(placement, text, &rect, font, 0.2f, &text_color, 6, 0, 0, 0, 0);
		}
	}

	namespace
	{
		game::dvar_t* r_drawLightOrigins;
		game::dvar_t* r_drawModelNames;
		game::dvar_t* r_playerDrawDebugDistance;

		enum model_draw_e : int
		{
			off,
			static_models,
			dynent_models,
			scene_models,
			all,
		};

		static const char* model_draw_s[] =
		{
			"off",
			"static models",
			"dynent dynents",
			"scene models",
			"all",
			nullptr
		};

		void VectorSubtract(const float va[3], const float vb[3], float out[3])
		{
			out[0] = va[0] - vb[0];
			out[1] = va[1] - vb[1];
			out[2] = va[2] - vb[2];
		}

		float Vec3SqrDistance(const float v1[3], const float v2[3])
		{
			float out[3];

			VectorSubtract(v2, v1, out);

			return (out[0] * out[0]) + (out[1] * out[1]) + (out[2] * out[2]);
		}

		inline void draw_text(const char* name, game::vec3_t& origin, game::vec4_t& color)
		{
			game::vec2_t screen{};
			if (game::CG_WorldPosToScreenPosReal(0, game::ScrPlace_GetActivePlacement(), origin, screen))
			{
				const auto font = game::R_RegisterFont("fonts/fira_mono_regular.ttf", 25);
				if (font)
				{
					game::R_AddCmdDrawText(name, 0x7FFFFFFF, font, screen[0], screen[1], 1.f, 1.f, 0.0f, color, 6);
				}
			}
		}

		void debug_draw_light_origins()
		{
			if (!r_drawLightOrigins || !r_drawLightOrigins->current.enabled)
			{
				return;
			}

			auto player = game::SV_GetPlayerstateForClientNum(0);
			float playerPosition[3]{ player->origin[0], player->origin[1], player->origin[2] };

			auto mapname = game::Dvar_FindVar("mapname");
			std::string asset_name = utils::string::va("maps/%s.d3dbsp", mapname->current.string);
			if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_SP)
			{
				asset_name = utils::string::va("maps/%s/%s.d3dbsp", game::Com_GameMode_GetActiveGameModeStr(), mapname->current.string);
			}

			auto comWorld = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_COMWORLD, asset_name.data(), 0).comWorld;
			if (comWorld == nullptr)
			{
				return;
			}

			auto distance = r_playerDrawDebugDistance->current.integer;
			auto sqrDist = distance * static_cast<float>(distance);

			float textColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

			for (size_t i = 0; i < comWorld->primaryLightCount; i++)
			{
				auto light = comWorld->primaryLights[i];
				const auto dist = Vec3SqrDistance(playerPosition, light.origin);
				if (dist < static_cast<float>(sqrDist))
				{
					const auto text = utils::string::va("%f, %f, %f (%d)", light.origin[0], light.origin[1], light.origin[2], i);
					draw_text(text, light.origin, textColor);
				}
			}
		}

		void debug_draw_model_names()
		{
			if (!r_drawModelNames || r_drawModelNames->current.integer == model_draw_e::off)
			{
				return;
			}

			auto player = game::SV_GetPlayerstateForClientNum(0);
			float playerPosition[3]{ player->origin[0], player->origin[1], player->origin[2] };

			auto mapname = game::Dvar_FindVar("mapname");
			std::string asset_name = utils::string::va("maps/%s.d3dbsp", mapname->current.string);
			if (game::Com_GameMode_GetActiveGameMode() != game::GAME_MODE_SP)
			{
				asset_name = utils::string::va("maps/%s/%s.d3dbsp", game::Com_GameMode_GetActiveGameModeStr(), mapname->current.string);
			}

			auto gfxAsset = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_GFXWORLD, asset_name.data(), 0).gfxWorld;
			if (gfxAsset == nullptr)
			{
				return;
			}

			auto mapEnts = game::DB_FindXAssetHeader(game::XAssetType::ASSET_TYPE_MAP_ENTS, asset_name.data(), 0).mapEnts;
			if (mapEnts == nullptr)
			{
				return;
			}

			auto distance = r_playerDrawDebugDistance->current.integer;
			auto sqrDist = distance * static_cast<float>(distance);

			static float staticModelsColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
			static float dynEntModelsColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
			static float sceneModelsColor[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
			static float dobjsColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
			auto scene = *game::scene;

			switch (r_drawModelNames->current.integer)
			{
			case model_draw_e::all:
			case model_draw_e::static_models:
				for (unsigned int i = 0; i < gfxAsset->dpvs.smodelCount; i++)
				{
					auto staticModel = gfxAsset->dpvs.smodelDrawInsts[i];
					if (!staticModel.model)
						continue;

					if (Vec3SqrDistance(playerPosition, staticModel.placement.origin) < static_cast<float>(sqrDist))
					{
						draw_text(staticModel.model->name, staticModel.placement.origin, staticModelsColor);
					}
				}
				if (r_drawModelNames->current.integer != model_draw_e::all) break;
			case model_draw_e::dynent_models:
				for (unsigned short i = 0; i < mapEnts->dynEntCount[0]; i++)
				{
					auto* dynent_client = &mapEnts->dynEntClientList[0][0][i];
					auto* dynent_pose = &mapEnts->dynEntPoseList[0][0][i];

					if (!dynent_client || !dynent_pose || !dynent_client->activeModel)
						continue;

					if (Vec3SqrDistance(playerPosition, dynent_pose->pose.origin) < static_cast<float>(sqrDist))
					{
						draw_text(dynent_client->activeModel->name, dynent_pose->pose.origin, dynEntModelsColor);
					}
				}
				if (r_drawModelNames->current.integer != model_draw_e::all) break;
			case model_draw_e::scene_models:
				for (int i = 0; i < scene.sceneModelCount; i++)
				{
					if (!scene.sceneModel[i].model)
						continue;

					if (Vec3SqrDistance(playerPosition, scene.sceneModel[i].placement.base.origin) < static_cast<float>(sqrDist))
					{
						draw_text(scene.sceneModel[i].model->name, scene.sceneModel[i].placement.base.origin, sceneModelsColor);
					}
				}
				break;
			default:
				break;
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

			r_drawLightOrigins = game::Dvar_RegisterBool("r_drawLightOrigins", false, game::DVAR_FLAG_CHEAT, "Draw comworld light origins");
			r_drawModelNames = game::Dvar_RegisterEnum("r_drawModelNames", model_draw_s, model_draw_e::off, game::DVAR_FLAG_CHEAT, "Draw all model names");
			r_playerDrawDebugDistance = game::Dvar_RegisterInt("r_drawDebugDistance", 500, 0, 50000, game::DVAR_FLAG_NONE, "r_draw debug functions draw distance relative to the player");

			cg_draw_material = game::Dvar_RegisterBool("cg_drawMaterial", false, game::DVAR_FLAG_NONE, "Draws material name on screen");

			scheduler::loop([]
			{
				if (game::CL_IsGameClientActive(0))
				{
					debug_draw_light_origins();
					debug_draw_model_names();

					render_draw_material();
				}
			}, scheduler::renderer);
		}
	};
}

REGISTER_COMPONENT(debug::component)
#endif