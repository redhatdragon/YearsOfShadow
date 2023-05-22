#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
out vec2 v_texCoord;
out vec3 posAsColor;
uniform mat4 u_MVP;  //model-view-projection matrix

void main() {
    gl_Position = u_MVP * vec4(position, 1.0);
    v_texCoord = texCoord;
    posAsColor = position;
}



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_texCoord;
in vec3 posAsColor;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    color = vec4(posAsColor, 1.0f);
}