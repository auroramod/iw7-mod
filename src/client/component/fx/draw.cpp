#include <std_include.hpp>

#include "fx.hpp"

namespace fx
{
	using namespace engine;

	namespace
	{
		using FxDrawElemHandler = void(*)(FxDrawState* draw);

		FxDrawElemHandler FX_GetDrawElemHandler(const int elemType)
		{
			switch (elemType)
			{
			case FX_ELEM_TYPE_SPRITE_BILLBOARD:
				return FXIR_DrawElem_BillboardSprite;
			case FX_ELEM_TYPE_SPRITE_ORIENTED:
				return FXIR_DrawElem_OrientedSprite;
			case FX_ELEM_TYPE_TAIL:
				return FXIR_DrawElem_Tail;
			case FX_ELEM_TYPE_FLARE:
				return FXIR_DrawElem_Flare;
			case FX_ELEM_TYPE_PARTICLE_SIM_ANIMATION:
				return FXIR_DrawElem_ParticleSimAnimation;
			case FX_ELEM_TYPE_CLOUD:
				return FX_DrawElem_Cloud;
			case FX_ELEM_TYPE_SPARK_CLOUD:
				return FX_DrawElem_SparkCloud;
			case FX_ELEM_TYPE_SPARK_FOUNTAIN:
				return FX_DrawElem_SparkFountain;
			case FX_ELEM_TYPE_MODEL:
				return FXIR_DrawElem_Model;
			case FX_ELEM_TYPE_OMNI_LIGHT:
				return FX_DrawElem_Light;
			case FX_ELEM_TYPE_SPOT_LIGHT:
				return FXIR_DrawElem_SpotLight;
			default:
				return nullptr;
			}
		}

		void FX_DrawElement(const FxElemDef* elemDef, const FxElem* elem, const unsigned int elemHandle, FxDrawState* draw)
		{
			const auto handler = FX_GetDrawElemHandler(elemDef->elemType);
			if (!handler || !elemDef->visualCount || elem->msecBegin > draw->msecDraw)
			{
				return;
			}

			draw->elem = elem;
			draw->elemDef = elemDef;
			draw->elemHandle = elemHandle;
			draw->randomSeed = FX_ElemRandomSeed(elemDef, draw->effect, elem->msecBegin, elem->sequence);

			const auto thermal = draw->camera->thermal;
			if ((elemDef->flags & FX_ELEM_DRAW_IN_THERMAL_VIEW_ONLY) != 0 && !thermal)
			{
				return;
			}

			if ((elemDef->flags & FX_ELEM_NODRAW_IN_THERMAL_VIEW) != 0 && thermal)
			{
				return;
			}

			handler(draw);
		}

		void FX_DrawElements(const FxSystem* system, const FxEffect* effect, const unsigned int elemClass, FxDrawState* draw)
		{
			const auto* elemDefs = effect->def->elemDefs;
			for (auto elemHandle = effect->firstElemHandle[elemClass]; elemHandle != -1; elemHandle = FX_NextElemHandle(system, elemHandle))
			{
				const auto* elem = FX_ElemFromHandle(system, elemHandle);
				FX_DrawElement(&elemDefs[elem->defIndex], elem, elemHandle, draw);
			}
		}

		float FX_UpdateOverTime(const float current, const float goal, const int fadeInTime, const int fadeOutTime, const int frameTime)
		{
			if (goal < current)
			{
				return fadeOutTime > 0 ? std::max(current - static_cast<float>(frameTime) / static_cast<float>(fadeOutTime), goal) : goal;
			}

			if (goal > current)
			{
				return fadeInTime > 0 ? std::min(current + static_cast<float>(frameTime) / static_cast<float>(fadeInTime), goal) : goal;
			}

			return current;
		}

