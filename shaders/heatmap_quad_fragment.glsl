#version 430 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;
uniform float minValue;
uniform float maxValue;

void main()
{             
    float heatValue = texture(tex, TexCoords).r;
    
    // Normalize the heat value between 0 and 1 based on the provided min and max values
    float normalizedHeat = (heatValue - minValue) / (maxValue - minValue);

    // Interpolate the colors based on the 'hot' color scheme
    vec3 color;
    color.r = normalizedHeat < 0.5 ? normalizedHeat * 2.0 : 1.0;
    color.g = normalizedHeat < 0.5 ? 0.0 : (normalizedHeat - 0.5) * 2.0;
    color.b = 0.0;

    FragColor = vec4(color, 1.0); // The alpha channel is set to 1.0
}
