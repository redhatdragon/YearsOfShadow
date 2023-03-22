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

#include "UnigineBase.h"

namespace Unigine
{

namespace
{
static unsigned int endian(unsigned int v)
{
	v = ((v << 8) & 0xff00ff00) | ((v >> 8) & 0x00ff00ff);
	return (v << 16) | (v >> 16);
}

constexpr unsigned int crc32_table[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

}

class CRC32
{
public:
	UNIGINE_INLINE CRC32() { begin(); }
	UNIGINE_INLINE ~CRC32() {}

	UNIGINE_INLINE void begin() { value = 0xffffffff; }
	UNIGINE_INLINE void update(const void *data, int size)
	{
		const unsigned char *d = (const unsigned char *)data;
		for (int i = 0; i < size; i++)
			value = crc32_table[(value ^ (*d++)) & 0xff] ^ (value >> 8);
	}
	UNIGINE_INLINE int end()
	{
		value ^= 0xffffffff;
		return (int)value;
	}

	// calculate checksum for the single data block
	UNIGINE_INLINE static int calculate(const void *data, int size, bool big_endian = true)
	{
		CRC32 crc32;
		crc32.update(data, size);
		return big_endian ? crc32.end() : endian(crc32.end());
	}

	UNIGINE_INLINE static unsigned int calcCrc32(const void *data, int size)
	{
		unsigned int crc = ~((unsigned int)0);
		const unsigned char *p = (const unsigned char *)data;
		const unsigned char *q = p + size;
		while (p < q)
			crc = (crc >> 8) ^ crc32_table[(crc ^ *p++) & 0xff];
		return ~crc;
	}

private:
	unsigned int value;
};

class MD5
{
public:
	UNIGINE_INLINE MD5() { begin(); }
	UNIGINE_INLINE ~MD5() {}

	UNIGINE_INLINE void begin()
	{
		state[0] = 0x67452301;
		state[1] = 0xefcdab89;
		state[2] = 0x98badcfe;
		state[3] = 0x10325476;

		counter[0] = 0;
		counter[1] = 0;
	}
	void update(const void *data, int size)
	{
		const unsigned char *src = (const unsigned char *)data;

		unsigned int old_counter = counter[0];
		int old_size = (old_counter >> 3) & 0x3f;

		counter[0] += size << 3;
		counter[1] += size >> 29;

		if (counter[0] < old_counter)
			counter[1]++;

		if (old_size)
		{
			unsigned char *b = (unsigned char *)buffer + old_size;
			old_size = 64 - old_size;
			if (size < old_size)
			{
				memcpy(b, src, size);
				return;
			}
			memcpy(b, src, old_size);
			transform(state, buffer);
			size -= old_size;
			src += old_size;
		}

		while (size >= 64)
		{
			memcpy(buffer, src, 64);
			transform(state, buffer);
			size -= 64;
			src += 64;
		}

		memcpy(buffer, src, size);
	}
	void endMD5(unsigned int *value, bool big_endian)
	{
		int size = (counter[0] >> 3) & 0x3f;

		unsigned char *b = (unsigned char *)buffer + size;

		*b++ = 0x80;

		size = 63 - size;

		if (size < 8)
		{
			memset(b, 0, size);
			transform(state, buffer);
			memset(buffer, 0, 56);
		} else
			memset(b, 0, size);

		buffer[14] = counter[0];
		buffer[15] = counter[1];
		transform(state, buffer);

		for (int i = 0; i < 4; ++i)
			value[i] = big_endian ? endian(state[i]) : state[i];
	}
	void endD3D(unsigned int *value)
	{
		int size = (counter[0] >> 3) & 0x3f;

		unsigned char *b = (unsigned char *)buffer + size;

		*b++ = 0x80;

		size = 63 - size;

		if (size < 8)
		{
			memset(b, 0, size);
			transform(state, buffer);
			memset(buffer + 1, 0, 56);
		} else
		{
			memset(b, 0, size);
			memmove(buffer + 1, buffer, 56);
		}

		buffer[0] = counter[0];
		buffer[15] = (counter[0] >> 2) | 1;
		transform(state, buffer);

		value[0] = state[0];
		value[1] = state[1];
		value[2] = state[2];
		value[3] = state[3];
	}

