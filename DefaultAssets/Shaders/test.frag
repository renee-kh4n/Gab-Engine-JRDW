#version 450

layout(binding = 1) uniform sampler2D colortex;

layout(binding = 2) uniform Shading {
    vec4 color;
} shading;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(colortex, fragTexCoord) * shading.color;
    outColor = vec4(1, 1, 1, 1);
}