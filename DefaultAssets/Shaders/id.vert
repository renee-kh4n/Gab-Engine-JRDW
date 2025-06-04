#version 450


layout(set = 1, binding = 0) uniform Transform {
    mat4 model;
    mat4 view;
    mat4 proj;
} transform;

layout(set = 0, binding = 0) uniform Identity {
    int obj_id;
} identity;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 8) flat out uint obj_id;

void main() {
    gl_Position = transform.proj * transform.view * transform.model * vec4(inPosition, 1.0);

    fragTexCoord = inTexCoord;
    fragNormal = inNormal;

    obj_id = identity.obj_id;
}