		void FX_EvaluateOcclusionFade(const FxDrawState* draw)
		{
			auto* effect = const_cast<FxEffect*>(draw->effect);

			auto visibility = 1.0f;
			if (effect->occlusionQueryHandle != 0xFFFF)
			{
				RB_SetOcclusionQueryPosition(effect->occlusionQueryHandle, effect->frameNow.origin);
				visibility = RB_GetOcclusionQueryVisibility(effect->occlusionQueryHandle);
			}

			const auto frameTime = std::max(draw->system->msecDelta, 0);
			effect->occlusionFade = FX_UpdateOverTime(effect->occlusionFade, visibility, effect->def->occlusionQueryFadeIn,
				effect->def->occlusionQueryFadeOut, frameTime);
		}

		unsigned int Vec3PackUnitVec(const float* v)
		{
			const auto pack = [](const float value)
			{
				return static_cast<unsigned int>(static_cast<int>((std::clamp(value, -1.0f, 1.0f) + 1.0f) * 511.5f)) & 0x3FF;
			};

			return pack(v[0]) | (pack(v[1]) << 10) | (pack(v[2]) << 20) | 0xC0000000;
		}

		unsigned short FloatToHalf(const float value)
		{
			const auto bits = *reinterpret_cast<const unsigned int*>(&value);
			const auto sign = static_cast<unsigned short>((bits >> 16) & 0x8000);
			const auto abs_bits = bits & 0x7FFFFFFF;

			if (abs_bits >= 0x7F800000)
			{
				return sign | (abs_bits > 0x7F800000 ? 0x7E00 : 0x7C00);
			}

			if (abs_bits >= 0x477FF000)
			{
				return sign | 0x7C00;
			}

			if (abs_bits < 0x38800000)
			{
				const auto abs_value = *reinterpret_cast<const float*>(&abs_bits);
				return sign | static_cast<unsigned short>(std::lround(abs_value * 16777216.0f));
			}

			const auto rounded = abs_bits + 0xFFF + ((abs_bits >> 13) & 1);
			return sign | static_cast<unsigned short>((rounded - 0x38000000) >> 13);
		}

		unsigned int Vec2PackTexCoords(const float s, const float t)
		{
			return FloatToHalf(s) | (static_cast<unsigned int>(FloatToHalf(t)) << 16);
		}

		float FX_GetTrailTextureOffset(const FxDrawState* draw, const float spawnDist)
		{
			const auto* trailDef = draw->elemDef->extended.trailDef;
			const auto offset = -std::floor(spawnDist / static_cast<float>(trailDef->repeatDist));
			if (!trailDef->scrollTimeMsec)
			{
				return offset;
			}

			const auto scroll = static_cast<float>(draw->msecDraw % trailDef->scrollTimeMsec) / static_cast<float>(trailDef->scrollTimeMsec);
			return trailDef->scrollTimeMsec > 0 ? offset - scroll : offset + (1.0f - scroll);
		}

		GfxSpriteVertex* FX_GenTrail_VertsForSegment(const FxTrailSegmentDrawState* segment, GfxSpriteVertex* verts)
		{
			const auto* trailDef = segment->trailDef;
			const auto sin = std::sin(segment->rotation);
			const auto cos = std::cos(segment->rotation);

			float left[3]{};
			float up[3]{};
			for (auto i = 0; i < 3; i++)
			{
				left[i] = segment->basis[0][i] * cos + segment->basis[1][i] * sin;
				up[i] = segment->basis[0][i] * sin - segment->basis[1][i] * cos;
			}

			const auto tangent = Vec3PackUnitVec(left);
			for (auto i = 0; i < trailDef->vertCount; i++)
			{
				const auto* trailVert = &trailDef->verts[i];
				const auto x = trailVert->pos[0] * segment->size[0];
				const auto y = trailVert->pos[1] * segment->size[1];

				float normal[3]{};
				for (auto j = 0; j < 3; j++)
				{
					verts->xyz[j] = left[j] * x + segment->posWorld[j] + up[j] * y;
					normal[j] = left[j] * trailVert->normal[0] + up[j] * trailVert->normal[1];
				}

				// iw7 geo trails store their intensity here, 0 hides them
				verts->extraData = Vec2PackTexCoords(-1.0f, 1.0f);
				std::memcpy(verts->color, segment->color, sizeof(verts->color));
				verts->lightingCoord = 0;
				verts->texCoord = Vec2PackTexCoords(segment->uCoord + trailVert->texCoord[0], trailVert->texCoord[1]);
				verts->normal = Vec3PackUnitVec(normal);
				verts->tangentBinormalSign = tangent;
				++verts;
			}

			return verts;
		}

