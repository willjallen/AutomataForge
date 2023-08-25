#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main()
{             
    // vec3 texCol = vexture(tex, TexCoords).rgb;      
    FragColor = texture(tex, TexCoords);
}