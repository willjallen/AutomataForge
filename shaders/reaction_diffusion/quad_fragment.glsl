#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{             
    vec4 colors = texture(tex, TexCoords).rgba;
    
    colors.r = 0.0;
    colors.b = colors.g;

    FragColor = colors;
} 

