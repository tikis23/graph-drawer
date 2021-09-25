#version 330 core

layout(location = 0) in vec4 in_position;
layout(location = 1) in vec3 in_color;

out vec3 color;
out vec2 texPos;

uniform vec2 u_windowSize;
void main()
{
    color = in_color;
    texPos = in_position.zw;
    gl_Position = vec4(in_position.xy / u_windowSize, -1, 1);
}