#version 330 core

layout(location = 0) out vec4 out_color;
       
in vec3 color;
in vec2 texPos;

uniform sampler2D fontTexture;

void main()
{
    vec4 fontColor = texture2D(fontTexture, texPos);
    if(fontColor.r < 0.5)
        discard;
    out_color = vec4(fontColor.rgb * color, 1.0);
}