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

#ifdef _MSC_VER
	#pragma warning( disable : 4458)
	#pragma warning( disable : 4457)
	#pragma warning( disable : 4456)
#endif

#include "UnigineBase.h"
#include "UniginePair.h"
#include "UnigineString.h"
#include "UnigineVector.h"
#include "UnigineSort.h"
#include <stdint.h>

#ifndef USE_SSE
	#define USE_SSE
#endif

#ifdef USE_SSE
	#include <emmintrin.h>
	#include <xmmintrin.h>
	#include <smmintrin.h>
#endif

#ifdef min
	#undef min
#endif
#ifdef max
	#undef max
#endif

#ifdef USE_SSE
	#define _MM_PERM2_X 0
	#define _MM_PERM2_Y 1
	#define _MM_PERM2_Z 2
	#define _MM_PERM2_W 3
	#define _MM_PERM2(X, Y, Z, W) _MM_SHUFFLE(_MM_PERM2_##W, _MM_PERM2_##Z, _MM_PERM2_##Y, _MM_PERM2_##X)
	#define _MM_SWIZZLE(V, X, Y, Z, W) _mm_shuffle_ps(V, V, _MM_PERM2(X, Y, Z, W))
	#define _MM_PERM22(X, Y) _MM_SHUFFLE2(_MM_PERM2_##Y, _MM_PERM2_##X)
	#define _MM_SWIZZLE2(V, X, Y) _mm_shuffle_pd(V, V, _MM_PERM22(X, Y))
#endif


namespace Unigine
{

namespace Math
{

//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////
namespace Consts
{
	constexpr float PI = 3.141592654f;
	constexpr float PI2 = 6.283185308f;
	constexpr float PI05 = 1.570796327f;
	constexpr float IPI2 = 0.159154943f;
	constexpr float LOG2 = 0.693147181f;
	constexpr float LOG10 = 2.302585093f;
	constexpr float SQRT2 = 1.414213562f;
	constexpr float DEG2RAD = (PI / 180.0f);
	constexpr float RAD2DEG = (180.0f / PI);
	constexpr float EPS = 1e-6f;
	constexpr float INF = 1e+9f;
	constexpr float GOLDEN_RATIO = 1.618033989f;
	constexpr float GRAVITY = 9.80665f;

	constexpr double EPS_D = 1e-7;
	constexpr double DEG2RAD_D = 0.01745329251994329577;
	constexpr double RAD2DEG_D = 57.29577951308232087685;
	constexpr double IPI2_D = 0.15915494309189533576;
	constexpr double PI_D = 3.14159265358979323846;
	constexpr double PI05_D = 1.57079632679489661923;
}

enum AXIS
{
	AXIS_X = 0,	// positive axes
	AXIS_Y,
	AXIS_Z,
	AXIS_NX,	// negative axes
	AXIS_NY,
	AXIS_NZ,
};

struct half;
struct vec2;
struct vec3;
struct vec4;
struct dvec2;
struct dvec3;
struct dvec4;
struct hvec2;
struct hvec3;
struct hvec4;
struct ivec2;
struct ivec3;
struct ivec4;
struct bvec4;
struct svec4;
struct mat2;
struct mat3;
struct mat4;
struct dmat4;
struct quat;

#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
	typedef double Scalar;
	typedef Unigine::Math::dvec2 Vec2;
	typedef Unigine::Math::dvec3 Vec3;
	typedef Unigine::Math::dvec4 Vec4;
	typedef Unigine::Math::dmat4 Mat4;
#else
	typedef float Scalar;
	typedef Unigine::Math::vec2 Vec2;
	typedef Unigine::Math::vec3 Vec3;
	typedef Unigine::Math::vec4 Vec4;
	typedef Unigine::Math::mat4 Mat4;
#endif

// Half float type
struct half
{
public:
	unsigned short h;

	UNIGINE_INLINE constexpr half(): h(0) {}
	UNIGINE_INLINE constexpr half(const half &h) : h(h.h) {}
	UNIGINE_INLINE constexpr explicit half(int i): h((unsigned short)i) {}
	UNIGINE_INLINE explicit half(float f) { setFloat(f); }

	UNIGINE_INLINE half &operator=(half v) { h = v.h; return *this; }
	UNIGINE_INLINE half &operator=(float f) { setFloat(f); return *this; }

	UNIGINE_INLINE operator float() const { return getFloat(); }
	UNIGINE_INLINE operator unsigned short() const { return h; }

	UNIGINE_INLINE void setFloat(float f)
	{
		union
		{
			float f;
			unsigned int i;
		} value = {f};
		unsigned int i = value.i;
		unsigned int e = (i >> 23) & 0x00ff;
		unsigned int m = i & 0x007fffff;
		if (e <= 127 - 15)
			h = (unsigned short)(((m | 0x00800000) >> (127 - 14 - e)) >> 13);
		else
			h = (unsigned short)(i >> 13) & 0x3fff;
		h |= (unsigned short)(i >> 16) & 0xc000;
	}
	UNIGINE_INLINE float getFloat() const
	{
		unsigned int i = (h << 16) & 0x80000000;
		unsigned int em = h & 0x7fff;
		if (em > 0x03ff)
		{
			i |= (em << 13) + ((127 - 15) << 23);
		} else
		{
			unsigned int m = em & 0x03ff;
			if (m != 0)
			{
				unsigned int e = (em >> 10) & 0x1f;
				while ((m & 0x0400) == 0)
				{
					m <<= 1;
					e--;
				}

				m &= 0x3ff;
				i |= ((e + (127 - 14)) << 23) | (m << 13);
			}
		}

		union
		{
			unsigned int i;
			float f;
		} value = {i};
		return value.f;
	}
};

//////////////////////////////////////////////////////////////////////////
// Type conversion
//////////////////////////////////////////////////////////////////////////

union IntFloat
{
	IntFloat() {}
	IntFloat(int i): i(i) {}
	IntFloat(float f): f(f) {}
	IntFloat(unsigned int ui): ui(ui) {}

	int i;
	float f;
	unsigned int ui;
};
union LongDouble
{
	LongDouble() {}
	LongDouble(double d): d(d) {}
	LongDouble(long long l): l(l) {}
	LongDouble(unsigned long long ul): ul(ul) {}