		r_double_index_t* FX_GenTrail_IndicesForSegment(const FxTrailDef* trailDef, const unsigned short vertIndexOffset, r_double_index_t* indices)
		{
			const auto nextVertIndexOffset = static_cast<unsigned short>(vertIndexOffset + trailDef->vertCount);
			for (auto i = 0; i < trailDef->indCount / 2; i++)
			{
				const auto a = trailDef->inds[2 * i];
				const auto b = trailDef->inds[2 * i + 1];
				indices[0] = { static_cast<unsigned short>(vertIndexOffset + a), static_cast<unsigned short>(vertIndexOffset + b) };
				indices[1] = { static_cast<unsigned short>(nextVertIndexOffset + a), static_cast<unsigned short>(nextVertIndexOffset + b) };
				indices[2] = { static_cast<unsigned short>(nextVertIndexOffset + a), static_cast<unsigned short>(vertIndexOffset + b) };
				indices += 3;
			}
			return indices;
		}

		void FX_LerpTrailSegment(const FxTrailSegmentDrawState* from, const FxTrailSegmentDrawState* to, const float frac, FxTrailSegmentDrawState* out)
		{
			out->uCoord = (to->uCoord - from->uCoord) * frac + from->uCoord;
			for (auto i = 0; i < 3; i++)
			{
				out->posWorld[i] = (to->posWorld[i] - from->posWorld[i]) * frac + from->posWorld[i];
				out->basis[0][i] = (to->basis[0][i] - from->basis[0][i]) * frac + from->basis[0][i];
				out->basis[1][i] = (to->basis[1][i] - from->basis[1][i]) * frac + from->basis[1][i];
			}
		}

		void FX_OrientTrailSegmentByVelocity(FxTrailSegmentDrawState* segment, const FxTrailSegmentDrawState* prevSegment)
		{
			float dir[3]{};
			for (auto i = 0; i < 3; i++)
			{
				dir[i] = segment->posWorld[i] - prevSegment->posWorld[i];
			}

			if (Vec3Normalize(dir) == 0.0f)
			{
				return;
			}

			float basis0[3] = { dir[1], -dir[0], 0.0f };
			if (Vec3Normalize(basis0) == 0.0f)
			{
				return;
			}

			std::memcpy(segment->basis[0], basis0, sizeof(basis0));
			Vec3Cross(basis0, dir, segment->basis[1]);
			Vec3Normalize(segment->basis[1]);
		}

		Material* FX_GetElemMaterial(const FxElemDef* elemDef, const int randomSeed)
		{
			if (elemDef->visualCount == 1)
			{
				return elemDef->visuals.instance.material;
			}
			return elemDef->visuals.array[(elemDef->visualCount * FX_RandomWord(randomSeed + 21)) >> 16].material;
		}

