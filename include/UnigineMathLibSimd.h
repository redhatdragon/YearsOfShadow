/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */

#pragma once
#include "UnigineMathLibCommon.h"

#include "UnigineMathLibVec3.h"
#include "UnigineMathLibVec4.h"

#include "UnigineMathLibDVec3.h"
#include "UnigineMathLibDVec4.h"

#include "UnigineMathLibMat4.h"
#include "UnigineMathLibDMat4.h"

namespace Unigine
{

namespace Math
{

namespace Simd
{
	// math
	UNIGINE_INLINE void dot(float &ret, const float *v0, const float *v1, int num)
	{
		#ifdef USE_SSE
			ret = 0.0f;

			if (num & ~3)
			{
				UNIGINE_ASSERT_ALIGNED16(ret);
				UNIGINE_ASSERT_ALIGNED16(v0);

				__m128 res_0, res_1, res_2;

				res_0 = _mm_load_ps(v0);
				res_1 = _mm_load_ps(v1);
				res_2 = _mm_mul_ps(res_0, res_1);

				v0 += 4;
				v1 += 4;

				for (size_t i = (num >> 2) - 1; i > 0; i--)
				{
					res_0 = _mm_load_ps(v0);
					res_1 = _mm_load_ps(v1);
					res_2 = _mm_add_ps(res_2, _mm_mul_ps(res_0, res_1));

					v0 += 4;
					v1 += 4;
				}

				res_2 = _mm_add_ps(res_2, _MM_SWIZZLE(res_2, Y, X, W, Z));
				res_2 = _mm_add_ss(res_2, _MM_SWIZZLE(res_2, Z, W, X, Y));
				_mm_store_ss(&ret, res_2);

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				ret += *v0++ * *v1++;
		#else
			ret = 0.0f;

			if (num & ~3)
			{
				float res_0 = 0.0f;
				float res_1 = 0.0f;
				float res_2 = 0.0f;
				float res_3 = 0.0f;

				for (size_t i = num >> 2; i > 0; i--)
				{
					res_0 += v0[0] * v1[0];
					res_1 += v0[1] * v1[1];
					res_2 += v0[2] * v1[2];
					res_3 += v0[3] * v1[3];

					v0 += 4;
					v1 += 4;
				}

				ret = res_0 + res_1 + res_2 + res_3;

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				ret += *v0++ * *v1++;
		#endif
	}
	UNIGINE_INLINE void mul(float *ret, const float *v0, float v1, int num)
	{
		#ifdef USE_SSE
			if (num & ~3)
			{
				UNIGINE_ASSERT_ALIGNED16(ret);
				UNIGINE_ASSERT_ALIGNED16(v0);

				__m128 res_1 = _mm_set1_ps(v1);

				for (size_t i = num >> 2; i > 0; i--)
				{
					__m128 res_0 = _mm_load_ps(v0);
					_mm_stream_ps(ret, _mm_mul_ps(res_0, res_1));

					ret += 4;
					v0 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * v1;
		#else
			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					ret[0] = v0[0] * v1;
					ret[1] = v0[1] * v1;
					ret[2] = v0[2] * v1;
					ret[3] = v0[3] * v1;

					ret += 4;
					v0 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * v1;
		#endif
	}
	UNIGINE_INLINE void mul(float *ret, const float *v0, const float *v1, int num)
	{
		#ifdef USE_SSE
			if (num & ~3)
			{
				UNIGINE_ASSERT_ALIGNED16(ret);
				UNIGINE_ASSERT_ALIGNED16(v0);
				UNIGINE_ASSERT_ALIGNED16(v1);

				for (size_t i = num >> 2; i > 0; i--)
				{
					__m128 res_0 = _mm_load_ps(v0);
					__m128 res_1 = _mm_load_ps(v1);
					_mm_stream_ps(ret, _mm_mul_ps(res_0, res_1));

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * *v1++;
		#else
			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					ret[0] = v0[0] * v1[0];
					ret[1] = v0[1] * v1[1];
					ret[2] = v0[2] * v1[2];
					ret[3] = v0[3] * v1[3];

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * *v1++;
		#endif
	}
	UNIGINE_INLINE void mad(float *ret, const float *v0, float v1, const float *v2, int num)
	{
		#ifdef USE_SSE
			if (num & ~3)
			{
				__m128 res_1 = _mm_set1_ps(v1);

				if (UNIGINE_IS_ALIGNED16(ret) && UNIGINE_IS_ALIGNED16(v0) && UNIGINE_IS_ALIGNED16(v2))
				{
					for (size_t i = num >> 2; i > 0; i--)
					{
						__m128 res_0 = _mm_load_ps(v0);
						__m128 res_2 = _mm_load_ps(v2);
						_mm_stream_ps(ret, _mm_add_ps(_mm_mul_ps(res_0, res_1), res_2));

						ret += 4;
						v0 += 4;
						v2 += 4;
					}
				} else
				{
					for (size_t i = num >> 2; i > 0; i--)
					{
						__m128 res_0 = _mm_loadu_ps(v0);
						__m128 res_2 = _mm_loadu_ps(v2);
						_mm_storeu_ps(ret, _mm_add_ps(_mm_mul_ps(res_0, res_1), res_2));

						ret += 4;
						v0 += 4;
						v2 += 4;
					}
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * v1 + *v2++;
		#else
			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					ret[0] = v0[0] * v1 + v2[0];
					ret[1] = v0[1] * v1 + v2[1];
					ret[2] = v0[2] * v1 + v2[2];
					ret[3] = v0[3] * v1 + v2[3];

					ret += 4;
					v0 += 4;
					v2 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ * v1 + *v2++;
		#endif
	}
	UNIGINE_INLINE void add(float *ret, const float *v0, const float *v1, int num)
	{
		#ifdef USE_SSE
			if (num & ~3)
			{
				UNIGINE_ASSERT_ALIGNED16(ret);
				UNIGINE_ASSERT_ALIGNED16(v0);
				UNIGINE_ASSERT_ALIGNED16(v1);

				for (size_t i = num >> 2; i > 0; i--)
				{
					__m128 res_0 = _mm_load_ps(v0);
					__m128 res_1 = _mm_load_ps(v1);
					_mm_stream_ps(ret, _mm_add_ps(res_0, res_1));

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ + *v1++;
		#else
			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					ret[0] = v0[0] + v1[0];
					ret[1] = v0[1] + v1[1];
					ret[2] = v0[2] + v1[2];
					ret[3] = v0[3] + v1[3];

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ + *v1++;
		#endif
	}
	UNIGINE_INLINE void sub(float *ret, const float *v0, const float *v1, int num)
	{
		#ifdef USE_SSE
			if (num & ~3)
			{
				UNIGINE_ASSERT_ALIGNED16(ret);
				UNIGINE_ASSERT_ALIGNED16(v0);
				UNIGINE_ASSERT_ALIGNED16(v1);

				for (size_t i = num >> 2; i > 0; i--)
				{
					__m128 res_0 = _mm_load_ps(v0);
					__m128 res_1 = _mm_load_ps(v1);
					_mm_stream_ps(ret, _mm_sub_ps(res_0, res_1));

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ - *v1++;
		#else
			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					ret[0] = v0[0] - v1[0];
					ret[1] = v0[1] - v1[1];
					ret[2] = v0[2] - v1[2];
					ret[3] = v0[3] - v1[3];

					ret += 4;
					v0 += 4;
					v1 += 4;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
				*ret++ = *v0++ - *v1++;
		#endif
	}

	// bounds
	#ifdef USE_SSE
		UNIGINE_INLINE void simd_sse2_min_max_vec(__m128d &min0, __m128d &min1, __m128d &max0, __m128d &max1, const void *src, int src_stride, int num)
		{
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const unsigned char *s = (const unsigned char *)src;

			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

					__m128d res_00 = _mm_load_pd((const double *)s);
					__m128d res_01 = _mm_load_pd((const double *)s + 2);
					s += src_stride;
					__m128d res_10 = _mm_load_pd((const double *)s);
					__m128d res_11 = _mm_load_pd((const double *)s + 2);
					s += src_stride;
					__m128d res_20 = _mm_load_pd((const double *)s);
					__m128d res_21 = _mm_load_pd((const double *)s + 2);
					s += src_stride;
					__m128d res_30 = _mm_load_pd((const double *)s);
					__m128d res_31 = _mm_load_pd((const double *)s + 2);
					s += src_stride;

					min0 = _mm_min_pd(min0, res_00);
					min1 = _mm_min_pd(min1, res_01);
					min0 = _mm_min_pd(min0, res_10);
					min1 = _mm_min_pd(min1, res_11);
					min0 = _mm_min_pd(min0, res_20);
					min1 = _mm_min_pd(min1, res_21);
					min0 = _mm_min_pd(min0, res_30);
					min1 = _mm_min_pd(min1, res_31);

					max0 = _mm_max_pd(max0, res_00);
					max1 = _mm_max_pd(max1, res_01);
					max0 = _mm_max_pd(max0, res_10);
					max1 = _mm_max_pd(max1, res_11);
					max0 = _mm_max_pd(max0, res_20);
					max1 = _mm_max_pd(max1, res_21);
					max0 = _mm_max_pd(max0, res_30);
					max1 = _mm_max_pd(max1, res_31);
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
			{
				__m128d res0 = _mm_load_pd((const double *)s);
				__m128d res1 = _mm_load_pd((const double *)s + 2);

				min0 = _mm_min_pd(min0, res0);
				min1 = _mm_min_pd(min1, res1);
				max0 = _mm_max_pd(max0, res0);
				max1 = _mm_max_pd(max1, res1);

				s += src_stride;
			}
		}
		UNIGINE_INLINE void simd_sse_min_max_vec(__m128 &min, __m128 &max, const void *src, int src_stride, int num)
		{
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const unsigned char *s = (const unsigned char *)src;

			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

					__m128 res_0 = _mm_load_ps((const float *)s);
					s += src_stride;
					__m128 res_1 = _mm_load_ps((const float *)s);
					s += src_stride;
					__m128 res_2 = _mm_load_ps((const float *)s);
					s += src_stride;
					__m128 res_3 = _mm_load_ps((const float *)s);
					s += src_stride;

					min = _mm_min_ps(min, res_0);
					min = _mm_min_ps(min, res_1);
					min = _mm_min_ps(min, res_2);
					min = _mm_min_ps(min, res_3);

					max = _mm_max_ps(max, res_0);
					max = _mm_max_ps(max, res_1);
					max = _mm_max_ps(max, res_2);
					max = _mm_max_ps(max, res_3);
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
			{
				__m128 res = _mm_load_ps((const float *)s);

				min = _mm_min_ps(min, res);
				max = _mm_max_ps(max, res);

				s += src_stride;
			}
		}
	#endif
	UNIGINE_INLINE void minMaxVec3(vec3 &min, vec3 &max, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(vec3));

		min = vec3_inf;
		max = -vec3_inf;

		#ifdef USE_SSE
			simd_sse_min_max_vec(min.vec, max.vec, src, src_stride, num);
		#else
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				const vec3 &v = *(const vec3 *)s;
				if (min.x > v.x)
					min.x = v.x;
				if (max.x < v.x)
					max.x = v.x;
				if (min.y > v.y)
					min.y = v.y;
				if (max.y < v.y)
					max.y = v.y;
				if (min.z > v.z)
					min.z = v.z;
				if (max.z < v.z)
					max.z = v.z;

				s += src_stride;
			}
		#endif
	}
	UNIGINE_INLINE void minMaxVec4(vec4 &min, vec4 &max, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(vec4));

		min = vec4_inf;
		max = -vec4_inf;

		#ifdef USE_SSE
			simd_sse_min_max_vec(min.vec, max.vec, src, src_stride, num);
		#else
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				const vec4 &v = *(const vec4 *)s;
				if (min.x > v.x)
					min.x = v.x;
				if (max.x < v.x)
					max.x = v.x;
				if (min.y > v.y)
					min.y = v.y;
				if (max.y < v.y)
					max.y = v.y;
				if (min.z > v.z)
					min.z = v.z;
				if (max.z < v.z)
					max.z = v.z;
				if (min.w > v.w)
					min.w = v.w;
				if (max.w < v.w)
					max.w = v.w;

				s += src_stride;
			}
		#endif
	}
	UNIGINE_INLINE void minMaxVec3(dvec3 &min, dvec3 &max, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(dvec3));

		min = dvec3_inf;
		max = -dvec3_inf;

		#ifdef USE_SSE
			simd_sse2_min_max_vec(min.sse.v0, min.sse.v1, max.sse.v0, max.sse.v1, src, src_stride, num);
		#else
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				const dvec3 &v = *(const dvec3 *)s;
				if (min.x > v.x)
					min.x = v.x;
				if (max.x < v.x)
					max.x = v.x;
				if (min.y > v.y)
					min.y = v.y;
				if (max.y < v.y)
					max.y = v.y;
				if (min.z > v.z)
					min.z = v.z;
				if (max.z < v.z)
					max.z = v.z;

				s += src_stride;
			}
		#endif
	}
	UNIGINE_INLINE void minMaxVec4(dvec4 &min, dvec4 &max, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(dvec4));

		min = dvec4_inf;
		max = -dvec4_inf;

		#ifdef USE_SSE
			simd_sse2_min_max_vec(min.sse.v0, min.sse.v1, max.sse.v0, max.sse.v1, src, src_stride, num);
		#else
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				const dvec4 &v = *(const dvec4 *)s;
				if (min.x > v.x)
					min.x = v.x;
				if (max.x < v.x)
					max.x = v.x;
				if (min.y > v.y)
					min.y = v.y;
				if (max.y < v.y)
					max.y = v.y;
				if (min.z > v.z)
					min.z = v.z;
				if (max.z < v.z)
					max.z = v.z;
				if (min.w > v.w)
					min.w = v.w;
				if (max.w < v.w)
					max.w = v.w;

				s += src_stride;
			}
		#endif
	}

	// transformed bounds
	UNIGINE_INLINE void minMaxMat4Vec3(vec3 &min, vec3 &max, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(vec3));

		min = vec3_inf;
		max = -vec3_inf;

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, W));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, W));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, W));
				temp = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3));

				min.vec = _mm_min_ps(min.vec, temp);
				max.vec = _mm_max_ps(max.vec, temp);

				s += src_stride;
			}
		#else
			vec3 res;

			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::mul(res, m, *(const vec3 *)s);
				if (min.x > res.x)
					min.x = res.x;
				if (max.x < res.x)
					max.x = res.x;
				if (min.y > res.y)
					min.y = res.y;
				if (max.y < res.y)
					max.y = res.y;
				if (min.z > res.z)
					min.z = res.z;
				if (max.z < res.z)
					max.z = res.z;

				s += src_stride;
			}
		#endif
	}
	UNIGINE_INLINE void minMaxMat4Vec4(vec4 &min, vec4 &max, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)src_stride >= sizeof(vec4));

		min.set(Consts::INF);
		max.set(-Consts::INF);

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, X));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, Y));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, Z));
				__m128 res_3 = _mm_mul_ps(col_3, _MM_SWIZZLE(temp, W, W, W, W));
				temp = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));

				min.vec = _mm_min_ps(min.vec, temp);
				max.vec = _mm_max_ps(max.vec, temp);

				s += src_stride;
			}

			min = min.vec;
			max = max.vec;
		#else
			vec4 res;

			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::mul(res, m, *(const vec4 *)s);
				if (min.x > res.x)
					min.x = res.x;
				if (max.x < res.x)
					max.x = res.x;
				if (min.y > res.y)
					min.y = res.y;
				if (max.y < res.y)
					max.y = res.y;
				if (min.z > res.z)
					min.z = res.z;
				if (max.z < res.z)
					max.z = res.z;
				if (min.w > res.w)
					min.w = res.w;
				if (max.w < res.w)
					max.w = res.w;

				s += src_stride;
			}
		#endif
	}

	// vector dot products
	UNIGINE_INLINE void dotVec3Vec3(void *ret, int ret_stride, const vec3 &v, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(float));
		assert((size_t)src_stride >= sizeof(vec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;

			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

					__m128 res_0 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_1 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_2 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_3 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;

					__m128 res_4 = _mm_add_ps(res_0, _MM_SWIZZLE(res_0, Y, X, Y, W));
					__m128 res_5 = _mm_add_ps(res_1, _MM_SWIZZLE(res_1, Y, X, Y, W));
					__m128 res_6 = _mm_add_ps(res_2, _MM_SWIZZLE(res_2, Y, X, Y, W));
					__m128 res_7 = _mm_add_ps(res_3, _MM_SWIZZLE(res_3, Y, X, Y, W));

					res_0 = _mm_add_ss(res_4, _MM_SWIZZLE(res_0, Z, Z, X, W));
					res_1 = _mm_add_ss(res_5, _MM_SWIZZLE(res_1, Z, Z, X, W));
					res_2 = _mm_add_ss(res_6, _MM_SWIZZLE(res_2, Z, Z, X, W));
					res_3 = _mm_add_ss(res_7, _MM_SWIZZLE(res_3, Z, Z, X, W));

					_mm_store_ss((float *)d, res_0);
					d += ret_stride;
					_mm_store_ss((float *)d, res_1);
					d += ret_stride;
					_mm_store_ss((float *)d, res_2);
					d += ret_stride;
					_mm_store_ss((float *)d, res_3);
					d += ret_stride;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
			{
				__m128 res_0 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
				__m128 res_1 = _mm_add_ps(res_0, _MM_SWIZZLE(res_0, Y, X, Y, W));
				res_0 = _mm_add_ss(res_1, _MM_SWIZZLE(res_0, Z, Z, X, W));

				_mm_store_ss((float *)d, res_0);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				*(float *)d = Math::dot(v, *(const vec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void dotVec4Vec4(void *ret, int ret_stride, const vec4 &v, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(float));
		assert((size_t)src_stride >= sizeof(vec4));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;

			if (num & ~3)
			{
				for (size_t i = num >> 2; i > 0; i--)
				{
					_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

					__m128 res_0 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_1 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_2 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;
					__m128 res_3 = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
					s += src_stride;

					res_0 = _mm_add_ps(res_0, _MM_SWIZZLE(res_0, Y, X, W, Z));
					res_1 = _mm_add_ps(res_1, _MM_SWIZZLE(res_1, Y, X, W, Z));
					res_2 = _mm_add_ps(res_2, _MM_SWIZZLE(res_2, Y, X, W, Z));
					res_3 = _mm_add_ps(res_3, _MM_SWIZZLE(res_3, Y, X, W, Z));

					res_0 = _mm_add_ss(res_0, _MM_SWIZZLE(res_0, Z, W, X, Y));
					res_1 = _mm_add_ss(res_1, _MM_SWIZZLE(res_1, Z, W, X, Y));
					res_2 = _mm_add_ss(res_2, _MM_SWIZZLE(res_2, Z, W, X, Y));
					res_3 = _mm_add_ss(res_3, _MM_SWIZZLE(res_3, Z, W, X, Y));

					_mm_store_ss((float *)d, res_0);
					d += ret_stride;
					_mm_store_ss((float *)d, res_1);
					d += ret_stride;
					_mm_store_ss((float *)d, res_2);
					d += ret_stride;
					_mm_store_ss((float *)d, res_3);
					d += ret_stride;
				}

				num &= 3;
			}

			for (size_t i = num; i > 0; i--)
			{
				__m128 res = _mm_mul_ps(v.vec, _mm_load_ps((const float *)s));
				res = _mm_add_ps(res, _MM_SWIZZLE(res, Y, X, W, Z));
				res = _mm_add_ss(res, _MM_SWIZZLE(res, Z, W, X, Y));

				_mm_store_ss((float *)d, res);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				*(float *)d = Math::dot(v, *(const vec4 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}

	// vector normalizations
	UNIGINE_INLINE void normalizeVec3(void *ret, int ret_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);

			unsigned char *d = (unsigned char *)ret;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)d + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)d);
				temp = _mm_normalize3_ps(temp);
				_mm_store_ps((float *)d, temp);

				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			for (size_t i = num; i > 0; i--)
			{
				((vec3 *)d)->normalize();

				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void normalizeVec4(void *ret, int ret_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec4));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);

			unsigned char *d = (unsigned char *)ret;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)d + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)d);
				temp = _mm_normalize4_ps(temp);
				_mm_store_ps((float *)d, temp);

				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			for (size_t i = num; i > 0; i--)
			{
				((vec4 *)d)->normalize();

				d += ret_stride;
			}
		#endif
	}

	// vector multiplications
	UNIGINE_INLINE void mulMat3Vec3(void *ret, int ret_stride, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec3));
		assert((size_t)src_stride >= sizeof(vec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, W));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, W));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, W));
				temp = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);

				_mm_stream_ps((float *)d, temp);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				mul3(*(vec3 *)d, m, *(const vec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat4Vec3(void *ret, int ret_stride, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec3));
		assert((size_t)src_stride >= sizeof(vec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, W));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, W));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, W));
				temp = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3));

				_mm_stream_ps((float *)d, temp);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::mul(*(vec3 *)d, m, *(const vec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat4Vec4(void *ret, int ret_stride, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec4));
		assert((size_t)src_stride >= sizeof(vec4));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, X));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, Y));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, Z));
				__m128 res_3 = _mm_mul_ps(col_3, _MM_SWIZZLE(temp, W, W, W, W));
				temp = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));

				_mm_stream_ps((float *)d, temp);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::mul(*(vec4 *)d, m, *(const vec4 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat3Vec3(void *ret, int ret_stride, const dmat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(dvec3));
		assert((size_t)src_stride >= sizeof(dvec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128d &col_0 = m.sse.v0;
			const __m128d &col_1 = m.sse.v1;
			const __m128d &col_2 = m.sse.v2;
			const __m128d &col_3 = m.sse.v3;
			const __m128d &col_4 = m.sse.v4;

			__m128d col_12 = _mm_shuffle_pd(col_1, col_2, _MM_PERM22(Y, X));
			__m128d col_22 = _mm_shuffle_pd(col_2, col_2, _MM_PERM22(Y, Y));

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128d temp0 = _mm_load_pd((const double *)s);
				__m128d temp1 = _mm_load_sd((const double *)s + 2);

				__m128d x = _MM_SWIZZLE2(temp0, X, X);
				__m128d y = _MM_SWIZZLE2(temp0, Y, Y);
				__m128d z = _MM_SWIZZLE2(temp1, X, X);

				__m128d res_0 = _mm_mul_pd(col_0, x);
				__m128d res_1 = _mm_mul_pd(col_12, y);
				__m128d res_2 = _mm_mul_pd(col_3, z);

				temp0 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

				res_0 = _mm_mul_sd(col_1, x);
				res_1 = _mm_mul_sd(col_22, y);
				res_2 = _mm_mul_sd(col_4, z);

				temp1 = _mm_add_sd(_mm_add_sd(res_0, res_1), res_2);

				_mm_stream_pd((double *)d, temp0);
				_mm_stream_pd((double *)d + 2, temp1);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				mul3(*(dvec3 *)d, m, *(const dvec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat4Vec3(void *ret, int ret_stride, const dmat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(dvec3));
		assert((size_t)src_stride >= sizeof(dvec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128d &col_0 = m.sse.v0;
			const __m128d &col_1 = m.sse.v1;
			const __m128d &col_2 = m.sse.v2;
			const __m128d &col_3 = m.sse.v3;
			const __m128d &col_4 = m.sse.v4;
			const __m128d &col_5 = m.sse.v5;

			__m128d col_12 = _mm_shuffle_pd(col_1, col_2, _MM_PERM22(Y, X));
			__m128d col_22 = _mm_shuffle_pd(col_2, col_2, _MM_PERM22(Y, Y));
			__m128d col_45 = _mm_shuffle_pd(col_4, col_5, _MM_PERM22(Y, X));
			__m128d col_55 = _mm_shuffle_pd(col_5, col_5, _MM_PERM22(Y, Y));

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128d temp0 = _mm_load_pd((const double *)s);
				__m128d temp1 = _mm_load_sd((const double *)s + 2);

				__m128d x = _MM_SWIZZLE2(temp0, X, X);
				__m128d y = _MM_SWIZZLE2(temp0, Y, Y);
				__m128d z = _MM_SWIZZLE2(temp1, X, X);

				__m128d res_0 = _mm_mul_pd(col_0, x);
				__m128d res_1 = _mm_mul_pd(col_12, y);
				__m128d res_2 = _mm_mul_pd(col_3, z);

				temp0 = _mm_add_pd(_mm_add_pd(res_0, res_1), _mm_add_pd(res_2, col_45));

				res_0 = _mm_mul_sd(col_1, x);
				res_1 = _mm_mul_sd(col_22, y);
				res_2 = _mm_mul_sd(col_4, z);

				temp1 = _mm_add_sd(_mm_add_sd(res_0, res_1), _mm_add_sd(res_2, col_55));

				_mm_stream_pd((double *)d, temp0);
				_mm_stream_pd((double *)d + 2, temp1);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::mul(*(dvec3 *)d, m, *(const dvec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat4Vec4(void *ret, int ret_stride, const dmat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(dvec4));
		assert((size_t)src_stride >= sizeof(dvec4));

		unsigned char *d = (unsigned char *)ret;
		const unsigned char *s = (const unsigned char *)src;
		for (size_t i = num; i > 0; i--)
		{
			Math::mul(*(dvec4 *)d, m, *(const dvec4 *)s);

			s += src_stride;
			d += ret_stride;
		}
	}

	// vector projections
	UNIGINE_INLINE void projMat4Vec3(void *ret, int ret_stride, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec3));
		assert((size_t)src_stride >= sizeof(vec3));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, X));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, Y));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, Z));
				__m128 res_3 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3));
				temp = _mm_div_ps(res_3, _MM_SWIZZLE(res_3, W, W, W, W));

				_mm_stream_ps((float *)d, temp);

				s += src_stride;
				d += ret_stride;
		}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::proj(*(vec3 *)d, m, *(const vec3 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}
	UNIGINE_INLINE void projMat4Vec4(void *ret, int ret_stride, const mat4 &m, const void *src, int src_stride, int num)
	{
		assert((size_t)ret_stride >= sizeof(vec4));
		assert((size_t)src_stride >= sizeof(vec4));

		#ifdef USE_SSE
			UNIGINE_ASSERT_ALIGNED16(ret);
			UNIGINE_ASSERT_ALIGNED16(ret_stride);
			UNIGINE_ASSERT_ALIGNED16(src);
			UNIGINE_ASSERT_ALIGNED16(src_stride);

			const __m128 &col_0 = m.sse.v0;
			const __m128 &col_1 = m.sse.v1;
			const __m128 &col_2 = m.sse.v2;
			const __m128 &col_3 = m.sse.v3;

			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				_mm_prefetch((const char *)s + 1024, _MM_HINT_NTA);

				__m128 temp = _mm_load_ps((const float *)s);

				__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(temp, X, X, X, X));
				__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(temp, Y, Y, Y, Y));
				__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(temp, Z, Z, Z, Z));
				__m128 res_3 = _mm_mul_ps(col_3, _MM_SWIZZLE(temp, W, W, W, W));
				__m128 res_4 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
				temp = _mm_div_ps(res_4, _MM_SWIZZLE(res_4, W, W, W, W));

				_mm_stream_ps((float *)d, temp);

				s += src_stride;
				d += ret_stride;
			}
		#else
			unsigned char *d = (unsigned char *)ret;
			const unsigned char *s = (const unsigned char *)src;
			for (size_t i = num; i > 0; i--)
			{
				Math::proj(*(vec4 *)d, m, *(const vec4 *)s);

				s += src_stride;
				d += ret_stride;
			}
		#endif
	}

	// matrix multiplications
	UNIGINE_INLINE void mulMat4Mat4(vec4 *ret, const mat4 &m, const mat4 **matrices, int num)
	{
		#ifdef USE_SSE
			const __m128 &m_col0 = m.sse.v0;
			const __m128 &m_col1 = m.sse.v1;
			const __m128 &m_col2 = m.sse.v2;

			__m128 col_0_x = _MM_SWIZZLE(m_col0, X, X, X, X);
			__m128 col_0_y = _MM_SWIZZLE(m_col0, Y, Y, Y, Y);
			__m128 col_0_z = _MM_SWIZZLE(m_col0, Z, Z, Z, Z);
			__m128 col_1_x = _MM_SWIZZLE(m_col1, X, X, X, X);
			__m128 col_1_y = _MM_SWIZZLE(m_col1, Y, Y, Y, Y);
			__m128 col_1_z = _MM_SWIZZLE(m_col1, Z, Z, Z, Z);
			__m128 col_2_x = _MM_SWIZZLE(m_col2, X, X, X, X);
			__m128 col_2_y = _MM_SWIZZLE(m_col2, Y, Y, Y, Y);
			__m128 col_2_z = _MM_SWIZZLE(m_col2, Z, Z, Z, Z);
			__m128 col_3_x = vec4(0.0f, 0.0f, 0.0f, m.m03).vec;
			__m128 col_3_y = vec4(0.0f, 0.0f, 0.0f, m.m13).vec;
			__m128 col_3_z = vec4(0.0f, 0.0f, 0.0f, m.m23).vec;

			for (int i = 0; i < num; i++)
			{
				const mat4 &m = *matrices[i];

				if (i + 4 < num)
					_mm_prefetch((const char *)matrices[i + 4], _MM_HINT_NTA);

				__m128 res_0 = _mm_shuffle_ps(m.sse.v0, m.sse.v1, _MM_PERM2(X, Y, X, Y));
				__m128 res_1 = _mm_shuffle_ps(m.sse.v0, m.sse.v1, _MM_PERM2(Z, W, Z, W));
				__m128 res_2 = _mm_shuffle_ps(m.sse.v2, m.sse.v3, _MM_PERM2(X, Y, X, Y));
				__m128 res_3 = _mm_shuffle_ps(m.sse.v2, m.sse.v3, _MM_PERM2(Z, W, Z, W));
				__m128 col_0 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(X, Z, X, Z));
				__m128 col_1 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(Y, W, Y, W));
				__m128 col_2 = _mm_shuffle_ps(res_1, res_3, _MM_PERM2(X, Z, X, Z));

				res_0 = _mm_mul_ps(col_0, col_0_x);
				res_1 = _mm_mul_ps(col_1, col_1_x);
				res_2 = _mm_mul_ps(col_2, col_2_x);
				_mm_store_ps(ret[0].v, _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3_x)));

				res_0 = _mm_mul_ps(col_0, col_0_y);
				res_1 = _mm_mul_ps(col_1, col_1_y);
				res_2 = _mm_mul_ps(col_2, col_2_y);
				_mm_store_ps(ret[1].v, _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3_y)));

				res_0 = _mm_mul_ps(col_0, col_0_z);
				res_1 = _mm_mul_ps(col_1, col_1_z);
				res_2 = _mm_mul_ps(col_2, col_2_z);
				_mm_store_ps(ret[2].v, _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, col_3_z)));

				ret += 3;
			}
		#else
			float m00 = m.m00;
			float m01 = m.m01;
			float m02 = m.m02;
			float m03 = m.m03;
			float m10 = m.m10;
			float m11 = m.m11;
			float m12 = m.m12;
			float m13 = m.m13;
			float m20 = m.m20;
			float m21 = m.m21;
			float m22 = m.m22;
			float m23 = m.m23;

			for (int i = 0; i < num; i++)
			{
				const mat4 *m = matrices[i];

				ret[0].x = m00 * m->m00 + m01 * m->m10 + m02 * m->m20;
				ret[0].y = m00 * m->m01 + m01 * m->m11 + m02 * m->m21;
				ret[0].z = m00 * m->m02 + m01 * m->m12 + m02 * m->m22;
				ret[0].w = m00 * m->m03 + m01 * m->m13 + m02 * m->m23 + m03;

				ret[1].x = m10 * m->m00 + m11 * m->m10 + m12 * m->m20;
				ret[1].y = m10 * m->m01 + m11 * m->m11 + m12 * m->m21;
				ret[1].z = m10 * m->m02 + m11 * m->m12 + m12 * m->m22;
				ret[1].w = m10 * m->m03 + m11 * m->m13 + m12 * m->m23 + m13;

				ret[2].x = m20 * m->m00 + m21 * m->m10 + m22 * m->m20;
				ret[2].y = m20 * m->m01 + m21 * m->m11 + m22 * m->m21;
				ret[2].z = m20 * m->m02 + m21 * m->m12 + m22 * m->m22;
				ret[2].w = m20 * m->m03 + m21 * m->m13 + m22 * m->m23 + m23;

				ret += 3;
			}
		#endif
	}
	UNIGINE_INLINE void mulMat4Mat4(vec4 *ret, const dmat4 &m, const dmat4 **matrices, int num)
	{
		double m00 = m.m00;
		double m01 = m.m01;
		double m02 = m.m02;
		double m03 = m.m03;
		double m10 = m.m10;
		double m11 = m.m11;
		double m12 = m.m12;
		double m13 = m.m13;
		double m20 = m.m20;
		double m21 = m.m21;
		double m22 = m.m22;
		double m23 = m.m23;

		for (int i = 0; i < num; i++)
		{
			const dmat4 &m = *matrices[i];

			ret[0].x = (float)(m00 * m.m00 + m01 * m.m10 + m02 * m.m20);
			ret[0].y = (float)(m00 * m.m01 + m01 * m.m11 + m02 * m.m21);
			ret[0].z = (float)(m00 * m.m02 + m01 * m.m12 + m02 * m.m22);
			ret[0].w = (float)(m00 * m.m03 + m01 * m.m13 + m02 * m.m23 + m03);

			ret[1].x = (float)(m10 * m.m00 + m11 * m.m10 + m12 * m.m20);
			ret[1].y = (float)(m10 * m.m01 + m11 * m.m11 + m12 * m.m21);
			ret[1].z = (float)(m10 * m.m02 + m11 * m.m12 + m12 * m.m22);
			ret[1].w = (float)(m10 * m.m03 + m11 * m.m13 + m12 * m.m23 + m13);

			ret[2].x = (float)(m20 * m.m00 + m21 * m.m10 + m22 * m.m20);
			ret[2].y = (float)(m20 * m.m01 + m21 * m.m11 + m22 * m.m21);
			ret[2].z = (float)(m20 * m.m02 + m21 * m.m12 + m22 * m.m22);
			ret[2].w = (float)(m20 * m.m03 + m21 * m.m13 + m22 * m.m23 + m23);

			ret += 3;
		}
	}
};

}
}