	// calculate checksum for the single data block
	UNIGINE_INLINE static int calculate(const void *data, int size, bool big_endian = true)
	{
		unsigned int value[4];
		MD5 md5;
		md5.update(data, size);
		md5.endMD5(value, big_endian);
		return (int)(value[0] ^ value[1] ^ value[2] ^ value[3]);
	}
	UNIGINE_INLINE static void calculate(unsigned int *value, const void *data, int size, bool big_endian = true)
	{
		MD5 md5;
		md5.update(data, size);
		md5.endMD5(value, big_endian);
	}
	UNIGINE_INLINE static void calculateD3D(unsigned int *value, const void *data, int size)
	{
		MD5 md5;
		md5.update(data, size);
		md5.endD3D(value);
	}

private:
	void transform(unsigned int *state, const unsigned int *data)
	{
		unsigned int a = state[0];
		unsigned int b = state[1];
		unsigned int c = state[2];
		unsigned int d = state[3];

		#define F0(X, Y, Z) (Z ^ (X & (Y ^ Z)))
		#define F1(X, Y, Z) (Y ^ (Z & (X ^ Y)))
		#define F2(X, Y, Z) (X ^ Y ^ Z)
		#define F3(X, Y, Z) (Y ^ (X | ~Z))

		#define STEP(NUM, A, B, C, D, DATA, S) \
			{ \
				A += F##NUM(B, C, D) + DATA; \
				A = (A << S) | (A >> (32 - S)); \
				A += B; \
			}

		STEP(0, a, b, c, d, data[0] + 0xd76aa478, 7)
		STEP(0, d, a, b, c, data[1] + 0xe8c7b756, 12)
		STEP(0, c, d, a, b, data[2] + 0x242070db, 17)
		STEP(0, b, c, d, a, data[3] + 0xc1bdceee, 22)
		STEP(0, a, b, c, d, data[4] + 0xf57c0faf, 7)
		STEP(0, d, a, b, c, data[5] + 0x4787c62a, 12)
		STEP(0, c, d, a, b, data[6] + 0xa8304613, 17)
		STEP(0, b, c, d, a, data[7] + 0xfd469501, 22)
		STEP(0, a, b, c, d, data[8] + 0x698098d8, 7)
		STEP(0, d, a, b, c, data[9] + 0x8b44f7af, 12)
		STEP(0, c, d, a, b, data[10] + 0xffff5bb1, 17)
		STEP(0, b, c, d, a, data[11] + 0x895cd7be, 22)
		STEP(0, a, b, c, d, data[12] + 0x6b901122, 7)
		STEP(0, d, a, b, c, data[13] + 0xfd987193, 12)
		STEP(0, c, d, a, b, data[14] + 0xa679438e, 17)
		STEP(0, b, c, d, a, data[15] + 0x49b40821, 22)

		STEP(1, a, b, c, d, data[1] + 0xf61e2562, 5)
		STEP(1, d, a, b, c, data[6] + 0xc040b340, 9)
		STEP(1, c, d, a, b, data[11] + 0x265e5a51, 14)
		STEP(1, b, c, d, a, data[0] + 0xe9b6c7aa, 20)
		STEP(1, a, b, c, d, data[5] + 0xd62f105d, 5)
		STEP(1, d, a, b, c, data[10] + 0x02441453, 9)
		STEP(1, c, d, a, b, data[15] + 0xd8a1e681, 14)
		STEP(1, b, c, d, a, data[4] + 0xe7d3fbc8, 20)
		STEP(1, a, b, c, d, data[9] + 0x21e1cde6, 5)
		STEP(1, d, a, b, c, data[14] + 0xc33707d6, 9)
		STEP(1, c, d, a, b, data[3] + 0xf4d50d87, 14)
		STEP(1, b, c, d, a, data[8] + 0x455a14ed, 20)
		STEP(1, a, b, c, d, data[13] + 0xa9e3e905, 5)
		STEP(1, d, a, b, c, data[2] + 0xfcefa3f8, 9)
		STEP(1, c, d, a, b, data[7] + 0x676f02d9, 14)
		STEP(1, b, c, d, a, data[12] + 0x8d2a4c8a, 20)

		STEP(2, a, b, c, d, data[5] + 0xfffa3942, 4)
		STEP(2, d, a, b, c, data[8] + 0x8771f681, 11)
		STEP(2, c, d, a, b, data[11] + 0x6d9d6122, 16)
		STEP(2, b, c, d, a, data[14] + 0xfde5380c, 23)
		STEP(2, a, b, c, d, data[1] + 0xa4beea44, 4)
		STEP(2, d, a, b, c, data[4] + 0x4bdecfa9, 11)
		STEP(2, c, d, a, b, data[7] + 0xf6bb4b60, 16)
		STEP(2, b, c, d, a, data[10] + 0xbebfbc70, 23)
		STEP(2, a, b, c, d, data[13] + 0x289b7ec6, 4)
		STEP(2, d, a, b, c, data[0] + 0xeaa127fa, 11)
		STEP(2, c, d, a, b, data[3] + 0xd4ef3085, 16)
		STEP(2, b, c, d, a, data[6] + 0x04881d05, 23)
		STEP(2, a, b, c, d, data[9] + 0xd9d4d039, 4)
		STEP(2, d, a, b, c, data[12] + 0xe6db99e5, 11)
		STEP(2, c, d, a, b, data[15] + 0x1fa27cf8, 16)
		STEP(2, b, c, d, a, data[2] + 0xc4ac5665, 23)

		STEP(3, a, b, c, d, data[0] + 0xf4292244, 6)
		STEP(3, d, a, b, c, data[7] + 0x432aff97, 10)
		STEP(3, c, d, a, b, data[14] + 0xab9423a7, 15)
		STEP(3, b, c, d, a, data[5] + 0xfc93a039, 21)
		STEP(3, a, b, c, d, data[12] + 0x655b59c3, 6)
		STEP(3, d, a, b, c, data[3] + 0x8f0ccc92, 10)
		STEP(3, c, d, a, b, data[10] + 0xffeff47d, 15)
		STEP(3, b, c, d, a, data[1] + 0x85845dd1, 21)
		STEP(3, a, b, c, d, data[8] + 0x6fa87e4f, 6)
		STEP(3, d, a, b, c, data[15] + 0xfe2ce6e0, 10)
		STEP(3, c, d, a, b, data[6] + 0xa3014314, 15)
		STEP(3, b, c, d, a, data[13] + 0x4e0811a1, 21)
		STEP(3, a, b, c, d, data[4] + 0xf7537e82, 6)
		STEP(3, d, a, b, c, data[11] + 0xbd3af235, 10)
		STEP(3, c, d, a, b, data[2] + 0x2ad7d2bb, 15)
		STEP(3, b, c, d, a, data[9] + 0xeb86d391, 21)

		#undef STEP

		#undef F0
		#undef F1
		#undef F2
		#undef F3

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;
	}

