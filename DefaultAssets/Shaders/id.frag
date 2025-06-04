#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 8) flat in uint obj_id;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(obj_id, 0, 0, 1.0f);
}