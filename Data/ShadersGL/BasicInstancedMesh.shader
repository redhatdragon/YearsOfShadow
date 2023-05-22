#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 offset;
out vec2 v_texCoord;
uniform mat4 u_MVP;  //model-view-projection matrix

void main() {
    gl_Position = u_MVP * vec4(position+offset, 1.0);
    v_texCoord = texCoord;
}


    
#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_texCoord;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    vec4 texColor = texture(u_texture, v_texCoord);
    color = u_color * texColor;
    color.w = 1;
}