	unsigned int state[4];
	unsigned int buffer[16];
	unsigned int counter[2];
};

class SHA1
{
public:
	UNIGINE_INLINE SHA1() { begin(); }
	UNIGINE_INLINE ~SHA1() {}

	UNIGINE_INLINE void begin()
	{
		state[0] = 0x67452301;
		state[1] = 0xefcdab89;
		state[2] = 0x98badcfe;
		state[3] = 0x10325476;
		state[4] = 0xc3d2e1f0;

		counter[0] = 0;
		counter[1] = 0;
	}
	void update(const void *data, int size)
	{
		const unsigned char *src = (const unsigned char *)data;

		unsigned int old_counter = counter[0];
		int old_size = (old_counter >> 3) & 0x3f;

		counter[0] += size << 3;
		counter[1] += size >> 29;

		if (counter[0] < old_counter)
			counter[1]++;

		if (old_size)
		{
			unsigned char *b = (unsigned char *)buffer + old_size;
			old_size = 64 - old_size;
			if (size < old_size)
			{
				memcpy(b, src, size);
				return;
			}
			memcpy(b, src, old_size);
			transform(state, buffer);
			size -= old_size;
			src += old_size;
		}

		while (size >= 64)
		{
			memcpy(buffer, src, 64);
			transform(state, buffer);
			size -= 64;
			src += 64;
		}

		memcpy(buffer, src, size);
	}
	void end(unsigned int *value, bool big_endian)
	{
		int size = (counter[0] >> 3) & 0x3f;

		unsigned char *b = (unsigned char *)buffer + size;

		*b++ = 0x80;

		size = 63 - size;

		if (size < 8)
		{
			memset(b, 0, size);
			transform(state, buffer);
			memset(buffer, 0, 56);
		} else
			memset(b, 0, size);

		buffer[14] = endian(counter[1]);
		buffer[15] = endian(counter[0]);
		transform(state, buffer);

		for (int i = 0; i < 5; ++i)
			value[i] = big_endian ? state[i] : endian(state[i]);
	}

