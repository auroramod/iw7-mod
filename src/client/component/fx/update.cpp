#include <std_include.hpp>

#include "fx.hpp"

namespace fx
{
	using namespace engine;

	namespace
	{
		constexpr auto MSEC_INFINITE = 0x7FFFFFFF;
		constexpr auto FX_COLLISION_STEP_MSEC = 50;
		constexpr auto FX_BRUSHMASK = 0x811;
		constexpr auto FX_BRUSHMASK_ITEM_CLIP = 0xC11;
		constexpr auto FX_ON_GROUND_NORMAL_Z = 0.69999999f;

		void FX_UpdateEffectPartial(FxSystem* system, int isDeferred, FxEffect* effect, unsigned int effectHandle, int msecUpdateBegin,
			int msecUpdateEnd, float distanceTravelledBegin, float distanceTravelledEnd, float arcDistanceTravelled,
			const unsigned int* elemHandleStart, const unsigned int* elemHandleStop, const unsigned int* trailElemStart,
			const unsigned int* trailElemStop);

		void FX_AddRefToEffect(FxSystem* system, FxEffect* effect)
		{
			FXW_PutEffectStatus(system, effect, FXW_GetEffectStatus(system, effect) + 1);
		}

		int FX_GetElemDefCount(const FxEffectDef* def)
		{
			return def->elemDefCountLooping + def->elemDefCountOneShot + def->elemDefCountEmission;
		}

		int FX_LimitStabilizeTimeForElemDef(const FxElemDef* elemDef, bool needToSpawnSystem, int originalUpdateTime);

		int FX_LimitStabilizeTimeForEffectDef(const FxEffectDef* def, const int originalUpdateTime)
		{
			auto msecStabilize = 0;
			const auto elemDefCount = FX_GetElemDefCount(def);
			for (auto i = 0; i < elemDefCount; i++)
			{
				msecStabilize = std::max(msecStabilize, FX_LimitStabilizeTimeForElemDef(&def->elemDefs[i], true, originalUpdateTime));
				if (msecStabilize >= originalUpdateTime)
				{
					return originalUpdateTime;
				}
			}
			return msecStabilize;
		}

		int FX_LimitStabilizeTimeForElemDef(const FxElemDef* elemDef, const bool needToSpawnSystem, const int originalUpdateTime)
		{
			auto msecStabilize = MSEC_INFINITE;
			if (elemDef->elemType != FX_ELEM_TYPE_TRAIL)
			{
				msecStabilize = elemDef->spawnDelayMsec.base + elemDef->spawnDelayMsec.amplitude + elemDef->lifeSpanMsec.base + elemDef->lifeSpanMsec.amplitude;
				if (needToSpawnSystem && elemDef->spawn.looping.count > 0)
				{
					if (elemDef->spawn.looping.count == MSEC_INFINITE)
					{
						msecStabilize = MSEC_INFINITE;
					}
					else
					{
						msecStabilize += elemDef->spawn.looping.intervalMsec * (elemDef->spawn.looping.count - 1);
					}
				}
			}

			if (msecStabilize >= originalUpdateTime)
			{
				return originalUpdateTime;
			}

			auto msecResult = msecStabilize;
			if (elemDef->elemType == FX_ELEM_TYPE_RUNNER)
			{
				for (auto i = 0; i < elemDef->visualCount; i++)
				{
					const auto* runnerDef = elemDef->visualCount == 1
						? elemDef->visuals.instance.effectDef.handle
						: elemDef->visuals.array[i].effectDef.handle;
					msecResult = std::max(msecResult, FX_LimitStabilizeTimeForEffectDef(runnerDef, originalUpdateTime));
					if (msecResult >= originalUpdateTime)
					{
						return originalUpdateTime;
					}
				}
			}

			const FxEffectDef* spawnedDefs[] = { elemDef->effectOnDeath.handle, elemDef->effectOnImpact.handle, elemDef->effectEmitted.handle };
			for (const auto* spawnedDef : spawnedDefs)
			{
				if (!spawnedDef)
				{
					continue;
				}

				msecResult = std::max(msecResult, FX_LimitStabilizeTimeForEffectDef(spawnedDef, originalUpdateTime) + msecStabilize);
				if (msecResult >= originalUpdateTime)
				{
					return originalUpdateTime;
				}
			}

			return msecResult;
		}

		void FX_SpawnLoopingElems(FxSystem* system, const int isDeferred, FxEffect* effect, const unsigned int effectHandle,
			const int elemDefIndex, const FxSpatialFrame* frameBegin, const FxSpatialFrame* frameEnd, const int msecWhenPlayed,
			const int msecUpdateBegin, const int msecUpdateEnd)
		{
			const auto* elemDef = &effect->def->elemDefs[elemDefIndex];
			if (elemDef->elemType == FX_ELEM_TYPE_TRAIL)
			{
				return;
			}

			auto msecSpawnBegin = msecUpdateBegin;
			if (msecUpdateEnd != MSEC_INFINITE)
			{
				const auto msecUpdateDuration = msecUpdateEnd - msecUpdateBegin;
				if (msecUpdateDuration > 128)
				{
					const auto msecStabilize = FX_LimitStabilizeTimeForElemDef(elemDef, false, msecUpdateDuration) + 1;
					if (msecUpdateDuration > msecStabilize)
					{
						msecSpawnBegin = msecUpdateEnd - msecStabilize;
					}
				}
			}

			const auto intervalMsec = elemDef->spawn.looping.intervalMsec;
			auto sequence = (msecSpawnBegin - msecWhenPlayed) / intervalMsec + 1;
			for (auto msecSpawn = msecWhenPlayed + sequence * intervalMsec; msecSpawn <= msecUpdateEnd;
				msecSpawn += effect->def->elemDefs[elemDefIndex].spawn.looping.intervalMsec)
			{
				if (sequence >= effect->def->elemDefs[elemDefIndex].spawn.looping.count)
				{
					break;
				}

				FxSpatialFrame frameWhenPlayed{};
				const auto frac = static_cast<float>(msecSpawn - msecSpawnBegin) / static_cast<float>(msecUpdateEnd - msecSpawnBegin);
				FX_LerpSpatialFrame(frameBegin, frameEnd, frac, &frameWhenPlayed);
				FXW_SpawnElem(system, isDeferred, effect, effectHandle, elemDefIndex, &frameWhenPlayed, msecSpawn, sequence++);
			}
		}

		void FX_ProcessLooping(FxSystem* system, const int isDeferred, FxEffect* effect, const unsigned int effectHandle,
			const int elemDefFirst, const int elemDefCount, const FxSpatialFrame* frameBegin, const FxSpatialFrame* frameEnd,
			const int msecWhenPlayed, const int msecUpdateBegin, const int msecUpdateEnd, const float distanceTravelledBegin,
			const float distanceTravelledEnd, const float arcDistanceTravelled)
		{
			const auto elemDefStop = elemDefFirst + elemDefCount;
			for (auto elemDefIndex = elemDefFirst; elemDefIndex != elemDefStop; elemDefIndex++)
			{
				FX_SpawnLoopingElems(system, isDeferred, effect, effectHandle, elemDefIndex, frameBegin, frameEnd, msecWhenPlayed, msecUpdateBegin, msecUpdateEnd);
			}

			for (auto trailHandle = effect->firstTrailHandle; trailHandle != -1;)
			{
				auto* trail = FX_TrailFromHandle(system, trailHandle);
				if (trail->defIndex >= elemDefFirst && trail->defIndex < elemDefStop)
				{
					FXW_SpawnTrailLoopingElems(system, effect, effectHandle, trail, frameBegin, frameEnd, msecWhenPlayed, msecUpdateBegin,
						msecUpdateEnd, distanceTravelledBegin, distanceTravelledEnd, arcDistanceTravelled);
				}
				trailHandle = trail->nextTrailHandle;
			}
		}

		void FX_EffectStopLooping(FxSystem* system, FxEffect* effect)
		{
			if ((effect->status & FX_STATUS_IS_LOOPING) != 0)
			{
				effect->status &= ~FX_STATUS_IS_LOOPING;
				FXW_DelRefToEffect(system, effect);
			}
		}

		void FX_SpawnAllFutureLooping(FxSystem* system, const int isDeferred, FxEffect* effect, const unsigned int effectHandle)
		{
			const auto msecBegin = effect->msecBegin;
			const auto msecLastUpdate = effect->msecLastUpdate;
			const auto elemDefCountLooping = effect->def->elemDefCountLooping;
			for (auto elemDefIndex = 0; elemDefIndex < elemDefCountLooping; elemDefIndex++)
			{
				const auto* elemDef = &effect->def->elemDefs[elemDefIndex];
				if (elemDef->spawn.looping.count == MSEC_INFINITE || elemDef->elemType == FX_ELEM_TYPE_TRAIL)
				{
					continue;
				}

				auto sequence = (msecLastUpdate - msecBegin) / elemDef->spawn.looping.intervalMsec + 1;
				auto msecWhenPlayed = msecBegin + sequence * elemDef->spawn.looping.intervalMsec;
				while (sequence < effect->def->elemDefs[elemDefIndex].spawn.looping.count)
				{
					FxSpatialFrame frameWhenPlayed{};
					const auto frac = static_cast<float>(msecWhenPlayed - msecLastUpdate) / static_cast<float>(MSEC_INFINITE - msecLastUpdate);
					FX_LerpSpatialFrame(&effect->framePrev, &effect->frameNow, frac, &frameWhenPlayed);
					FXW_SpawnElem(system, isDeferred, effect, effectHandle, elemDefIndex, &frameWhenPlayed, msecWhenPlayed, sequence++);
					msecWhenPlayed += effect->def->elemDefs[elemDefIndex].spawn.looping.intervalMsec;
				}
			}
		}

		void FX_ForcePushElemUpdate(FxSystem* system, const unsigned int effectHandle, const unsigned int elemHandle)
		{
			auto* update = &system->updateElement[system->numUpdateElement++];
			update->effectHandle = effectHandle;
			update->elemHandle = elemHandle;
		}

		float FX_GetArcDistanceTravelledThisFrame(const FxEffect* effect)
		{
			const auto dot = effect->framePrev.quat[0] * effect->frameNow.quat[0] + effect->framePrev.quat[1] * effect->frameNow.quat[1]
				+ effect->framePrev.quat[2] * effect->frameNow.quat[2] + effect->framePrev.quat[3] * effect->frameNow.quat[3];
			const auto angle = std::acos(std::clamp(dot, -1.0f, 1.0f)) * 2.0f;
			constexpr auto two_pi = 6.2831855f;
			return (two_pi - angle) < angle ? two_pi - angle : angle;
		}

		void FX_GetQuatForOrientation(const FxEffect* effect, const FxElemDef* elemDef, const FxSpatialFrame* frameNow,
			const orientation_t* orient, float* quat, const FxCamera* camera)
		{
			if ((effect->flags & FX_EFFECT_FLAG_BOLTED_TO_ENTITY) != 0)
			{
				std::memcpy(quat, frameNow->quat, sizeof(float[4]));
				return;
			}

			switch (elemDef->flags & FX_ELEM_RUN_MASK)
			{
			case FX_ELEM_RUN_RELATIVE_TO_CAMERA:
				AxisToQuat(camera->axis, quat);
				break;
			case FX_ELEM_RUN_RELATIVE_TO_WORLD:
				quat[0] = 0.0f;
				quat[1] = 0.0f;
				quat[2] = 0.0f;
				quat[3] = 1.0f;
				break;
			case FX_ELEM_RUN_RELATIVE_TO_SPAWN:
				std::memcpy(quat, effect->frameAtSpawn.quat, sizeof(float[4]));
				break;
			case FX_ELEM_RUN_RELATIVE_TO_EFFECT:
				std::memcpy(quat, frameNow->quat, sizeof(float[4]));
				break;
			default:
				AxisToQuat(orient->axis, quat);
				break;
			}
		}

