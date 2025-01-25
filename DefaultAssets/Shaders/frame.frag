#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D colorBufferTexture;
uniform sampler2D depthBufferTexture;

void main()
{
    vec4 color = texture(colorBufferTexture, TexCoords);
    float depth = texture(depthBufferTexture, TexCoords).x;

    FragColor = color;
}