	// calculate checksum for the single data block
	UNIGINE_INLINE static int calculate(const void *data, int size, bool big_endian = true)
	{
		unsigned int value[5];
		SHA1 sha1;
		sha1.update(data, size);
		sha1.end(value, big_endian);
		return (int)(value[0] ^ value[1] ^ value[2] ^ value[3] ^ value[4]);
	}
	UNIGINE_INLINE static void calculate(unsigned int *value, const void *data, int size, bool big_endian = true)
	{
		SHA1 sha1;
		sha1.update(data, size);
		sha1.end(value, big_endian);
	}

private:
	void transform(unsigned int *state, const unsigned int *src)
	{
		unsigned int a = state[0];
		unsigned int b = state[1];
		unsigned int c = state[2];
		unsigned int d = state[3];
		unsigned int e = state[4];

		const unsigned int K[] = {
			0x5a827999,
			0x6ed9eba1,
			0x8f1bbcdc,
			0xca62c1d6,
		};

		#define F0(X, Y, Z) ((X & Y) | (~X & Z))
		#define F1(X, Y, Z) (X ^ Y ^ Z)
		#define F2(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
		#define F3(X, Y, Z) (X ^ Y ^ Z)

		#define SHIFT(DATA, NUM) ((DATA << NUM) | (DATA >> (32 - NUM)))

		#define STEP(NUM, A, B, C, D, E, DATA) \
			{ \
				E += SHIFT(A, 5) + F##NUM(B, C, D) + DATA + K[NUM]; \
				B = SHIFT(B, 30); \
			}

		unsigned int data[80];
		for (int i = 0; i < 16; i++)
			data[i] = endian(src[i]);
		for (int i = 16; i < 80; i++)
		{
			unsigned int v = data[i - 3] ^ data[i - 8] ^ data[i - 14] ^ data[i - 16];
			data[i] = SHIFT(v, 1);
		}

		STEP(0, a, b, c, d, e, data[0])
		STEP(0, e, a, b, c, d, data[1])
		STEP(0, d, e, a, b, c, data[2])
		STEP(0, c, d, e, a, b, data[3])
		STEP(0, b, c, d, e, a, data[4])
		STEP(0, a, b, c, d, e, data[5])
		STEP(0, e, a, b, c, d, data[6])
		STEP(0, d, e, a, b, c, data[7])
		STEP(0, c, d, e, a, b, data[8])
		STEP(0, b, c, d, e, a, data[9])
		STEP(0, a, b, c, d, e, data[10])
		STEP(0, e, a, b, c, d, data[11])
		STEP(0, d, e, a, b, c, data[12])
		STEP(0, c, d, e, a, b, data[13])
		STEP(0, b, c, d, e, a, data[14])
		STEP(0, a, b, c, d, e, data[15])
		STEP(0, e, a, b, c, d, data[16])
		STEP(0, d, e, a, b, c, data[17])
		STEP(0, c, d, e, a, b, data[18])
		STEP(0, b, c, d, e, a, data[19])

		STEP(1, a, b, c, d, e, data[20])
		STEP(1, e, a, b, c, d, data[21])
		STEP(1, d, e, a, b, c, data[22])
		STEP(1, c, d, e, a, b, data[23])
		STEP(1, b, c, d, e, a, data[24])
		STEP(1, a, b, c, d, e, data[25])
		STEP(1, e, a, b, c, d, data[26])
		STEP(1, d, e, a, b, c, data[27])
		STEP(1, c, d, e, a, b, data[28])
		STEP(1, b, c, d, e, a, data[29])
		STEP(1, a, b, c, d, e, data[30])
		STEP(1, e, a, b, c, d, data[31])
		STEP(1, d, e, a, b, c, data[32])
		STEP(1, c, d, e, a, b, data[33])
		STEP(1, b, c, d, e, a, data[34])
		STEP(1, a, b, c, d, e, data[35])
		STEP(1, e, a, b, c, d, data[36])
		STEP(1, d, e, a, b, c, data[37])
		STEP(1, c, d, e, a, b, data[38])
		STEP(1, b, c, d, e, a, data[39])

		STEP(2, a, b, c, d, e, data[40])
		STEP(2, e, a, b, c, d, data[41])
		STEP(2, d, e, a, b, c, data[42])
		STEP(2, c, d, e, a, b, data[43])
		STEP(2, b, c, d, e, a, data[44])
		STEP(2, a, b, c, d, e, data[45])
		STEP(2, e, a, b, c, d, data[46])
		STEP(2, d, e, a, b, c, data[47])
		STEP(2, c, d, e, a, b, data[48])
		STEP(2, b, c, d, e, a, data[49])
		STEP(2, a, b, c, d, e, data[50])
		STEP(2, e, a, b, c, d, data[51])
		STEP(2, d, e, a, b, c, data[52])
		STEP(2, c, d, e, a, b, data[53])
		STEP(2, b, c, d, e, a, data[54])
		STEP(2, a, b, c, d, e, data[55])
		STEP(2, e, a, b, c, d, data[56])
		STEP(2, d, e, a, b, c, data[57])
		STEP(2, c, d, e, a, b, data[58])
		STEP(2, b, c, d, e, a, data[59])

		STEP(3, a, b, c, d, e, data[60])
		STEP(3, e, a, b, c, d, data[61])
		STEP(3, d, e, a, b, c, data[62])
		STEP(3, c, d, e, a, b, data[63])
		STEP(3, b, c, d, e, a, data[64])
		STEP(3, a, b, c, d, e, data[65])
		STEP(3, e, a, b, c, d, data[66])
		STEP(3, d, e, a, b, c, data[67])
		STEP(3, c, d, e, a, b, data[68])
		STEP(3, b, c, d, e, a, data[69])
		STEP(3, a, b, c, d, e, data[70])
		STEP(3, e, a, b, c, d, data[71])
		STEP(3, d, e, a, b, c, data[72])
		STEP(3, c, d, e, a, b, data[73])
		STEP(3, b, c, d, e, a, data[74])
		STEP(3, a, b, c, d, e, data[75])
		STEP(3, e, a, b, c, d, data[76])
		STEP(3, d, e, a, b, c, data[77])
		STEP(3, c, d, e, a, b, data[78])
		STEP(3, b, c, d, e, a, data[79])

		#undef STEP

		#undef SHIFT

		#undef F0
		#undef F1
		#undef F2
		#undef F3
		#undef F4

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;
		state[4] += e;
	}