		void FX_IntegrateVelocityInSegment(const int elemDefFlags, const orientation_t* orient, const FxElemVelStateSample* velState,
			const float t0, const float t1, const float* amplitudeScale, const float integralScale, float* posLocal, float* posWorld)
		{
			const auto weight1 = (t1 * t1 - t0 * t0) * (integralScale * 0.5f);
			const auto weight0 = (t1 - t0) * integralScale - weight1;

			if ((elemDefFlags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0)
			{
				for (auto i = 0; i < 3; i++)
				{
					posLocal[i] += (velState[0].local.velocity.amplitude[i] * amplitudeScale[i] + velState[0].local.velocity.base[i]) * weight0;
					posLocal[i] += (velState[1].local.velocity.amplitude[i] * amplitudeScale[i] + velState[1].local.velocity.base[i]) * weight1;
				}
			}

			FX_OrientationPosToWorld(orient, posLocal, posWorld);

			if ((elemDefFlags & FX_ELEM_HAS_VELOCITY_GRAPH_WORLD) != 0)
			{
				for (auto i = 0; i < 3; i++)
				{
					posWorld[i] += (velState[0].world.velocity.amplitude[i] * amplitudeScale[i] + velState[0].world.velocity.base[i]) * weight0;
					posWorld[i] += (velState[1].world.velocity.amplitude[i] * amplitudeScale[i] + velState[1].world.velocity.base[i]) * weight1;
				}
			}
		}

		void FX_IntegrateVelocityFromZeroInSegment(const FxElemVelStateInFrame* statePrev, const FxElemVelStateInFrame* stateNext,
			const float* weight, const float* amplitudeScale, const float integralScale, float* pos)
		{
			for (auto i = 0; i < 3; i++)
			{
				pos[i] += (statePrev->totalDelta.amplitude[i] * amplitudeScale[i] + statePrev->totalDelta.base[i]) * integralScale;
				pos[i] += (statePrev->velocity.amplitude[i] * amplitudeScale[i] + statePrev->velocity.base[i]) * weight[0];
				pos[i] += (stateNext->velocity.amplitude[i] * amplitudeScale[i] + stateNext->velocity.base[i]) * weight[1];
			}
		}

		void FX_IntegrateVelocityAcrossSegments(const int elemDefFlags, const orientation_t* orient, const FxElemVelStateSample* velState0,
			const FxElemVelStateSample* velState1, const float t0, const float t1, const float* amplitudeScale, const float integralScale,
			float* posLocal, float* posWorld)
		{
			const auto scaled0 = -(t0 * integralScale);
			float weight0[2]{};
			weight0[1] = t0 * 0.5f * scaled0;
			weight0[0] = scaled0 - weight0[1];

			const auto scaled1 = t1 * integralScale;
			float weight1[2]{};
			weight1[1] = t1 * 0.5f * scaled1;
			weight1[0] = scaled1 - weight1[1];

			if ((elemDefFlags & FX_ELEM_HAS_VELOCITY_GRAPH_LOCAL) != 0)
			{
				FX_IntegrateVelocityFromZeroInSegment(&velState1[0].local, &velState1[1].local, weight1, amplitudeScale, integralScale, posLocal);
				FX_IntegrateVelocityFromZeroInSegment(&velState0[0].local, &velState0[1].local, weight0, amplitudeScale, -integralScale, posLocal);
			}

			FX_OrientationPosToWorld(orient, posLocal, posWorld);

			if ((elemDefFlags & FX_ELEM_HAS_VELOCITY_GRAPH_WORLD) != 0)
			{
				FX_IntegrateVelocityFromZeroInSegment(&velState1[0].world, &velState1[1].world, weight1, amplitudeScale, integralScale, posWorld);
				FX_IntegrateVelocityFromZeroInSegment(&velState0[0].world, &velState0[1].world, weight0, amplitudeScale, -integralScale, posWorld);
			}
		}

		void FX_IntegrateVelocity(const FxUpdateElem* update, const float t0, const float t1, float* posLocal, float* posWorld)
		{
			const auto* elemDef = &update->effect->def->elemDefs[update->elemIndex];
			const auto integralScale = update->msecLifeSpan;
			const float amplitudeScale[3] =
			{
				FX_RandomFloat(update->randomSeed),
				FX_RandomFloat(update->randomSeed + 1),
				FX_RandomFloat(update->randomSeed + 2),
			};

			const auto* velSamples = elemDef->velSamples;
			const auto velIntervalCount = elemDef->velIntervalCount;
			if (velIntervalCount == 1)
			{
				FX_IntegrateVelocityInSegment(elemDef->flags, &update->orient, velSamples, t0, t1, amplitudeScale, integralScale, posLocal, posWorld);
				return;
			}

			const auto sample0 = static_cast<float>(velIntervalCount) * t0;
			const auto sample1 = static_cast<float>(velIntervalCount) * t1;
			const auto segment0 = static_cast<int>(std::floor(sample0));
			const auto segment1 = static_cast<int>(std::ceil(sample1)) - 1;
			const auto frac0 = sample0 - static_cast<float>(segment0);
			const auto frac1 = sample1 - static_cast<float>(segment1);

			if (segment0 == segment1)
			{
				FX_IntegrateVelocityInSegment(elemDef->flags, &update->orient, &velSamples[segment0], frac0, frac1, amplitudeScale, integralScale, posLocal, posWorld);
			}
			else
			{
				FX_IntegrateVelocityAcrossSegments(elemDef->flags, &update->orient, &velSamples[segment0], &velSamples[segment1],
					frac0, frac1, amplitudeScale, integralScale, posLocal, posWorld);
			}
		}

		void FX_NextElementPosition(FxUpdateElem* update, const int msecUpdateBegin, const int msecUpdateEnd)
		{
			float posLocal[3]{};
			std::memcpy(posLocal, update->elemOrigin, sizeof(posLocal));

			FX_IntegrateVelocity(update,
				static_cast<float>(msecUpdateBegin - update->msecElemBegin) * update->invMsecLifeSpan,
				static_cast<float>(msecUpdateEnd - update->msecElemBegin) * update->invMsecLifeSpan,
				posLocal, update->posWorld);

			auto* effect = update->effect;
			const auto* elemDef = &effect->def->elemDefs[update->elemIndex];
			const auto deltaTime = static_cast<float>(msecUpdateEnd - msecUpdateBegin) * 0.001f;
			const auto gravity = (FX_RandomFloat(update->randomSeed + 15) * elemDef->gravity.amplitude + elemDef->gravity.base) * 800.0f * deltaTime;

			auto* baseVel = update->elemBaseVel;
			auto* posWorld = update->posWorld;
			if ((elemDef->flags & FX_ELEM_USE_VECTORFIELDS) != 0 && effect->numVectorFields)
			{
				float sampledVel[3]{};
				if (CG_VectorField_SamplePosAgainstInstances(posWorld, sampledVel, effect->numVectorFields, effect->vectorFields, VECTOR_FIELD_TYPE_VELOCITY))
				{
					std::memcpy(baseVel, sampledVel, sizeof(sampledVel));
				}

				for (auto i = 0; i < 3; i++)
				{
					posWorld[i] += deltaTime * baseVel[i];
				}
				baseVel[2] -= gravity;

				float sampledForce[3]{};
				CG_VectorField_SamplePosAgainstInstances(posWorld, sampledForce, effect->numVectorFields, effect->vectorFields, VECTOR_FIELD_TYPE_FORCE);
				for (auto i = 0; i < 3; i++)
				{
					baseVel[i] += sampledForce[i];
				}
			}
			else
			{
				for (auto i = 0; i < 3; i++)
				{
					posWorld[i] += deltaTime * baseVel[i];
				}
				baseVel[2] -= gravity;
			}

			posWorld[2] -= gravity * deltaTime * 0.5f;
		}

		void FX_PlayEffectWithParent(FxSystem* system, const unsigned int owner, const FxEffectDef* def, const int msecBegin,
			const float* origin, const float(*axis)[3])
		{
			if ((system->systemFlags & FX_SYSTEM_FLAG_PAUSED) != 0)
			{
				return;
			}

			int spawnCmdIndex;
			do
			{
				spawnCmdIndex = *system->spawnCmdWrite;
				if (spawnCmdIndex < 0)
				{
					return;
				}
			} while (spawnCmdIndex != _InterlockedCompareExchange(reinterpret_cast<volatile long*>(system->spawnCmdWrite),
				system->spawnCmdList[spawnCmdIndex].nextCmd, spawnCmdIndex));

			auto* cmd = &system->spawnCmdList[spawnCmdIndex];
			cmd->info.effectFlags = 0;
			cmd->info.remoteDef = def;
			cmd->info.msecBegin = msecBegin;
			std::memcpy(cmd->info.frame.origin, origin, sizeof(float[3]));
			AxisToQuat(axis, cmd->info.frame.quat);
			cmd->info.dobjHandle = FX_DOBJ_HANDLE_NONE;
			cmd->info.boneIndex = FX_BONE_INDEX_NONE;
			cmd->info.runnerSortOrder = 255;
			cmd->info.owner = owner;
			cmd->info.markEntnum = FX_ENTITYNUM_NONE;
			cmd->info.markViewmodelClientIndex = 0;
			cmd->removeRef = 1;
			cmd->elemHandle = static_cast<unsigned int>(-1);
			cmd->elemParent = static_cast<unsigned int>(-1);
			cmd->elemClass = static_cast<unsigned int>(-1);
			cmd->entnum = -1;
			cmd->timeValue = -1;

			if (owner != -1)
			{
				FX_AddRefToEffect(system, FX_EffectFromHandle(system, owner));
			}

			int nextCmd;
			do
			{
				nextCmd = *system->spawnCmdRead;
				cmd->nextCmd = nextCmd;
			} while (nextCmd != _InterlockedCompareExchange(reinterpret_cast<volatile long*>(system->spawnCmdRead), spawnCmdIndex, nextCmd));
		}

		void FX_SpawnDeathEffect(FxSystem* system, const FxUpdateElem* update)
		{
			auto* effect = update->effect;
			const auto* elemDef = &effect->def->elemDefs[update->elemIndex];

			orientation_t orientPrev{};
			FX_GetOrientation(elemDef, effect, &effect->framePrev, update->randomSeed, &orientPrev, &system->camera);

			float origin[3]{};
			FX_OrientationPosToWorld(&orientPrev, update->elemOrigin, origin);
			FX_PlayEffectWithParent(system, effect->owner, elemDef->effectOnDeath.handle, update->msecUpdateBegin, origin, orientPrev.axis);
		}

		void FX_SpawnImpactEffect(FxSystem* system, const FxUpdateElem* update, const FxEffectDef* impactEffect, const int msecOnImpact,
			const float* impactNormal)
		{
			float axis[3][3]{};
			std::memcpy(axis[0], impactNormal, sizeof(float[3]));
			Vec3Basis_RightHanded(axis[0], axis[1], axis[2]);
			FX_PlayEffectWithParent(system, update->effect->owner, impactEffect, msecOnImpact, update->posWorld, axis);
		}

		// iw7's per-surface impact effects are usually particle systems, which can't be parented
		void FX_SpawnSurfaceTypeImpactEffect(FxSystem* system, const FxUpdateElem* update, const FxEffectDef* impactEffect,
			const int msecOnImpact, const trace_t* trace)
		{
			const FXRegisteredDef registered{ database::FX_COMBINED_FX, const_cast<FxEffectDef*>(impactEffect) };
			const auto surfaceType = (trace->surfaceFlags >> 20) & 0x1F;
			const auto* surfaceEffect = FX_GetSurfaceTypeImpactEffect(&registered, surfaceType);
			if (!surfaceEffect || !surfaceEffect->fx)
			{
				return;
			}

			if (surfaceEffect->type == database::FX_COMBINED_FX)
			{
				FX_SpawnImpactEffect(system, update, static_cast<const FxEffectDef*>(surfaceEffect->fx), msecOnImpact, trace->normal);
				return;
			}

			float axis[3][3]{};
			std::memcpy(axis[0], trace->normal, sizeof(float[3]));
			Vec3Basis_RightHanded(axis[0], axis[1], axis[2]);
			FX_PlayOrientedEffect(system->localClientNum, surfaceEffect, msecOnImpact, update->posWorld, axis);
		}

		int FX_ProcessEmitting(FxSystem* system, FxUpdateElem* update, const unsigned char emitResidual, const FxSpatialFrame* frameBegin,
			const FxSpatialFrame* frameEnd)
		{
			float axis[3][3]{};
			for (auto i = 0; i < 3; i++)
			{
				axis[0][i] = frameEnd->origin[i] - frameBegin->origin[i];
			}

			const auto distance = Vec3Normalize(axis[0]);
			if (distance == 0.0f)
			{
				return emitResidual;
			}

			const auto* elemDef = &update->effect->def->elemDefs[update->elemIndex];
			const auto orientByElem = (elemDef->flags & FX_ELEM_EMIT_ORIENT_BY_ELEM) != 0;
			if (!orientByElem)
			{
				PerpendicularVector(axis[0], axis[1]);
				Vec3Cross(axis[0], axis[1], axis[2]);
			}

			const auto emitDist = FX_RandomFloat(update->randomSeed + 20) * elemDef->emitDist.amplitude + elemDef->emitDist.base + elemDef->emitDistVariance.base;
			const auto emitDistMax = emitDist + elemDef->emitDistVariance.amplitude;
			const auto get_emit_step = [&]
			{
				return static_cast<float>(std::rand()) * 0.000030517578f * update->effect->def->elemDefs[update->elemIndex].emitDistVariance.amplitude + emitDist;
			};

			auto distLastEmit = -(static_cast<float>(emitResidual) * emitDistMax * 0.00390625f);
			for (auto distNextEmit = distLastEmit + get_emit_step(); distNextEmit <= distance; distNextEmit += get_emit_step())
			{
				distNextEmit = std::max(distNextEmit, 0.0f);

				const auto frac = distNextEmit / distance;
				const auto msecBegin = update->msecUpdateBegin + static_cast<int>(std::floor(static_cast<float>(update->msecUpdateEnd - update->msecUpdateBegin) * frac));

				FxSpatialFrame frameEmit{};
				FX_LerpSpatialFrame(frameBegin, frameEnd, frac, &frameEmit);
				if (orientByElem)
				{
					UnitQuatToAxis(frameEmit.quat, axis);
				}

				FX_PlayEffectWithParent(system, update->effect->owner, update->effect->def->elemDefs[update->elemIndex].effectEmitted.handle,
					msecBegin, frameEmit.origin, axis);
				distLastEmit = distNextEmit;
			}

			return static_cast<int>(std::floor((distance - distLastEmit) * 256.0f / emitDistMax + 0.5f));
		}

		void FX_HandleEmitting(FxSystem* system, FxElem* elem, const unsigned int elemHandle, FxUpdateElem* update,
			const float* elemOriginPrev, FxUpdateResult* outUpdateResult)
		{
			auto* effect = update->effect;
			const auto* elemDef = &effect->def->elemDefs[update->elemIndex];

			if ((elemDef->flags & FX_ELEM_EMIT_BOLT) != 0 && elemDef->effectEmitted.handle)
			{
				const auto childEffectHandle = system->elemChild[FX_ElemHandleToIndex(elemHandle)].lightingHandle;
				if (childEffectHandle == 0xFFFF)
				{
					return;
				}

				FxSpatialFrame frameNow{};
				std::memcpy(frameNow.origin, update->posWorld, sizeof(frameNow.origin));

				if ((elemDef->flags & FX_ELEM_EMIT_ORIENT_BY_ELEM) != 0)
				{
					FX_GetQuatForOrientation(effect, elemDef, &effect->frameNow, &update->orient, frameNow.quat, &system->camera);
				}
				else
				{
					orientation_t orientPrev{};
					FX_GetOrientation(elemDef, effect, &effect->framePrev, update->randomSeed, &orientPrev, &system->camera);

					float originPrev[3]{};
					FX_OrientationPosToWorld(&orientPrev, elemOriginPrev, originPrev);

					float axis[3][3]{};
					for (auto i = 0; i < 3; i++)
					{
						axis[0][i] = update->posWorld[i] - originPrev[i];
						if (std::abs(axis[0][i]) - 0.001f < 0.0f)
						{
							axis[0][i] = 0.0f;
						}
					}

					if (Vec3Normalize(axis[0]) < 0.001f)
					{
						axis[0][0] = 0.0f;
						axis[0][1] = 0.0f;
						axis[0][2] = 1.0f;
					}

					PerpendicularVector(axis[0], axis[1]);
					Vec3Cross(axis[0], axis[1], axis[2]);
					AxisToQuat(axis, frameNow.quat);
				}

				auto* childEffect = FX_EffectFromHandle(system, childEffectHandle);
				childEffect->flags |= FX_EFFECT_FLAG_EMITTER_DRIVEN;
				childEffect->framePrev = childEffect->frameNow;
				childEffect->frameNow = frameNow;
				return;
			}

			if (!update->hasNonBoltedEmitter)
			{
				return;
			}

			orientation_t orientPrev{};
			FX_GetOrientation(elemDef, effect, &effect->framePrev, update->randomSeed, &orientPrev, &system->camera);

			FxSpatialFrame frameBegin{};
			FX_OrientationPosToWorld(&orientPrev, elemOriginPrev, frameBegin.origin);
			FX_GetQuatForOrientation(effect, elemDef, &effect->framePrev, &orientPrev, frameBegin.quat, &system->camera);

			FxSpatialFrame frameEnd{};
			std::memcpy(frameEnd.origin, update->posWorld, sizeof(frameEnd.origin));
			FX_GetQuatForOrientation(effect, elemDef, &effect->frameNow, &update->orient, frameEnd.quat, &system->camera);

			elem->emitResidual = static_cast<unsigned char>(FX_ProcessEmitting(system, update, elem->emitResidual, &frameBegin, &frameEnd));
			if (update->msecUpdateEnd == update->msecElemEnd)
			{
				*outUpdateResult = FX_UPDATE_REMOVE;
			}
		}

		// the game's trace_t is bigger than ours, PhysicsQuery_LegacyTrace also writes a float at 0x40
		struct FxTraceResult
		{
			trace_t trace;
			char unk[0x10];
		};

		void FX_TraceWorld(const FxSystem* system, trace_t* results, const float* start, const float* end, const Bounds* bounds, const int brushmask)
		{
			PhysicsQuery_LegacyTrace(3 * system->localClientNum + 2, results, start, end, bounds, FX_ENTITYNUM_NONE, 0, brushmask, 0, nullptr, 0);
		}

		int FX_CollisionResponse(FxSystem* system, FxUpdateElem* update, const trace_t* trace, const int msecUpdateBegin,
			const int msecUpdateEnd, float* xyzWorldOld)
		{
			const auto* elemDef = &update->effect->def->elemDefs[update->elemIndex];
			auto* posWorld = update->posWorld;
			for (auto i = 0; i < 3; i++)
			{
				posWorld[i] = (posWorld[i] - xyzWorldOld[i]) * trace->fraction + xyzWorldOld[i];
			}

			auto msecOnImpact = msecUpdateBegin + static_cast<int>(std::floor(static_cast<float>(msecUpdateEnd - msecUpdateBegin) * trace->fraction));
			if ((elemDef->flags & FX_ELEM_DIE_ON_TOUCH) != 0 || msecOnImpact == update->msecElemEnd)
			{
				if (elemDef->effectOnImpact.handle)
				{
					if ((elemDef->flags & FX_ELEM_SPAWN_IMPACT_FX_WITH_SURFACE_NAME) != 0)
					{
						FX_SpawnSurfaceTypeImpactEffect(system, update, elemDef->effectOnImpact.handle, msecOnImpact, trace);
					}
					else
					{
						FX_SpawnImpactEffect(system, update, elemDef->effectOnImpact.handle, msecOnImpact, trace->normal);
					}
				}
				return msecOnImpact;
			}

			update->elemBaseVel[2] += (FX_RandomFloat(update->randomSeed + 15) * elemDef->gravity.amplitude + elemDef->gravity.base)
				* 0.80000001f * static_cast<float>(msecUpdateEnd - msecOnImpact);

			float velocity[3]{};
			FX_GetVelocityAtTime(elemDef, update->randomSeed, update->msecLifeSpan, static_cast<float>(msecOnImpact - update->msecElemBegin),
				&update->orient, update->elemBaseVel, velocity);

			const auto reflectionFactor = FX_RandomFloat(update->randomSeed + 16) * elemDef->reflectionFactor.amplitude + elemDef->reflectionFactor.base;
			float reflected[3]{};
			for (auto i = 0; i < 3; i++)
			{
				reflected[i] = velocity[i] * reflectionFactor;
			}

			if (elemDef->effectOnImpact.handle && Vec3Dot(velocity, velocity) > 1.0f)
			{
				FX_SpawnImpactEffect(system, update, elemDef->effectOnImpact.handle, msecOnImpact, trace->normal);
			}

			if (msecOnImpact == msecUpdateBegin)
			{
				++msecOnImpact;
				if (Vec3Dot(reflected, reflected) <= 1.0f && trace->normal[2] > FX_ON_GROUND_NORMAL_Z)
				{
					update->atRestFraction = static_cast<int>(std::ceil((static_cast<float>(msecOnImpact) - static_cast<float>(update->msecElemBegin))
						* 255.0f * update->invMsecLifeSpan - 0.25f));
					update->elemBaseVel[0] = 0.0f;
					update->elemBaseVel[1] = 0.0f;
					update->elemBaseVel[2] = 0.0f;
					return msecUpdateEnd;
				}
			}

			const auto reflect = Vec3Dot(trace->normal, reflected) * -2.0f;
			for (auto i = 0; i < 3; i++)
			{
				update->elemBaseVel[i] += trace->normal[i] * reflect + reflected[i] - velocity[i];
			}

			FX_OrientationPosFromWorld(&update->orient, posWorld, update->elemOrigin);
			std::memcpy(xyzWorldOld, posWorld, sizeof(float[3]));
			return msecOnImpact;
		}

		FxUpdateResult FX_UpdateElementPosition_CollidingStep(FxSystem* system, FxUpdateElem* update, int msecUpdateBegin,
			const int msecUpdateEnd, float* xyzWorldOld)
		{
			const auto* elemDef = &update->effect->def->elemDefs[update->elemIndex];
			const auto brushmask = elemDef->useItemClip ? FX_BRUSHMASK_ITEM_CLIP : FX_BRUSHMASK;

			while (true)
			{
				update->onGround = false;
				FX_NextElementPosition(update, msecUpdateBegin, msecUpdateEnd);

				FxTraceResult result{};
				auto& trace = result.trace;
				FX_TraceWorld(system, &trace, xyzWorldOld, update->posWorld, &elemDef->collBounds, brushmask);
				if (trace.allsolid || trace.startsolid || trace.fraction == 1.0f)
				{
					break;
				}

				if (trace.normal[2] > FX_ON_GROUND_NORMAL_Z)
				{
					update->onGround = true;
				}

				msecUpdateBegin = FX_CollisionResponse(system, update, &trace, msecUpdateBegin, msecUpdateEnd, xyzWorldOld);
				elemDef = &update->effect->def->elemDefs[update->elemIndex];
				if ((elemDef->flags & FX_ELEM_DIE_ON_TOUCH) != 0)
				{
					return FX_UPDATE_REMOVE;
				}

				if (msecUpdateBegin == msecUpdateEnd)
				{
					break;
				}
			}

			FX_OrientationPosFromWorld(&update->orient, update->posWorld, update->elemOrigin);
			std::memcpy(xyzWorldOld, update->posWorld, sizeof(float[3]));
			return FX_UPDATE_KEEP;
		}

		FxUpdateResult FX_UpdateElementPosition_Colliding(FxSystem* system, FxUpdateElem* update)
		{
			if (update->atRestFraction != 255)
			{
				FX_OrientationPosToWorld(&update->orient, update->elemOrigin, update->posWorld);
				return FX_UPDATE_KEEP;
			}

			float xyzWorldOld[3]{};
			FX_OrientationPosToWorld(&update->orient, update->elemOrigin, xyzWorldOld);

			auto msecStepBegin = update->msecUpdateBegin;
			for (auto msecStepEnd = msecStepBegin + FX_COLLISION_STEP_MSEC; msecStepEnd < update->msecUpdateEnd; msecStepEnd += FX_COLLISION_STEP_MSEC)
			{
				if (!FX_UpdateElementPosition_CollidingStep(system, update, msecStepBegin, msecStepEnd, xyzWorldOld))
				{
					return FX_UPDATE_REMOVE;
				}

				if (update->atRestFraction != 255)
				{
					return FX_UPDATE_KEEP;
				}

				msecStepBegin = msecStepEnd;
			}

			return FX_UpdateElementPosition_CollidingStep(system, update, msecStepBegin, update->msecUpdateEnd, xyzWorldOld);
		}

		FxUpdateResult FX_UpdateElementPosition(FxSystem* system, FxUpdateElem* update)
		{
			auto* effect = update->effect;
			const auto* elemDef = &effect->def->elemDefs[update->elemIndex];
			FX_GetOrientation(elemDef, effect, &effect->frameNow, update->randomSeed, &update->orient, &system->camera);

			if (elemDef->elemType == FX_ELEM_TYPE_MODEL && (elemDef->flags & FX_ELEM_USE_MODEL_PHYSICS) != 0)
			{
				return FX_UPDATE_KEEP;
			}

			if ((elemDef->flags & FX_ELEM_USE_COLLISION) != 0)
			{
				return FX_UpdateElementPosition_Colliding(system, update);
			}

			if ((elemDef->flags & (FX_ELEM_HAS_GRAVITY | FX_ELEM_USE_VECTORFIELDS)) != 0)
			{
				FX_NextElementPosition(update, update->msecUpdateBegin, update->msecUpdateEnd);
				FX_OrientationPosFromWorld(&update->orient, update->posWorld, update->elemOrigin);
			}
			else
			{
				FX_IntegrateVelocity(update,
					static_cast<float>(update->msecUpdateBegin - update->msecElemBegin) * update->invMsecLifeSpan,
					static_cast<float>(update->msecUpdateEnd - update->msecElemBegin) * update->invMsecLifeSpan,
					update->elemOrigin, update->posWorld);
			}

			return FX_UPDATE_KEEP;
		}

		float FX_SolveQuadraticForSmallestPositiveOrZero(const float a, const float b, const float c)
		{
			if (std::abs(a) < 1.0e-11f)
			{
				return b != 0.0f ? std::max(-c / b, 0.0f) : 0.0f;
			}

			const auto discriminant = b * b - a * 4.0f * c;
			if (discriminant < 0.0f)
			{
				return 0.0f;
			}

			const auto root = std::sqrt(discriminant);
			const auto t0 = (-b - root) / (a + a);
			const auto t1 = (-b + root) / (a + a);
			const auto lo = std::min(t0, t1);
			const auto hi = std::max(t0, t1);
			return lo >= 0.0f ? lo : std::max(hi, 0.0f);
		}

		bool FX_CollideTrajectory(const FxSystem* system, const float* pos, const float* vel, const float* gravity, const float timeLeft,
			float* outPos, float* outNormal, float* outTime, float* outVel)
		{
			const auto get_pos = [&](const float time, float* out)
			{
				for (auto i = 0; i < 3; i++)
				{
					out[i] = time * vel[i] + pos[i] + time * time * 0.5f * gravity[i];
				}
			};

			auto timeBegin = 10.0f;
			auto timeEnd = 20.0f;
			float traceStart[3]{};
			float traceEnd[3]{};
			get_pos(timeBegin, traceStart);

			constexpr Bounds pointBounds{};
			FxTraceResult result{};
			auto& trace = result.trace;
			while (true)
			{
				timeEnd = std::min(timeEnd, timeLeft);
				get_pos(timeEnd, traceEnd);
				FX_TraceWorld(system, &trace, traceStart, traceEnd, &pointBounds, 0x2001);
				if (trace.fraction != 1.0f)
				{
					break;
				}

				if (timeEnd == timeLeft)
				{
					return false;
				}

				timeBegin = timeEnd;
				timeEnd *= 3.0f;
				std::memcpy(traceStart, traceEnd, sizeof(traceStart));
			}

			for (auto i = 0; i < 3; i++)
			{
				outPos[i] = (traceEnd[i] - traceStart[i]) * trace.fraction + traceStart[i];
			}

			const auto hitTime = FX_SolveQuadraticForSmallestPositiveOrZero(gravity[2] * 0.5f, gravity[2] * timeBegin + vel[2], traceStart[2] - outPos[2]) + timeBegin;
			*outTime = hitTime;
			std::memcpy(outNormal, trace.normal, sizeof(float[3]));

			if (hitTime <= 0.0f)
			{
				std::memcpy(outVel, vel, sizeof(float[3]));
				return true;
			}

			float predicted[3]{};
			get_pos(hitTime, predicted);
			for (auto i = 0; i < 3; i++)
			{
				outVel[i] = vel[i] + (outPos[i] - predicted[i]) / hitTime;
			}
			return true;
		}

		void FX_RandomizeNormal(const float* normal, const float randFrac, float* out)
		{
			float dir[3]{};
			for (auto& value : dir)
			{
				value = static_cast<float>(std::rand()) * 0.000061037019f - 1.0f;
			}

			if (Vec3Normalize(dir) == 0.0f)
			{
				dir[2] = 1.0f;
			}

			const auto scale = (Vec3Dot(dir, normal) < 0.0f ? -1.0f : 1.0f) * randFrac;
			for (auto i = 0; i < 3; i++)
			{
				out[i] = (1.0f - randFrac) * normal[i] + scale * dir[i];
			}
			Vec3Normalize(out);
		}

		void FX_CollideTrajectories(const FxSystem* system, const int trajCount, const int maxCollisions, const float maxTime,
			const float bounceFrac, const float bounceRandFrac, const float* gravity, FxSpark* sparks)
		{
			for (auto spark = sparks; spark != sparks + trajCount; spark++)
			{
				float pos[3]{};
				float vel[3]{};
				std::memcpy(pos, spark->pos[0], sizeof(pos));
				std::memcpy(vel, spark->vel[0], sizeof(vel));

				auto time = 0.0f;
				auto alive = true;
				for (auto segment = 0; segment < maxCollisions; segment++)
				{
					if (!alive)
					{
						spark->time[segment] = std::numeric_limits<float>::max();
						std::memset(spark->pos[segment], 0, sizeof(spark->pos[segment]));
						std::memset(spark->vel[segment], 0, sizeof(spark->vel[segment]));
						continue;
					}

					spark->time[segment] = time;
					std::memcpy(spark->pos[segment], pos, sizeof(pos));
					std::memcpy(spark->vel[segment], vel, sizeof(vel));
					if (segment == maxCollisions - 1)
					{
						break;
					}

					float hitPos[3]{};
					float hitNormal[3]{};
					float hitTime{};
					float fixedVel[3]{};
					if (!FX_CollideTrajectory(system, pos, vel, gravity, maxTime - time, hitPos, hitNormal, &hitTime, fixedVel))
					{
						alive = false;
						continue;
					}

					std::memcpy(spark->vel[segment], fixedVel, sizeof(fixedVel));
					std::memcpy(pos, hitPos, sizeof(pos));
					time += hitTime;

					float impactVel[3]{};
					for (auto i = 0; i < 3; i++)
					{
						impactVel[i] = hitTime * gravity[i] + fixedVel[i];
					}

					float bounceNormal[3]{};
					FX_RandomizeNormal(hitNormal, bounceRandFrac, bounceNormal);
					const auto reflect = Vec3Dot(bounceNormal, impactVel) * (bounceFrac * -2.0f);
					for (auto i = 0; i < 3; i++)
					{
						vel[i] = bounceNormal[i] * reflect + impactVel[i] * bounceFrac;
					}
				}
			}
		}

		void FX_UpdateSparkFountain(FxSystem* system, const FxUpdateElem* update, const unsigned int elemHandle)
		{
			auto* fountain = reinterpret_cast<FxSparkFountain*>(reinterpret_cast<char*>(system->sparkFountains) + system->elemChild[FX_ElemHandleToIndex(elemHandle)].lightingHandle);
			auto* header = &fountain->header;
			if (header->computedClusterCount == header->clusterCount)
			{
				return;
			}

			const auto* fountainDef = update->effect->def->elemDefs[update->elemIndex].extended.sparkFountainDef;
			const float gravity[3] = { 0.0f, 0.0f, fountainDef->gravity };

			auto* sparks = reinterpret_cast<FxSpark*>(reinterpret_cast<char*>(system->sparkFountainClusters)
				+ (static_cast<size_t>(fountain->clusters[header->computedClusterCount]) << 10) + sizeof(FxSpark) * header->computedClusterPortion);
			FX_CollideTrajectories(system, 16, 4, 5000.0f, fountainDef->bounceFrac, fountainDef->bounceRand, gravity, sparks);

			header->computedClusterPortion += 16;
			if (header->computedClusterPortion == std::extent_v<decltype(FxSparkFountainCluster::sparks)>)
			{
				++header->computedClusterCount;
				header->computedClusterPortion = 0;
			}
		}

		void FX_PopulateSparkCloudFromUpdate(FxSystem* system, FxUpdateElem* update, FxElem* elem, const unsigned int elemHandle, GfxParticleCloud* cloud)
		{
			auto* effect = update->effect;
			const auto* elemDef = &effect->def->elemDefs[update->elemIndex];

			FxDrawState draw{};
			draw.system = system;
			draw.effect = effect;
			draw.elem = elem;
			draw.elemDef = elemDef;
			draw.elemHandle = elemHandle;
			draw.randomSeed = update->randomSeed;
			draw.orient = update->orient;
			draw.msecLifeSpan = update->msecLifeSpan;
			draw.msecElapsed = update->msecElapsed;
			draw.normTimeUpdateEnd = update->msecElapsed * update->invMsecLifeSpan;
			std::memcpy(draw.posWorld, update->posWorld, sizeof(draw.posWorld));

			FX_SetupVisualState(elemDef, effect, update->randomSeed, draw.normTimeUpdateEnd, &draw.preVisState);
			FX_EvaluateSize(&draw.preVisState, &draw.visState);
			FX_EvaluateScale(&draw.preVisState, &draw.visState);
			FX_DrawElem_Cloud_PopulateGfxParticleCloud(&draw, FX_ELEM_TYPE_SPARK_CLOUD, cloud);
		}

		void FX_UpdateSparkCloud(FxSystem* system, FxUpdateElem* update, FxElem* elem, const unsigned int elemHandle)
		{
			const auto sparkCloudHandle = system->elemChild[FX_ElemHandleToIndex(elemHandle)].lightingHandle;
			if (sparkCloudHandle == 0xFFFF)
			{
				return;
			}

			auto* sparkCloud = reinterpret_cast<FxSparkCloud*>(reinterpret_cast<char*>(system->sparkClouds) + 16 * sparkCloudHandle);
			if (!sparkCloud->frameEnd || update->msecElapsed - sparkCloud->lastTime >= 16.666f)
			{
				sparkCloud->lastTime = update->msecElapsed;
				if (++sparkCloud->frameEnd - sparkCloud->frameBegin > std::size(sparkCloud->frames))
				{
					++sparkCloud->frameBegin;
				}
			}

			auto* cloud = &sparkCloud->frames[(sparkCloud->frameEnd - 1) & (std::size(sparkCloud->frames) - 1)];
			FX_PopulateSparkCloudFromUpdate(system, update, elem, elemHandle, cloud);
		}

		void FX_UpdateVectorFieldElement(const int msecUpdateEnd, FxEffect* effect, const unsigned int elemHandle, const FxUpdateElem* update,
			const FxElemDef* elemDef)
		{
			FxElemPreVisualState preVisState{};
			FX_SetupVisualState(elemDef, effect, effect->randomSeed, static_cast<float>(msecUpdateEnd - update->msecElemBegin) * update->invMsecLifeSpan, &preVisState);

			const auto amplitudeScale = FX_RandomFloat(effect->randomSeed + 23);
			const auto* ref = preVisState.refState;
			const auto strength0 = amplitudeScale * ref[0].amplitude.color[3] + (1.0f - amplitudeScale) * ref[0].base.color[3];
			const auto strength1 = amplitudeScale * ref[1].amplitude.color[3] + (1.0f - amplitudeScale) * ref[1].base.color[3];
			const auto strength = strength1 * preVisState.sampleLerp + strength0 * preVisState.sampleLerpInv;

			for (auto i = 0; i < VECTOR_FIELD_INSTANCE_LIMIT; i++)
			{
				auto* instance = &s_vectorFieldInstancePool[i];
				if (instance->inUse && instance->elemHandle == elemHandle)
				{
					std::memcpy(instance->orient.origin, update->posWorld, sizeof(float[3]));
					std::memset(instance->orient.axis, 0, sizeof(instance->orient.axis));
					instance->strengthScale = strength;
				}
			}
		}

		unsigned int FX_GetIntersectingVectorFields(const float* pos, const float radius, unsigned int* instances, const unsigned int maxInstances)
		{
			auto count = 0u;
			for (auto i = 0; i < VECTOR_FIELD_INSTANCE_LIMIT; i++)
			{
				const auto* instance = &s_vectorFieldInstancePool[i];
				if (!instance->inUse || !instance->vf)
				{
					continue;
				}

				const auto& bounds = instance->vf->worldBounds;
				const auto fieldRadius = std::max({ bounds.maxs[0] - bounds.mins[0], bounds.maxs[1] - bounds.mins[1], bounds.maxs[2] - bounds.mins[2] }) * 0.5f;
				const float delta[3] = { pos[0] - instance->orient.origin[0], pos[1] - instance->orient.origin[1], pos[2] - instance->orient.origin[2] };
				if (fieldRadius * fieldRadius + radius * radius > Vec3Dot(delta, delta))
				{
					if (count >= maxInstances)
					{
						break;
					}
					instances[count++] = i;
				}
			}
			return count;
		}

		FxUpdateResult FX_UpdateElement(FxSystem* system, FxEffect* effect, const unsigned int effectHandle, FxElem* elem,
			const unsigned int elemHandle, const int msecUpdateBegin, const int msecUpdateEnd)
		{
			const auto msecElemBegin = elem->msecBegin;
			if (msecUpdateEnd < msecElemBegin)
			{
				return FX_UPDATE_KEEP;
			}

			FxUpdateElem update{};
			update.effect = effect;
			update.effectHandle = effectHandle;
			update.elemIndex = elem->defIndex;
			update.atRestFraction = elem->atRestFraction;
			update.sequence = elem->sequence;
			update.msecElemBegin = msecElemBegin;
			update.msecUpdateBegin = msecUpdateBegin;
			update.msecUpdateEnd = msecUpdateEnd;
			update.elemOrigin = elem->origin;
			update.elemBaseVel = elem->baseVel;

			const auto* elemDef = &effect->def->elemDefs[update.elemIndex];
			update.randomSeed = FX_ElemRandomSeed(elemDef, effect, msecElemBegin, elem->sequence);
			update.hasNonBoltedEmitter = elemDef->effectEmitted.handle && (elemDef->flags & FX_ELEM_EMIT_BOLT) == 0;

			const auto msecLifeSpan = FX_GetElemLifeSpan(elemDef, update.randomSeed);
			update.msecElemEnd = msecElemBegin + msecLifeSpan;
			update.msecLifeSpan = static_cast<float>(msecLifeSpan);

			float elemOriginPrev[3]{};
			auto updateResult = FX_UPDATE_REMOVE;
			if (msecUpdateEnd < update.msecElemEnd || update.hasNonBoltedEmitter)
			{
				update.msecUpdateEnd = std::min(msecUpdateEnd, update.msecElemEnd);
				update.msecUpdateBegin = std::max(msecUpdateBegin, msecElemBegin);
				if (update.msecUpdateBegin == update.msecUpdateEnd)
				{
					return update.msecUpdateBegin < update.msecElemEnd ? FX_UPDATE_KEEP : FX_UPDATE_REMOVE;
				}

				std::memcpy(elemOriginPrev, elem->origin, sizeof(elemOriginPrev));
				update.invMsecLifeSpan = 1.0f / static_cast<float>(msecLifeSpan);
				update.msecElapsed = static_cast<float>(update.msecUpdateEnd - msecElemBegin);

				updateResult = FX_UpdateElementPosition(system, &update);
				FX_HandleEmitting(system, elem, elemHandle, &update, elemOriginPrev, &updateResult);
			}

			elemDef = &update.effect->def->elemDefs[update.elemIndex];
			if (elemDef->elemType == FX_ELEM_TYPE_SOUND)
			{
				FXIW_SpawnSound(system->localClientNum, effect, elemDef, &update.effect->frameNow, update.randomSeed, update.msecElemBegin);
				return FX_UPDATE_REMOVE;
			}

			if (updateResult == FX_UPDATE_REMOVE)
			{
				if (elemDef->effectOnDeath.handle)
				{
					FX_SpawnDeathEffect(system, &update);
				}
				return FX_UPDATE_REMOVE;
			}

			switch (elemDef->elemType)
			{
			case FX_ELEM_TYPE_SPARK_CLOUD:
				FX_UpdateSparkCloud(system, &update, elem, elemHandle);
				break;
			case FX_ELEM_TYPE_SPARK_FOUNTAIN:
				FX_UpdateSparkFountain(system, &update, elemHandle);
				break;
			case FX_ELEM_TYPE_VECTORFIELD:
				FX_UpdateVectorFieldElement(msecUpdateEnd, effect, elemHandle, &update, elemDef);
				break;
			default:
				break;
			}

			if ((elemDef->flags & FX_ELEM_USE_COLLISION) != 0 && update.onGround && update.atRestFraction == 255
				&& std::memcmp(elem->origin, elemOriginPrev, sizeof(elemOriginPrev)) == 0)
			{
				elem->atRestFraction = static_cast<unsigned char>(std::ceil((static_cast<float>(update.msecUpdateEnd) - static_cast<float>(update.msecElemBegin))
					* 255.0f * update.invMsecLifeSpan - 0.25f));
			}
			else
			{
				elem->atRestFraction = static_cast<unsigned char>(update.atRestFraction);
			}

			return FX_UPDATE_KEEP;
		}

		unsigned int FX_GetElemClass(const FxElemDef* elemDef)
		{
			if (elemDef->elemType <= FX_ELEM_TYPE_LAST_SPRITE)
			{
				return FX_ELEM_CLASS_SPRITE;
			}
			if (elemDef->elemType == FX_ELEM_TYPE_CLOUD || elemDef->elemType == FX_ELEM_TYPE_SPARK_CLOUD)
			{
				return FX_ELEM_CLASS_CLOUD;
			}
			return FX_ELEM_CLASS_NONSPRITE;
		}

		void FX_UpdateEffectPartialForClass(FxSystem* system, FxEffect* effect, const unsigned int effectHandle, const int msecUpdateBegin,
			const int msecUpdateEnd, unsigned int elemHandleStart, unsigned int elemHandleStop, const unsigned int elemClass)
		{
			auto elemHandleFirst = effect->firstElemHandle[elemClass];
			while (elemHandleStart != elemHandleStop)
			{
				for (auto elemHandle = elemHandleStart; elemHandle != elemHandleStop;)
				{
					const auto nextElemHandle = FX_NextElemHandle(system, elemHandle);
					if (FX_UpdateElement(system, effect, effectHandle, FX_ElemFromHandle(system, elemHandle), elemHandle, msecUpdateBegin, msecUpdateEnd) == FX_UPDATE_REMOVE)
					{
						FXW_FreeElem(system, elemHandle, effect, effectHandle, elemClass);
						if (elemHandleFirst == elemHandle)
						{
							elemHandleFirst = nextElemHandle;
						}
					}
					elemHandle = nextElemHandle;
				}

				elemHandleStart = effect->firstElemHandle[elemClass];
				elemHandleStop = elemHandleFirst;
				elemHandleFirst = elemHandleStart;
			}
		}

		void FX_PackTrailBasis(const float(*basis)[3], char(*out)[3])
		{
			for (auto i = 0; i < 2; i++)
			{
				for (auto j = 0; j < 3; j++)
				{
					out[i][j] = static_cast<char>(std::clamp(static_cast<int>(basis[i][j] * 127.0f), -128, 127));
				}
			}
		}

		FxUpdateResult FX_UpdateTrailElement(FxSystem* system, FxEffect* effect, const unsigned int effectHandle, FxTrail* trail,
			FxTrailElem* trailElem, const int msecUpdateBegin, const int msecUpdateEnd)
		{
			const auto msecElemBegin = trailElem->msecBegin;
			if (msecUpdateEnd < msecElemBegin)
			{
				return FX_UPDATE_KEEP;
			}

			FxUpdateElem update{};
			update.effect = effect;
			update.effectHandle = effectHandle;
			update.elemIndex = trail->defIndex;
			update.sequence = trailElem->sequence;
			update.msecElemBegin = msecElemBegin;
			update.msecUpdateBegin = msecUpdateBegin;
			update.msecUpdateEnd = msecUpdateEnd;
			update.elemOrigin = trailElem->origin;

			const auto* elemDef = &effect->def->elemDefs[trail->defIndex];
			update.randomSeed = static_cast<int>(static_cast<unsigned int>(elemDef->randomSeed + 10111 * trailElem->sequence + effect->randomSeed) % FX_RANDOM_SEED_MODULUS);
			update.hasNonBoltedEmitter = elemDef->effectEmitted.handle && (elemDef->flags & FX_ELEM_EMIT_BOLT) == 0;

			const auto msecLifeSpan = FX_GetElemLifeSpan(elemDef, update.randomSeed);
			update.msecElemEnd = msecElemBegin + msecLifeSpan;
			update.msecLifeSpan = static_cast<float>(msecLifeSpan);

			if (msecUpdateEnd >= update.msecElemEnd && !update.hasNonBoltedEmitter)
			{
				return FX_UPDATE_REMOVE;
			}

			update.msecUpdateEnd = std::min(msecUpdateEnd, update.msecElemEnd);
			update.msecUpdateBegin = std::max(msecUpdateBegin, msecElemBegin);
			if (update.msecUpdateBegin == update.msecUpdateEnd)
			{
				return update.msecUpdateBegin < update.msecElemEnd ? FX_UPDATE_KEEP : FX_UPDATE_REMOVE;
			}

			float baseVel[3] = { 0.0f, 0.0f, static_cast<float>(trailElem->baseVelZ) * 0.001f };
			update.elemBaseVel = baseVel;
			update.invMsecLifeSpan = 1.0f / static_cast<float>(msecLifeSpan);
			update.msecElapsed = static_cast<float>(update.msecUpdateEnd - msecElemBegin);

			const auto result = FX_UpdateElementPosition(system, &update);
			trailElem->baseVelZ = static_cast<short>(std::clamp(static_cast<int>(baseVel[2] / 0.001f), -32768, 32767));
			return result;
		}

		void FX_FreeTrailElem(FxSystem* system, const unsigned int trailElemHandle, FxEffect* effect, FxTrail* trail)
		{
			auto* trailElem = FX_TrailElemFromHandle(system, trailElemHandle);
			if (trail->lastTrailElemHandle == trailElemHandle)
			{
				trail->lastTrailElemHandle = static_cast<unsigned int>(-1);
			}
			trail->firstTrailElemHandle = trailElem->nextTrailElemHandle;

			std::memset(trailElem, 0, sizeof(FxTrailElem));
			auto* pool = reinterpret_cast<FxPool<FxTrailElem>*>(trailElem);
			pool->nextFree = system->firstFreeTrailElem;
			system->firstFreeTrailElem = static_cast<int>(pool - system->trailElems);

			FXW_DelRefToEffect(system, effect);
		}

		void FX_UpdateEffectPartialTrail(FxSystem* system, FxEffect* effect, const unsigned int effectHandle, FxTrail* trail,
			const int msecUpdateBegin, const int msecUpdateEnd, const float distanceTravelledEnd, const unsigned int trailElemHandleStart,
			const unsigned int trailElemHandleStop, const FxSpatialFrame* frameNow)
		{
			auto trailElemHandle = trailElemHandleStart == -1 ? trail->firstTrailElemHandle : trailElemHandleStart;
			if (trailElemHandle == trailElemHandleStop)
			{
				return;
			}

			auto canFree = trailElemHandle == trail->firstTrailElemHandle;
			auto prevTrailElemHandle = static_cast<unsigned int>(-1);
			FxTrailElem* trailElem = nullptr;
			do
			{
				trailElem = FX_TrailElemFromHandle(system, trailElemHandle);
				const auto nextTrailElemHandle = trailElem->nextTrailElemHandle;
				if (FX_UpdateTrailElement(system, effect, effectHandle, trail, trailElem, msecUpdateBegin, msecUpdateEnd) != FX_UPDATE_REMOVE)
				{
					canFree = false;
				}
				else if (canFree && prevTrailElemHandle != -1)
				{
					FX_FreeTrailElem(system, prevTrailElemHandle, effect, trail);
				}

				prevTrailElemHandle = trailElemHandle;
				trailElemHandle = nextTrailElemHandle;
			} while (trailElemHandle != trailElemHandleStop);

			if (prevTrailElemHandle == -1 || prevTrailElemHandle != trail->lastTrailElemHandle)
			{
				return;
			}

			if (canFree)
			{
				FX_FreeTrailElem(system, prevTrailElemHandle, effect, trail);
				return;
			}

			if ((effect->status & FX_STATUS_IS_LOOPING) == 0)
			{
				return;
			}

			const auto* elemDef = &effect->def->elemDefs[trail->defIndex];
			trailElem->spawnDist = distanceTravelledEnd;
			if (!elemDef->spawnDelayMsec.base)
			{
				trailElem->msecBegin = msecUpdateEnd;
			}

			float basis[2][3]{};
			const auto randomSeed = static_cast<int>(static_cast<unsigned int>(elemDef->randomSeed + effect->randomSeed + 10111 * trailElem->sequence) % FX_RANDOM_SEED_MODULUS);
			FX_GetOriginForTrailElem(elemDef, trail, frameNow, randomSeed, trailElem->origin, basis[0], basis[1]);
			FX_PackTrailBasis(basis, trailElem->basis);
		}

		void FX_UpdateEffectPartial(FxSystem* system, const int isDeferred, FxEffect* effect, const unsigned int effectHandle, const int msecUpdateBegin,
			const int msecUpdateEnd, const float distanceTravelledBegin, const float distanceTravelledEnd, const float arcDistanceTravelled,
			const unsigned int* elemHandleStart, const unsigned int* elemHandleStop, const unsigned int* trailElemStart,
			const unsigned int* trailElemStop)
		{
			if ((effect->status & FX_STATUS_IS_LOOPING) != 0)
			{
				const auto* def = effect->def;
				FX_ProcessLooping(system, isDeferred, effect, effectHandle, 0, def->elemDefCountLooping, &effect->framePrev, &effect->frameNow,
					effect->msecBegin, msecUpdateBegin, msecUpdateEnd, distanceTravelledBegin, distanceTravelledEnd, arcDistanceTravelled);

				if (msecUpdateEnd - effect->msecBegin > def->msecLoopingLife)
				{
					FX_EffectStopLooping(system, effect);
				}
			}

			for (auto elemClass = 0u; elemClass < FX_ELEM_CLASS_COUNT; elemClass++)
			{
				if (isDeferred)
				{
					for (auto elemHandle = elemHandleStart[elemClass]; elemHandle != elemHandleStop[elemClass]; elemHandle = FX_NextElemHandle(system, elemHandle))
					{
						FX_ForcePushElemUpdate(system, effectHandle, elemHandle);
					}
				}
				else
				{
					FX_UpdateEffectPartialForClass(system, effect, effectHandle, msecUpdateBegin, msecUpdateEnd,
						elemHandleStart[elemClass], elemHandleStop[elemClass], elemClass);
				}
			}

			auto trailIndex = 0;
			for (auto trailHandle = effect->firstTrailHandle; trailHandle != -1; trailIndex++)
			{
				auto* trail = FX_TrailFromHandle(system, trailHandle);
				const auto trailElemHandleStart = trailElemStart ? trailElemStart[trailIndex] : static_cast<unsigned int>(-1);
				const auto trailElemHandleStop = trailElemStop ? trailElemStop[trailIndex] : static_cast<unsigned int>(-1);
				FX_UpdateEffectPartialTrail(system, effect, effectHandle, trail, msecUpdateBegin, msecUpdateEnd, distanceTravelledEnd,
					trailElemHandleStart, trailElemHandleStop, &effect->frameNow);
				trailHandle = trail->nextTrailHandle;
			}

			if (!isDeferred)
			{
				effect->msecLastUpdate = msecUpdateEnd;
			}
		}

		void FX_UpdateEffect(FxSystem* system, const int isDeferred, FxEffect* effect, const unsigned int effectHandle)
		{
			if ((effect->status & FX_STATUS_REF_COUNT_MASK) == 0)
			{
				return;
			}

			const auto msecUpdateBegin = effect->msecLastUpdate;
			const auto msecUpdateEnd = system->msecNow;
			if (msecUpdateBegin >= msecUpdateEnd)
			{
				return;
			}

			float delta[3]{};
			for (auto i = 0; i < 3; i++)
			{
				delta[i] = effect->frameNow.origin[i] - effect->framePrev.origin[i];
			}

			const auto distanceTravelledBegin = effect->distanceTravelled;
			const auto distanceTravelledEnd = std::sqrt(Vec3Dot(delta, delta)) + distanceTravelledBegin;
			const auto arcDistanceTravelled = effect->firstTrailHandle == -1 ? 0.0f : FX_GetArcDistanceTravelledThisFrame(effect);

			const unsigned int elemHandleStop[FX_ELEM_CLASS_COUNT] = { static_cast<unsigned int>(-1), static_cast<unsigned int>(-1), static_cast<unsigned int>(-1) };
			FX_UpdateEffectPartial(system, isDeferred, effect, effectHandle, msecUpdateBegin, msecUpdateEnd, distanceTravelledBegin,
				distanceTravelledEnd, arcDistanceTravelled, effect->firstElemHandle, elemHandleStop, nullptr, nullptr);

			if ((effect->def->flags & FX_EFFECT_NEEDS_LIGHTING_PER_FRAME_AT_ORIGIN) != 0)
			{
				FX_CalculateLighting(effect->frameNow.origin, effect->lighting);
			}

			if ((effect->def->flags & FX_EFFECT_USE_VECTORFIELDS) != 0)
			{
				effect->numVectorFields = FX_GetIntersectingVectorFields(effect->frameNow.origin, effect->def->elemMaxRadius,
					effect->vectorFields, static_cast<unsigned int>(std::size(effect->vectorFields)));
			}

			++effect->updateCount;
			effect->distanceTravelled = distanceTravelledEnd;

			if (!isDeferred)
			{
				FXW_SortNewElemsInEffect(system, effect);
				effect->framePrev = effect->frameNow;
			}
		}

		void FX_SystemUpdateEffects(FxSystem* system, const int isDeferred, const unsigned int* effectHandles, const int skipBolt)
		{
			for (auto i = 0; i < FX_UPDATE_LIST_LIMIT && effectHandles[i] != -1; i++)
			{
				auto* effect = FX_EffectFromHandle(system, effectHandles[i]);
				if (!skipBolt || effect->bolt == 0xFFFF)
				{
					FX_UpdateEffect(system, isDeferred, effect, effectHandles[i]);
				}
			}
		}

		void FX_SystemUpdateDeferredElements(FxSystem* system)
		{
			g_autoAddElemForUpdate.get()[0] = 1;

			for (auto i = 0; i < system->numUpdateElement; i++)
			{
				const auto elemHandle = system->updateElement[i].elemHandle;
				const auto effectHandle = system->updateElement[i].effectHandle;
				auto* elem = FX_ElemFromHandle(system, elemHandle);
				auto* effect = FX_EffectFromHandle(system, effectHandle);
				if (elem->valid && FX_UpdateElement(system, effect, effectHandle, elem, elemHandle, effect->msecLastUpdate, system->msecNow) == FX_UPDATE_REMOVE)
				{
					FXW_FreeElem(system, elemHandle, effect, effectHandle, FX_GetElemClass(&effect->def->elemDefs[elem->defIndex]));
				}
			}

			g_autoAddElemForUpdate.get()[0] = 0;
			system->numUpdateElement = 0;
		}

		void FX_SystemUpdateEffectsFinalize(FxSystem* system, unsigned int* effectHandles, const int skipBolt)
		{
			auto numSkipped = 0;
			for (auto i = 0; i < FX_UPDATE_LIST_LIMIT && effectHandles[i] != -1; i++)
			{
				const auto effectHandle = effectHandles[i];
				auto* effect = FX_EffectFromHandle(system, effectHandle);
				if (!skipBolt || effect->bolt == 0xFFFF)
				{
					effect->msecLastUpdate = system->msecNow;
					FXW_SortNewElemsInEffect(system, effect);
					if ((effect->flags & FX_EFFECT_FLAG_EMITTER_DRIVEN) == 0)
					{
						effect->framePrev = effect->frameNow;
					}
				}
				else
				{
					effectHandles[numSkipped++] = effectHandle;
				}

				effectHandles[i] = static_cast<unsigned int>(-1);
			}

			if (numSkipped < FX_UPDATE_LIST_LIMIT)
			{
				effectHandles[numSkipped] = static_cast<unsigned int>(-1);
			}
		}

		void FX_FreeBolt(FxSystem* system, FxEffect* effect)
		{
			auto* pool = &system->bolts[effect->bolt];
			std::memset(pool, 0, sizeof(*pool));
			pool->nextFree = system->firstFreeBolt;
			system->firstFreeBolt = static_cast<int>(pool - system->bolts);
			effect->bolt = 0xFFFF;
		}

		void FX_UpdateEffectBolt(FxSystem* system, FxEffect* effect)
		{
			if (effect->bolt == 0xFFFF)
			{
				return;
			}

			auto* bolt = FX_BoltFromHandle(system, effect->bolt);
			if (bolt->dobjHandle == FX_DOBJ_HANDLE_NONE)
			{
				return;
			}

			effect->status &= ~FX_STATUS_HIDDEN;

			const auto localClientNum = static_cast<unsigned int>(system->localClientNum);
			const auto temporalMatch = static_cast<unsigned int>(FX_GetBoltTemporalBits(localClientNum, bolt->dobjHandle)) == bolt->temporalBits;
			const auto boltedToEntity = (effect->flags & FX_EFFECT_FLAG_BOLTED_TO_ENTITY) != 0;

			orientation_t orient{};
			const auto haveOrient = FX_GetBoneOrientation(localClientNum, bolt->dobjHandle, boltedToEntity ? -1 : static_cast<int>(bolt->boneIndex), &orient);
			if (!temporalMatch || !haveOrient)
			{
				FXW_PreventRewind(system, effect);
				if ((boltedToEntity && !haveOrient) || (effect->flags & FX_EFFECT_FLAG_KILL_ON_STOP) != 0)
				{
					FXW_KillEffect(system, effect, FX_EffectToHandle(system, effect), 0);
				}
				else
				{
					FXW_StopEffect(system, effect);
				}

				FX_FreeBolt(system, effect);
				return;
			}

			if (!boltedToEntity || (effect->flags & 0x40) != 0)
			{
				FxSpatialFrame boneFrame{};
				std::memcpy(boneFrame.origin, orient.origin, sizeof(boneFrame.origin));
				AxisToQuat(orient.axis, boneFrame.quat);
				FX_SpatialFrameMultiply(&bolt->offset, &boneFrame, &effect->frameNow);
			}
			else
			{
				FX_OrientationPosToWorld(&orient, bolt->offset.origin, effect->frameNow.origin);
			}

			auto negated = true;
			for (auto i = 0; i < 4; i++)
			{
				if (std::abs(effect->frameNow.quat[i] + effect->framePrev.quat[i]) > 0.001f)
				{
					negated = false;
					break;
				}
			}

			if (negated)
			{
				std::memcpy(effect->framePrev.quat, effect->frameNow.quat, sizeof(effect->framePrev.quat));
			}
		}

		void FX_ProcessRetriggerRequests(FxSystem* system, const RetriggerEffectRequest* requests, const int numRequests)
		{
			for (auto i = 0; i < numRequests; i++)
			{
				const auto effectHandle = requests[i].effectHandle;
				const auto msecBegin = requests[i].msecBegin;
				auto* effect = FX_EffectFromHandle(system, effectHandle);
				FX_AddRefToEffect(system, effect);

				if ((effect->status & FX_STATUS_IS_LOOPING) != 0)
				{
					FX_SpawnAllFutureLooping(system, 0, effect, effectHandle);
					FXW_StopEffect(system, effect);
				}

				const unsigned int firstOldElemHandle[FX_ELEM_CLASS_COUNT] = { effect->firstElemHandle[0], effect->firstElemHandle[1], effect->firstElemHandle[2] };
				unsigned int lastOldTrailElemHandle[64]{};
				auto trailIndex = 0;
				for (auto trailHandle = effect->firstTrailHandle; trailHandle != -1 && trailIndex < std::size(lastOldTrailElemHandle); trailIndex++)
				{
					const auto* trail = FX_TrailFromHandle(system, trailHandle);
					lastOldTrailElemHandle[trailIndex] = trail->lastTrailElemHandle;
					trailHandle = trail->nextTrailHandle;
				}

				const auto msecLastUpdate = effect->msecLastUpdate;
				if (msecBegin > msecLastUpdate)
				{
					const unsigned int elemHandleStop[FX_ELEM_CLASS_COUNT] = { static_cast<unsigned int>(-1), static_cast<unsigned int>(-1), static_cast<unsigned int>(-1) };
					FX_UpdateEffectPartial(system, 0, effect, effectHandle, msecLastUpdate, msecBegin, 0.0f, 0.0f, 0.0f,
						firstOldElemHandle, elemHandleStop, nullptr, lastOldTrailElemHandle);
				}

				effect->msecBegin = msecBegin;
				effect->distanceTravelled = 0.0f;
				effect->occlusionFade = 1.0f;
				FXIW_BeginLooping(system, 0, effect, effectHandle, 1);
				FXIW_TriggerOneShot(system, 0, effect, effectHandle, 1);
				if (effect->def->msecLoopingLife)
				{
					FX_EffectRestartLooping(system, effect);
				}

				if (msecBegin < msecLastUpdate)
				{
					FX_UpdateEffectPartial(system, 0, effect, effectHandle, effect->msecBegin, effect->msecLastUpdate, 0.0f, 0.0f, 0.0f,
						effect->firstElemHandle, firstOldElemHandle, lastOldTrailElemHandle, nullptr);
				}

				FXW_SortNewElemsInEffect(system, effect);
				FXW_DelRefToEffect(system, effect);
			}
		}

		const StopBoltedEffectsRequest* FX_FindStopBoltedEffectRequest(const FxSystem* system, const StopBoltedEffectsRequest* requests,
			const int numRequests, const FxEffect* effect)
		{
			const auto* bolt = FX_BoltFromHandle(system, effect->bolt);
			for (auto i = 0; i < numRequests; i++)
			{
				if (effect->def == requests[i].defHandle && bolt->dobjHandle == requests[i].dobjHandle && bolt->boneIndex == requests[i].boneIndex)
				{
					return &requests[i];
				}
			}
			return nullptr;
		}

		void FX_ProcessStopBoltedEffectRequests(FxSystem* system, const FxPass4Cmd* passCmd)
		{
			if ((system->systemFlags & FX_SYSTEM_FLAG_PAUSED) != 0 || !passCmd->numStopBoltedEffectsRequest)
			{
				return;
			}

			for (auto i = system->firstActiveEffect; i != system->firstNewEffect; i++)
			{
				const auto effectHandle = system->allEffectHandles[i & FX_EFFECT_HANDLE_MASK];
				auto* effect = FX_EffectFromHandle(system, effectHandle);
				if (effect->bolt == 0xFFFF)
				{
					continue;
				}

				const auto* request = FX_FindStopBoltedEffectRequest(system, passCmd->stopBoltedEffectsRequest, passCmd->numStopBoltedEffectsRequest, effect);
				if (!request)
				{
					continue;
				}

				FXW_PreventRewind(system, effect);
				if (request->killFlag)
				{
					FXW_KillEffect(system, effect, effectHandle, 0);
				}
				else
				{
					FXW_StopEffect(system, effect);
				}
			}
		}

		int FX_GetOcclusionQueryRefCount(const FxSystem* system, const FxEffect* effect)
		{
			auto count = 0;
			for (auto elemHandle = effect->firstElemHandle[FX_ELEM_CLASS_SPRITE]; elemHandle != -1; elemHandle = FX_NextElemHandle(system, elemHandle))
			{
				if ((effect->def->elemDefs[FX_ElemFromHandle(system, elemHandle)->defIndex].flags & FX_ELEM_USE_OCCLUSION_QUERY) != 0)
				{
					++count;
				}
			}
			return count;
		}

		void FX_ProcessKillRequests(FxSystem* system, const FxPass4Cmd* passCmd)
		{
			for (auto i = 0; i < system->numDeleteElementRequest; i++)
			{
				const auto* request = &system->deleteElementRequest[i];
				if (request->elemHandle == -1)
				{
					if (request->effectHandle != -1)
					{
						auto* effect = FX_EffectFromHandle(system, request->effectHandle);
						if (effect->occlusionQueryHandle != 0xFFFF && !FX_GetOcclusionQueryRefCount(system, effect))
						{
							RB_FreeOcclusionQuery(effect->occlusionQueryHandle);
							effect->occlusionQueryHandle = 0xFFFF;
						}
					}
					continue;
				}

				const auto* effect = FX_EffectFromHandle(system, request->effectHandle);
				const auto* elemDef = &effect->def->elemDefs[request->elemDefIndex];
				if ((elemDef->flags & FX_ELEM_EMIT_BOLT) != 0 && elemDef->effectEmitted.handle)
				{
					FXW_FreeChildEffectElem(system, request->elemHandle);
				}

				auto* pool = reinterpret_cast<FxPool<FxElem>*>(FX_ElemFromHandle(system, request->elemHandle));
				pool->nextFree = system->firstFreeElem;
				system->firstFreeElem = static_cast<int>(pool - system->elems);
			}
			system->numDeleteElementRequest = 0;

			for (auto i = 0; i < passCmd->numThroughWithEffectIndex; i++)
			{
				auto* effect = passCmd->throughWithEffectRequest[i];
				FXW_PreventRewind(system, effect);
				if ((effect->flags & FX_EFFECT_FLAG_KILL_ON_STOP) != 0)
				{
					FXW_KillEffect(system, effect, FX_EffectToHandle(system, effect), 0);
				}
				else
				{
					FXW_StopEffect(system, effect);
				}
				FXW_DelRefToEffect(system, effect);
			}

			for (auto i = 0; i < passCmd->numKillEffectDefIndex; i++)
			{
				const auto* request = &passCmd->killEffectDefRequest[i];
				for (auto j = system->firstActiveEffect; j != system->firstNewEffect; j++)
				{
					const auto effectHandle = system->allEffectHandles[j & FX_EFFECT_HANDLE_MASK];
					auto* effect = FX_EffectFromHandle(system, effectHandle);
					if (effect->def == request->defHandle && (request->time < 0 || effect->msecBegin >= request->time))
					{
						FXW_KillEffect(system, effect, effectHandle, 0);
					}
				}
			}
		}

		void FX_RunGarbageCollection(FxSystem* system)
		{
			system->systemFlags &= ~FX_SYSTEM_FLAG_NEEDS_GARBAGE_COLLECTION;

			unsigned int freedHandles[FX_EFFECT_LIMIT];
			auto numFreed = 0u;
			auto activeIndex = system->firstNewEffect;
			while (activeIndex != system->firstActiveEffect)
			{
				const auto effectHandle = system->allEffectHandles[--activeIndex & FX_EFFECT_HANDLE_MASK];
				auto* effect = FX_EffectFromHandle(system, effectHandle);
				if ((effect->status & FX_STATUS_REF_COUNT_MASK) != 0)
				{
					system->allEffectHandles[(activeIndex + numFreed) & FX_EFFECT_HANDLE_MASK] = effectHandle;
					continue;
				}

				if (effect->bolt != 0xFFFF)
				{
					FX_FreeBolt(system, effect);
				}

				while (effect->firstTrailHandle != -1)
				{
					auto* trail = FX_TrailFromHandle(system, effect->firstTrailHandle);
					effect->firstTrailHandle = trail->nextTrailHandle;

					auto* pool = reinterpret_cast<FxPool<FxTrail>*>(trail);
					std::memset(pool, 0, sizeof(*pool));
					pool->nextFree = system->firstFreeTrail;
					system->firstFreeTrail = static_cast<int>(pool - system->trails);
				}

				FXW_RunGarbageCollection_FreeSparkFountains(system, effect);

				if (effect->occlusionQueryHandle != 0xFFFF)
				{
					RB_FreeOcclusionQuery(effect->occlusionQueryHandle);
					effect->occlusionQueryHandle = 0xFFFF;
				}

				freedHandles[numFreed++] = effectHandle;
			}

			while (numFreed)
			{
				const auto effectHandle = freedHandles[--numFreed];
				system->allEffectHandles[activeIndex++ & FX_EFFECT_HANDLE_MASK] = effectHandle;
				std::memset(FX_EffectFromHandle(system, effectHandle), 0, sizeof(FxEffect));
			}

			system->firstActiveEffect = activeIndex;
		}

		void FX_FreeSpotLightElem(FxSystem* system, const FxSpotLightInfoData* spotLight)
		{
			auto* effect = FX_EffectFromHandle(system, spotLight->effectHandle);
			auto* pool = reinterpret_cast<FxPool<FxElem>*>(FX_ElemFromHandle(system, spotLight->elemHandle));
			const auto index = FX_ElemHandleToIndex(spotLight->elemHandle);

			std::memset(pool, 0, sizeof(*pool));
			system->elemChild[index].lightingHandle = 0xFFFF;
			system->nextElemHandleInEffect[index] = static_cast<unsigned int>(-1);
			system->prevElemHandleInEffect[index] = static_cast<unsigned int>(-1);
			pool->nextFree = system->firstFreeElem;
			system->firstFreeElem = static_cast<int>(pool - system->elems);

			FXW_DelRefToEffect(system, effect);
		}

		// spot lights aren't linked into their effect, iw6 only kept the newest one alive
		void FX_UpdateSpotLights(FxSystem* system)
		{
			auto* info = &system->spotLightInfo;
			auto count = 0;
			for (auto i = 0; i < info->index; i++)
			{
				const auto spotLight = info->arr[i];
				auto* effect = FX_EffectFromHandle(system, spotLight.effectHandle);
				const auto* elem = FX_ElemFromHandle(system, spotLight.elemHandle);
				const auto* elemDef = &effect->def->elemDefs[elem->defIndex];
				const auto randomSeed = FX_ElemRandomSeed(elemDef, effect, elem->msecBegin, elem->sequence);

				auto replaced = false;
				for (auto j = i + 1; j < info->index; j++)
				{
					if (info->arr[j].effectHandle == spotLight.effectHandle)
					{
						replaced = true;
						break;
					}
				}

				if (replaced || system->msecNow >= elem->msecBegin + FX_GetElemLifeSpan(elemDef, randomSeed))
				{
					FX_FreeSpotLightElem(system, &spotLight);
					continue;
				}

				info->arr[count++] = spotLight;
			}

			info->index = count;
		}

		void FX_UpdatePass4Create(FxSystem* system, FxPass4Cmd* passCmd)
		{
			do
			{
				for (auto i = 0; i < FX_UPDATE_LIST_LIMIT && passCmd->createPass[i] != -1; i++)
				{
					const auto effectHandle = passCmd->createPass[i];
					auto* effect = FX_EffectFromHandle(system, effectHandle);
					if (effect->bolt != 0xFFFF)
					{
						FX_UpdateEffectBolt(system, effect);
					}
					FX_UpdateEffect(system, 0, effect, effectHandle);
				}

				passCmd->createPass[0] = static_cast<unsigned int>(-1);

			} while (FXW_CreateNewEffects(system, 0, passCmd->createPass, 0, passCmd->fxEntityArray));
		}
	}

