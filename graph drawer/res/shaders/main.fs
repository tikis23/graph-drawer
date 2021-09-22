#version 110
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) varying out vec4 color;
       
uniform vec3 u_color;

void main()
{
    color = vec4(u_color, 1.0);
}