	unsigned int state[5];
	unsigned int buffer[16];
	unsigned int counter[2];
};

class SHA256
{
public:
	UNIGINE_INLINE SHA256()
	{
		buffer_len = 0;
		bit_len = 0;
		state[0] = 0x6a09e667;
		state[1] = 0xbb67ae85;
		state[2] = 0x3c6ef372;
		state[3] = 0xa54ff53a;
		state[4] = 0x510e527f;
		state[5] = 0x9b05688c;
		state[6] = 0x1f83d9ab;
		state[7] = 0x5be0cd19;
	}
	void update(const void *data, int size)
	{
		const unsigned char *src = (const unsigned char *)data;

		for (int i = 0; i < size; ++i)
		{
			buffer[buffer_len] = src[i];
			buffer_len++;
			if (buffer_len == 64)
			{
				transform();
				bit_len += 512;
				buffer_len = 0;
			}
		}
	}
	void end(unsigned int *value, bool big_endian = true)
	{
		unsigned int i = buffer_len;

		// Pad whatever data is left in the buffer.
		if (buffer_len < 56)
		{
			buffer[i++] = 0x80;
			while (i < 56)
				buffer[i++] = 0x00;
		} else
		{
			buffer[i++] = 0x80;
			while (i < 64)
				buffer[i++] = 0x00;
			transform();
			memset(buffer, 0, 56);
		}

		// Append to the padding the total message's length in bits and transform.
		bit_len += buffer_len * 8;
		buffer[63] = (unsigned char)(bit_len);
		buffer[62] = (unsigned char)(bit_len >> 8);
		buffer[61] = (unsigned char)(bit_len >> 16);
		buffer[60] = (unsigned char)(bit_len >> 24);
		buffer[59] = (unsigned char)(bit_len >> 32);
		buffer[58] = (unsigned char)(bit_len >> 40);
		buffer[57] = (unsigned char)(bit_len >> 48);
		buffer[56] = (unsigned char)(bit_len >> 56);
		transform();

		for (int i = 0; i < 8; ++i)
			value[i] = big_endian ? state[i] : endian(state[i]);
	}

