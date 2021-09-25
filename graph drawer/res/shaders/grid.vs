#version 330 core

layout(location = 0) in vec2 in_position;

uniform vec2 u_windowSize;


void main()
{
    gl_Position = vec4(in_position / u_windowSize, 0, 1);
}