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

#include <UnigineMemory.h>
#include <UnigineVector.h>
#include <UnigineHashMap.h>
#include <UnigineHashSet.h>

namespace Unigine
{

class String;

template <int Capacity = 256>
class StringStack;

template <int Capacity = 256>
class StringArray;

class UNIGINE_API String
{
public:

	enum class Direction
	{
		LTR,
		RTL,
		Neutral,
		Digit,
	};

	UNIGINE_INLINE String() : capacity(STACK_CAPACITY), dynamic(false), data(stack_data)
	{
		data[0] = '\0';
	}

	UNIGINE_INLINE String(String &&s) : capacity(STACK_CAPACITY), dynamic(false), data(stack_data)
	{
		if (s.dynamic)
		{
			length = s.length;
			capacity = s.capacity;
			dynamic = s.dynamic;
			data = s.data;

			s.length = 0;
			s.capacity = STACK_CAPACITY;
			s.dynamic = false;
			s.data = s.stack_data;
			s.data[s.length] = '\0';
		} else
		{
			do_assign(s.data, s.length);
			s.clear();
		}
	}
	UNIGINE_INLINE String(const String &s) : capacity(STACK_CAPACITY), dynamic(false), data(stack_data)
	{
		do_assign(s.data, s.length);
	}
	UNIGINE_INLINE String(const char *s) : capacity(STACK_CAPACITY), dynamic(false), data(stack_data)
	{
		do_assign(s);
	}
	UNIGINE_INLINE String(const char *s, int size) : capacity(STACK_CAPACITY), dynamic(false), data(stack_data)
	{
		do_assign(s, size);
	}
	explicit String(const wchar_t *s);
	explicit String(const unsigned int *s);

	UNIGINE_INLINE ~String()
	{
		if (dynamic)
			Memory::deallocate(data);
	}
	
protected:
	UNIGINE_INLINE static auto get_ncompare_func(bool case_sensitive)
	{
		#ifdef _WIN32
			return case_sensitive ? &strncmp : &_strnicmp;
		#else
			return case_sensitive ? &strncmp : &strncasecmp;
		#endif
	}
	UNIGINE_INLINE static auto get_compare_func(bool case_sensitive)
	{
		#ifdef _WIN32
			return case_sensitive ? &strcmp : &_stricmp;
		#else
			return case_sensitive ? &strcmp : &strcasecmp;
		#endif
	}

public:
	UNIGINE_INLINE String &operator=(const char *s)
	{
		if (data == s)
			return *this;
		if (s == nullptr)
			clear();
		else
			do_copy(s, (int)strlen(s));
		return *this;
	}
	UNIGINE_INLINE String &operator=(const String &s)
	{
		if (data == s.data)
			return *this;
		do_copy(s, s.length);
		return *this;
	}
	UNIGINE_INLINE String &operator=(String &&s)
	{
		if (data == s.data)
			return *this;
		if (s.dynamic)
		{
			if (dynamic)
				Memory::deallocate(data);

			length = s.length;
			capacity = s.capacity;
			dynamic = s.dynamic;
			data = s.data;

			s.length = 0;
			s.capacity = STACK_CAPACITY;
			s.dynamic = false;
			s.data = s.stack_data;
			s.data[s.length] = '\0';
		} else
		{
			do_copy(s.data, s.length);
			s.clear();
		}
		return *this;
	}

	UNIGINE_INLINE String &operator+=(char c)
	{
		do_append(c);
		return *this;
	}
	UNIGINE_INLINE String &operator+=(const char *s)
	{
		if (s == nullptr)
			return *this;
		do_append(s, (int)strlen(s));
		return *this;
	}
	UNIGINE_INLINE String &operator+=(const String &s)
	{
		do_append(s.data, s.length);
		return *this;
	}

	UNIGINE_INLINE const char *get() const { return data; }
	UNIGINE_INLINE char *getRaw() const { return data; }
	UNIGINE_INLINE operator const char *() const { return data; }
	UNIGINE_INLINE operator const void *() const { return data; }
	UNIGINE_INLINE char &get(int index)
	{
		assert(index <= length && index >= 0 && "String::get(): bad index");
		return data[index];
	}
	UNIGINE_INLINE char get(int index) const
	{
		assert(index <= length && index >= 0 && "String::get(): bad index");
		return data[index];
	}

	UNIGINE_INLINE char &operator[](int index) { return get(index); }
	UNIGINE_INLINE char operator[](int index) const { return get(index); }

	UNIGINE_INLINE char &first() { return get(0); }
	UNIGINE_INLINE char first() const { return get(0); }
	UNIGINE_INLINE char &last() { return get(length - 1); }
	UNIGINE_INLINE char last() const { return get(length - 1); }

	UNIGINE_INLINE int size() const { return length; }
	UNIGINE_INLINE int space() const { return static_cast<int>(capacity); }
	UNIGINE_INLINE bool empty() const { return length == 0; }

	UNIGINE_INLINE void resize(int size)
	{
		allocate(size);
		length = size;
		data[length] = '\0';
	}
	UNIGINE_INLINE void allocate(int size)
	{
		assert(size >= 0 && size < INT_MAX);
		if ((size + 1) <= space())
			return;

		capacity = size + 1;
		char *new_data = (char *)Memory::allocate(capacity);
		memcpy(new_data, data, length + 1);
		if (dynamic)
			Memory::deallocate(data);

		dynamic = true;
		data = new_data;
	}
	UNIGINE_INLINE void reserve(int size)
	{
		assert(size >= 0 && size < INT_MAX);
		if ((size + 1) <= space())
			return;

		capacity = grow_to(size);
		char *new_data = (char *)Memory::allocate(capacity);
		memcpy(new_data, data, length + 1);
		if (dynamic)
			Memory::deallocate(data);

		dynamic = true;
		data = new_data;
	}
	UNIGINE_INLINE void shrink()
	{
		const int new_capacity = length + 1;
		if (new_capacity == space() || !dynamic)
			return;

		char *new_data;
		if (new_capacity <= STACK_CAPACITY)
		{
			capacity = STACK_CAPACITY;
			dynamic = false;
			new_data = stack_data;
		} else
		{
			capacity = new_capacity;
			dynamic = true;
			new_data = (char *)Memory::allocate(new_capacity);
		}

		memmove(new_data, data, length);

		Memory::deallocate(data);

		data = new_data;
		data[length] = '\0';
	}