	UNIGINE_INLINE static int calculate(const void *data, int size, bool big_endian = true)
	{
		unsigned int value[8];
		SHA256 sha256;
		sha256.update(data, size);
		sha256.end(value, big_endian);
		return (int)(value[0] ^ value[1] ^ value[2] ^ value[3] ^ value[4] ^ value[5] ^ value[6] ^ value[7]);
	}
	UNIGINE_INLINE static void calculate(unsigned int *value, const void *data, int size, bool big_endian = true)
	{
		SHA256 sha256;
		sha256.update(data, size);
		sha256.end(value, big_endian);
	}

private:
	void transform()
	{
		#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

		#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
		#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
		#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
		#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
		#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
		#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

		const unsigned int k[64] = {
			0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
			0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
			0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
			0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
			0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
			0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
			0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
			0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
		};

		unsigned int a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

		for (i = 0, j = 0; i < 16; ++i, j += 4)
			m[i] = (buffer[j] << 24) | (buffer[j + 1] << 16) | (buffer[j + 2] << 8) | (buffer[j + 3]);
		for (; i < 64; ++i)
			m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

		a = state[0];
		b = state[1];
		c = state[2];
		d = state[3];
		e = state[4];
		f = state[5];
		g = state[6];
		h = state[7];

		for (i = 0; i < 64; ++i)
		{
			t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
			t2 = EP0(a) + MAJ(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + t1;
			d = c;
			c = b;
			b = a;
			a = t1 + t2;
		}

		state[0] += a;
		state[1] += b;
		state[2] += c;
		state[3] += d;
		state[4] += e;
		state[5] += f;
		state[6] += g;
		state[7] += h;

		#undef ROTRIGHT
		#undef CH
		#undef MAJ
		#undef EP0
		#undef EP1
		#undef SIG0
		#undef SIG1
	}

	unsigned char buffer[64];
	unsigned int buffer_len;

	unsigned long long bit_len;
	unsigned int state[8];
};

class Checksum
{
public:

	/**
		* Calculates the 32-bit CRC checksum.
		*
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		* @return The 32-bit CRC checksum.
		*/
	UNIGINE_INLINE static int CRC32(const void *data, int size, bool big_endian = true) { return CRC32::calculate(data, size, big_endian); }

	/**
		* Calculates the 32-bit MD5 checksum.
		*
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		* @return The 32-bit MD5 checksum.
		*/
	UNIGINE_INLINE static int MD5(const void *data, int size, bool big_endian = true) { return MD5::calculate(data, size, big_endian); }

	/**
		* Calculates the 64-bit MD5 checksum.
		*
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		* @return The 64-bit MD5 checksum.
		*/
	UNIGINE_INLINE static unsigned long long MD5_64(const void *data, int size, bool big_endian = true)
	{
		unsigned long long v[2];
		MD5::calculate((unsigned int *)v, data, size, big_endian);
		return v[0] ^ (v[1] + 0x9ddfea08eb382d69ULL + (v[1] << 16) + (v[1] >> 12));
	}

	/**
		* Calculates the 128-bit MD5 checksum.
		*
		* @param value The 128-bit MD5 checksum (array of 4 unsigned int elements).
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		*/
	UNIGINE_INLINE static void MD5(unsigned int *value, const void *data, int size, bool big_endian = true) { MD5::calculate(value, data, size, big_endian); }
	
	/**
		* Calculates the 32-bit SHA1 checksum.
		*
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		* @return The 32-bit SHA1 checksum.
		*/
	UNIGINE_INLINE static int SHA1(const void *data, int size, bool big_endian = true) { return SHA1::calculate(data, size, big_endian); }

	/**
		* Calculates the 160-bit SHA1 checksum.
		*
		* @param value The 160-bit SHA1 checksum (array of 5 unsigned int elements).
		* @param data The input data pointer.
		* @param size The input data size in bytes.
		* @param big_endian Flag to control endianness of the result.
		*/
	UNIGINE_INLINE static void SHA1(unsigned int *value, const void *data, int size, bool big_endian = true) { SHA1::calculate(value, data, size, big_endian); }

	/**
	* Calculates the 32-bit SHA256 checksum.
	*
	* @param data The input data pointer.
	* @param size The input data size in bytes.
	* @param big_endian Flag to control endianness of the result.
	* @return The 32-bit SHA256 checksum.
	*/
	UNIGINE_INLINE static int SHA256(const void *data, int size, bool big_endian = true) { return SHA256::calculate(data, size, big_endian); }

	/**
	* Calculates the 256-bit SHA256 checksum.
	*
	* @param value The 256-bit SHA256 checksum (array of 8 unsigned int elements).
	* @param data The input data pointer.
	* @param size The input data size in bytes.
	* @param big_endian Flag to control endianness of the result.
	*/
	UNIGINE_INLINE static void SHA256(unsigned int *value, const void *data, int size, bool big_endian = true) { SHA256::calculate(value, data, size, big_endian); }
};
	
} /* namespace Unigine */