	void FX_UpdatePass0InitUpdateLists(FxSystem* system)
	{
		auto* command = system->command;
		command->numUpdatePass0 = 0;
		command->numUpdatePass4 = 0;
		command->updatePass0[0] = static_cast<unsigned int>(-1);
		command->updatePass4[0] = static_cast<unsigned int>(-1);

		if ((system->systemFlags & FX_SYSTEM_FLAG_PAUSED) != 0)
		{
			return;
		}

		for (auto i = system->firstActiveEffect; i != system->firstNewEffect; i++)
		{
			const auto effectHandle = system->allEffectHandles[i & FX_EFFECT_HANDLE_MASK];
			auto* effect = FX_EffectFromHandle(system, effectHandle);

			const auto frameCount = effect->frameCount;
			effect->frameCount = system->frameCount;
			if (frameCount == system->frameCount || effect->msecLastUpdate > system->msecNow)
			{
				continue;
			}

			if (effect->bolt == 0xFFFF)
			{
				command->updatePass0[command->numUpdatePass0++] = effectHandle;
			}
			else
			{
				command->updatePass4[command->numUpdatePass4++] = effectHandle;
			}
		}

		if (command->numUpdatePass0 < FX_UPDATE_LIST_LIMIT)
		{
			command->updatePass0[command->numUpdatePass0] = static_cast<unsigned int>(-1);
		}

		if (command->numUpdatePass4 < FX_UPDATE_LIST_LIMIT)
		{
			command->updatePass4[command->numUpdatePass4] = static_cast<unsigned int>(-1);
		}
	}