	UNIGINE_INLINE void clear()
	{
		length = 0;
		data[0] = '\0';
	}
	UNIGINE_INLINE void destroy()
	{
		length = 0;
		capacity = STACK_CAPACITY;
		if (dynamic)
			Memory::deallocate(data);
		dynamic = false;
		data = stack_data;
		data[length] = '\0';
	}

	UNIGINE_INLINE int find(bool (*filter)(char)) const
	{
		for (int i = 0; i < length; i++)
		{
			if (filter(data[i]))
				return i;
		}

		return -1;
	}

	UNIGINE_INLINE int find(char c, bool case_sensitive = true) const
	{
		if (case_sensitive)
		{
			for (int i = 0; i < length; i++)
			{
				if (data[i] == c)
					return i;
			}
		} else
		{
			char lc = toLower(c);
			for (int i = 0; i < length; i++)
			{
				if (toLower(data[i]) == lc)
					return i;
			}
		}
		return -1;
	}
	UNIGINE_INLINE int find(const char *s, bool case_sensitive = true) const
	{
		if (s == nullptr)
			return -1;
		int size = (int)strlen(s);
		auto compare = get_ncompare_func(case_sensitive);
		for (int i = 0; i < length; i++)
			if (!compare(data + i, s, size))
				return i;
		return -1;
	}
	UNIGINE_INLINE int find(const String &s, bool case_sensitive = true) const
	{
		auto compare = get_ncompare_func(case_sensitive);
		for (int i = 0; i < length; i++)
			if (!compare(data + i, s.data, s.length))
				return i;
		return -1;
	}


	UNIGINE_INLINE int rfind(bool (*filter)(char)) const
	{
		for (int i = length - 1; i >= 0; i--)
		{
			if (filter(data[i]))
				return i;
		}

		return -1;
	}
	UNIGINE_INLINE int rfind(char c, bool case_sensitive = true) const
	{
		if (case_sensitive)
		{
			for (int i = length - 1; i >= 0; i--)
				if (data[i] == c)
					return i;
		} else
		{
			char lc = toLower(c);
			for (int i = length - 1; i >= 0; i--)
				if (toLower(data[i]) == lc)
					return i;
		}
		return -1;
	}
	UNIGINE_INLINE int rfind(const char *s, bool case_sensitive = true) const
	{
		if (s == nullptr)
			return -1;
		int size = (int)strlen(s);
		auto compare = get_ncompare_func(case_sensitive);
		for (int i = length - 1; i >= 0; i--)
			if (!compare(data + i, s, size))
				return i;
		return -1;
	}
	UNIGINE_INLINE int rfind(const String &s, bool case_sensitive = true) const
	{
		auto compare = get_ncompare_func(case_sensitive);
		for (int i = length - 1; i >= 0; i--)
			if (!compare(data + i, s.data, s.length))
				return i;
		return -1;
	}

	UNIGINE_INLINE bool contains(char c, bool case_sensitive = true) const { return find(c, case_sensitive) != -1; }
	UNIGINE_INLINE bool contains(const char *s, bool case_sensitive = true) const { return find(s, case_sensitive) != -1; }
	UNIGINE_INLINE bool contains(const String &s, bool case_sensitive = true) const { return find(s, case_sensitive) != -1; }
	