		void FX_DrawTrail(FxDrawState* draw, const FxTrail* trail)
		{
			const auto* system = draw->system;
			const auto* elemDef = &draw->effect->def->elemDefs[trail->defIndex];
			draw->elemDef = elemDef;
			if (!elemDef->visualCount)
			{
				return;
			}

			const FxTrailElem* trailElems[FX_TRAIL_ELEM_DRAW_LIMIT];
			auto trailElemCount = 0;
			auto drawCount = 0;
			for (auto handle = trail->firstTrailElemHandle; handle != -1 && trailElemCount < FX_TRAIL_ELEM_DRAW_LIMIT;)
			{
				const auto* trailElem = FX_TrailElemFromHandle(system, handle);
				trailElems[trailElemCount++] = trailElem;
				if (trailElem->msecBegin <= draw->msecDraw)
				{
					++drawCount;
				}
				handle = trailElem->nextTrailElemHandle;
			}

			if (drawCount < 2)
			{
				return;
			}

			const auto* trailDef = elemDef->extended.trailDef;
			const auto indexCountPerSegment = 3 * trailDef->indCount;
			GfxCodeSurfBuffers buffers{};
			if (!R_ReserveCodeSurfBuffers(&buffers, draw->codeSurfGlob, drawCount * trailDef->vertCount, (drawCount - 1) * indexCountPerSegment, 0))
			{
				return;
			}

			auto* verts = &draw->codeSurfGlob->verts[buffers.vertIndexBase + buffers.vertIndexOffset];
			auto segmentCount = 0;

			auto firstIndex = 0;
			while (firstIndex < trailElemCount && trailElems[firstIndex]->msecBegin > draw->msecDraw)
			{
				++firstIndex;
			}
			const auto textureOffset = FX_GetTrailTextureOffset(draw, trailElems[firstIndex]->spawnDist);

			FxTrailSegmentDrawState lastSegment{};
			auto lastNormTime = 1.0f;
			for (auto i = firstIndex; i < trailElemCount && segmentCount < drawCount; i++)
			{
				const auto* trailElem = trailElems[i];
				if (trailElem->msecBegin > draw->msecDraw)
				{
					continue;
				}

				draw->randomSeed = static_cast<int>(static_cast<unsigned int>(elemDef->randomSeed + draw->effect->randomSeed + 10111 * trailElem->sequence) % FX_RANDOM_SEED_MODULUS);

				float normTime{};
				FX_DrawElement_Setup_Time(draw, trailElem->msecBegin, &normTime);
				FX_DrawElement_Setup_VisualState(draw);
				FX_EvaluateVisualState(&draw->preVisState, draw->msecLifeSpan, &draw->visState);
				FX_EvaluateVisualState_DoLighting(draw->visState.color, elemDef);

				FxTrailSegmentDrawState segment{};
				segment.trailDef = trailDef;
				std::memcpy(segment.posWorld, trailElem->origin, sizeof(segment.posWorld));
				for (auto j = 0; j < 2; j++)
				{
					for (auto k = 0; k < 3; k++)
					{
						segment.basis[j][k] = static_cast<float>(trailElem->basis[j][k]) * 0.0078740157f;
					}
				}
				segment.rotation = draw->visState.rotationTotal;
				segment.size[0] = draw->visState.size[0];
				segment.size[1] = draw->visState.size[1];
				segment.uCoord = trailElem->spawnDist / static_cast<float>(trailDef->repeatDist) + textureOffset;
				std::memcpy(segment.color, draw->visState.color, sizeof(segment.color));

				if ((elemDef->flags & FX_ELEM_TRAIL_ORIENT_BY_VELOCITY) != 0 && i)
				{
					FX_OrientTrailSegmentByVelocity(&segment, &lastSegment);
				}

				if (normTime < 1.0f)
				{
					if (!i)
					{
						if (!trailElem->sequence)
						{
							segment.color[3] = 0.0f;
						}
					}
					else if (lastNormTime >= 1.0f)
					{
						auto tail = lastSegment.trailDef ? lastSegment : segment;
						FX_LerpTrailSegment(&lastSegment, &segment, (1.0f - lastNormTime) / (normTime - lastNormTime), &tail);
						verts = FX_GenTrail_VertsForSegment(&tail, verts);
						if (++segmentCount == drawCount)
						{
							break;
						}
					}

					verts = FX_GenTrail_VertsForSegment(&segment, verts);
					++segmentCount;
				}

				lastNormTime = normTime;
				lastSegment = segment;
			}

			if (segmentCount < 2)
			{
				return;
			}

			auto* indices = buffers.indices;
			auto vertIndexOffset = static_cast<unsigned short>(buffers.vertIndexOffset);
			for (auto i = 0; i < segmentCount - 1; i++)
			{
				indices = FX_GenTrail_IndicesForSegment(trailDef, vertIndexOffset, indices);
				vertIndexOffset = static_cast<unsigned short>(vertIndexOffset + trailDef->vertCount);
			}

			FX_SpriteDeferFlush(draw->codeSurfGlob, draw->effect->def->name, &draw->sprite, FX_GetElemMaterial(elemDef, draw->randomSeed),
				buffers.vertIndexBase, buffers.indices, draw->reactiveTurbulenceGroup, 0, 0);
			draw->sprite.indexCount += (segmentCount - 1) * indexCountPerSegment;
		}

