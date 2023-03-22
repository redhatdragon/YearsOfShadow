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

#include <UnigineBase.h>
#include "UnigineChecksum.h"
#include "UnigineString.h"
#include "UnigineMathLib.h"

namespace Unigine
{

class UNIGINE_API UGUID
{
public:
	static constexpr size_t VALUE_SIZE = 24;
	static const UGUID empty;
	static constexpr size_t HASH_SIZE = 40;

	UNIGINE_INLINE UGUID() { clear(); }
	UNIGINE_INLINE explicit UGUID(const char *str)
	{
		clear();
		setString(str);
	}
	UNIGINE_INLINE explicit UGUID(unsigned char(&value_)[VALUE_SIZE])
	{
		memcpy(value, value_, VALUE_SIZE);
		write_string(value_int, 5, get_str_data());
	}

	UNIGINE_INLINE UGUID(const UGUID &g)
	{
		memcpy(value_long, g.value_long, sizeof(value_long));
		memcpy(str, g.str, sizeof(str));
	}
	UNIGINE_INLINE UGUID &operator=(const UGUID &g)
	{
		memcpy(value_long, g.value_long, sizeof(value_long));
		memcpy(str, g.str, sizeof(str));
		return *this;
	}
	UNIGINE_INLINE UGUID &operator=(const char *str)
	{
		setString(str);
		return *this;
	}

	UNIGINE_INLINE bool operator==(const UGUID &g) const
	{
		return	value_long[0] == g.value_long[0] &&
				value_long[1] == g.value_long[1] &&
				value_long[2] == g.value_long[2];
	}
	UNIGINE_INLINE bool operator!=(const UGUID &g) const
	{
		return	value_long[0] != g.value_long[0] ||
				value_long[1] != g.value_long[1] ||
				value_long[2] != g.value_long[2];
	}
	UNIGINE_INLINE bool operator<=(const UGUID &g) const
	{
		return *this == g || *this > g;
	}
	UNIGINE_INLINE bool operator>=(const UGUID &g) const
	{
		return *this == g || *this > g;
	}
	UNIGINE_INLINE bool operator<(const UGUID &g) const
	{
		if (value_long[0] > g.value_long[0])  return 0;
		if (value_long[0] < g.value_long[0])  return 1;

		if (value_long[1] > g.value_long[1])  return 0;
		if (value_long[1] < g.value_long[1])  return 1;

		if (value_long[2] > g.value_long[2])  return 0;
		if (value_long[2] < g.value_long[2])  return 1;

		return 0;
	}
	UNIGINE_INLINE bool operator>(const UGUID &g) const
	{
		if (value_long[0] < g.value_long[0]) return 0;
		if (value_long[0] > g.value_long[0]) return 1;

		if (value_long[1] < g.value_long[1]) return 0;
		if (value_long[1] > g.value_long[1]) return 1;
		
		if (value_long[2] < g.value_long[2]) return 0;
		if (value_long[2] > g.value_long[2]) return 1;

		return 0;
	}

	UNIGINE_INLINE void generate()
	{
		get_random_values(value_int, 5);
		write_string(value_int, 5, get_str_data());
	}
	UNIGINE_INLINE void generate(const char *str_)
	{
		Checksum::SHA1(value_int, str_, (int)(sizeof(char) * strlen(str_)));
		write_string(value_int, 5, get_str_data());
	}
	UNIGINE_INLINE void generate(int &seed_)
	{
		for (int i = 0; i < 5; ++i)
			value_int[i] = seed_ = (unsigned int)((unsigned long long)seed_ * Math::Random::A + Math::Random::C);
		write_string(value_int, 5, get_str_data());
	}
	UNIGINE_INLINE void generate(const void *data, int size)
	{
		Checksum::SHA1(value_int, data, size);
		write_string(value_int, 5, get_str_data());
	}