	UNIGINE_INLINE bool containsChar(const char *s) const
	{
		if (s == nullptr)
			return false;
		int size = (int)strlen(s);
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (data[i] == s[j])
					return true;
			}
		}
		return false;
	}
	UNIGINE_INLINE bool containsChar(const String &s) const
	{
		if (s.empty())
			return false;
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < s.length; j++)
			{
				if (data[i] == s[j])
					return true;
			}
		}
		return false;
	}

	UNIGINE_INLINE void copy(const char *s, int size = -1)
	{
		if (s == nullptr)
			clear();
		else
			do_copy(s, size == -1 ? int(strlen(s)) : size);
	}
	UNIGINE_INLINE void copy(const String &s, int size = -1)
	{
		do_copy(s.data, size == -1 ? s.length : size);
	}

	UNIGINE_INLINE void swap(String &s)
	{
		std::swap(length, s.length);

		unsigned int temp_capacity = capacity;
		capacity = s.capacity;
		s.capacity = temp_capacity;
		
		unsigned int temp_dynamic = dynamic;
		dynamic = s.dynamic;
		s.dynamic = temp_dynamic;

		std::swap(data, s.data);
		std::swap(stack_data, s.stack_data);
	}

	UNIGINE_INLINE String &append(char c)
	{
		do_append(c);
		return *this;
	}
	UNIGINE_INLINE String &append(const char *s, int size = -1)
	{
		if (!s)
			return *this;
		do_append(s, size == -1 ? int(strlen(s)) : size);
		return *this;
	}
	UNIGINE_INLINE String &append(const String &s, int size = -1)
	{
		do_append(s.data, size == -1 ? s.length : size);
		return *this;
	}
	UNIGINE_INLINE String &append(String &&s, int size = -1)
	{
		int new_size = size == -1 ? s.length : size;
		if (length == 0 && s.dynamic)
		{
			if (dynamic)
				Memory::deallocate(data);

			data = s.data;
			capacity = s.capacity;
			dynamic = s.dynamic;
			length = new_size;
			data[length] = '\0';

			s.length = 0;
			s.capacity = STACK_CAPACITY;
			s.dynamic = false;
			s.data = s.stack_data;
			s.data[s.length] = '\0';

			return *this;
		}

		do_append(s.data, new_size);
		return *this;
	}

	UNIGINE_INLINE String &append(int pos, char c)
	{
		do_append(pos, c);
		return *this;
	}
	UNIGINE_INLINE String &append(int pos, const char *s, int size = -1)
	{
		if (s == nullptr)
			return *this;
		do_append(pos, s, size == -1 ? int(strlen(s)) : size);
		return *this;
	}
	UNIGINE_INLINE String &append(int pos, const String &s, int size = -1)
	{
		do_append(pos, s.data, size == -1 ? s.length : size);
		return *this;
	}

	UNIGINE_INLINE String &remove()
	{
		assert(length > 0 && "String::remove(): bad length");
		data[--length] = '\0';
		return *this;
	}
	UNIGINE_INLINE String &remove(int pos, int size = 1)
	{
		assert(pos <= length && pos >= 0 && "String::remove(): bad position");
		assert(size >= 0 && pos + size <= length && "String::remove(): bad length");
		memmove(data + pos, data + pos + size, length - pos - size);
		length -= size;
		data[length] = '\0';
		return *this;
	}

	UNIGINE_INLINE String &lower()
	{
		char *s = data;
		while (*s)
		{
			char c = *s;
			if (c >= 'A' && c <= 'Z')
				*s += ('a' - 'A');
			s++;
		}
		return *this;
	}
	UNIGINE_INLINE String &upper()
	{
		char *s = data;
		while (*s)
		{
			char c = *s;
			if (c >= 'a' && c <= 'z')
				*s -= ('a' - 'A');
			s++;
		}
		return *this;
	}
	UNIGINE_INLINE String &capitalize()
	{
		if (data)
			data[0] = toUpper(data[0]);
		return *this;
	}
	StringStack<> getLower() const;
	StringStack<> getUpper() const;

	StringStack<> trim(const char *symbols = 0) const;
	StringStack<> trimLast(const char *symbols = 0) const;
	StringStack<> trimFirst(const char *symbols = 0) const;
	StringStack<> replace(char before, char after, bool case_sensitive = true) const;
	StringStack<> replace(const char *before, const char *after, bool case_sensitive = true) const;
	StringStack<> substr(int pos, int size = -1) const;
	StringStack<> dirname() const;
	StringStack<> basename() const;
	StringStack<> filename() const;
	StringStack<> pathname() const;
	StringStack<> extension() const;

	UNIGINE_INLINE int utf8ToUnicode(wchar_t *dest, int size) const { return String::utf8ToUnicode(data, dest, size); }
	UNIGINE_INLINE int utf8ToUnicode(unsigned int *dest, int size) const { return String::utf8ToUnicode(data, dest, size); }
	UNIGINE_INLINE int utf8strlen() const { return String::utf8strlen(data); }
	StringStack<> utf8substr(int pos, int size) const;

	UNIGINE_INLINE bool getBool() const { return String::atob(data); }
	UNIGINE_INLINE int getInt() const { return String::atoi(data); }
	UNIGINE_INLINE long long getLong() const { return String::atol(data); }
	UNIGINE_INLINE float getFloat() const { return String::atof(data); }
	UNIGINE_INLINE double getDouble() const { return String::atod(data); }

	int startsWith(const char *s, bool case_sensitive = true, int size = -1) const;
	int startsWith(const String &s, bool case_sensitive = true, int size = -1) const;
	int endsWith(const char *s, bool case_sensitive = true, int size = -1) const;
	int endsWith(const String &s, bool case_sensitive = true, int size = -1) const;

	void vprintf(const char *format, va_list argptr);
	UNIGINE_INLINE void printf(const char *format, ...) UNIGINE_PRINTF(2, 3)
	{
		va_list argptr;
		va_start(argptr, format);
		vprintf(format, argptr);
		va_end(argptr);
	}

	int vscanf(const char *format, va_list argptr);
	UNIGINE_INLINE int scanf(const char *format, ...) UNIGINE_SCANF(2, 3)
	{
		va_list argptr;
		va_start(argptr, format);
		int ret = vscanf(format, argptr);
		va_end(argptr);
		return ret;
	}

	UNIGINE_INLINE static int compare(const char *str0, const char *str1)
	{
		if (str0 == str1)
			return 0;
		if (!str0 || !str1)
			return -1;
		return strcmp(str0, str1);
	}
	UNIGINE_INLINE static int compare(const String &s0, const String &s1)
	{
		if (s0.empty() && s1.empty())
			return 0;
		return strcmp(s0.get(), s1.get());
	}
	UNIGINE_INLINE static int compare(const String &s0, const char *s1)
	{
		if (s0.empty() && (s1 == nullptr))
			return 0;
		return compare(s0.get(), s1);
	}
	UNIGINE_INLINE static int compare(const char *s0, const String &s1)
	{
		if ((s0 == nullptr) && s1.empty())
			return 0;
		return compare(s0, s1.get());
	}

	UNIGINE_INLINE static int compare(const char *str0, const char *str1, bool case_sensitive)
	{
		if (str0 == str1)
			return 0;
		if (!str0 || !str1)
			return -1;

		#ifdef _WIN32
			return case_sensitive ? strcmp(str0, str1) : _stricmp(str0, str1);
		#else
			return case_sensitive ? strcmp(str0, str1) : strcasecmp(str0, str1);
		#endif
	}
	UNIGINE_INLINE static int ncompare(const char *str0, const char *str1, int size)
	{
		if (str0 == str1)
			return 0;
		if (!str0 || !str1)
			return -1;
		
		return strncmp(str0, str1, size);
	}
	UNIGINE_INLINE static int ncompare(const char *str0, const char *str1, int size, bool case_sensitive)
	{
		if (str0 == str1)
			return 0;
		if (!str0 || !str1)
			return -1;
		
		#ifdef _WIN32
			return case_sensitive ? strncmp(str0, str1, size) : _strnicmp(str0, str1, size);
		#else
			return case_sensitive ? strncmp(str0, str1, size) : strncasecmp(str0, str1, size);
		#endif
	}

	UNIGINE_INLINE static bool isspace(int code) { return code == ' ' || code == '\t'; }
	UNIGINE_INLINE static bool isdigit(int code) { return code >= '0' && code <= '9'; }
	UNIGINE_INLINE static bool islower(int code) { return code >= 'a' && code <= 'z'; }
	UNIGINE_INLINE static bool isupper(int code) { return code >= 'A' && code <= 'Z'; }
	UNIGINE_INLINE static bool isalpha(int code) { return (code >= 'a' && code <= 'z') || (code >= 'A' && code <= 'Z'); }

	UNIGINE_INLINE static char toLower(char c) { return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; }
	UNIGINE_INLINE static char toUpper(char c) { return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c; }

	UNIGINE_INLINE static bool atob(const char *str)
	{
		return String::equal(str, "1") || String::equal(str, "true") || String::equal(str, "TRUE");
	}
	static int atoi(const char *str)
	{
		int ret = 0;
		int negative = 0;
		const char *s = str;
		while (*s == ' ' || *s == '\t')
		{
			s++;
		}
		if (*s == '-' || *s == '+')
		{
			if (*s == '-')
				negative = 1;
			s++;
		}
		if (*s == '0' && (*(s + 1) == 'b' || *(s + 1) == 'B'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '1')
					ret = (ret << 1) + (c - '0');
				else
					break;
			}
		} else if (*s == '0' && (*(s + 1) == 'o' || *(s + 1) == 'O'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '8')
					ret = (ret << 3) + (c - '0');
				else
					break;
			}
		} else if (*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '9')
					ret = (ret << 4) + (c - '0');
				else if (c >= 'a' && c <= 'f')
					ret = (ret << 4) + (c - 'a' + 10);
				else if (c >= 'A' && c <= 'F')
					ret = (ret << 4) + (c - 'A' + 10);
				else
					break;
			}
		} else
		{
			while (*s)
			{
				char c = *s++;
				if (c < '0' || c > '9')
					break;
				ret = ret * 10 + (c - '0');
			}
		}
		if (negative)
			return -ret;
		return ret;
	}
	static long long atol(const char *str)
	{
		int negative = 0;
		long long ret = 0;
		const char *s = str;
		while (*s == ' ' || *s == '\t')
		{
			s++;
		}
		if (*s == '-' || *s == '+')
		{
			if (*s == '-')
				negative = 1;
			s++;
		}
		if (*s == '0' && (*(s + 1) == 'b' || *(s + 1) == 'B'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '1')
					ret = (ret << 1) + (c - '0');
				else
					break;
			}
		} else if (*s == '0' && (*(s + 1) == 'o' || *(s + 1) == 'O'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '8')
					ret = (ret << 3) + (c - '0');
				else
					break;
			}
		} else if (*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
		{
			s += 2;
			while (*s)
			{
				char c = *s++;
				if (c >= '0' && c <= '9')
					ret = (ret << 4) + (c - '0');
				else if (c >= 'a' && c <= 'f')
					ret = (ret << 4) + (c - 'a' + 10);
				else if (c >= 'A' && c <= 'F')
					ret = (ret << 4) + (c - 'A' + 10);
				else
					break;
			}
		} else
		{
			while (*s)
			{
				char c = *s++;
				if (c < '0' || c > '9')
					break;
				ret = ret * 10 + (c - '0');
			}
		}
		if (negative)
			return -ret;
		return ret;
	}
	UNIGINE_INLINE static float atof(const char *str)
	{
		return static_cast<float>(atod(str));
	}
	static double atod(const char *str)
	{
		double ret = 0.0;
		const char *s = str;
		int negative = 0;
		while (*s == ' ' || *s == '\t')
		{
			s++;
		}
		if (*s == '-' || *s == '+')
		{
			if (*s == '-')
				negative = 1;
			s++;
		}
		if (*s >= '0' && *s <= '9')
		{
			while (*s)
			{
				char c = *s;
				if (c < '0' || c > '9')
					break;
				ret = ret * 10.0 + (c - '0');
				s++;
			}
		}
		if (*s == '.')
		{
			s++;
			if (*s >= '0' && *s <= '9')
			{
				double frac = 0.0;
				double scale = 0.1;
				while (*s)
				{
					char c = *s;
					if (c < '0' || c > '9')
						break;
					frac += scale * (c - '0');
					scale *= 0.1;
					s++;
				}
				ret += frac;
			}
		}

		if (*s == 'e' || *s == 'E')
		{
			s++;
			double exp = 0.0;
			int exp_negative = 0;
			if (*s == '-' || *s == '+')
			{
				if (*s == '-')
					exp_negative = 1;
				s++;
			}
			if (*s >= '0' && *s <= '9')
			{
				while (*s)
				{
					char c = *s++;
					if (c < '0' || c > '9')
						break;
					exp = exp * 10.0 + (c - '0');
				}
			}
			if (exp_negative)
				exp = -exp;
			ret *= ::pow(10.0, exp);
		}

		if (negative)
			return -ret;
		return ret;
	}

	static StringStack<> itoa(int value, int radix = 10);
	static StringStack<> ltoa(long long value, int radix = 10);
	static StringStack<> ftoa(float value, int precision = -1);
	static StringStack<> dtoa(double value, int precision = -1, bool use_inf = false);

	static StringStack<> memory(size_t memory);

	static StringStack<> format(const char *format, va_list argptr);
	static StringStack<> format(const char *format, ...) UNIGINE_PRINTF(1, 2);

	static StringStack<> prettyFormat(int value);
	static StringStack<> prettyFormat(float value, int precision = -1);
	static StringStack<> prettyFormat(double value, int precision = -1);
	static StringStack<> prettyFormat(const char *str, bool vocabulary_on = true);

	static int vsscanf(const char *str, const char *format, va_list argptr);
	static int sscanf(const char *str, const char *format, ...) UNIGINE_SCANF(2, 3);

	static StringStack<> trim(const char *str, const char *symbols);
	static StringStack<> trimFirst(const char *str, const char *symbols);
	static StringStack<> trimLast(const char *str, const char *symbols);
	static StringStack<> replace(const char *str, char before, char after, bool case_sensitive = true);
	static StringStack<> replace(const char *str, const char *before, const char *after, bool case_sensitive = true);

	UNIGINE_INLINE static bool startsWith(const char *data, const char *str, bool case_sensitive = true, int data_size = -1, int str_size = -1)
	{
		data_size = data_size == -1 ? int(strlen(data)) : data_size;
		str_size = str_size == -1 ? int(strlen(str)) : str_size;

		if (str_size == 0 || str_size > data_size)
			return false;
		return ncompare(data, str, str_size, case_sensitive) == 0;
	}
	UNIGINE_INLINE static bool endsWith(const char *data, const char *str, bool case_sensitive = true, int data_size = -1, int str_size = -1)
	{
		data_size = data_size == -1 ? int(strlen(data)) : data_size;
		str_size = str_size == -1 ? int(strlen(str)) : str_size;

		if (str_size == 0 || str_size > data_size)
			return false;
		return ncompare(data + data_size - str_size, str, str_size, case_sensitive) == 0;
	}

	static int getStringArray(const char *str, Vector<String> &dest);
	static void split(StringArray<> &ret, const char *str, const char *delimiters);
	static StringArray<> split(const char *str, const char *delimiters);
	static StringStack<> substr(const char *str, int pos, int size = -1);
	static StringStack<> substr(const String &str, int pos, int size = -1);
	static StringStack<> addslashes(const char *str);
	static StringStack<> stripslashes(const char *str);

	// Hash function (found at: 'http://www.azillionmonkeys.com/qed/hash.html')
	static unsigned int hash(const char *str, int size = -1)
	{
		if (str == nullptr)
			return 0;

		if (size == -1)
			size = int(strlen(str));

		if (size == 0)
			return 0;

		unsigned int hash = 0;

		const int tail_size = size & 3;

		// main loop with a step of 32 bits
		for (size >>= 2; size > 0; size--)
		{
			hash += *(const unsigned short *)(str);
			unsigned int temp = (*(const unsigned short *)(str + sizeof(unsigned short)) << 11) ^ hash;
			hash = (hash << 16) ^ temp;
			hash += hash >> 11;

			str += sizeof(unsigned int);
		}

		// tail
		switch (tail_size)
		{
			case 3:
				hash += *(const unsigned short *)(str);
				hash ^= hash << 16;
				hash ^= str[sizeof(unsigned short)] << 18;
				hash += hash >> 11;
				break;
			case 2:
				hash += *(const unsigned short *)(str);
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
			case 1:
				hash += *str;
				hash ^= hash << 10;
				hash += hash >> 1;
				break;
		}

		// Force "avalanching" of final 127 bits.
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
	}
	UNIGINE_INLINE static unsigned int hash(const String &str) { return hash(str.get(), str.size()); }

	static StringStack<> dirname(const char *str);
	static StringStack<> basename(const char *str);
	static StringStack<> filename(const char *str);
	static StringStack<> pathname(const char *str);
	static StringStack<> extension(const char *str);
	static StringStack<> removeExtension(const char *str);

	static StringStack<> absname(const char *path, const char *str);
	static StringStack<> relname(const char *path, const char *str);
	static StringStack<> extension(const char *str, const char *ext);

	static StringStack<> title(const char *str);

	static StringStack<> joinPaths(const char *p0, const char *p1);
	static StringStack<> joinPaths(const char *p0, const String &p1);
	static StringStack<> joinPaths(const String &p0, const char *p1);
	static StringStack<> joinPaths(const String &p0, const String &p1);
	UNIGINE_INLINE static String &joinPaths(String &ret, const char *p0, const char *p1, int size0 = -1, int size1 = -1)
	{
		String::normalizeDirPath(ret, p0, size0);
		if (ret.size() && p1 && ((*p1 == '/') || (*p1 == '\\')))
		{
			++p1;
			size1 = (size1 == -1) ? size1 : (size1 - 1);
		}
		String::normalizePath(ret, p1, size1);
		return ret;
	}

	static StringStack<> normalizePath(const char *path);
	static StringStack<> normalizeDirPath(const char *path);

	UNIGINE_INLINE static String &normalizePath(String &ret, const char *path, int size = -1)
	{
		if (!path)
			return ret;

		auto ret_length = ret.length;
		ret.append(path, size);

		char *s = ret.data + ret_length;
		while (*s)
		{
			*s = (*s == '\\') ? '/' : *s;
			++s;
		}
		return ret;
	}
	UNIGINE_INLINE static String &normalizeDirPath(String &ret, const char *path, int size = -1)
	{
		normalizePath(ret, path, size);

		if (ret.size() && ret.last() != '/')
			ret.append('/');

		return ret;
	}

	UNIGINE_INLINE static int unicodeToUtf8(unsigned int code, char *dest)
	{
		unsigned char *d = (unsigned char *)dest;
		if (code < 0x80)
			*d++ = (unsigned char)(code & 0x7f);
		else if (code < 0x800)
		{
			*d++ = (unsigned char)(0xc0 | (code >> 6));
			*d++ = (unsigned char)(0x80 | (code & 0x3f));
		} else if (code < 0x10000)
		{
			*d++ = (unsigned char)(0xe0 | (code >> 12));
			*d++ = (unsigned char)(0x80 | ((code >> 6) & 0x3f));
			*d++ = (unsigned char)(0x80 | (code & 0x3f));
		} else if (code < 0x200000)
		{
			*d++ = (unsigned char)(0xf0 | (code >> 18));
			*d++ = (unsigned char)(0x80 | ((code >> 12) & 0x3f));
			*d++ = (unsigned char)(0x80 | ((code >> 6) & 0x3f));
			*d++ = (unsigned char)(0x80 | (code & 0x3f));
		}
		return (int)(d - (unsigned char *)dest);
	}
	static StringStack<> unicodeToUtf8(unsigned int code);
	static StringStack<> unicodeToUtf8(const wchar_t *src);
	static StringStack<> unicodeToUtf8(const unsigned int *src);
	UNIGINE_INLINE static int utf8ToUnicode(const char *src, unsigned int &code)
	{
		const unsigned char *s = (const unsigned char *)src;
		code = *s;
		if (code >= 0xf0)
		{
			code = (unsigned int)(*s++ & 0x07) << 18;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f) << 12;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f) << 6;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f);
		} else if (code >= 0xe0)
		{
			code = (unsigned int)(*s++ & 0x0f) << 12;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f) << 6;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f);
		} else if (code >= 0xc0)
		{
			code = (unsigned int)(*s++ & 0x1f) << 6;
			if (*s)
				code |= (unsigned int)(*s++ & 0x3f);
		} else
			s++;
		return (int)(s - (const unsigned char *)src);
	}
	UNIGINE_INLINE static int utf8ToUnicode(const char *src, unsigned int *dest, int size)
	{
		const char *s = src;
		unsigned int *d = dest;
		unsigned int code = 0;
		while (*s && --size)
		{
			s += utf8ToUnicode(s, code);
			*d++ = code;
		}
		*d = '\0';
		return (int)(d - dest);
	}
	UNIGINE_INLINE static int utf8ToUnicode(const char *src, wchar_t *dest, int size)
	{
		const char *s = src;
		wchar_t *d = dest;
		unsigned int code = 0;
		while (*s && --size)
		{
			s += utf8ToUnicode(s, code);
			if (code < 0x10000)
			{
				*d++ = (wchar_t)code;
			} else if (--size)
			{
				code -= 0x10000;
				*d++ = (wchar_t)((code >> 10) + 0xd800);
				*d++ = (wchar_t)((code & 0x3ff) + 0xdc00);
			} else
			{
				*d++ = 0x25a1;
			}
		}
		*d = '\0';
		return (int)(d - dest);
	}

	UNIGINE_INLINE static bool isEmpty(const char *str) { return str == nullptr || *str == 0; }

	UNIGINE_INLINE static bool equal(const char *s0, const char *s1) { return compare(s0, s1) == 0; }
	UNIGINE_INLINE static bool equal(const String &s0, const char *s1) { return equal(s0.get(), s1); }
	UNIGINE_INLINE static bool equal(const char *s0, const String &s1) { return equal(s0, s1.get()); }
	UNIGINE_INLINE static bool equal(const String &s0, const String &s1) { return s0.length == s1.length && equal(s0.get(), s1.get()); }

	UNIGINE_INLINE static int utf8strlen(const char *str)
	{
		const unsigned char *s = (const unsigned char *)str;
		int length = 0;
		while (*s)
		{
			int code = *s++;
			if (code >= 0xf0)
			{
				if (*s)
					s++;
				if (*s)
					s++;
				if (*s)
					s++;
			} else if (code >= 0xe0)
			{
				if (*s)
					s++;
				if (*s)
					s++;
			} else if (code >= 0xc0)
			{
				if (*s)
					s++;
			}
			length++;
		}
		return length;
	}
	static StringStack<> utf8substr(const char *str, int pos, int size = -1);

	// bidirectional text support
	UNIGINE_INLINE static void reverseUtf8BiDirectional(String &string)
	{
		if (string.empty())
			return;

		// split by paragraphs
		char *s = &string.first();
		char *d = s;

		while (*s)
		{
			if (*s == '\n')
			{
				*s = '\0';
				reverse_utf8_bidirectional(d);
				*s = '\n';
				s++;
				d = s;
			} else
				s++;
		}

		if (s != d)
			reverse_utf8_bidirectional(d);
	}
	static Direction unicodeGetDirection(unsigned int code);

	UNIGINE_INLINE static bool match(const char *pattern, const char *str)
	{
		bool ret = true;
		const char *p = pattern;
		const char *s = str;
		while (*p)
		{
			if (*p == '*' && ret)
			{
				if (*(p + 1) == '\0' || *(p + 1) == '|')
					break;
				p++;
				int size = 0;
				while (p[size] && p[size] != '|')
					size++;
				while (*s)
					s++;
				if (s - str >= size)
					s -= size;
			} else if (*p == '|')
			{
				if (ret)
					break;
				p++;
				s = str;
			} else
			{
				if (*p != '?' && *p != *s)
					ret = 0;
				if (*s)
					s++;
				p++;
			}
		}
		if (*s != '\0' && *p == '\0')
			ret = false;
		return ret;
	}

	static const String null;