		void FX_DrawSpriteEffect(FxSystem* system, const FxEffect* effect, const int drawTime, FxDrawState* draw)
		{
			draw->effect = effect;
			draw->msecDraw = drawTime;
			draw->reactiveTurbulenceGroup = -1;

			if (effect->firstElemHandle[FX_ELEM_CLASS_SPRITE] != -1)
			{
				FX_EvaluateOcclusionFade(draw);
				FX_DrawElements(system, effect, FX_ELEM_CLASS_SPRITE, draw);
			}

			for (auto trailHandle = effect->firstTrailHandle; trailHandle != -1;)
			{
				const auto* trail = FX_TrailFromHandle(system, trailHandle);
				FX_DrawTrail(draw, trail);
				trailHandle = trail->nextTrailHandle;
			}

			if (draw->reactiveTurbulenceGroup >= 0 && draw->sprite.indexCount)
			{
				FX_SpriteDirectFlush(draw->codeSurfGlob, &draw->sprite, draw->reactiveTurbulenceGroup);
			}
		}

		void FX_DrawNonSpriteEffect(const FxSystem* system, const FxEffect* effect, const unsigned int elemClass, const int drawTime)
		{
			if ((effect->status & FX_STATUS_HIDDEN) != 0 || effect->firstElemHandle[elemClass] == -1)
			{
				return;
			}

			FxDrawState draw{};
			draw.system = system;
			draw.effect = effect;
			draw.msecDraw = drawTime;
			FX_DrawElement_Setup_Camera(&draw, nullptr);
			FX_DrawElements(system, effect, elemClass, &draw);
		}
	}

	namespace
	{
		struct FxQuadParams
		{
			const float* origin;
			float rotation;
			float size[2];
			const float* color;
			const float* tangent;
			const float* bitangent;
			const float* normal;
		};

		// matches FX_QuadGenQuad
		void FX_WriteQuad(FxDrawState* draw, const unsigned int quadIndex, const FxQuadParams& params)
		{
			auto* codeSurfGlob = draw->codeSurfGlob;
			const auto* elemDef = draw->elemDef;

			auto* quad = R_CodeSurfQuadIter_Begin(codeSurfGlob, quadIndex);
			std::memset(quad, 0, sizeof(*quad));
			std::memcpy(quad->origin, params.origin, sizeof(quad->origin));
			quad->rotation = params.rotation;
			std::memcpy(quad->normal, params.normal, sizeof(quad->normal));
			quad->intensity = 1.0f;
			std::memcpy(quad->tangent, params.tangent, sizeof(quad->tangent));
			std::memcpy(quad->bitangent, params.bitangent, sizeof(quad->bitangent));
			std::memcpy(quad->color, params.color, sizeof(quad->color));
			quad->size[0] = params.size[0];
			quad->size[1] = params.size[1];

			FX_GetSpriteTexCoordsAtlasBlend(draw, &quad->texCoordOrigin[0], &quad->texCoordSize[0], &quad->texCoordOrigin[1], &quad->texCoordSize[1],
				&quad->nextTexCoordOrigin[0], &quad->nextTexCoordOrigin[1], &quad->atlasBlend);

			const auto* gpuLighting = *fx_gpu_lighting.get();
			if ((elemDef->flags2 & FX_ELEM_FLAG2_GPU_LIGHTING) != 0 && draw->camera->lightmap && gpuLighting && gpuLighting->current.enabled && R_UseBakedLighting())
			{
				// light from a single sample, lighting the whole area breaks on large sprites that are mostly inside the floor or walls
				auto lightingOrigin = _mm_setr_ps(params.origin[0], params.origin[1], params.origin[2], 0.0f);
				auto lightingSizeX = _mm_set1_ps(1.0f);
				auto lightingSizeY = _mm_set1_ps(1.0f);
				auto lightingRotation = _mm_set1_ps(params.rotation);
				const auto tangent = _mm_setr_ps(params.tangent[0], params.tangent[1], params.tangent[2], 0.0f);
				const auto bitangent = _mm_setr_ps(params.bitangent[0], params.bitangent[1], params.bitangent[2], 0.0f);
				FX_SetupLighting(codeSurfGlob, draw->camera, &lightingOrigin, &tangent, &bitangent, &lightingSizeX, &lightingSizeY, &lightingRotation,
					(elemDef->flags2 & FX_ELEM_FLAG2_GPU_LIGHTING_TYPE) != 0, &quad->lightingCoord[0], &quad->lightingCoord[1], &quad->lightingScale);
			}
		}