	void FX_UpdatePass0(FxSystem* system, unsigned int* updatePass, const int isDeferred, const int skipBolt)
	{
		FX_SystemUpdateEffects(system, isDeferred, updatePass, skipBolt);
		if (system->numUpdateElement > 0)
		{
			FX_SystemUpdateDeferredElements(system);
		}
		FX_SystemUpdateEffectsFinalize(system, updatePass, skipBolt);
	}

	void FX_UpdateBoltedEffects(FxSystem* system)
	{
		auto* command = system->command;
		auto* updatePass = command->updatePass0;
		auto* boltPass = command->updatePass4;

		auto count = 0;
		for (; count < FX_UPDATE_LIST_LIMIT && updatePass[count] != -1; count++)
		{
			FX_UpdateEffectBolt(system, FX_EffectFromHandle(system, updatePass[count]));
		}

		for (auto i = 0; count < FX_UPDATE_LIST_LIMIT && i < FX_UPDATE_LIST_LIMIT && boltPass[i] != -1; i++)
		{
			FX_UpdateEffectBolt(system, FX_EffectFromHandle(system, boltPass[i]));
			updatePass[count++] = boltPass[i];
		}

		boltPass[0] = static_cast<unsigned int>(-1);
		if (count < FX_UPDATE_LIST_LIMIT)
		{
			updatePass[count] = static_cast<unsigned int>(-1);
		}

		if (updatePass[0] != -1)
		{
			FX_UpdatePass0(system, updatePass, 0, 0);
		}
	}