protected:
	template <int>
	friend class StringStack;

	UNIGINE_INLINE String(int c, char *d) : length(0), capacity(c), dynamic(false), data(d) {}

	enum
	{
		STACK_CAPACITY = 8,
	};

	// bidirectional text
	static void reverse_utf8_bidirectional(char *str);

	UNIGINE_INLINE void do_assign(const char *s, int size = -1)
	{
		assert(length == 0 && !dynamic);
		if (s == nullptr)
		{
			data[0] = '\0';
			return;
		}

		length = size == -1 ? int(strlen(s)) : size;
		if ((length + 1) > space())
		{
			capacity = length + 1;
			dynamic = true;
			data = (char *)Memory::allocate(capacity);
		}

		memcpy(data, s, length);
		data[length] = '\0';
	}
	UNIGINE_INLINE void do_copy(const char *s, int size)
	{
		reserve(size);
		length = size;
		memmove(data, s, length);
		data[length] = '\0';
	}

	UNIGINE_INLINE void do_append(char c)
	{
		reserve(length + 1);
		data[length] = c;
		data[++length] = '\0';
	}
	UNIGINE_INLINE void do_append(const char *s, int size)
	{
		if ((size + length + 1) <= space())
		{
			memcpy(data + length, s, size);
		} else
		{
			capacity = grow_to(length + size);
			char *new_data = (char *)Memory::allocate(capacity);
			memcpy(new_data, data, length);
			memcpy(new_data + length, s, size);
			if (dynamic)
				Memory::deallocate(data);
			dynamic = true;
			data = new_data;
		}

		length += size;
		data[length] = '\0';
	}
	UNIGINE_INLINE void do_append(int pos, char c)
	{
		assert((unsigned int)pos <= (unsigned int)length && "String::do_append(): bad position");
		reserve(length + 1);
		memmove(data + pos + 1, data + pos, length - pos);
		data[pos] = c;
		data[++length] = '\0';
	}
	UNIGINE_INLINE void do_append(int pos, const char *s, int size)
	{
		assert((unsigned int)pos <= (unsigned int)length && "String::do_append(): bad position");
		if ((size + length + 1) <= space())
		{
			memmove(data + pos + size, data + pos, length - pos);
			memmove(data + pos, s, size);
		} else
		{
			capacity = grow_to(length + size);
			char *new_data = (char *)Memory::allocate(capacity);
			memcpy(new_data, data, pos);
			memcpy(new_data + pos, s, size);
			memcpy(new_data + pos + size, data + pos, length - pos);

			if (dynamic)
				Memory::deallocate(data);
			dynamic = true;
			data = new_data;
		}

		length += size;
		data[length] = '\0';
	}

	UNIGINE_INLINE int grow_to(int new_length) const
	{
		assert(capacity < INT_MAX && "String::grow_to: capacity limit");
		const size_t ret = static_cast<size_t>(new_length) + new_length / 2 + 2;
		return static_cast<int>(ret > INT_MAX ? INT_MAX : ret);
	}

	int length{0};
	unsigned int capacity : 31;
	unsigned int dynamic : 1;
	char stack_data[STACK_CAPACITY];
	char *data;
};