	int i[2];
	double d;
	long long l;
	unsigned int ui[2];
	unsigned long long ul;
};

UNIGINE_INLINE int floatToIntBits(float v) { IntFloat i = v; return i.i; }
UNIGINE_INLINE unsigned int floatToUIntBits(float v) { IntFloat i = v; return i.ui; }
UNIGINE_INLINE float intBitsToFloat(int v) { IntFloat i = v; return i.f; }
UNIGINE_INLINE float intBitsToFloat(unsigned int v) { IntFloat i = v; return i.f; }

UNIGINE_INLINE long long floatToLongBits(double v) { LongDouble i = v; return i.l; }
UNIGINE_INLINE unsigned long long floatToULongBits(double v) { LongDouble i = v; return i.ul; }
UNIGINE_INLINE double longBitsToFloat(long long v) { LongDouble i = v; return i.d; }
UNIGINE_INLINE double longBitsToFloat(unsigned long long v) { LongDouble i = v; return i.d; }

UNIGINE_INLINE unsigned long long doubleIntToLong(unsigned int a1, unsigned int a2)
{
	unsigned long long b = a2;
	b = b << 32;
	b = b | a1;
	return b;
}

UNIGINE_INLINE constexpr float itof(int v) { return static_cast<float>(v); }
UNIGINE_INLINE constexpr int ftoi(float v) { return static_cast<int>(v); }
UNIGINE_INLINE constexpr char ftoc(float v) { return static_cast<char>(v); }
UNIGINE_INLINE constexpr double itod(int v) { return static_cast<double>(v); }
UNIGINE_INLINE constexpr int dtoi(double v) { return static_cast<int>(v); }
UNIGINE_INLINE constexpr float dtof(double v) { return static_cast<float>(v); }
UNIGINE_INLINE constexpr Scalar itos(int v) { return static_cast<Scalar>(v); }
UNIGINE_INLINE constexpr int stoi(Scalar v) { return static_cast<int>(v); }
UNIGINE_INLINE constexpr float ltof(long long v) { return static_cast<float>(v); }
UNIGINE_INLINE constexpr long long ftol(float v) { return static_cast<long long>(v); }
UNIGINE_INLINE constexpr double ltod(long long v) { return static_cast<double>(v); }
UNIGINE_INLINE constexpr long long dtol(double v) { return static_cast<long long>(v); }

UNIGINE_INLINE constexpr float toFloat(double value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(long double value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(char value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(int value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(long long value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(unsigned int value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(unsigned char value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(size_t value) { return static_cast<float>(value); }
UNIGINE_INLINE constexpr float toFloat(bool value) { return value ? 1.0f : 0.0f; }
UNIGINE_INLINE float toFloat(half value) { return value.getFloat(); }
UNIGINE_INLINE float toFloat(const String &value) { return value.getFloat(); }
UNIGINE_INLINE float toFloat(const char *value) { return String::atof(value); }

UNIGINE_INLINE half toHalf(float value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(double value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(long double value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(char value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(int value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(long long value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(unsigned int value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(unsigned char value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(bool value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(const String &value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(const char *value) { return half(toFloat(value)); }
UNIGINE_INLINE half toHalf(size_t value) { return half(toFloat(value)); }

UNIGINE_INLINE constexpr double toDouble(float value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(char value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(int value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(long long value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(unsigned int value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(unsigned char value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(size_t value) { return static_cast<double>(value); }
UNIGINE_INLINE constexpr double toDouble(bool value) { return value ? 1.0 : 0.0; }
UNIGINE_INLINE double toDouble(half value) { return static_cast<double>(value.getFloat()); }
UNIGINE_INLINE double toDouble(const String &value) { return value.getDouble(); }
UNIGINE_INLINE double toDouble(const char *value) { return String::atod(value); }

UNIGINE_INLINE constexpr Scalar toScalar(double value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(long double value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(float value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(char value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(int value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(long long value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(unsigned int value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(unsigned char value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(size_t value) { return static_cast<Scalar>(value); }
UNIGINE_INLINE constexpr Scalar toScalar(bool value) { return value ? 1.0f : 0.0f; }
UNIGINE_INLINE Scalar toScalar(half value) { return static_cast<Scalar>(value.getFloat()); }
UNIGINE_INLINE Scalar toScalar(const String &value) { return static_cast<Scalar>(value.getDouble()); }
UNIGINE_INLINE Scalar toScalar(const char *value) { return static_cast<Scalar>(String::atod(value)); }

UNIGINE_INLINE constexpr int toInt(float value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(double value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(char value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(long long value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(unsigned char value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(unsigned int value) { return static_cast<int>(value); }
UNIGINE_INLINE constexpr int toInt(size_t value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr int toInt(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE int toInt(half value) { return static_cast<int>(value.getFloat()); }
UNIGINE_INLINE int toInt(const String &value) { return value.getInt(); }
UNIGINE_INLINE int toInt(const char *value) { return String::atoi(value); }

UNIGINE_INLINE constexpr unsigned int toUInt(float value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(double value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(char value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(int value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(long long value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(unsigned char value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(size_t value) { return static_cast<unsigned int>(value); }
UNIGINE_INLINE constexpr unsigned int toUInt(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE unsigned int toUInt(half value) { return static_cast<unsigned int>(value.getFloat()); }
UNIGINE_INLINE unsigned int toUInt(const String &value) { return toUInt(value.getInt()); }
UNIGINE_INLINE unsigned int toUInt(const char *value) { return toUInt(String::atoi(value)); }

UNIGINE_INLINE constexpr char toChar(float value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(double value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(int value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(long long value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(unsigned char value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(unsigned int value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(size_t value) { return static_cast<char>(value); }
UNIGINE_INLINE constexpr char toChar(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE char toChar(half value) { return static_cast<char>(value.getFloat()); }
UNIGINE_INLINE char toChar(const String &value) { return toChar(value.getInt()); }
UNIGINE_INLINE char toChar(const char *value) { return toChar(String::atoi(value)); }

UNIGINE_INLINE constexpr short toShort(float value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(double value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(char value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(int value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(long long value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(unsigned char value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(size_t value) { return static_cast<short>(value); }
UNIGINE_INLINE constexpr short toShort(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE short toShort(half value) { return static_cast<short>(value.getFloat()); }
UNIGINE_INLINE short toShort(const String &value) { return toShort(value.getInt()); }
UNIGINE_INLINE short toShort(const char *value) { return toShort(String::atoi(value)); }

UNIGINE_INLINE constexpr unsigned char toUChar(float value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(double value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(char value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(int value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(long long value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(size_t value) { return static_cast<unsigned char>(value); }
UNIGINE_INLINE constexpr unsigned char toUChar(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE unsigned char toUChar(half value) { return static_cast<unsigned char>(value.getFloat()); }
UNIGINE_INLINE unsigned char toUChar(const String &value) { return toUChar(value.getInt()); }
UNIGINE_INLINE unsigned char toUChar(const char *value) { return toUChar(String::atoi(value)); }

UNIGINE_INLINE constexpr unsigned short toUShort(float value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(double value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(char value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(int value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(long long value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(unsigned char value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(size_t value) { return static_cast<unsigned short>(value); }
UNIGINE_INLINE constexpr unsigned short toUShort(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE unsigned short toUShort(half value) { return static_cast<unsigned short>(value.getFloat()); }
UNIGINE_INLINE unsigned short toUShort(const String &value) { return toUShort(value.getInt()); }
UNIGINE_INLINE unsigned short toUShort(const char *value) { return toUShort(String::atoi(value)); }

UNIGINE_INLINE constexpr unsigned long long toULong(float value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(double value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(char value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(int value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(long long value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(unsigned char value) { return static_cast<unsigned long long>(value); }
UNIGINE_INLINE constexpr unsigned long long toULong(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE unsigned long long toULong(half value) { return static_cast<unsigned long long>(value.getFloat()); }
UNIGINE_INLINE unsigned long long toULong(const String &value) { return toULong(value.getInt()); }
UNIGINE_INLINE unsigned long long toULong(const char *value) { return toULong(String::atoi(value)); }

UNIGINE_INLINE constexpr long long toLong(float value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(double value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(char value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(int value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(unsigned long long value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(unsigned char value) { return static_cast<long long>(value); }
UNIGINE_INLINE constexpr long long toLong(bool value) { return value ? 1 : 0; }
UNIGINE_INLINE long long toLong(half value) { return static_cast<long long>(value.getFloat()); }
UNIGINE_INLINE long long toLong(const String &value) { return toLong(value.getInt()); }
UNIGINE_INLINE long long toLong(const char *value) { return toLong(String::atoi(value)); }

UNIGINE_INLINE constexpr bool toBool(float value) { return toInt(value) != 0; }
UNIGINE_INLINE constexpr bool toBool(double value) { return toInt(value) != 0.0; }
UNIGINE_INLINE constexpr bool toBool(int value) { return value != 0; }
UNIGINE_INLINE constexpr bool toBool(char value) { return value != 0; }
UNIGINE_INLINE constexpr bool toBool(long long value) { return value != 0; }
UNIGINE_INLINE constexpr bool toBool(unsigned char value) { return value != 0; }
UNIGINE_INLINE constexpr bool toBool(unsigned int value) { return value != 0; }
UNIGINE_INLINE constexpr bool toBool(size_t value) { return value != 0; }
UNIGINE_INLINE bool toBool(half value) { return toInt(value) != 0.0; }
UNIGINE_INLINE bool toBool(const String &value) { return value.getBool(); }
UNIGINE_INLINE bool toBool(const char *value) { return String::atob(value); }

//////////////////////////////////////////////////////////////////////////
// Bits
//////////////////////////////////////////////////////////////////////////

template <class Type>
struct Bits
{
public:
	enum
	{
		Capacity = sizeof(Type) * 8
	};

	UNIGINE_INLINE Bits(): data(0) {}
	UNIGINE_INLINE Bits(Type data_) : data(data_) {}
	UNIGINE_INLINE ~Bits() {}

	UNIGINE_INLINE void set(int index, int value)
	{
		assert(index < Capacity && "Bits::set(): bad index");
		if (value)
			data |= 1ULL << index;
		else
			data &= ~(1ULL << index);
	}
	UNIGINE_INLINE void set(int index, bool value)
	{
		assert(index < Capacity && "Bits::set(): bad index");
		if (value)
			data |= 1ULL << index;
		else
			data &= ~(1ULL << index);
	}

	UNIGINE_INLINE bool get(int index) const
	{
		assert(index < Capacity && "Bits::get(): bad index");
		return ((data >> index) & 0x01) != 0;
	}
	UNIGINE_INLINE bool operator[](int index) const { return get(index); }

	UNIGINE_INLINE void set(Type mask) { data = mask; }
	UNIGINE_INLINE Type get() const { return data; }

	UNIGINE_INLINE int size() const { return (int)Capacity; }
	UNIGINE_INLINE void clear() { data = 0; }

	Type data;
};

template <int Capacity = 128>
class BitStream
{
public:
	UNIGINE_INLINE BitStream()
		: length(0)
		, buffer(0)
		, num_bits(0)
	{	}
	UNIGINE_INLINE ~BitStream() {}

	UNIGINE_INLINE const unsigned char *get() const { return data; }
	UNIGINE_INLINE unsigned char *get() { return data; }
	UNIGINE_INLINE int size() const { return length + 1; }

	UNIGINE_INLINE void clear()
	{
		length = 0;
		buffer = 0;
		num_bits = 0;
	}

	UNIGINE_INLINE void align()
	{
		buffer = 0;
		num_bits = 0;
	}

	// unsigned bits
	int readUBits(int num)
	{
		int ret = 0;
		int ret_bits = 0;
		if (num > num_bits)
		{
			ret = buffer;
			num -= num_bits;
			ret_bits = num_bits;
			while (num > 8)
			{
				assert(length + 1 < Capacity && "BitStream::readUBits(): data underflow");
				ret |= data[length++] << ret_bits;
				ret_bits += 8;
				num -= 8;
			}
			if (num > 0)
			{
				num_bits = 8;
				assert(length + 1 < Capacity && "BitStream::readUBits(): data underflow");
				buffer = data[length++];
			}
		}
		if (num > 0)
		{
			ret |= (buffer & ((1 << num) - 1)) << ret_bits;
			num_bits -= num;
			buffer >>= num;
		}
		return ret;
	}
	void writeUBits(int value, int num)
	{
		while (num > 8)
		{
			buffer |= (value & 0xff) << num_bits;
			assert(length + 1 < Capacity && "BitStream::writeUBits(): data overflow");
			data[length++] = buffer;
			buffer >>= 8;
			value >>= 8;
			num -= 8;
		}
		if (num > 0)
		{
			buffer |= value << num_bits;
			data[length] = buffer;
			num_bits += num;
			if (num_bits >= 8)
			{
				assert(length + 1 < Capacity && "BitStream::writeUBits(): data overflow");
				data[length++] = buffer;
				num_bits -= 8;
				buffer >>= 8;
			}
		}
	}

	// signed bits
	UNIGINE_INLINE int readBits(int num)
	{
		int ret = readUBits(num);
		if (ret & (1 << (num - 1)))
			return ret - (1 << num);
		return ret;
	}
	UNIGINE_INLINE void writeBits(int value, int num)
	{
		if (value < 0)
			value += (1 << num);
		writeUBits(value, num);
	}

private:
	int length;
	int buffer;
	int num_bits;
	unsigned char data[Capacity];
};

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE float fsqrt(float v) { return ::sqrtf(v); }
UNIGINE_INLINE double dsqrt(double v) { return ::sqrt(v); }
UNIGINE_INLINE Scalar ssqrt(Scalar v)
{
	#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
		return dsqrt(v);
	#else
		return fsqrt(v);
	#endif
}
UNIGINE_INLINE float sqrtFast(float v)
{
	IntFloat i = v;
	i.i = 0x5f3759df - (i.i >> 1);
	v = i.f * v;
	return v * (1.5f - (i.f * v * 0.5f));
}

UNIGINE_INLINE float rsqrt(float v)
{
	return (v < 1e-18f) ? Consts::INF : 1.0f / ::sqrtf(v);
}
UNIGINE_INLINE double rsqrt(double v)
{
	return (v < 1e-18) ? Consts::INF : 1.0 / ::sqrt(v);
}
UNIGINE_INLINE float rsqrtFast(float v)
{
	#ifdef USE_SSE
		_mm_store_ss(&v, _mm_rsqrt_ss(_mm_set_ss(v)));
		return v;
	#else
		IntFloat i = v;
		i.i = 0x5f3759df - (i.i >> 1);
		return i.f * (1.5f - (i.f * i.f * v * 0.5f));
	#endif
}

UNIGINE_INLINE float sin(float a) { return ::sinf(a); }
UNIGINE_INLINE double sin(double a) { return ::sin(a); }
UNIGINE_INLINE float sinFast(float a)
{
	a *= Consts::IPI2;
	a -= 0.5f + ::floorf(a);
	a *= 16.f * (abs(a) - 0.5f);
	a += 0.225f * a * (abs(a) - 1.0f);

	return a;
}

UNIGINE_INLINE float sinFastFixed(float a)
{
	float a2 = a * a;
	return a * (((((-2.39e-08f * a2 + 2.7526e-06f) * a2 - 1.98409e-04f) * a2 + 0.0083333315f) * a2 - 0.1666666664f) * a2 + 1.0f);
}

UNIGINE_INLINE float cos(float a) { return ::cosf(a); }
UNIGINE_INLINE double cos(double a) { return ::cos(a); }
UNIGINE_INLINE float cosFast(float a)
{
	a *= Consts::IPI2;
	a -= 0.25f + ::floorf(a + 0.25f);
	a *= 16.f * (abs(a) - 0.5f);
	a += 0.225f * a * (abs(a) - 1.0f);

	return a;
}

UNIGINE_INLINE float cosFastFixed(float a)
{
	float a2 = a * a;
	return (((((-2.605e-07f * a2 + 2.47609e-05f) * a2 - 0.0013888397f) * a2 + 0.0416666418f) * a2 - 0.4999999963f) * a2 + 1.0f);
}

UNIGINE_INLINE float tan(float a) { return ::tanf(a); }
UNIGINE_INLINE double tan(double a) { return ::tan(a); }

UNIGINE_INLINE float asin(float v) { return ::asinf(v); }
UNIGINE_INLINE double asin(double v) { return ::asin(v); }
UNIGINE_INLINE float asinFast(float v)
{
	return Consts::PI05 - (((((((-0.0012624911f * v + 0.0066700901f) * v - 0.0170881256f) * v + 0.0308918810f) * v - 0.0501743046f) * v + 0.0889789874f) * v - 0.2145988016f) * v + 1.5707963050f) * Math::fsqrt(1.0f - v);
}

UNIGINE_INLINE float acos(float v) { return ::acosf(v); }
UNIGINE_INLINE double acos(double v) { return ::acos(v); }
UNIGINE_INLINE float acosFast(float v)
{
	return (((((((-0.0012624911f * v + 0.0066700901f) * v - 0.0170881256f) * v + 0.0308918810f) * v - 0.0501743046f) * v + 0.0889789874f) * v - 0.2145988016f) * v + 1.5707963050f) * Math::fsqrt(1.0f - v);
}

UNIGINE_INLINE float atan(float v) { return ::atanf(v); }
UNIGINE_INLINE double atan(double v) { return ::atan(v); }

UNIGINE_INLINE float atan2(float y, float x) { return ::atan2f(y, x); }
UNIGINE_INLINE double atan2(double y, double x) { return ::atan2(y, x); }

UNIGINE_INLINE void sincos(float angle, float &s, float &c)
{
	#if _LINUX
		double res_0, res_1;
		asm volatile("fsincos"
					 : "=t"(res_0), "=u"(res_1)
					 : "0"(angle));
		s = (float)res_1;
		c = (float)res_0;
	#else
		s = toFloat(sin(angle));
		c = toFloat(cos(angle));
	#endif
}
UNIGINE_INLINE void sincos(double angle, double &s, double &c)
{
	#if _LINUX
		asm volatile("fsincos"
					 : "=t"(c), "=u"(s)
					 : "0"(angle));
	#else
		s = sin(angle);
		c = cos(angle);
	#endif
}
UNIGINE_INLINE void sincosFast(float a, float &s, float &c)
{
	if (a < 0.0f)
		a -= toInt(a * (1.0f / Consts::PI2)) * Consts::PI2 - Consts::PI2;
	else if (a >= Consts::PI2)
		a -= toInt(a * (1.0f / Consts::PI2)) * Consts::PI2;
	c = 1.0f;
	s = Consts::PI - a;
	if (s < -Consts::PI05)
		s = -Consts::PI - s;
	else if (s > Consts::PI05)
		s = Consts::PI - s;
	else
		c = -1.0f;
	float a2 = s * s;
	s *= ((0.00761f * a2 - 0.16605f) * a2 + 1.0f);
	c *= ((0.03705f * a2 - 0.49670f) * a2 + 1.0f);
}


UNIGINE_INLINE float sign(float v)
{
	return IntFloat((IntFloat(v).ui & 0x80000000) | 0x3f800000).f;
}
UNIGINE_INLINE double sign(double v) { return (v >= 0.0) ? 1.0 : -1.0; }
UNIGINE_INLINE int signMask(int v) { return v >> 31; }

UNIGINE_INLINE int abs(int v) { return (v >= 0) ? v : -v; }
UNIGINE_INLINE long long abs(long long v) { return (v >= 0) ? v : -v; }
UNIGINE_INLINE float abs(float v) { return (v >= 0) ? v : -v; }
UNIGINE_INLINE double abs(double v) { return (v >= 0) ? v : -v; }

UNIGINE_INLINE float round(float v) { return ::roundf(v); }
UNIGINE_INLINE double round(double v) { return ::round(v); }
UNIGINE_INLINE int roundFast(float v)
{
	// Actually this is the fastest code we could get.
	// Both SSE intrinsics and raw assembler didn't give any performance gain on supported systems.
	return static_cast<int>(v + 0.5f);
}

UNIGINE_INLINE float ceil(float v) { return ::ceilf(v); }
UNIGINE_INLINE double ceil(double v) { return ::ceil(v); }

UNIGINE_INLINE int ceilInt(float v) { return toInt(::ceilf(v)); }
UNIGINE_INLINE int ceilInt(double v) { return toInt(::ceil(v)); }

UNIGINE_INLINE float floor(float v) { return ::floorf(v); }
UNIGINE_INLINE double floor(double v) { return ::floor(v); }

UNIGINE_INLINE int floorInt(float v) { return toInt(::floorf(v)); }
UNIGINE_INLINE int floorInt(double v) { return toInt(::floor(v)); }

UNIGINE_INLINE float frac(float v) { return v - ::floorf(v); }
UNIGINE_INLINE double frac(double v) { return v - ::floor(v); }

UNIGINE_INLINE float log2(float v) { return ::logf(v) * (1.0f / Consts::LOG2); }
UNIGINE_INLINE double log2(double v) { return ::log(v) * (1.0 / Consts::LOG2); }
UNIGINE_INLINE int log2(int v)
{
	int ret = 0;
	if (v >= 1 << 16)
	{
		v >>= 16;
		ret |= 16;
	}
	if (v >= 1 << 8)
	{
		v >>= 8;
		ret |= 8;
	}
	if (v >= 1 << 4)
	{
		v >>= 4;
		ret |= 4;
	}
	if (v >= 1 << 2)
	{
		v >>= 2;
		ret |= 2;
	}
	if (v >= 1 << 1)
		ret |= 1;
	return ret;
}
UNIGINE_INLINE float log2Fast(float v)
{
	int i = IntFloat(v).i;
	int e = ((i >> 23) & 0xff) - 127;
	v = IntFloat((i & 0x007fffff) | 0x3f800000).f;
	return toFloat(e) + (v - 1.0f) * (((((-0.034436006f * v + 0.31821337f) * v - 1.2315303f) * v + 2.5988452f) * v - 3.3241990f) * v + 3.1157899f);
}

UNIGINE_INLINE float log(float v) { return ::logf(v); }
UNIGINE_INLINE double log(double v) { return ::log(v); }
UNIGINE_INLINE float logFast(float v) { return log2Fast(v) * Consts::LOG2; }

UNIGINE_INLINE float log10(float v) { return ::logf(v) * (1.0f / Consts::LOG10); }
UNIGINE_INLINE double log10(double v) { return ::log(v) * (1.0f / Consts::LOG10); }

UNIGINE_INLINE float rcp(float v) { return 1.0f / v; }
UNIGINE_INLINE double rcp(double v) { return 1.0 / v; }
UNIGINE_INLINE float rcpFast(float v)
{
#ifdef USE_SSE
	_mm_store_ss(&v, _mm_rcp_ss(_mm_set_ss(v)));
	return v;
#else
	IntFloat i = v;
	i.i = 0x7f000000 - i.i;
	return i.f * (2.0f - v * i.f);
#endif
}

UNIGINE_INLINE float exp2(float v) { return ::expf(v * Consts::LOG2); }
UNIGINE_INLINE double exp2(double v) { return ::exp(v * Consts::LOG2); }
UNIGINE_INLINE float exp2Fast(float v)
{
	int i = toInt(v - 0.5f);
	v = v - toFloat(i);
	return IntFloat((i + 127) << 23).f * (((((0.0018775767f * v + 0.0089893397f) * v + 0.055826318f) * v + 0.24015361f) * v + 0.69315308f) * v + 0.99999994f);
}

UNIGINE_INLINE float exp(float v) { return ::expf(v); }
UNIGINE_INLINE double exp(double v) { return ::exp(v); }
UNIGINE_INLINE float expFast(float v) { return exp2Fast(v * (1.0f / Consts::LOG2)); }

UNIGINE_INLINE float pow(float x, float y) { return ::powf(x, y); }
UNIGINE_INLINE double pow(double x, double y) { return ::pow(x, y); }
UNIGINE_INLINE float powFast(float x, float y) { return exp2Fast(log2Fast(x) * y); }

UNIGINE_INLINE float pow2(float x) { return x * x; }
UNIGINE_INLINE double pow2(double x) { return x * x; }
UNIGINE_INLINE int pow2(int x) { return x * x; }

UNIGINE_INLINE float srgb(float x) { return pow(x, 2.2f); }
UNIGINE_INLINE float isrgb(float x) { return pow(x, 1.0f / 2.2f); }

UNIGINE_INLINE float mod(float x, float y) { return ::fmodf(x, y); }
UNIGINE_INLINE double mod(double x, double y) { return ::fmod(x, y); }

UNIGINE_INLINE float mad(float a, float b, float c) { return a * b + c; }
UNIGINE_INLINE double mad(double a, double b, double c) { return a * b + c; }

UNIGINE_INLINE int select(int c, int v0, int v1)
{
	int mask = signMask(c | -c);
	return (v0 & mask) | (v1 & ~mask);
}
UNIGINE_INLINE float select(int c, float v0, float v1)
{
	int mask = signMask(c | -c);
	return IntFloat((IntFloat(v0).i & mask) | (IntFloat(v1).i & ~mask)).f;
}
UNIGINE_INLINE float select(float c, float v0, float v1)
{
	int mask = signMask(IntFloat(c).i);
	return IntFloat((IntFloat(v0).i & mask) | (IntFloat(v1).i & ~mask)).f;
}

UNIGINE_INLINE int npot(int v)
{
	int i = 1;
	while (i < v)
		i += i;
	return i;
}
UNIGINE_INLINE int ispot(int v) { return ((v - 1) & v) == 0; }
UNIGINE_INLINE int isqrt(int v)
{
	int ret = 0;
	int bit = 1 << 30;
	for (; bit > v; bit >>= 2)
		;
	for (; bit != 0; bit >>= 2)
	{
		if (v >= ret + bit)
		{
			v -= ret + bit;
			ret = (ret >> 1) + bit;
		} else
			ret = ret >> 1;
	}
	return ret;
}

UNIGINE_INLINE int udiv(int x, int y) { return x / y + (x % y != 0); }

UNIGINE_INLINE int compare(float v0, float v1, float epsilon = Consts::EPS) { return abs(v0 - v1) < epsilon; }
UNIGINE_INLINE int compare(double v0, double v1, double epsilon = Consts::EPS) { return abs(v0 - v1) < epsilon; }
UNIGINE_INLINE int compare(int v0, int v1) { return v0 == v1; }
UNIGINE_INLINE int compare(long long v0, long long v1) { return v0 == v1; }

template <typename T>
UNIGINE_INLINE int compare(const Pair<T, T> &p0, const Pair<T, T> &p1)
{
	return compare(p0.first, p1.first) && compare(p0.second, p1.second);
}

UNIGINE_INLINE float min(float v0, float v1) { return (v0 < v1) ? v0 : v1; }
UNIGINE_INLINE double min(double v0, double v1) { return (v0 < v1) ? v0 : v1; }
UNIGINE_INLINE int min(int v0, int v1) { return (v0 < v1) ? v0 : v1; }
UNIGINE_INLINE char min(char v0, char v1) { return (v0 < v1) ? v0 : v1; }
UNIGINE_INLINE size_t min(size_t v0, size_t v1) { return (v0 < v1) ? v0 : v1; }
UNIGINE_INLINE long long min(long long v0, long long v1) { return (v0 < v1) ? v0 : v1; }

UNIGINE_INLINE float max(float v0, float v1) { return (v0 > v1) ? v0 : v1; }
UNIGINE_INLINE double max(double v0, double v1) { return (v0 > v1) ? v0 : v1; }
UNIGINE_INLINE int max(int v0, int v1) { return (v0 > v1) ? v0 : v1; }
UNIGINE_INLINE char max(char v0, char v1) { return (v0 > v1) ? v0 : v1; }
UNIGINE_INLINE size_t max(size_t v0, size_t v1) { return (v0 > v1) ? v0 : v1; }
UNIGINE_INLINE long long max(long long v0, long long v1) { return (v0 > v1) ? v0 : v1; }

UNIGINE_INLINE float clamp(float v, float v0, float v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }
UNIGINE_INLINE double clamp(double v, double v0, double v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }
UNIGINE_INLINE int clamp(int v, int v0, int v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }
UNIGINE_INLINE char clamp(char v, char v0, char v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }
UNIGINE_INLINE size_t clamp(size_t v, size_t v0, size_t v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }
UNIGINE_INLINE long long clamp(long long v, long long v0, long long v1) { return (v < v0) ? v0 : ((v > v1) ? v1 : v); }

UNIGINE_INLINE float saturate(float v) { return (v < 0.0f) ? 0.0f : ((v > 1.0f) ? 1.0f : v); }
UNIGINE_INLINE double saturate(double v) { return (v < 0.0) ? 0.0 : ((v > 1.0) ? 1.0 : v); }

UNIGINE_INLINE float lerpOne(float v0, float k) { return v0 * (1.0f - k) + k; }
UNIGINE_INLINE double lerpOne(double v0, double k) { return v0 * (1.0 - k) + k; }

UNIGINE_INLINE float lerpZero(float v0, float k) { return v0 * (1.0f - k); }
UNIGINE_INLINE double lerpZero(double v0, double k) { return v0 * (1.0 - k); }

UNIGINE_INLINE float lerp(float v0, float v1, float k) { return v0 + (v1 - v0) * k; }
UNIGINE_INLINE double lerp(double v0, double v1, double k) { return v0 + (v1 - v0) * k; }
UNIGINE_INLINE int lerp(int v0, int v1, int k) { return v0 + (((v1 - v0) * k) >> 16); }
UNIGINE_INLINE long long lerp(long long v0, long long v1, long long k) { return v0 + (v1 - v0) * k; }

UNIGINE_INLINE float inverseLerp(float v0, float v1, float v) { return saturate((v - v0) / (v1 - v0)); }
UNIGINE_INLINE double inverseLerp(double v0, double v1, double v) { return saturate((v - v0) / (v1 - v0)); }

UNIGINE_INLINE bool step(float a, float b) { return a >= b; }

UNIGINE_INLINE float overlay(float a, float b, float x) { return max(lerp(a, lerp(1.0f - 2.0f * (1.0f - a) * (1.0f - b), 2.0f * a * b, toFloat(step(a, 0.5f))), x), 0.0f); }

UNIGINE_INLINE float smoothstep(float x) { return x * x * (3.0f - 2.0f * x); }
UNIGINE_INLINE double smoothstep(double x) { return x * x * (3.0 - 2.0 * x); }

UNIGINE_INLINE float smoothstep(float edge0, float edge1, float x)
{
	float t = saturate((x - edge0) / (edge1 - edge0));
	return t * t * (3.0f - 2.0f * t);
}
UNIGINE_INLINE double smoothstep(double edge0, double edge1, double x)
{
	double t = saturate((x - edge0) / (edge1 - edge0));
	return t * t * (3.0 - 2.0 * t);
}

UNIGINE_INLINE float smootherstep(float x) { return x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f); }
UNIGINE_INLINE double smootherstep(double x) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }

UNIGINE_INLINE float smootherstep(float edge0, float edge1, float x)
{
	float t = saturate((x - edge0) / (edge1 - edge0));
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}
UNIGINE_INLINE double smootherstep(double edge0, double edge1, double x)
{
	double t = saturate((x - edge0) / (edge1 - edge0));
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

UNIGINE_INLINE float normalizeAngle(float angle)
{
	if (angle >= 180.0f)
		angle = mod(angle + 180.0f, 360.0f) - 180.0f;
	else if (angle <= -180.0f)
		angle = -mod(-angle - 180.0f, 360.0f) + 180.0f;
	return angle;
}

UNIGINE_INLINE float *mul3(float *UNIGINE_RESTRICT ret, const float *UNIGINE_RESTRICT v0, float v1)
{
	ret[0] = v0[0] * v1;
	ret[1] = v0[1] * v1;
	ret[2] = v0[2] * v1;
	return ret;
}
UNIGINE_INLINE float *add3(float *UNIGINE_RESTRICT ret, const float *UNIGINE_RESTRICT v0, const float *UNIGINE_RESTRICT v1)
{
	ret[0] = v0[0] + v1[0];
	ret[1] = v0[1] + v1[1];
	ret[2] = v0[2] + v1[2];
	return ret;
}
UNIGINE_INLINE float *sub3(float *UNIGINE_RESTRICT ret, const float *UNIGINE_RESTRICT v0, const float *UNIGINE_RESTRICT v1)
{
	ret[0] = v0[0] - v1[0];
	ret[1] = v0[1] - v1[1];
	ret[2] = v0[2] - v1[2];
	return ret;
}
UNIGINE_INLINE float *cross3(float *UNIGINE_RESTRICT ret, const float *UNIGINE_RESTRICT v0, const float *UNIGINE_RESTRICT v1)
{
	ret[0] = v0[1] * v1[2] - v0[2] * v1[1];
	ret[1] = v0[2] * v1[0] - v0[0] * v1[2];
	ret[2] = v0[0] * v1[1] - v0[1] * v1[0];
	return ret;
}

UNIGINE_INLINE float dot2(const float *v, float x, float y)
{
	return v[0] * x + v[1] * y;
}
UNIGINE_INLINE float dot3(const float *v, float x, float y, float z)
{
	return v[0] * x + v[1] * y + v[2] * z;
}

UNIGINE_INLINE void normalize2(float *v)
{
	float ilength = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1]);
	v[0] = v[0] * ilength;
	v[1] = v[1] * ilength;
}
UNIGINE_INLINE void normalize3(float *v)
{
	float ilength = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] * ilength;
	v[1] = v[1] * ilength;
	v[2] = v[2] * ilength;
}

template <typename Type>
UNIGINE_INLINE bool getBit(Type value, int bit) { return (((value) >> (bit)) & 0x01ULL); }

UNIGINE_INLINE bool checkMask(const int mask, const int bits)
{
	return (mask & bits) != 0;
}
UNIGINE_INLINE bool checkMask(const unsigned int mask, const unsigned int bits)
{
	return (mask & bits) != 0;
}
UNIGINE_INLINE bool checkMask(const unsigned int mask, const int bits)
{
	return (mask & bits) != 0;
}
UNIGINE_INLINE bool checkMask(const int mask, const unsigned int bits)
{
	return (mask & bits) != 0;
}

UNIGINE_INLINE float reRange(float in, float in_range_min, float in_range_max, float out_range_min, float out_range_max)
{
	float temp = ((out_range_max - out_range_min) / (in_range_max - in_range_min));
	return in * temp + (out_range_min - in_range_min * temp);
}

UNIGINE_INLINE double reRange(double in, double in_range_min, double in_range_max, double out_range_min, double out_range_max)
{
	double temp = ((out_range_max - out_range_min) / (in_range_max - in_range_min));
	return in * temp + (out_range_min - in_range_min * temp);
}

constexpr bool checkRange(double value, double range_min, double range_max) { return value >= range_min && value <= range_max; }
constexpr bool checkRange(float value, float range_min, float range_max) { return value >= range_min && value <= range_max; }
constexpr bool checkRange(long long value, long long range_min, long long range_max) { return value >= range_min && value <= range_max; }
constexpr bool checkRange(int value, int range_min, int range_max) { return value >= range_min && value <= range_max; }
constexpr bool checkRange(short value, short range_min, short range_max) { return value >= range_min && value <= range_max; }
constexpr bool checkRange(char value, char range_min, char range_max) { return value >= range_min && value <= range_max; }

// bezier curve 
UNIGINE_INLINE float bezier_func(const float *curve, float k0)
{
	float k1 = 1.0f - k0;
	float k00 = k0 * k0;
	float k11 = k1 * k1;
	return curve[0] * k11 * k1 + (curve[1] * k11 * k0 + curve[2] * k00 * k1) * 3.0f + curve[3] * k00 * k0;
}
UNIGINE_INLINE double bezier_func(const double *curve, double k0)
{
	double k1 = 1.0 - k0;
	double k00 = k0 * k0;
	double k11 = k1 * k1;
	return curve[0] * k11 * k1 + (curve[1] * k11 * k0 + curve[2] * k00 * k1) * 3.0 + curve[3] * k00 * k0;
}
UNIGINE_INLINE float bezier(const float *times, const float *values, float time)
{
	float k0 = 0.0f;
	float k1 = 1.0f;
	float t1 = bezier_func(times, k1) - time;
	if (Math::abs(t1) < Consts::EPS)
		return bezier_func(values, k1);
	float t0 = bezier_func(times, k0) - time;
	if (Math::abs(t0) < Consts::EPS || t0 * t1 > 0.0f)
		return bezier_func(values, k0);
	for (int i = 0; i < 20; i++)
	{
		float k = (k0 + k1) * 0.5f;
		float t = bezier_func(times, k) - time;
		float v = t0 * t;
		if (v < 0.0f)
		{
			k1 = k;
			t1 = t;
		} else if (v > 0.0f)
		{
			k0 = k;
			t0 = t;
		} else
			return bezier_func(values, k);
	}
	float k = (k0 + k1) * 0.5f;
	return bezier_func(values, k);
}
UNIGINE_INLINE double bezier(const float *times, const double *values, float time)
{
	float k0 = 0.0f;
	float k1 = 1.0f;
	float t1 = bezier_func(times, k1) - time;
	if (Math::abs(t1) < Consts::EPS)
		return bezier_func(values, k1);
	float t0 = bezier_func(times, k0) - time;
	if (Math::abs(t0) < Consts::EPS || t0 * t1 > 0.0f)
		return bezier_func(values, k0);
	for (int i = 0; i < 20; i++)
	{
		float k = (k0 + k1) * 0.5f;
		float t = bezier_func(times, k) - time;
		float v = t0 * t;
		if (v < 0.0f)
		{
			k1 = k;
			t1 = t;
		} else if (v > 0.0f)
		{
			k0 = k;
			t0 = t;
		} else
			return bezier_func(values, k);
	}
	float k = (k0 + k1) * 0.5f;
	return bezier_func(values, k);
}

// 32bit hash
UNIGINE_INLINE unsigned int hashInteger(unsigned int v)
{
	v ^= 0xdeadbeef;
	v ^= v >> 16;
	v *= 0x85ebca6b;
	v ^= v >> 13;
	v *= 0xc2b2ae35;
	v ^= v >> 16;
	return v;
}

UNIGINE_INLINE unsigned int hashMixer(unsigned int hash_0, unsigned int hash_1)
{
	hash_0 ^= 0x85ebca6b;
	hash_0 = hash_0 ^ ((hash_1 ^ 0xc2b2ae35) + 0x9e3779b9 + ((hash_1 ^ 0x85ebca6b) << 16) + (hash_1 >> 12));
	hash_0 = hash_0 * 1664525 + 1013904223;
	return hash_0;
}
UNIGINE_INLINE unsigned int hashCombine(unsigned int hash, unsigned int value)
{
	return hashMixer(hash, hashInteger(value));
}
UNIGINE_INLINE unsigned int hashInteger(float value) { return hashInteger(floatToUIntBits(value)); }
UNIGINE_INLINE unsigned int hashCombine(unsigned int hash, float value) { return hashCombine(hash, floatToUIntBits(value)); }
UNIGINE_INLINE unsigned int hashInteger(int value) { return hashInteger(toUInt(value)); }
UNIGINE_INLINE unsigned int hashCombine(unsigned int hash, int value) { return hashCombine(hash, toUInt(value)); }

// 64bit hash
UNIGINE_INLINE unsigned long long hashInteger(unsigned long long v)
{
	v ^= 0x2127599bf4325c37ULL;
	v ^= v >> 33;
	v *= 0xff51afd7ed558ccdULL;
	v ^= v >> 33;
	v *= 0xc4ceb9fe1a85ec53ULL;
	v ^= v >> 33;
	return v;
}
UNIGINE_INLINE unsigned long long hashMixer(unsigned long long hash_0, unsigned long long hash_1)
{
	hash_0 ^= 0xc4ceb9fe1a85ec53ULL;
	hash_0 = hash_0 ^ ((hash_1 ^ 0xff51afd7ed558ccdULL) + 0x9ddfea08eb382d69ULL + ((hash_1 ^ 0x2127599bf4325c37ULL) << 16) + (hash_1 >> 12));
	return hash_0 * 1664525 + 1013904223;
}
UNIGINE_INLINE unsigned long long hashCombine(unsigned long long hash, unsigned long long value)
{
	return hashMixer(hash, hashInteger(value));
}
UNIGINE_INLINE unsigned long long hashInteger(double value) { return hashInteger(floatToULongBits(value)); }
UNIGINE_INLINE unsigned long long hashCombine(unsigned long long hash, double value) { return hashCombine(hash, floatToULongBits(value)); }
UNIGINE_INLINE unsigned long long hashInteger(long long value) { return hashInteger(toULong(value)); }
UNIGINE_INLINE unsigned long long hashCombine(unsigned long long hash, long long value) { return hashCombine(hash, toULong(value)); }

UNIGINE_INLINE unsigned int hashCast64To32(unsigned long long value) { return static_cast<unsigned int>(value % UINT_MAX); }

// alignment
template<class A, class B>
inline A makeAligned(A location, B align_value)
{
	UNIGINE_ASSERT((align_value & (align_value - 1)) == 0);
	UNIGINE_ASSERT(align_value != 0);
	return ((location + (align_value - 1)) & ~(align_value - 1));
}

// SSE
#ifdef USE_SSE
UNIGINE_INLINE __m128 _mm_rcp_ss_nr(__m128 v)
{
	__m128 iv = _mm_rcp_ss(v);
	iv = _mm_sub_ss(_mm_add_ss(iv, iv), _mm_mul_ss(v, _mm_mul_ss(iv, iv)));
	return _mm_sub_ss(_mm_add_ss(iv, iv), _mm_mul_ss(v, _mm_mul_ss(iv, iv)));
}

UNIGINE_INLINE __m128 _mm_rcp_ps_nr(__m128 v)
{
	__m128 iv = _mm_rcp_ps(v);
	iv = _mm_sub_ps(_mm_add_ps(iv, iv), _mm_mul_ps(v, _mm_mul_ps(iv, iv)));
	return _mm_sub_ps(_mm_add_ps(iv, iv), _mm_mul_ps(v, _mm_mul_ps(iv, iv)));
}

UNIGINE_INLINE __m128 _mm_rsqrt_ss_nr(__m128 v)
{
	__m128 iv = _mm_rsqrt_ss(v);
	__m128 nr = _mm_mul_ss(_mm_mul_ss(v, iv), iv);
	return _mm_mul_ss(_mm_mul_ss(_mm_set1_ps(0.5f), iv), _mm_sub_ss(_mm_set1_ps(3.0f), nr));
}

UNIGINE_INLINE __m128 _mm_rsqrt_ps_nr(__m128 v)
{
	__m128 iv = _mm_rsqrt_ps(v);
	__m128 nr = _mm_mul_ps(_mm_mul_ps(v, iv), iv);
	return _mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), iv), _mm_sub_ps(_mm_set1_ps(3.0f), nr));
}

UNIGINE_INLINE __m128 _mm_dot33_ps(__m128 v0, __m128 v1)
{
	__m128 v2 = _mm_mul_ps(v0, v1);
	__m128 v3 = _mm_add_ps(v2, _MM_SWIZZLE(v2, Y, X, Y, W));
	return _mm_add_ps(v3, _MM_SWIZZLE(v2, Z, Z, X, W));
}

UNIGINE_INLINE __m128 _mm_dot44_ps(__m128 v0, __m128 v1)
{
	__m128 v2 = _mm_mul_ps(v0, v1);
	v2 = _mm_add_ps(v2, _MM_SWIZZLE(v2, Y, X, W, Z));
	return _mm_add_ps(v2, _MM_SWIZZLE(v2, Z, W, X, Y));
}

UNIGINE_INLINE __m128 _mm_normalize3_ps(__m128 v)
{
	__m128 length2 = _mm_dot33_ps(v, v);
	return _mm_mul_ps(v, _mm_rsqrt_ps_nr(length2));
}

UNIGINE_INLINE __m128 _mm_normalize4_ps(__m128 v)
{
	__m128 length2 = _mm_dot44_ps(v, v);
	return _mm_mul_ps(v, _mm_rsqrt_ps_nr(length2));
}

UNIGINE_INLINE __m128 _mm_cross_ps(__m128 v0, __m128 v1)
{
	__m128 v0_yzxw = _MM_SWIZZLE(v0, Y, Z, X, W);
	__m128 v1_yzxw = _MM_SWIZZLE(v1, Y, Z, X, W);
	__m128 v2 = _mm_sub_ps(_mm_mul_ps(v0, v1_yzxw), _mm_mul_ps(v1, v0_yzxw));
	return _MM_SWIZZLE(v2, Y, Z, X, W);
}

#endif // USE_SSE

} // end namespace Math

namespace MathLib
{
using Math::AXIS;
}

} // end namespace Unigine