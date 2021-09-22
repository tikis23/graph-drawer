#version 110
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) varying in vec4 in_position;
layout(location = 1) varying in vec3 in_color;

varying out vec3 color;
varying out vec2 texPos;

uniform vec2 u_windowSize;
void main()
{
    color = in_color;
    texPos = in_position.zw;
    gl_Position = vec4(in_position.xy / u_windowSize, -1, 1);
}