	void FX_ProcessPass4Commands(FxSystem* system, FxPass4Cmd* passCmd)
	{
		FX_ProcessRetriggerRequests(system, passCmd->retriggerEffectRequest, passCmd->numRetriggerEffectRequest);
		passCmd->numRetriggerEffectRequest = 0;
		FX_ProcessStopBoltedEffectRequests(system, passCmd);
		FX_ProcessKillRequests(system, passCmd);
		if ((system->systemFlags & FX_SYSTEM_FLAG_NEEDS_GARBAGE_COLLECTION) != 0)
		{
			FX_RunGarbageCollection(system);
		}
	}

	void FX_UpdatePass4(FxSystem* system, FxPass4Cmd* passCmd)
	{
		FX_ProcessRetriggerRequests(system, passCmd->retriggerEffectRequest, passCmd->numRetriggerEffectRequest);
		passCmd->numRetriggerEffectRequest = 0;
		FX_UpdatePass4Create(system, passCmd);
		FX_UpdateSpotLights(system);
		FX_ProcessPass4Commands(system, passCmd);
	}

	// iw7 still sets up the call, but the function was stripped
	void FX_ElemEmitEffect(FxSystem* system, const unsigned int elem, const unsigned int elemParent, const unsigned int elemClass,
		const unsigned int owner, const FxEffectDef* def, const int msecBegin, const float* origin, const float(*axis)[3])
	{
		FX_InitLegacySpawnCmd(system, elem, elemParent, elemClass, -1, -1, 1, def, msecBegin, origin, axis, FX_DOBJ_HANDLE_NONE,
			FX_BONE_INDEX_NONE, owner, FX_ENTITYNUM_NONE, false, false, 0, 0);
	}