static_assert(sizeof(String) == 24, "String size is changed");

class StringPtr
{
public:
	UNIGINE_INLINE StringPtr() = default;
	UNIGINE_INLINE StringPtr(const String &s)
	{
		if (s.size())
			string = new String(s);
	}
	UNIGINE_INLINE StringPtr(const StringPtr &s)
	{
		if (s.string && s.string->size())
			string = new String(*s.string);
	}
	UNIGINE_INLINE StringPtr(const char *s)
	{
		if (s && strlen(s))
			string = new String(s);
	}
	UNIGINE_INLINE ~StringPtr()
	{
		delete string;
	}

	UNIGINE_INLINE StringPtr &operator=(const char *s)
	{
		if (s && strlen(s))
		{
			if (string == nullptr)
				string = new String(s);
			else
				*string = s;
		} else
			clear();
		return *this;
	}
	UNIGINE_INLINE StringPtr &operator=(const String &s)
	{
		if (s.size())
		{
			if (string == nullptr)
				string = new String(s);
			else
				*string = s;
		} else
			clear();
		return *this;
	}
	UNIGINE_INLINE StringPtr &operator=(const StringPtr &s)
	{
		if (string == s.string)
			return *this;
		if (s.string && s.string->size())
		{
			if (string == nullptr)
				string = new String(*s.string);
			else
				*string = *s.string;
		} else
			clear();
		return *this;
	}
	UNIGINE_INLINE void swap(StringPtr &s)
	{
		if (string == s.string)
			return;
		String *temp = string;
		string = s.string;
		s.string = temp;
	}

