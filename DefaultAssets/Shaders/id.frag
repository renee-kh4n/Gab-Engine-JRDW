#version 450
//UBOS
layout(binding = 1) uniform Identity {
    int id;
} identity;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(identity.id, 0, 0, 1.0f);
}