	void FX_InitLegacySpawnCmd(FxSystem* system, const unsigned int elem, const unsigned int elemParent, const unsigned int elemClass, const int fxEntNum,
		const int timeValue, const int removeRef, const FxEffectDef* def, const int msecBegin, const float* origin, const float(*axis)[3],
		const int dobjHandle, const int boneIndex, const unsigned int owner, const unsigned int markEntnum, const bool markGivenModelsOnly,
		const bool markViewmodel, const int markViewmodelClientIndex, const unsigned short initSpawnFlags)
	{
		if ((system->systemFlags & FX_SYSTEM_FLAG_PAUSED) != 0)
		{
			return;
		}

		int spawnCmdIndex;
		do
		{
			spawnCmdIndex = *system->spawnCmdWrite;
			if (spawnCmdIndex < 0)
			{
				return;
			}
		} while (spawnCmdIndex != _InterlockedCompareExchange(reinterpret_cast<volatile long*>(system->spawnCmdWrite),
			system->spawnCmdList[spawnCmdIndex].nextCmd, spawnCmdIndex));

		auto* cmd = &system->spawnCmdList[spawnCmdIndex];
		cmd->info.effectFlags = 0;
		cmd->info.remoteDef = def;
		cmd->info.msecBegin = msecBegin;
		std::memcpy(cmd->info.frame.origin, origin, sizeof(float[3]));
		AxisToQuat(axis, cmd->info.frame.quat);
		cmd->info.dobjHandle = dobjHandle;
		cmd->info.boneIndex = boneIndex;
		cmd->info.owner = owner;
		cmd->info.runnerSortOrder = 255;
		cmd->info.markEntnum = markEntnum;
		if (markViewmodel)
		{
			cmd->info.effectFlags |= 0x100;
		}
		if (markGivenModelsOnly)
		{
			cmd->info.effectFlags |= 0x80;
		}
		cmd->info.markViewmodelClientIndex = markViewmodelClientIndex;
		cmd->info.effectFlags |= initSpawnFlags;
		cmd->elemHandle = elem;
		cmd->elemParent = elemParent;
		cmd->removeRef = removeRef;
		cmd->elemClass = elemClass;
		cmd->entnum = fxEntNum;
		cmd->timeValue = timeValue;

		if (dobjHandle != FX_DOBJ_HANDLE_NONE)
		{
			cmd->info.boltOffset = {};
			cmd->info.boltOffset.quat[3] = 1.0f;

			const auto* pose = boneIndex == FX_BONE_INDEX_NONE ? static_cast<const char*>(CG_GetPose(system->localClientNum, dobjHandle)) : nullptr;
			if (pose)
			{
				const auto* poseOrigin = reinterpret_cast<const float*>(pose + 0x1C);
				const auto* poseAngles = reinterpret_cast<const float*>(pose + 0x28);

				float poseAxis[3][3]{};
				AnglesToAxis(poseAngles, poseAxis);

				FxSpatialFrame poseFrame{};
				std::memcpy(poseFrame.origin, poseOrigin, sizeof(poseFrame.origin));
				AxisToQuat(poseAxis, poseFrame.quat);

				FxSpatialFrame poseFrameInv{};
				FX_SpatialFrameInvert(&poseFrame, &poseFrameInv);
				FX_SpatialFrameMultiply(&cmd->info.frame, &poseFrameInv, &cmd->info.boltOffset);
			}
		}

		if (owner != -1)
		{
			FX_AddRefToEffect(system, FX_EffectFromHandle(system, owner));
		}

		int nextCmd;
		do
		{
			nextCmd = *system->spawnCmdRead;
			cmd->nextCmd = nextCmd;
		} while (nextCmd != _InterlockedCompareExchange(reinterpret_cast<volatile long*>(system->spawnCmdRead), spawnCmdIndex, nextCmd));
	}
}