		void FX_WriteQuadIndices(r_double_index_t* indices, const unsigned short offset)
		{
			indices[0] = { offset, static_cast<unsigned short>(offset + 1) };
			indices[1] = { static_cast<unsigned short>(offset + 2), static_cast<unsigned short>(offset + 2) };
			indices[2] = { static_cast<unsigned short>(offset + 3), offset };
		}
	}

	// iw7's effect sprite shaders expand quads from one record per sprite, FXV_GenSpriteVerts still writes corner vertices
	char FX_GenSpriteQuad(FxDrawState* draw, const __m128* tangent, const __m128* bitangent, const __m128* normal)
	{
		auto* codeSurfGlob = draw->codeSurfGlob;
		GfxCodeSurfBuffers buffers{};
		if (!R_ReserveCodeSurfQuads(&buffers, codeSurfGlob, 1, 0))
		{
			return false;
		}

		FX_SpriteDeferFlush(codeSurfGlob, draw->effect->def->name, &draw->sprite, FX_GetElemMaterial(draw->elemDef, draw->randomSeed),
			buffers.vertIndexBase, buffers.indices, draw->reactiveTurbulenceGroup, 0, 0);
		draw->sprite.indexCount += 6;
		FX_WriteQuadIndices(buffers.indices, buffers.vertIndexOffset);

		FxQuadParams params{};
		params.origin = draw->posWorld;
		params.rotation = draw->visState.rotationTotal;
		params.size[0] = draw->visState.size[0];
		params.size[1] = draw->visState.size[1];
		params.color = draw->visState.color;
		params.tangent = reinterpret_cast<const float*>(tangent);
		params.bitangent = reinterpret_cast<const float*>(bitangent);
		params.normal = reinterpret_cast<const float*>(normal);
		FX_WriteQuad(draw, (buffers.vertIndexBase + buffers.vertIndexOffset) >> 2, params);

		return true;
	}

