#version 450

//TEXTURES
// specify set so as not to conflict with vertex shader in 3d.vert

layout(set=1, binding = 0) uniform sampler2D colortex;

// layout loc must match with vertex shader in 3d.vert
layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;


layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(colortex, fragTexCoord);
}