	UNIGINE_INLINE const char *getFileSystemString() const { return str; }
	void setFileSystemString(const char *str_)
	{
		if (!str_)
		{
			clear();
			return;
		}

		int str_size = static_cast<int>(strlen(str_));
		if (str_size != (PREFIX_SIZE + HASH_SIZE))
		{
			clear();
			return;
		}

		if (!String::startsWith(str_, PREFIX, 1, str_size, int(PREFIX_SIZE)))
		{
			clear();
			return;
		}

		read_string(str_ + PREFIX_SIZE, value_int);
		write_string(value_int, 5, get_str_data());
	}

	UNIGINE_INLINE const unsigned char *getValue() const { return value; }
	UNIGINE_INLINE const char *getString() const { return str + PREFIX_SIZE; }
	void setString(const char *str_)
	{
		if (!str_ || strlen(str_) != HASH_SIZE)
		{
			clear();
			return;
		}

		read_string(str_, value_int);
		write_string(value_int, 5, get_str_data());
	}

	UNIGINE_INLINE void clear()
	{
		memset(str, 0, sizeof(str));
		memset(value_long, 0, sizeof(value_long));
		memcpy(str, PREFIX, PREFIX_SIZE);
	}
	UNIGINE_INLINE bool isEmpty() const { return getString()[0] == 0; }
	UNIGINE_INLINE bool isValid() const { return !isEmpty(); }

	UNIGINE_INLINE const char *get() const { return str; }
	UNIGINE_INLINE char &get(int index)
	{
		assert(index < 48 && index >= 0 && "UGUID::get(): bad index");
		return str[index];
	}
	UNIGINE_INLINE char get(int index) const
	{
		assert(index < 48 && index >= 0 && "UGUID::get(): bad index");
		return str[index];
	}
	UNIGINE_INLINE char &operator[](int index) { return get(index); }
	UNIGINE_INLINE char operator[](int index) const { return get(index); }

	UNIGINE_INLINE unsigned int hash() const
	{
		return (((value_int[0] ^ value_int[1]) << 5) ^ value_int[2] ^ value_int[3] ^ value_int[4]);
	}
	UNIGINE_INLINE unsigned long long hashLong() const
	{
		return Math::hashMixer(value_long[0], Math::hashMixer(value_long[1], value_long[2]));
	}

private:
	static constexpr auto PREFIX = "guid://";
	static constexpr size_t PREFIX_SIZE = constexpr_strlen(PREFIX);

	UNIGINE_INLINE char *get_str_data() { return str + PREFIX_SIZE; }
	void read_string(const char *str, unsigned int *values)
	{
		int index = 0;
		while (*str)
		{
			unsigned int &value = values[index++];
			int value_size = 8;
			while (value_size--)
			{
				char c = *str++;
				if (c >= '0' && c <= '9')
					value = (value << 4) + (c - '0');
				else if (c >= 'a' && c <= 'f')
					value = (value << 4) + (c - 'a' + 10);
				else if (c >= 'A' && c <= 'F')
					value = (value << 4) + (c - 'A' + 10);
			}
		}
	}
	void write_string(const unsigned int *values, int size, char *str)
	{
		const unsigned int radix = 16;
		const char *digits = "0123456789abcdef";

		char *p = str + HASH_SIZE;
		*p = '\0';

		while (size--)
		{
			unsigned int value = values[size];
			int value_size = 8;
			while (value_size--)
			{
				*--p = digits[value & (radix - 1)];
				value /= radix;
			}
		}
	}
	static void get_random_values(unsigned int *values, int size);

	union
	{
		unsigned char value[VALUE_SIZE];
		unsigned int value_int[6];
		unsigned long long value_long[3];
	};
	char str[48];
};

template<typename Type>
struct Hasher;

template<>
struct Hasher<UGUID>
{
	using HashType = unsigned int;
	UNIGINE_INLINE static HashType create(const UGUID &v) { return v.hash(); }
};

} // namespace