	UNIGINE_INLINE StringPtr &operator+=(char ch)
	{
		if (string == nullptr)
			string = new String();
		*string += ch;
		return *this;
	}
	UNIGINE_INLINE StringPtr &operator+=(const char *s)
	{
		if (s && strlen(s))
		{
			if (string == nullptr)
				string = new String(s);
			else
				*string += s;
		}
		return *this;
	}
	UNIGINE_INLINE StringPtr &operator+=(const String &s)
	{
		if (s.size())
		{
			if (string == nullptr)
				string = new String(s);
			else
				*string += s;
		}
		return *this;
	}

	UNIGINE_INLINE const char *get() const { return (string) ? *string : String::null; }
	UNIGINE_INLINE const String &getString() const { return (string) ? *string : String::null; }
	UNIGINE_INLINE operator const char *() const { return (string) ? *string : String::null; }
	UNIGINE_INLINE operator const void *() const { return (string) ? *string : String::null; }
	UNIGINE_INLINE int size() const { return (string) ? string->size() : 0; }
	UNIGINE_INLINE bool empty() const { return (string) ? string->empty() : true; }
	UNIGINE_INLINE void clear()
	{
		delete string;
		string = nullptr;
	}

private:
	String *string{nullptr};
};

template <int Capacity>
class StringStack : public String
{
public:
	UNIGINE_INLINE StringStack() : String(Capacity, stack_data)
	{
		data[0] = '\0';
	}
	UNIGINE_INLINE StringStack(const String &s) : String(Capacity, stack_data)
	{
		do_assign(s.data, s.length);
	}
	UNIGINE_INLINE StringStack(String &&s) : String(Capacity, stack_data)
	{
		if (s.dynamic)
		{
			length = s.length;
			capacity = s.capacity;
			dynamic = s.dynamic;
			data = s.data;

			s.length = 0;
			s.capacity = STACK_CAPACITY;
			s.dynamic = false;
			s.data = s.stack_data;
			s.data[s.length] = '\0';
		} else
		{
			do_assign(s.data, s.length);
			s.clear();
		}
	}
	UNIGINE_INLINE StringStack(const StringStack &s) : String(Capacity, stack_data)
	{
		do_assign(s.data, s.length);
	}