	void FX_GenParticleSimAnimationQuads(FxDrawState* draw, const float* left, const float* up, const __m128i* normal)
	{
		const auto* elemDef = draw->elemDef;
		const auto* anim = elemDef->visualCount == 1
			? elemDef->visuals.instance.particleSimAnimation
			: elemDef->visuals.array[(elemDef->visualCount * FX_RandomWord(draw->randomSeed + 21)) >> 16].particleSimAnimation;

		auto time = draw->msecElapsed * 0.001f;
		if (time >= anim->header.duration)
		{
			return;
		}

		time *= anim->header.playbackRate;
		const auto frameTime = std::floor(time);
		const auto frameLerp = time - frameTime;
		const auto& frame = anim->frames[static_cast<int>(frameTime)];
		const auto particleCount = frame.numActiveParticles;
		if (!particleCount)
		{
			return;
		}

		auto* codeSurfGlob = draw->codeSurfGlob;
		if (draw->sprite.indexCount)
		{
			FX_SpriteDirectFlush(codeSurfGlob, &draw->sprite, draw->reactiveTurbulenceGroup);
		}

		GfxCodeSurfBuffers buffers{};
		if (!R_ReserveCodeSurfQuads(&buffers, codeSurfGlob, particleCount, 0))
		{
			return;
		}

		// iw7 turns sorting off for huge frames
		const auto sort = anim->header.sortParticlesAtRuntime && particleCount <= 0x4080;
		thread_local std::vector<std::pair<float, unsigned short>> sorted;
		sorted.clear();

		const auto* elem = draw->elem;
		auto* indices = buffers.indices;
		auto offset = buffers.vertIndexOffset;
		for (auto i = 0u; i < particleCount; i++, offset = static_cast<unsigned short>(offset + 4))
		{
			const auto* particle = &anim->particleData[frame.particleDataOffset + i];

			float sample[13]{};
			FX_ParticleSimAnimation_EvaluateParticle(anim, particle, frameLerp, sample);

			const float local[3] = { sample[0] + elem->origin[0], sample[1] + elem->origin[1], sample[2] + elem->origin[2] };
			float origin[3]{};
			FX_OrientationPosToWorld(&draw->orient, local, origin);

			draw->normTimeUpdateEnd = sample[12];
			draw->randomSeed = static_cast<int>((elemDef->randomSeed + draw->effect->randomSeed + 10111u * particle->particleID) % FX_RANDOM_SEED_MODULUS);
			if (anim->header.evalVisStatePerParticle)
			{
				FX_SetupVisualState(elemDef, draw->effect, draw->randomSeed, sample[12], &draw->preVisState);
				FX_EvaluateSize(&draw->preVisState, &draw->visState);
				FX_EvaluateVisualState(&draw->preVisState, draw->msecLifeSpan, &draw->visState);
				FX_EvaluateVisualState_DoLighting(draw->visState.color, elemDef);
			}

			float color[4]{};
			for (auto c = 0; c < 4; c++)
			{
				color[c] = sample[8 + c] * draw->visState.color[c];
			}

			FxQuadParams params{};
			params.origin = origin;
			params.rotation = sample[7] + draw->visState.rotationTotal;
			params.size[0] = sample[3] * draw->visState.size[0];
			params.size[1] = sample[4] * draw->visState.size[1];
			params.color = color;
			params.tangent = left;
			params.bitangent = up;
			params.normal = reinterpret_cast<const float*>(normal);
			FX_WriteQuad(draw, (buffers.vertIndexBase + offset) >> 2, params);

			if (sort)
			{
				const auto* cameraOrigin = draw->camera->origin;
				const float delta[3] = { cameraOrigin[0] - origin[0], cameraOrigin[1] - origin[1], cameraOrigin[2] - origin[2] };
				sorted.emplace_back(Vec3Dot(delta, delta), offset);
			}
			else
			{
				FX_WriteQuadIndices(indices, offset);
				indices += 3;
			}
		}

		if (sort)
		{
			std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b)
			{
				return a.first > b.first;
			});

