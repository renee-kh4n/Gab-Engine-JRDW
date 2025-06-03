#version 450

//UBOS

//TEXTURES
layout(binding = 2) uniform sampler2D colortex;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(colortex, fragTexCoord);
}