#version 450

layout(set = 0, binding = 0) uniform sampler2D uAccumTex;  // Texture with RGB * alpha
layout(set = 0, binding = 1) uniform sampler2D uAlphaTex;  // Texture with accumulated alpha

layout(location = 0) in vec2 vTexCoord; // From vertex shader

layout(location = 0) out vec4 outColor;

void main() {
    vec3 accumColor = texture(uAccumTex, vTexCoord).rgb;
    float accumAlpha = texture(uAlphaTex, vTexCoord).r;

    if (accumAlpha > 0.0001)
        outColor = vec4(accumColor.rgb / accumAlpha, accumAlpha);
    else
        outColor = vec4(0.0);
}