#version 450
//UBOS

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

//TEXTURES
layout(binding = 2) uniform sampler2D colortex;

layout(location = 0) out vec4 outColor;

void main() {
    vec2 size = vec2(15, 15);

    float total = floor(fragTexCoord.x * float(size.x)) +
                  floor(fragTexCoord.y * float(size.y));
    bool isEven = mod(total, 2.0) == 0.0;
    vec4 col1 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 col2 = vec4(1.0, 1.0, 1.0, 1.0);
    outColor = (isEven) ? texture(colortex, fragTexCoord) : col2;
}