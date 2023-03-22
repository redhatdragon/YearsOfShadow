// Copyright (C) 2005-2018, Unigine Corp. All rights reserved.

#define THICKNESS 0.0025f

#include <core/materials/shaders/render/common.h>

STRUCT_FRAG_BEGIN
	INIT_COLOR(float4)
STRUCT_FRAG_END

MAIN_FRAG_BEGIN(FRAGMENT_IN)
	
	float2 uv = IN_UV;
	
	float4 color = TEXTURE_BIAS_ZERO(tex_color,uv); 
	float4 auxiliary = TEXTURE_BIAS_ZERO(tex_auxiliary,uv); 
	
	float iaspect = s_viewport.y / s_viewport.x;
	float offset_y = THICKNESS;
	float offset_x = offset_y * iaspect;
	float4 selection = TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(0, offset_y));
		   selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(0, -offset_y));
		   selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(offset_x, 0));
		   selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(-offset_x, 0));
		   //selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(offset_x, offset_y));
		   //selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(-offset_x, offset_y));
		   //selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(offset_x, -offset_y));
		   //selection += TEXTURE_BIAS_ZERO(tex_auxiliary, uv + float2(-offset_x, -offset_y));
	selection = saturate(selection);
	
	OUT_COLOR = color + selection - auxiliary;
	
MAIN_FRAG_END
