#version 110
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) varying out vec4 out_color;
       
varying in vec3 color;
varying in vec2 texPos;

uniform sampler2D fontTexture;

void main()
{
    vec4 fontColor = texture2D(fontTexture, texPos);
    if(fontColor.r < 0.5)
        discard;
    out_color = vec4(fontColor.rgb * color, 1.0);
}