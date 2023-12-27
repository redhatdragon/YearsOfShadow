#define _CRT_SECURE_NO_WARNINGS
#define STB_VORBIS_HEADER_ONLY // <-- Exclude stb_vorbis' implementation
#include "stb_vorbis.h"
#define CUTE_SOUND_IMPLEMENTATION
#include "cute_sound.h"
#undef STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.h"