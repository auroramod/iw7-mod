#pragma once

#include "structs.hpp"
#include "engine.hpp"

namespace fx
{
	void FX_UpdatePass0InitUpdateLists(FxSystem* system);
	void FX_UpdatePass0(FxSystem* system, unsigned int* updatePass, int isDeferred, int skipBolt);
	void FX_UpdateBoltedEffects(FxSystem* system);
	void FX_UpdatePass4(FxSystem* system, FxPass4Cmd* passCmd);
	void FX_ProcessPass4Commands(FxSystem* system, FxPass4Cmd* passCmd);

	void FX_InitLegacySpawnCmd(FxSystem* system, unsigned int elem, unsigned int elemParent, unsigned int elemClass, int fxEntNum,
		int timeValue, int removeRef, const FxEffectDef* def, int msecBegin, const float* origin, const float(*axis)[3],
		int dobjHandle, int boneIndex, unsigned int owner, unsigned int markEntnum, bool markGivenModelsOnly, bool markViewmodel,
		int markViewmodelClientIndex, unsigned short initSpawnFlags);
	void FX_ElemEmitEffect(FxSystem* system, unsigned int elem, unsigned int elemParent, unsigned int elemClass,
		unsigned int owner, const FxEffectDef* def, int msecBegin, const float* origin, const float(*axis)[3]);

	void FX_DrawNonSpriteElems(FxSystem* system, FxPass5Cmd* passCmd);
	void FX_DrawSpriteElems(GfxCodeSurfGlob* codeSurfGlob, FxSystem* system, int drawTime, const FxCamera* camera, int surfList);
	void FX_DrawElem_Light(FxDrawState* draw);
	char FX_GenSpriteQuad(FxDrawState* draw, const __m128* tangent, const __m128* bitangent, const __m128* normal);
	void FX_GenParticleSimAnimationQuads(FxDrawState* draw, const float* left, const float* up, const __m128i* normal);

	inline float FX_RandomFloat(const int index)
	{
		return engine::fx_randomTable.get()[index];
	}

	inline unsigned int FX_RandomWord(const int index)
	{
		return *reinterpret_cast<const unsigned short*>(&engine::fx_randomTable.get()[index]);
	}

	inline int FX_ElemRandomSeed(const FxElemDef* elemDef, const FxEffect* effect, const int msecBegin, const int sequence)
	{
		return static_cast<int>(static_cast<unsigned int>(msecBegin + elemDef->randomSeed + effect->randomSeed + 10111 * sequence) % FX_RANDOM_SEED_MODULUS);
	}

	inline int FX_GetElemLifeSpan(const FxElemDef* elemDef, const int randomSeed)
	{
		return elemDef->lifeSpanMsec.base + static_cast<int>(((elemDef->lifeSpanMsec.amplitude + 1) * FX_RandomWord(randomSeed + 17)) >> 16);
	}

	inline float Vec3Dot(const float* a, const float* b)
	{
		return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	}

	inline void Vec3Cross(const float* a, const float* b, float* out)
	{
		out[0] = a[1] * b[2] - a[2] * b[1];
		out[1] = a[2] * b[0] - a[0] * b[2];
		out[2] = a[0] * b[1] - a[1] * b[0];
	}

	inline float Vec3Normalize(float* v)
	{
		const auto length = std::sqrt(Vec3Dot(v, v));
		if (length != 0.0f)
		{
			const auto inv = 1.0f / length;
			v[0] *= inv;
			v[1] *= inv;
			v[2] *= inv;
		}
		return length;
	}

	inline void Vec4Normalize(float* v)
	{
		const auto length_sq = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		if (length_sq != 0.0f)
		{
			const auto inv = 1.0f / std::sqrt(length_sq);
			v[0] *= inv;
			v[1] *= inv;
			v[2] *= inv;
			v[3] *= inv;
		}
	}

	inline void FX_LerpSpatialFrame(const FxSpatialFrame* from, const FxSpatialFrame* to, const float frac, FxSpatialFrame* out)
	{
		for (auto i = 0; i < 3; i++)
		{
			out->origin[i] = (to->origin[i] - from->origin[i]) * frac + from->origin[i];
		}
		for (auto i = 0; i < 4; i++)
		{
			out->quat[i] = (to->quat[i] - from->quat[i]) * frac + from->quat[i];
		}
		Vec4Normalize(out->quat);
	}