	template<int OtherCapacity>
	UNIGINE_INLINE StringStack(StringStack<OtherCapacity> &&s) : String(Capacity, stack_data)
	{
		if (s.dynamic)
		{
			length = s.length;
			capacity = s.capacity;
			dynamic = s.dynamic;
			data = s.data;

			s.length = 0;
			s.data = nullptr;
			s.capacity = 0;
			s.dynamic = false;
			s.destroy();
		} else
		{
			do_copy(s.data, s.length);
			s.clear();
		}
	}

	UNIGINE_INLINE StringStack(const char *s) : String(Capacity, stack_data)
	{
		do_assign(s);
	}
	UNIGINE_INLINE StringStack(const char *s, int size) : String(Capacity, stack_data)
	{
		do_assign(s, size);
	}

	UNIGINE_INLINE StringStack &operator=(const char *s)
	{
		if (data == s)
			return *this;
		copy(s);
		return *this;
	}
	UNIGINE_INLINE StringStack &operator=(const String &s)
	{
		if (data == s.data)
			return *this;
		copy(s);
		return *this;
	}
	UNIGINE_INLINE StringStack &operator=(const StringStack &s)
	{
		if (data == s.data)
			return *this;
		copy(s);
		return *this;
	}

	template<int OtherCapacity>
	UNIGINE_INLINE StringStack &operator=(StringStack<OtherCapacity> &&s)
	{
		if (data == s.data)
			return *this;
		if (s.dynamic)
		{
			destroy();
			length = s.length;
			capacity = s.capacity;
			dynamic = s.dynamic;
			data = s.data;

			s.length = 0;
			s.data = nullptr;
			s.capacity = 0;
			s.dynamic = false;
			s.destroy();
		} else
		{
			do_copy(s.data, s.length);
			s.clear();
		}
		return *this;
	}

	UNIGINE_INLINE void destroy()
	{
		String::destroy();
		capacity = Capacity;
		dynamic = false;
		data = stack_data;
		data[length] = '\0';
	}

private:
	char stack_data[Capacity];
};

template <int Capacity>
class StringArray
{
public:
	UNIGINE_INLINE StringArray()
		: data_length(0)
		, data(stack_data)
		, indices_length(0)
		, indices_capacity(Capacity)
		, indices(stack_indices)
	{

	}
	UNIGINE_INLINE StringArray(const StringArray &s)
		: data_length(s.data_length)
		, data(stack_data)
		, indices_length(s.indices_length)
		, indices_capacity(Capacity)
		, indices(stack_indices)
	{
		if (data_length + 1 > Capacity)
			data = (char *)Memory::allocate(data_length + 1);
		memcpy(data, s.data, data_length);
		data[data_length] = '\0';
		if (indices_length > indices_capacity)
		{
			indices_capacity = indices_length;
			indices = (int *)Memory::allocate(indices_capacity * sizeof(int));
		}
		memcpy(indices, s.indices, indices_length * sizeof(int));
	}

	UNIGINE_INLINE ~StringArray()
	{
		if (data != stack_data)
			Memory::deallocate(data);
		if (indices != stack_indices)
			Memory::deallocate(indices);
	}

