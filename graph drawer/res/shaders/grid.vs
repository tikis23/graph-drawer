#version 110
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) varying in vec2 in_position;

uniform vec2 u_windowSize;


void main()
{
    gl_Position = vec4(in_position / u_windowSize, 0, 1);
}