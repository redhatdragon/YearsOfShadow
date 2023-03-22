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
#include <UnigineMathLib.h>

namespace Unigine
{

struct Palette
{
	static const Palette zero;
	static const Palette one;

	enum : int
	{
		RED = 0, ORANGE, YELLOW, CHARTREUSE,
		GREEN, SPRINGGREEN, CYAN, AZURE,
		BLUE, VIOLET, MAGENTA, ROSE,

		SIZE,
		INVALID = -1
	};

	UNIGINE_INLINE Palette() { clear(); }
	UNIGINE_INLINE Palette(const Palette &palette) { copy(palette); }
	UNIGINE_INLINE Palette(const float (&palette)[SIZE])
	{
		for (int i = 0; i < SIZE; i++)
			colors[i] = palette[i];
	}
	UNIGINE_INLINE explicit Palette(float value)
	{
		clear(value);
	}

	UNIGINE_INLINE Palette &operator=(const Palette &palette)
	{
		copy(palette);
		return *this;
	}

	UNIGINE_INLINE bool operator==(const Palette &palette) const
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (Math::abs(colors[i] - palette.colors[i]) > Math::Consts::EPS)
				return false;
		}
		return true;
	}
	UNIGINE_INLINE bool operator==(const float (&palette)[SIZE]) const
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (Math::abs(colors[i] - palette[i]) > Math::Consts::EPS)
				return false;
		}
		return true;
	}
	UNIGINE_INLINE bool operator==(float value) const
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (Math::abs(colors[i] - value) > Math::Consts::EPS)
				return false;
		}
		return true;
	}

	UNIGINE_INLINE bool operator!=(const Palette &palette) const
	{
		return !(*this == palette);
	}
	UNIGINE_INLINE bool operator!=(const float (&palette)[SIZE]) const
	{
		return !(*this == palette);
	}
	UNIGINE_INLINE bool operator!=(float value) const
	{
		return !(*this == value);
	}

	UNIGINE_INLINE float& operator[](StringStack<> color)
	{
		int index = colorNameToIndex(color);

		assert((index >= 0) && (index < SIZE) && "Palette::operator[](): index out of bounds");
		return colors[index];
	}
	UNIGINE_INLINE float operator[](StringStack<> color) const
	{
		int index = colorNameToIndex(color);
		
		if (index == INVALID)
			return 0.0f;
		
		assert((index >= 0) && (index < SIZE) && "Palette::operator[](): index out of bounds");
		return colors[index];
	}

	UNIGINE_INLINE float& operator[](int index)
	{
		assert((index >= 0) && (index < SIZE) && "Palette::operator[](): index out of bounds");
		return colors[index];
	}
	UNIGINE_INLINE float operator[](int index) const
	{
		assert((index >= 0) && (index < SIZE) && "Palette::operator[](): index out of bounds");
		return colors[index];
	}

	UNIGINE_INLINE void clear(float value = 0.0f)
	{
		red = value;
		orange = value;
		yellow = value;
		chartreuse = value;
		green = value;
		spring_green = value;
		cyan = value;
		azure = value;
		blue = value;
		violet = value;
		magenta = value;
		rose = value;
	}
	UNIGINE_INLINE void copy(const Palette &src)
	{
		for (int i = 0; i < SIZE; i++)
			colors[i] = src.colors[i];
	}

	StringStack<> toString()
	{
		return String::format("%f %f %f %f %f %f %f %f %f %f %f %f", red, orange, yellow, chartreuse, 
								green, spring_green, cyan, azure, blue, violet, magenta, rose);
	}

	static int colorNameToIndex(const char *src)
	{
		if (String::compare(src, "red",				false) == 0) return RED;
		if (String::compare(src, "orange",			false) == 0) return ORANGE;
		if (String::compare(src, "yellow",			false) == 0) return YELLOW;
		if (String::compare(src, "chartreuse",		false) == 0) return CHARTREUSE;
		if (String::compare(src, "green",			false) == 0) return GREEN;
		if (String::compare(src, "spring_green",	false) == 0) return SPRINGGREEN;
		if (String::compare(src, "springgreen",		false) == 0) return SPRINGGREEN;
		if (String::compare(src, "cyan",			false) == 0) return CYAN;
		if (String::compare(src, "azure",			false) == 0) return AZURE;
		if (String::compare(src, "blue",			false) == 0) return BLUE;
		if (String::compare(src, "violet",			false) == 0) return VIOLET;
		if (String::compare(src, "magenta",			false) == 0) return MAGENTA;
		if (String::compare(src, "rose",			false) == 0) return ROSE;

		return INVALID;
	}

	static const char *colorIndexToName(int index)
	{
		assert(index > -1 && index < Palette::SIZE && "Palette::colorIndexToName(): Unknown color index\n");

		switch (index)
		{
			default:
			case RED:			return "red";
			case ORANGE:		return "orange";
			case YELLOW:		return "yellow";
			case CHARTREUSE:	return "chartreuse";
			case GREEN:			return "green";
			case SPRINGGREEN:	return "springgreen";
			case CYAN:			return "cyan";
			case AZURE:			return "azure";
			case BLUE:			return "blue";
			case VIOLET:		return "violet";
		}
	}

	union
	{
		struct
		{
			float red, orange, yellow, chartreuse, green, spring_green, cyan, azure, blue, violet, magenta, rose;
		};
		float colors[SIZE]{ 0.0f };
	};
};

} // namespace