			for (const auto& [distance, quadOffset] : sorted)
			{
				FX_WriteQuadIndices(indices, quadOffset);
				indices += 3;
			}
		}

		GfxCodeSurfArgs args{};
		args.material = anim->material;
		args.vertIndexBase = buffers.vertIndexBase;
		args.indices = buffers.indices;
		args.indexCount = 6 * particleCount;
		args.fxName = "ParticleSimAnimation";
		args.reactiveTurbulenceGroup = -1;
		args.unk = static_cast<unsigned int>(draw->unk168) << 17;
		R_AddCodeSurf(codeSurfGlob, &args);
	}

	void FX_DrawElem_Light(FxDrawState* draw)
	{
		// iw7 stubbed out adding the light to the scene
		if ((draw->system->systemFlags & 0x82) != 0)
		{
			return;
		}

		FX_DrawElement_Setup_Time(draw, draw->elem->msecBegin, nullptr);
		FX_DrawElement_Setup_Position(draw, draw->elem->origin);
		FX_DrawElement_Setup_VisualState(draw);
		if (FX_CullElementForDraw_Light(draw))
		{
			return;
		}

		FX_EvaluateVisualState(&draw->preVisState, draw->msecLifeSpan, &draw->visState);
		FX_EvaluateScale(&draw->preVisState, &draw->visState);

		ParticleModuleInitLightOmni module{};
		module.type = PARTICLE_MODULE_INIT_LIGHT_OMNI;
		module.m_disableVolumetric = false;
		module.m_exponent = 0;
		module.m_intensityIR = 0.0f;
		module.m_tonemappingScaleFactor = 1.0f;

		R_AddOmniLightToScene(draw->posWorld, draw->visState.size[0],
			draw->visState.scale * draw->visState.color[0],
			draw->visState.scale * draw->visState.color[1],
			draw->visState.scale * draw->visState.color[2],
			1.0f, &module, nullptr);
	}

	void FX_DrawNonSpriteElems(FxSystem* system, FxPass5Cmd* passCmd)
	{
		FX_WaitEnterReadSystemLock(system->lock);

		for (auto i = _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(passCmd->effectIter), 1); i < system->firstNewEffect;
			i = _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(passCmd->effectIter), 1))
		{
			const auto* effect = FX_EffectFromHandle(system, system->allEffectHandles[i & FX_EFFECT_HANDLE_MASK]);
			FX_DrawNonSpriteEffect(system, effect, FX_ELEM_CLASS_NONSPRITE, system->msecDraw);
		}

		// spot lights aren't linked into their effects
		if ((system->systemFlags & 0x4) == 0)
		{
			for (auto i = 0; i < system->spotLightInfo.index; i++)
			{
				const auto* spotLight = &system->spotLightInfo.arr[i];
				const auto* effect = FX_EffectFromHandle(system, spotLight->effectHandle);
				if ((effect->status & FX_STATUS_HIDDEN) != 0)
				{
					continue;
				}

				FxDrawState draw{};
				draw.system = system;
				draw.effect = effect;
				draw.msecDraw = system->msecDraw;
				FX_DrawElement_Setup_Camera(&draw, nullptr);

				const auto* elem = FX_ElemFromHandle(system, spotLight->elemHandle);
				FX_DrawElement(&effect->def->elemDefs[elem->defIndex], elem, spotLight->elemHandle, &draw);
			}
		}

		FX_ExitReadSystemLock(system->lock);
	}

	void FX_DrawSpriteElems(GfxCodeSurfGlob* codeSurfGlob, FxSystem* system, const int drawTime, const FxCamera* camera, const int surfList)
	{
		FxDrawState draw{};
		if (camera->lightmap)
		{
			R_BeginFlareSurfs(&draw.flareSurfGlob);
			draw.flaresEnabled = true;
		}

		system->gfxCloudCount = 0;
		draw.system = system;
		FX_DrawElement_Setup_Camera(&draw, camera);
		draw.surfList = surfList;
		draw.codeSurfGlob = codeSurfGlob;
		draw.reactiveTurbulenceGroup = -1;

		FX_DrawModularParticles(particleManagers.get() + PARTICLE_MANAGER_SIZE * static_cast<size_t>(system->localClientNum), &draw);

		// legacy sprites must not continue the particle system batch
		if (draw.sprite.indexCount)
		{
			FX_SpriteDirectFlush(codeSurfGlob, &draw.sprite, -1);
		}
		draw.sprite = {};
		draw.reactiveTurbulenceGroup = -1;

		for (auto i = system->firstActiveEffect; i != system->firstNewEffect; i++)
		{
			const auto* effect = FX_EffectFromHandle(system, system->allEffectHandles[i & FX_EFFECT_HANDLE_MASK]);
			if ((effect->status & FX_STATUS_HIDDEN) != 0)
			{
				continue;
			}

			FX_DrawSpriteEffect(system, effect, drawTime, &draw);
			FX_DrawNonSpriteEffect(system, effect, FX_ELEM_CLASS_CLOUD, drawTime);
		}

		if (draw.sprite.indexCount)
		{
			FX_SpriteDirectFlush(codeSurfGlob, &draw.sprite, -1);
		}

		draw.unk164 = false;
		if (camera->lightmap)
		{
			R_EndFlareSurfs(&draw.flareSurfGlob);
		}
	}
}
