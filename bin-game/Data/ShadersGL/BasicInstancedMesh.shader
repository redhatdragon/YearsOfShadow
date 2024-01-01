#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 offset;

out vec2 v_texCoord;
out float distToCamera;
out vec3 v_diffuse;
uniform mat4 u_M;  	// model
uniform mat4 u_MVP;  	// model-view-projection matrix
uniform vec3 u_scale;

uniform vec3 u_camPos;
// uniform vec3 u_camDir;

void main() 
{
	vec4 pos = u_M * vec4((position*u_scale)+offset, 1.0);
    	gl_Position = u_MVP * vec4((position*u_scale)+offset, 1.0);
    	v_texCoord = texCoord;
    	distToCamera = length(gl_Position.xyz);

	vec3 lightDir = normalize(u_camPos - pos.xyz);
	float diff = max(dot(normal, lightDir), 0.0);
	v_diffuse = diff * vec3(1.0, 1.0, 1.0);
}


    
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_texCoord;
in float distToCamera;
in vec3 v_diffuse;

uniform vec4 u_color;
uniform sampler2D u_texture;

layout(std140) uniform PointLightsBlock
{
    vec3 pos[200];
	float dist[200];
	float intensity[200];
	int count;
} pointLightBlock;

void main() {
    //vec4 texColor = texture(u_texture, v_texCoord);
    //color = u_color * texColor;
    //color.w = 1;

    	vec4 texColor = texture(u_texture, v_texCoord);
    	float distMod = distToCamera;
    	float maxDist = 64;
	if (distMod > maxDist)
        	distMod = maxDist;
    	distMod = maxDist / distMod;
    	distMod = distMod - 1;
    	if (distMod > 1)
        	distMod = 1;
	color = u_color * texColor * distMod;
	color.w = 1;
	
	color.x *= (v_diffuse.x + 0.5);
	color.y *= (v_diffuse.y + 0.5);
	color.z *= (v_diffuse.z + 0.5);
}