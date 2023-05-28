#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_texCoord;
out float distToCamera;
uniform mat4 u_MVP;  //model-view-projection matrix

void main() {
    gl_Position = u_MVP * vec4(position, 1.0);
    v_texCoord = texCoord;
    distToCamera = length(gl_Position.xyz);
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_texCoord;
in float distToCamera;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    vec4 texColor = texture(u_texture, v_texCoord);
    float distMod = distToCamera;
    float maxDist = 64;
    if (distMod > maxDist)
        distMod = maxDist;
    distMod = maxDist / distMod;
    distMod = distMod - 1;
    if (distMod > 1)
        distMod = 1;
    color = u_color*texColor*distMod;
    color.w = 1;
}