	UNIGINE_INLINE const char *operator[](int index) const
	{
		assert(index < indices_length && indices_length >= 0 && "StringArray::operator[](): bad index");
		return data + indices[index];
	}

	UNIGINE_INLINE int size() const { return indices_length; }
	UNIGINE_INLINE bool empty() const { return indices_length == 0; }

private:
	friend class String;

	UNIGINE_INLINE StringArray(int size)
		: data_length(size)
		, data(stack_data)
		, indices_length(0)
		, indices_capacity(Capacity)
		, indices(stack_indices)
	{
		if (data_length + 1 > Capacity)
			data = (char *)Memory::allocate(data_length + 1);
		data[data_length] = '\0';
	}

	UNIGINE_INLINE void append(int index)
	{
		if (indices_length + 1 > indices_capacity)
		{
			indices_capacity = (indices_length + 1) * 2;
			int *new_indices = (int *)Memory::allocate(indices_capacity * sizeof(int));
			memcpy(new_indices, indices, indices_length * sizeof(int));
			if (indices != stack_indices)
				Memory::deallocate(indices);
			indices = new_indices;
		}
		indices[indices_length++] = index;
	}

	int data_length;
	char *data;
	char stack_data[Capacity];

	int indices_length;
	int indices_capacity;
	int *indices;
	int stack_indices[Capacity];
};

UNIGINE_INLINE StringStack<> operator+(String &&s0, String &&s1)
{
	StringStack<> ret = std::move(s0);
	ret.append(std::move(s1));
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(String &&s0, const String &s1)
{
	StringStack<> ret = std::move(s0);
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const String &s0, String &&s1)
{
	StringStack<> ret = s0;
	ret.append(std::move(s1));
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(String &&s0, const char *s1)
{
	StringStack<> ret = std::move(s0);
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const char *s0, String &&s1)
{
	StringStack<> ret = s0;
	ret.append(std::move(s1));
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const String &s0, const String &s1)
{
	StringStack<> ret = s0;
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const String &s0, const char *s1)
{
	StringStack<> ret = s0;
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const char *s0, const String &s1)
{
	StringStack<> ret = StringStack<>(s0);
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(const String &s0, char c1)
{
	StringStack<> ret = s0;
	ret.append(c1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(String &&s0, char c1)
{
	StringStack<> ret(std::move(s0));
	ret.append(c1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(char c0, const String &s1)
{
	StringStack<> ret;
	ret.append(c0);
	ret.append(s1);
	return ret;
}
UNIGINE_INLINE StringStack<> operator+(char c0, String &&s1)
{
	StringStack<> ret;
	ret.append(c0);
	ret.append(std::move(s1));
	return ret;
}

UNIGINE_INLINE bool operator==(const String &s0, const String &s1) { return String::equal(s0, s1); }
UNIGINE_INLINE bool operator==(const String &s0, const char *s1) { return String::equal(s0, s1); }
UNIGINE_INLINE bool operator==(const char *s0, const String &s1) { return String::equal(s0, s1); }

UNIGINE_INLINE bool operator!=(const String &s0, const String &s1) { return s0.size() != s1.size() || strcmp(s0.get(), s1.get()) != 0; }
UNIGINE_INLINE bool operator!=(const String &s0, const char *s1) { return String::compare(s0, s1) != 0; }
UNIGINE_INLINE bool operator!=(const char *s0, const String &s1) { return String::compare(s0, s1) != 0; }

UNIGINE_INLINE bool operator<(const String &s0, const String &s1) { return String::compare(s0, s1) < 0; }
UNIGINE_INLINE bool operator<(const String &s0, const char *s1) { return String::compare(s0, s1) < 0; }
UNIGINE_INLINE bool operator<(const char *s0, const String &s1) { return String::compare(s0, s1) < 0; }

UNIGINE_INLINE bool operator>(const String &s0, const String &s1) { return String::compare(s0, s1) > 0; }
UNIGINE_INLINE bool operator>(const String &s0, const char *s1) { return String::compare(s0, s1) > 0; }
UNIGINE_INLINE bool operator>(const char *s0, const String &s1) { return String::compare(s0, s1) > 0; }

UNIGINE_INLINE bool operator<=(const String &s0, const String &s1) { return String::compare(s0, s1) <= 0; }
UNIGINE_INLINE bool operator<=(const String &s0, const char *s1) { return String::compare(s0, s1) <= 0; }
UNIGINE_INLINE bool operator<=(const char *s0, const String &s1) { return String::compare(s0, s1) <= 0; }

UNIGINE_INLINE bool operator>=(const String &s0, const String &s1) { return String::compare(s0, s1) >= 0; }
UNIGINE_INLINE bool operator>=(const String &s0, const char *s1) { return String::compare(s0, s1) >= 0; }
UNIGINE_INLINE bool operator>=(const char *s0, const String &s1) { return String::compare(s0, s1) >= 0; }

UNIGINE_INLINE bool operator==(const StringPtr &s0, const StringPtr &s1) { return s0.getString() == s1.getString(); }
UNIGINE_INLINE bool operator==(const StringPtr &s0, const char *s1) { return s0.getString() == s1; }
UNIGINE_INLINE bool operator==(const char *s0, const StringPtr &s1) { return s0 == s1.getString(); }

UNIGINE_INLINE bool operator!=(const StringPtr &s0, const StringPtr &s1) { return s0.getString() != s1.getString(); }
UNIGINE_INLINE bool operator!=(const StringPtr &s0, const char *s1) { return s0.getString() != s1; }
UNIGINE_INLINE bool operator!=(const char *s0, const StringPtr &s1) { return s0 != s1.getString(); }

template<typename Type>
struct Hasher;

template<>
struct Hasher<String>
{
	using HashType = unsigned int;
	UNIGINE_INLINE static HashType create(const char *v) { return String::hash(v); }
	UNIGINE_INLINE static HashType create(const String &v) { return String::hash(v.get(), v.size()); }
};

template<int size>
struct Hasher<StringStack<size>>
{
	using HashType = unsigned int;
	UNIGINE_INLINE static HashType create(const char *v) { return String::hash(v); }
	UNIGINE_INLINE static HashType create(const StringStack<size> &v) { return String::hash(v.get(), v.size()); }
};

template <typename Type, typename Counter = unsigned int>
using StringMap = HashMap<String, Type, Counter>;

template <typename Counter = unsigned int>
using StringSet = HashSet<String, Counter>;

} // namespace Unigine