	inline void PerpendicularVector(const float* src, float* dst)
	{
		const float src_sq[3] = { src[0] * src[0], src[1] * src[1], src[2] * src[2] };
		auto pos = src_sq[0] > src_sq[1] ? 1 : 0;
		if (src_sq[pos] > src_sq[2])
		{
			pos = 2;
		}

		const auto d = -src[pos];
		dst[0] = src[0] * d;
		dst[1] = src[1] * d;
		dst[2] = src[2] * d;
		dst[pos] += 1.0f;
		Vec3Normalize(dst);
	}

	inline void Vec3Basis_RightHanded(const float* forward, float* left, float* up)
	{
		PerpendicularVector(forward, up);
		Vec3Cross(up, forward, left);
	}

	inline void AxisToQuat(const float(*mat)[3], float* out)
	{
		float test[4][4]{};
		test[0][0] = mat[1][2] - mat[2][1];
		test[0][1] = mat[2][0] - mat[0][2];
		test[0][2] = mat[0][1] - mat[1][0];
		test[0][3] = mat[0][0] + mat[1][1] + mat[2][2] + 1.0f;

		const auto length_sq = [](const float* v)
		{
			return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
		};

		auto best = 0;
		auto size_sq = length_sq(test[0]);
		if (size_sq < 1.0f)
		{
			test[1][0] = mat[2][0] + mat[0][2];
			test[1][1] = mat[2][1] + mat[1][2];
			test[1][2] = mat[2][2] - mat[1][1] - mat[0][0] + 1.0f;
			test[1][3] = test[0][2];
			best = 1;
			size_sq = length_sq(test[1]);
			if (size_sq < 1.0f)
			{
				test[2][0] = mat[0][0] - mat[1][1] - mat[2][2] + 1.0f;
				test[2][1] = mat[1][0] + mat[0][1];
				test[2][2] = test[1][0];
				test[2][3] = test[0][0];
				best = 2;
				size_sq = length_sq(test[2]);
				if (size_sq < 1.0f)
				{
					test[3][0] = test[2][1];
					test[3][1] = mat[1][1] - mat[0][0] - mat[2][2] + 1.0f;
					test[3][2] = test[1][1];
					test[3][3] = test[0][1];
					best = 3;
					size_sq = length_sq(test[3]);
				}
			}
		}

		const auto inv = size_sq > 0.0f ? 1.0f / std::sqrt(size_sq) : 0.0f;
		for (auto i = 0; i < 4; i++)
		{
			out[i] = test[best][i] * inv;
		}
	}

	inline void UnitQuatToAxis(const float* quat, float(*axis)[3])
	{
		const auto x2 = quat[0] + quat[0];
		const auto xx = x2 * quat[0];
		const auto xy = x2 * quat[1];
		const auto xz = x2 * quat[2];
		const auto xw = x2 * quat[3];
		const auto y2 = quat[1] + quat[1];
		const auto yy = y2 * quat[1];
		const auto yz = y2 * quat[2];
		const auto yw = y2 * quat[3];
		const auto z2 = quat[2] + quat[2];
		const auto zz = z2 * quat[2];
		const auto zw = z2 * quat[3];

		axis[0][0] = 1.0f - (yy + zz);
		axis[0][1] = xy + zw;
		axis[0][2] = xz - yw;
		axis[1][0] = xy - zw;
		axis[1][1] = 1.0f - (xx + zz);
		axis[1][2] = yz + xw;
		axis[2][0] = xz + yw;
		axis[2][1] = yz - xw;
		axis[2][2] = 1.0f - (xx + yy);
	}

	inline void FX_OrientationPosToWorld(const orientation_t* orient, const float* pos, float* out)
	{
		for (auto i = 0; i < 3; i++)
		{
			out[i] = orient->axis[0][i] * pos[0] + orient->origin[i] + orient->axis[1][i] * pos[1] + orient->axis[2][i] * pos[2];
		}
	}

	inline void FX_OrientationPosFromWorld(const orientation_t* orient, const float* pos, float* out)
	{
		const float delta[3] = { pos[0] - orient->origin[0], pos[1] - orient->origin[1], pos[2] - orient->origin[2] };
		for (auto i = 0; i < 3; i++)
		{
			out[i] = Vec3Dot(delta, orient->axis[i]);
		}
	}
}
