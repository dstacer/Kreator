#shader vertex
#version 330 core

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexSlot;

out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexSlot;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_pos;
    v_Color = a_Color;
    v_TexCoords = a_TexCoords;
    v_TexSlot = a_TexSlot;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;
in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexSlot;

uniform sampler2DArray u_TexUnit;

void main()
{
    vec4 texColor = texture(u_TexUnit, vec3(v_TexCoords, v_TexSlot));
    color = texColor;
}