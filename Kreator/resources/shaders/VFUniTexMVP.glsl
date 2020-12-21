#shader vertex
#version 330 core

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_pos;
    v_texCoord = a_texCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec2 v_texCoord;

uniform vec4 u_Color;
uniform sampler2D u_TexSlot;

void main()
{
    vec4 texColor = texture(u_TexSlot, v_texCoord);
    color = texColor;//u_Color;
}