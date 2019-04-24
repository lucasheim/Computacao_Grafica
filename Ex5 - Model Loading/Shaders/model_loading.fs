#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float colorPercentage;

void main()
{    
    FragColor = mix(texture(texture_diffuse1, TexCoords), vec4(0.0f, 0.0f, 1.0f, 0.0f), colorPercentage);
}